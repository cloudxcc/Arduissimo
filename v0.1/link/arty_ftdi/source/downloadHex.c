#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#ifdef __WIN32__
#define _WINDOWS
#define usleep(x) Sleep(x/1000 +1)
#include <windows.h>
#endif
#include <ftd2xx.h>

FILE *hex;
const unsigned programOffset = 0x00010000;
const unsigned programSize = 0xc00;
char program [0xc00];

/////////////////////////////////////////////////////////////////////////////////
//	readHex
/////////////////////////////////////////////////////////////////////////////////
unsigned readHex ()
{
   int c;
   int stat1 = 0;
   unsigned ptr = 0;
   int count = 0;
   while (1)
   {
      c = getc(hex);
      if (c == -1)
      {
         break;
      }
      if (   (c == '\n') |
             (c == 13)   |
             (c == 10)   )
      {
         if (stat1 == 1)
         {          
            ptr = count;
            if (ptr >= programOffset + programSize)
              break;
            stat1 += 1;
         }
      } else
      if (   (c == ' ')   )
      {
         if (stat1 == 1)
         {
            ptr = count;      
            if (ptr >= programOffset + programSize)
               break;
            stat1 += 1;
         } else
         {
            program[ptr - programOffset] = count;
            ptr += 1;
           if (ptr >= programOffset + programSize)
              break;
         }
         count = 0;
      } else
      if (c == '@')
      {
         if (stat1 == 1)
            break;
         stat1 += 1;
      } else
      {
         if (c < 58)
            count = count * 16 + (c - 48);
         else
            count = count * 16 + (c - 55);
      }
   }
   return ptr;
}

/////////////////////////////////////////////////////////////////////////////////
//	sendVerify
/////////////////////////////////////////////////////////////////////////////////
void sendVerify (FT_HANDLE ftHandle, int ptr)
{
   DWORD BytesWritten,BytesReturn,RxBytes,TxBytes,EventWord; 

   Sleep (20);   

   FT_STATUS ftStatus = FT_GetStatus(ftHandle,&RxBytes,&TxBytes,&EventWord);

   char RxBuffer[15];

   if (RxBytes > 0)
   {
      ftStatus = FT_Read(ftHandle,RxBuffer,RxBytes,&BytesReturn);

      if(ftStatus != FT_OK)
      {
         fprintf(stderr, "Error: Receiving error during sendVerify.\n");
         //return -255;
      }

      for (int i = 0; i < RxBytes; i++)
      {
         if (program[ptr + i] != RxBuffer[i])
         {
            printf("Pattern mismatch: send: %2.2x received: %2.2x\n",program[ptr + i] & 0xff, RxBuffer[i] & 0xff);
         }
      }
   }
}

/////////////////////////////////////////////////////////////////////////////////
//	main
//	Original source: http://www.b-redemann.de/sp-DMM-auslesen.shtml
/////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
   // init
   FT_HANDLE ftHandle;
   FT_STATUS ftStatus;
   DWORD BytesWritten,BytesReturn,RxBytes,TxBytes,EventWord; 

   ftStatus = FT_Open(1, &ftHandle);
   if(ftStatus != FT_OK)
   {
      fprintf(stderr, "Error: Unable to open ftdi device.\n");
      return -255;
   }
   ftStatus = FT_SetBaudRate(ftHandle, 5000000); // Set baud rate to 115200 

   if(ftStatus != FT_OK)
   {
      fprintf(stderr, "Error: Unable to set baudrate.\n");
      return -255;
   }

   ftStatus = FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);

   if(ftStatus != FT_OK)
   {
     fprintf(stderr, "Error: Unable to set data characteristics.\n");
     return -255;
   }

   // check for default configuration
   char defaultConf = 0;
   char srbParam = 0;
   char sreParam = 0;
   if (argc == 1)
   {
      if ((hex = fopen("main_0.hex","r")) != NULL) defaultConf = 0x1;
      if ((hex = fopen("main_1.hex","r")) != NULL) defaultConf += 0x2;
      if ((hex = fopen("main_2.hex","r")) != NULL) defaultConf += 0x4;
      if ((hex = fopen("main_3.hex","r")) != NULL) defaultConf += 0x8;
      if (defaultConf > 0) {
         srbParam = 0x1f; // set reset of all cores 
         sreParam = 0x10; // clear reset of all cores
      }
   }

   // Set Reset At Beginning
   for (int i = 1; i < argc; i++)
   {
      if (strcmp("-srb", argv[i]) == 0)
      if (argc > ++i)
      {
         if (strcmp(argv[i], "0") == 0) srbParam = 0x10; 
         else if (strcmp(argv[i], "1") == 0) srbParam = 0x11; 
         else if (strcmp(argv[i], "2") == 0) srbParam = 0x12; 
         else if (strcmp(argv[i], "3") == 0) srbParam = 0x13; 
         else if (strcmp(argv[i], "4") == 0) srbParam = 0x14; 
         else if (strcmp(argv[i], "5") == 0) srbParam = 0x15; 
         else if (strcmp(argv[i], "6") == 0) srbParam = 0x16; 
         else if (strcmp(argv[i], "7") == 0) srbParam = 0x17; 
         else if (strcmp(argv[i], "8") == 0) srbParam = 0x18; 
         else if (strcmp(argv[i], "9") == 0) srbParam = 0x19; 
         else if (strcmp(argv[i], "a") == 0) srbParam = 0x1a; 
         else if (strcmp(argv[i], "b") == 0) srbParam = 0x1b; 
         else if (strcmp(argv[i], "c") == 0) srbParam = 0x1c; 
         else if (strcmp(argv[i], "d") == 0) srbParam = 0x1d; 
         else if (strcmp(argv[i], "e") == 0) srbParam = 0x1e; 
         else if (strcmp(argv[i], "f") == 0) srbParam = 0x1f; 
         else if (strcmp(argv[i], "A") == 0) srbParam = 0x1a; 
         else if (strcmp(argv[i], "B") == 0) srbParam = 0x1b; 
         else if (strcmp(argv[i], "C") == 0) srbParam = 0x1c; 
         else if (strcmp(argv[i], "D") == 0) srbParam = 0x1d; 
         else if (strcmp(argv[i], "E") == 0) srbParam = 0x1e; 
         else if (strcmp(argv[i], "F") == 0) srbParam = 0x1f; 
         if (srbParam == 0)
         {
            fprintf(stderr, "Error: Invalid given parameter for Set Reset At Beginning option (-srb).\n");
            return -255;
         }
      } else {
         fprintf(stderr, "Error: Missing parameter for Set Reset At Beginning option (-srb).\n");
         return -255;
      }
   }

   if (srbParam != 0) {   // 0x1x   	set reset
      printf("Set reset at beginning, core 0: %x, core 1: %x, core 2: %x, core 3: %x\n", srbParam & 0x1, (srbParam >> 1) & 0x1, (srbParam >> 2) & 0x1, (srbParam >> 3) & 0x1);
      char ToSend[1] = {srbParam};
      FT_Write(ftHandle,ToSend,1,&BytesWritten);
      Sleep (1);
   }

  // Loopback test  
   int lb = -1;
   for (int i = 1; i < argc; i++)
   {
      if (strcmp("-lb", argv[i]) == 0) {
         lb = 1;
         i = argc;
      }
   }

   if (lb == 1)
   {
      char ToSend1[1] = {0x20};
      FT_Write(ftHandle,ToSend1,1,&BytesWritten);
      Sleep (1);
      char ToSend2[1] = {0xA5};
      FT_Write(ftHandle,ToSend2,1,&BytesWritten);
      Sleep (1);

      Sleep (100);

      ftStatus = FT_GetStatus(ftHandle,&RxBytes,&TxBytes,&EventWord);  

      if (RxBytes != 1)
      {
         fprintf(stderr, "Error: Received %d bytes during loopack test, expecting 1.\n", RxBytes);
         return -255;
      }

      char RxBuffer[1];

      ftStatus = FT_Read(ftHandle,RxBuffer,RxBytes,&BytesReturn);

      if(ftStatus != FT_OK)
      {
         fprintf(stderr, "Error: Receiving data during loopback test.\n");
         return -255;
      }

      if ((RxBuffer[0] & 0xff) != 0xA5)
      {
         fprintf(stderr, "Error: Received invalid byte (0x%2X) during loopack test, expecting 0xA5.\n", RxBuffer[0] & 0xff);
         return -255;
       }

      printf("Loopback test successful.\n");
   }

   // Download to cores
   int ptr;

   if (defaultConf)
   {
      for (int c = 0; c < 4; c++)
      {
         if ((defaultConf >> c) & 0x1)
         {
            if (c == 0) hex = fopen("main_0.hex","r"); 
            if (c == 1) hex = fopen("main_1.hex","r"); 
            if (c == 2) hex = fopen("main_2.hex","r"); 
            if (c == 3) hex = fopen("main_3.hex","r"); 
            for (int tmpPtr = 0; tmpPtr < programSize; tmpPtr = tmpPtr + 4) 
            {
               program[tmpPtr + 0] = 0x13;
               program[tmpPtr + 1] = 0;
               program[tmpPtr + 2] = 0;
               program[tmpPtr + 3] = 0;
            }
            unsigned length = (readHex() - programOffset) / 256;
            if (length > 12) length = 12; 
               printf("Writing to core: %d, blocks: %d, start: %x, end: %x, file: main_%d.hex\n", c, length, programOffset, programOffset + length * 256 - 1, c);

            char ToSend1[1] = {0x30};
            FT_Write(ftHandle,ToSend1,1,&BytesWritten);
            Sleep (1);
            char ToSend2[1] = {c * 2};
            FT_Write(ftHandle,ToSend2,1,&BytesWritten);
            Sleep (1);
            char ToSend3[1] = {0x00};
            FT_Write(ftHandle,ToSend3,1,&BytesWritten);
            Sleep (1);
            char ToSend4[1] = {0x00};
            FT_Write(ftHandle,ToSend4,1,&BytesWritten);
            Sleep (1);
            char ToSend5[1] = {length - 1};
            FT_Write(ftHandle,ToSend5,1,&BytesWritten);
            Sleep (1);
            char ToSend6[1] = {256 - 1};
            FT_Write(ftHandle,ToSend6,1,&BytesWritten);
            Sleep (1); 
            for (int b = 0; b < length * 256; b = b + 4) 
            {
               char ToSend1[1] = {program[b]}; 
               FT_Write(ftHandle,ToSend1,1,&BytesWritten);
               Sleep (1); 
               char ToSend2[1] = {program[b + 1]}; 
               FT_Write(ftHandle,ToSend2,1,&BytesWritten);
               Sleep (1); 
               char ToSend3[1] = {program[b + 2]}; 
               FT_Write(ftHandle,ToSend3,1,&BytesWritten);
               Sleep (1); 
               char ToSend4[1] = {program[b + 3]}; 
               FT_Write(ftHandle,ToSend4,1,&BytesWritten);
               Sleep (1); 
               sendVerify(ftHandle, b);
            }
         }
      }
   } else
   for (int i = 1; i < argc; i++)
   {
      int c = -1;
      if (strcmp("-dc", argv[i]) == 0) 
      if (argc > ++i + 1)
      {
         if (strcmp(argv[i], "0") == 0) c = 0x0; 
         else if (strcmp(argv[i], "1") == 0) c = 0x1; 
         else if (strcmp(argv[i], "2") == 0) c = 0x2; 
         else if (strcmp(argv[i], "3") == 0) c = 0x3; 
         if (c > -1)
         {
            i++;
            if ((hex = fopen(argv[i],"r")) != NULL)
            {
               for (int tmpPtr = 0; tmpPtr < programSize; tmpPtr = tmpPtr + 4) 
               {
                  program[tmpPtr + 0] = 0x13;
                  program[tmpPtr + 1] = 0;
                  program[tmpPtr + 2] = 0;
                  program[tmpPtr + 3] = 0;
               }
               unsigned length = (readHex() - programOffset) / 256;
               if (length > 12) length = 12; 
               printf("Writing to core: %d, blocks: %d, file: %s\n", c, length, argv[i]);              

               char ToSend1[1] = {0x30};
               FT_Write(ftHandle,ToSend1,1,&BytesWritten);
               Sleep (1);
               char ToSend2[1] = {c * 2};
               FT_Write(ftHandle,ToSend2,1,&BytesWritten);
               Sleep (1);
               char ToSend3[1] = {0x00};
               FT_Write(ftHandle,ToSend3,1,&BytesWritten);
               Sleep (1);
               char ToSend4[1] = {0x00};
               FT_Write(ftHandle,ToSend4,1,&BytesWritten);
               Sleep (1);
               char ToSend5[1] = {length - 1};
               FT_Write(ftHandle,ToSend5,1,&BytesWritten);
               Sleep (1);
               char ToSend6[1] = {256 - 1};
               FT_Write(ftHandle,ToSend6,1,&BytesWritten);
               Sleep (1); 
               for (int b = 0; b < length * 256; b = b + 4) 
               {
                  char ToSend1[1] = {program[b]}; 
                  FT_Write(ftHandle,ToSend1,1,&BytesWritten);
                  Sleep (1); 
                  char ToSend2[1] = {program[b + 1]}; 
                  FT_Write(ftHandle,ToSend2,1,&BytesWritten);
                  Sleep (1); 
                  char ToSend3[1] = {program[b + 2]}; 
                  FT_Write(ftHandle,ToSend3,1,&BytesWritten);
                  Sleep (1); 
                  char ToSend4[1] = {program[b + 3]}; 
                  FT_Write(ftHandle,ToSend4,1,&BytesWritten);
                  Sleep (1); 
                  sendVerify(ftHandle, b + 0);
               }
            } else
            {
               fprintf(stderr, "Error: Download Core option (-dc), core %d, file does not exist: %s\n", c, argv[i]);
               return -255;
            }
         } else
         {
            fprintf(stderr, "Error: Invalid given core ID parameter for Downlaod Core option (-dc).\n");
            return -255;
         }
      } else {
         fprintf(stderr, "Error: Missing parameter for Downlaod Core option (-dc).\n");
         return -255;
      }
   }

   // Set Reset At End
   for (int i = 1; i < argc; i++)
   {
      if (strcmp("-sre", argv[i]) == 0)
      if (argc > ++i)
      {
         if (strcmp(argv[i], "0") == 0) sreParam = 0x10; 
         else if (strcmp(argv[i], "1") == 0) sreParam = 0x11; 
         else if (strcmp(argv[i], "2") == 0) sreParam = 0x12; 
         else if (strcmp(argv[i], "3") == 0) sreParam = 0x13; 
         else if (strcmp(argv[i], "4") == 0) sreParam = 0x14; 
         else if (strcmp(argv[i], "5") == 0) sreParam = 0x15; 
         else if (strcmp(argv[i], "6") == 0) sreParam = 0x16; 
         else if (strcmp(argv[i], "7") == 0) sreParam = 0x17; 
         else if (strcmp(argv[i], "8") == 0) sreParam = 0x18; 
         else if (strcmp(argv[i], "9") == 0) sreParam = 0x19; 
         else if (strcmp(argv[i], "a") == 0) sreParam = 0x1a; 
         else if (strcmp(argv[i], "b") == 0) sreParam = 0x1b; 
         else if (strcmp(argv[i], "c") == 0) sreParam = 0x1c; 
         else if (strcmp(argv[i], "d") == 0) sreParam = 0x1d; 
         else if (strcmp(argv[i], "e") == 0) sreParam = 0x1e; 
         else if (strcmp(argv[i], "f") == 0) sreParam = 0x1f; 
         else if (strcmp(argv[i], "A") == 0) sreParam = 0x1a; 
         else if (strcmp(argv[i], "B") == 0) sreParam = 0x1b; 
         else if (strcmp(argv[i], "C") == 0) sreParam = 0x1c; 
         else if (strcmp(argv[i], "D") == 0) sreParam = 0x1d; 
         else if (strcmp(argv[i], "E") == 0) sreParam = 0x1e; 
         else if (strcmp(argv[i], "F") == 0) sreParam = 0x1f; 
         if (sreParam == 0)
         {
            fprintf(stderr, "Error: Invalid given parameter for Set Reset At Beginning option (-srb).\n");
            return -255;
         }
      } else {
         fprintf(stderr, "Error: Missing parameter for Set Reset At Beginning option (-srb).\n");
         return -255;
      }
   }

   srbParam += defaultConf;

   if (sreParam != 0) {   // 0x1x   	set reset
      printf("Set reset at finish, core 0: %x, core 1: %x, core 2: %x, core 3: %x\n", sreParam & 0x1, (sreParam >> 1) & 0x1, (sreParam >> 2) & 0x1, (sreParam >> 3) & 0x1);
      char ToSend[1] = {sreParam};
      FT_Write(ftHandle,ToSend,1,&BytesWritten);
      Sleep (1);
   }

   // Finish
   FT_Close(ftHandle);

   fprintf(stdout, "Program downloadHex finished.\n");
   return 0;
}
