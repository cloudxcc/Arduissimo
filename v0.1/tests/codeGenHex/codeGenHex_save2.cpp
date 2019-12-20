//
//	simulate 
//		rand
//
//	datapath
//		reg
//			shift
//				TEQ, TST
//				SSAT, USAT
//				CMN, CMP
//
//		immediate
//			
//			expand
//				TEQ_imm, TST_imm
//				MOV_imm
//				MVN_imm
//				add flag check
//				
//			shift
//				ASR_imm
//				ROR_imm
//				
//		inside/outside it block
//				MVN
//				
//	--------------------------------------
//	IT
//
//	push/pop		(done)
//	branch
//		B		(m3_fde)
//		BL, BLx	(m3_fde)
//		Bx		(m3_fde)
//		CBNZ		(m3_fde)
//		CBZ		(m3_fde)
//
//	--------------------------------------
//	load/store/move/mvn
//
//		ldmdb
//		ldmea
//		ldmia			(m3_fde)
//		ldmfd
//		ldr (imm, lit, reg)
//		ldrb (imm, lit, reg)
//		ldrbt
//		ldrd (imm)
//		ldrex
//		ldrexb
//		ldrexh
//		ldrh (imm, lit, reg)
//		ldrht
//		ldrsb (imm, lit, reg)
//		ldrsbt
//		ldrsh (imm, lit, reg)
//		ldrsht
//		ldrt
//
//		stmdb
//		stmfd
//		stmia
//		stmea
//		str (imm, reg)
//		strb (imm, reg)
//		strbt
//		strd (imm)
//		strex
//		strexb
//		strexh
//		strh (imm, reg)
//		strht
//		strt
//
//		ADR	
//
//	------------------------------------------
//	generate "longer" sequential tests
//

#include <stdio.h>
//#include <stdint.h>

FILE *disasm = fopen("generate.disasm", "w");

void execute_instruction32(unsigned instruction);
void execute_instruction16(unsigned instruction);
void execute_add_reg_t3(unsigned rd, unsigned rn, unsigned rm, bool setflag, unsigned type, unsigned imm5);
void execute_mov_reg_t1(unsigned rd, unsigned rm);
void execute_addi32(unsigned rd, unsigned rs1, unsigned imm);

unsigned pc;
unsigned sp;
unsigned instruction_array [100000];
bool z, c, n, v;
bool z_valid, c_valid, n_valid, n_false_valid, v_valid;    

signed a, b;  
signed long long sx;
unsigned long long ux;

//////////////////////////////////////////////////////////////////////////////////////
//	init
//////////////////////////////////////////////////////////////////////////////////////
void init()
{
	fprintf(disasm, "write init\n");
	pc = 0x0;
	sp = 0x0; 
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
/*
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
	execute_instruction32(0x00000013); //          	lui	a5,0x0
*/
	execute_instruction32(0xfe010113); //          	addi	sp,sp,-32
	execute_instruction32(0x00812e23); //          	sw	s0,28(sp)
	execute_instruction32(0x02010413); //          	addi	s0,sp,32
	execute_instruction32(0x100127b7); //          	lui	a5,0x10012
	execute_instruction32(0x00c78793); //          	addi	a5,a5,12 # 1001200c <.L2+0x10011fb0>
	execute_instruction32(0xfef42623); //          	sw	a5,-20(s0)
	execute_instruction32(0x000007b7); //          	lui	a5,0x0
};

void write_failed ()
{
	fprintf(disasm, "\twrite failed\n");
	execute_instruction32(0xfec42783); //          	lw	a5,-20(s0)
	execute_instruction32(0x00080737); //          	lui	a4,0x80
	execute_instruction32(0x00e7a023); //          	sw	a4,0(a5)
}

void write_passed()
{
	fprintf(disasm, "write passed\n");
	execute_instruction32(0xfec42783); //          	lw	a5,-20(s0)
	execute_instruction32(0x00400737); //          	lui	a4,0x400
	execute_instruction32(0x00e7a023); //          	sw	a4,0(a5)
	//execute_instruction32(0x00078513); //          	mv	a0,a5
   	//execute_instruction32(0x01c12403); //          	lw	s0,28(sp)
	//execute_instruction32(0x02010113); //          	addi	sp,sp,32
	//execute_instruction32(0x00008067); //          	ret
};

void	execute_instruction32 (unsigned instruction)
{
	int ptr = (pc & 0x7fff) >> 1;

	fprintf(disasm, "\t%d    %x    %8.8x\n", ptr, pc, instruction);

	//printf("%d %x %8.8x\n", ptr, pc, instruction);
	instruction_array[ptr] = instruction & 0xffff;
	pc += 2;
	ptr = (pc & 0x7fff) >> 1;
	instruction_array[ptr] = instruction >> 16;
	pc += 2;
};

void	execute_instruction16(unsigned instruction)
{
	int ptr = (pc & 0x7fff) >> 1;

	fprintf(disasm, "\t%d    %x    %8.8x\n", ptr, pc, instruction);

	instruction_array[ptr] = instruction;
	//printf("%d %x %4.4x\n", ptr, pc, instruction);
	pc += 2;
};

void write_r_old(unsigned r, unsigned a)
{
	fprintf(disasm, "\twrite %d    %8.8x\n", r, a);
	//execute_instruction32(0x000007b7); //          	lui	a5,0x0
	
	unsigned instruction = 0x37;
	instruction += r << 7;
	instruction += ((a >> 12) & 0xfffff) << 12;
	execute_instruction32(instruction);

	//execute_instruction32(0x00100793); //          	li	a5,1
	instruction = 0x6013;
	instruction += r << 7;
	instruction += r << 15;
	instruction += (a & 0xfff) << 20;
	execute_instruction32(instruction);
};                                      

void write_r(unsigned r, unsigned a)
{
	unsigned instruction = 0x37;
	instruction += r << 7;       
	if (((a >> 11) & 0x1) == 0)
		instruction += ((a >> 12) & 0xfffff) << 12;
	else
		instruction += (((a >> 12) & 0xfffff) + 0x1) << 12;
	execute_instruction32(instruction);

	//execute_instruction32(0x00100793); //          	li	a5,1
	//instruction = 0x6013;
	//instruction += r << 7;
	//instruction += r << 15;
	//instruction += (a & 0xfff) << 20;
	//execute_instruction32(instruction);

	//execute_addi32(r, r, (a & 0xfff));
	instruction = 0x13;
	instruction += (r & 0x1F) << 7;
	instruction += (r & 0x1F) << 15;
	instruction += (a & 0xfff) << 20;
	execute_instruction32(instruction);

};

void check_rd(unsigned rd, unsigned result, unsigned rc)
{
	fprintf(disasm, "\tcheck %d (%d)   %8.8x\n", rd, rc, result);
	write_r(rc, result);

   	//801d4:	4290      	cmp	r0, r2
   	//801d6:	d006      	beq.n	801e6 <main+0x42>
	//unsigned instruction = 0x4280;
	//instruction += rd;
	//instruction += rc << 3;
	//execute_instruction16(instruction);

	unsigned instruction = 0xebb00f00;
	instruction += rd;
	instruction += rc << 16;
	execute_instruction32(instruction);

	execute_instruction16(0xd007);
	write_failed();
};


//////////////////////////////////////////////////////////////////////////////////////
//	end
////////////////////////////////////////////////////////////////////////////////////// 
#define atoa(x) #x    

#include <string.h>
void write_hex(int tc)
{              
	char filename [] = "generate_";
	char id [3];                             
	snprintf(id, 3, "%d", tc);
	strcat(filename, id);
	strcat(filename, ".hex");
	FILE *test = fopen(filename, "w");
	fprintf(test, "@00000000\n");

	int n = 0;
	for (n = 0x0; n < pc; n += 8)
	{
		unsigned checksum = 0x10;
		int ptr = n;

		unsigned instruction = instruction_array[ptr + 0];
		fprintf(test, "%2.2X ", instruction & 0xff);
		fprintf(test, "%2.2X ", (instruction >> 8) & 0xff);
 		checksum += ((instruction >> 8) & 0xff);
 		checksum += ((instruction >> 0) & 0xff);

		instruction = instruction_array[ptr + 1];
		fprintf(test, "%2.2X ", instruction & 0xff);
		fprintf(test, "%2.2X ", (instruction >> 8) & 0xff);
 		checksum += ((instruction >> 8) & 0xff);
 		checksum += ((instruction >> 0) & 0xff);

		instruction = instruction_array[ptr + 2];
		fprintf(test, "%2.2X ", instruction & 0xff);
		fprintf(test, "%2.2X ", (instruction >> 8) & 0xff);
 		checksum += ((instruction >> 8) & 0xff);
 		checksum += ((instruction >> 0) & 0xff);

		instruction = instruction_array[ptr + 3];
		fprintf(test, "%2.2X ", instruction & 0xff);
		fprintf(test, "%2.2X ", (instruction >> 8) & 0xff);
 		checksum += ((instruction >> 8) & 0xff);
 		checksum += ((instruction >> 0) & 0xff);

		instruction = instruction_array[ptr + 4];
		fprintf(test, "%2.2X ", instruction & 0xff);
		fprintf(test, "%2.2X ", (instruction >> 8) & 0xff);
 		checksum += ((instruction >> 8) & 0xff);
 		checksum += ((instruction >> 0) & 0xff);

		instruction = instruction_array[ptr + 5];
		fprintf(test, "%2.2X ", instruction & 0xff);
		fprintf(test, "%2.2X ", (instruction >> 8) & 0xff);
 		checksum += ((instruction >> 8) & 0xff);
 		checksum += ((instruction >> 0) & 0xff);

		instruction = instruction_array[ptr + 6];
		fprintf(test, "%2.2X ", instruction & 0xff);
		fprintf(test, "%2.2X ", (instruction >> 8) & 0xff);
 		checksum += ((instruction >> 8) & 0xff);
 		checksum += ((instruction >> 0) & 0xff);

		instruction = instruction_array[ptr + 7];
		fprintf(test, "%2.2X ", instruction & 0xff);
		fprintf(test, "%2.2X\n", (instruction >> 8) & 0xff);
 		checksum += ((instruction >> 8) & 0xff);
 		checksum += ((instruction >> 0) & 0xff);

		//fprintf(test, "%2.2X\n", ((0x100 - (checksum & 0xff))  & 0xff));
	}

	fclose(test);
}

//////////////////////////////////////////////////////////////////////////////////////
//	JAL32, ...
//////////////////////////////////////////////////////////////////////////////////////
void execute_jal32(unsigned rd, unsigned imm)
{
	unsigned instruction = 0x6F;
	instruction += (rd & 0x1F) << 7;
	instruction += ((imm & 0x0007ff) >> 1) << 21;
	instruction += ((imm & 0x000800) >> 11) << 20;
	instruction += ((imm & 0x0ff000) >> 12) << 12;
	instruction += ((imm & 0x100000) >> 20) << 31;
	execute_instruction32(instruction);
};

void execute_jalr32(unsigned rs1, unsigned rd, unsigned imm)
{
	unsigned instruction = 0x67;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rd & 0x1F) << 7;
	instruction += ((imm & 0xfff) >> 0) << 20;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	JAL16, ...
//////////////////////////////////////////////////////////////////////////////////////
void execute_j16(unsigned offset)
{
	unsigned instruction = 0xA001;
	//instruction += (r1 & 0x7) << 7;
	instruction += ((offset & 0x0000e) >>  1) << 3;
	instruction += ((offset & 0x00010) >>  4) << 11;
	instruction += ((offset & 0x00020) >>  5) << 2;
	instruction += ((offset & 0x00040) >>  6) << 7;
	instruction += ((offset & 0x00080) >>  7) << 6;
	instruction += ((offset & 0x00300) >>  8) << 9;
	instruction += ((offset & 0x00400) >> 10) << 8;
	instruction += ((offset & 0x00800) >> 11) << 12;
	execute_instruction16(instruction);
};
        
void execute_jal16(unsigned offset)
{
	unsigned instruction = 0x2001;
	//instruction += (r1 & 0x7) << 7;
	instruction += ((offset & 0x0000e) >>  1) << 3;
	instruction += ((offset & 0x00010) >>  4) << 11;
	instruction += ((offset & 0x00020) >>  5) << 2;
	instruction += ((offset & 0x00040) >>  6) << 7;
	instruction += ((offset & 0x00080) >>  7) << 6;
	instruction += ((offset & 0x00300) >>  8) << 9;
	instruction += ((offset & 0x00400) >> 10) << 8;
	instruction += ((offset & 0x00800) >> 11) << 12;
	execute_instruction16(instruction);
};
        
void execute_jr16(unsigned rs1, unsigned offset)
{
	unsigned instruction = 0x8002;
	instruction += (rs1 & 0x7) << 7;
	execute_instruction16(instruction);
};
        
void execute_jalr16(unsigned rs1, unsigned offset)
{
	unsigned instruction = 0x9002;
	instruction += (rs1 & 0x7) << 7;
	execute_instruction16(instruction);
};
        
//////////////////////////////////////////////////////////////////////////////////////
//	BEQ32, BNE32, ...
//////////////////////////////////////////////////////////////////////////////////////
void execute_beq32(unsigned rs1, unsigned rs2, unsigned imm)
{
	unsigned instruction = 0x63;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	instruction += (imm & 0x1000) << (31 - 12);
	instruction += (imm & 0x800) >> 4; // 11
	instruction += (imm & 0x7E0) << (25 - 5);
	instruction += (imm & 0x1E) << (7 - 0);
	execute_instruction32(instruction);
};

void execute_bne32(unsigned rs1, unsigned rs2, unsigned imm)
{
	unsigned instruction = 0x1063;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	instruction += (imm & 0x1000) << (31 - 12);
	instruction += (imm & 0x800) >> 4; // 11
	instruction += (imm & 0x7E0) << (25 - 5);
	instruction += (imm & 0x1E) << (7 - 0);
	execute_instruction32(instruction);
};

void execute_blt32(unsigned rs1, unsigned rs2, unsigned imm)
{
	unsigned instruction = 0x4063;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	instruction += (imm & 0x1000) << (31 - 12);
	instruction += (imm & 0x800) >> 4; // 11
	instruction += (imm & 0x7E0) << (25 - 5);
	instruction += (imm & 0x1E) << (7 - 0);
	execute_instruction32(instruction);
};

void execute_bltu32(unsigned rs1, unsigned rs2, unsigned imm)
{
	unsigned instruction = 0x6063;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	instruction += (imm & 0x1000) << (31 - 12);
	instruction += (imm & 0x800) >> 4; // 11
	instruction += (imm & 0x7E0) << (25 - 5);
	instruction += (imm & 0x1E) << (7 - 0);
	execute_instruction32(instruction);
};

void execute_bge32(unsigned rs1, unsigned rs2, unsigned imm)
{
	unsigned instruction = 0x5063;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	instruction += (imm & 0x1000) << (31 - 12);
	instruction += (imm & 0x800) >> 4; // 11
	instruction += (imm & 0x7E0) << (25 - 5);
	instruction += (imm & 0x1E) << (7 - 0);
	execute_instruction32(instruction);
};

void execute_bgeu32(unsigned rs1, unsigned rs2, unsigned imm)
{
	unsigned instruction = 0x7063;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	instruction += (imm & 0x1000) << (31 - 12);
	instruction += (imm & 0x800) >> 4; // 11
	instruction += (imm & 0x7E0) << (25 - 5);
	instruction += (imm & 0x1E) << (7 - 0);
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	BEQZ16, BNEZ16
//////////////////////////////////////////////////////////////////////////////////////
void execute_beqz16(unsigned r1, unsigned offset)
{
	unsigned instruction = 0xc001;
	instruction += (r1 & 0x7) << 7;
	instruction += ((offset & 0x6) >> 1) << 3;
	instruction += ((offset & 0x18) >> 3) << 10;
	instruction += ((offset & 0x2) >> 5) << 2;
	instruction += ((offset & 0xc0) >> 6) << 5;
	instruction += ((offset & 0x100) >> 8) << 12;
	execute_instruction16(instruction);
};
        
void execute_bnez16(unsigned r1, unsigned offset)
{
	unsigned instruction = 0xe001;
	instruction += (r1 & 0x7) << 7;
	instruction += ((offset & 0x6) >> 1) << 3;
	instruction += ((offset & 0x18) >> 3) << 10;
	instruction += ((offset & 0x2) >> 5) << 2;
	instruction += ((offset & 0xc0) >> 6) << 5;
	instruction += ((offset & 0x100) >> 8) << 12;
	execute_instruction16(instruction);
};
        
//////////////////////////////////////////////////////////////////////////////////////
//	ADDI32, ADD32, ADDI16
//////////////////////////////////////////////////////////////////////////////////////
void execute_addi32(unsigned rd, unsigned rs1, unsigned imm)
{
	unsigned instruction = 0x13;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += imm << 20;
	execute_instruction32(instruction);
};

void execute_add32(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned instruction = 0x33;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	execute_instruction32(instruction);
};

void execute_addi16(unsigned rd, unsigned imm)
{
	unsigned instruction = 0x0001;
	instruction += (rd & 0x1F) << 7;
	instruction += (imm & 0x1F) << 2;
	instruction += ((imm & 0x20) >> 0x5) << 12;
	//printf("%d %x\n", pc, instruction);
	execute_instruction16(instruction); 
};

void execute_addi16sp_old(unsigned imm)
{
	unsigned instruction = 0x6101;
	instruction += (imm & 0x1F) << 2;
	instruction += ((imm & 0x20) >> 0x5) << 12;
	//printf("%d %x\n", pc, instruction);
	execute_instruction16(instruction); 
};

void execute_addi16sp(unsigned imm)
{
	unsigned instruction = 0x6101;
	instruction += ((imm >> 0x9) & 0x1) << 12;
	instruction += ((imm >> 0x4) & 0x1) << 6;
	instruction += ((imm >> 0x6) & 0x1) << 5;
	instruction += ((imm >> 0x7) & 0x3) << 3;
	instruction += ((imm >> 0x5) & 0x1) << 2;
	//printf("%d %x\n", pc, instruction);
	execute_instruction16(instruction); 
};

void execute_addi4spn(unsigned rd, unsigned imm)
{
	unsigned instruction = 0x0;
	instruction += (rd & 0x7) << 2;
	instruction += ((imm & 0x30) >> 0x4) << 11;
	instruction += ((imm & 0x3C0) >> 0x6) << 7;
	instruction += ((imm & 0x4) >> 0x2) << 6;
	instruction += ((imm & 0x8) >> 0x5) << 5;
	//printf("%d %x\n", pc, instruction);
	execute_instruction16(instruction); 
};

void execute_add16(unsigned rd, unsigned r2)
{
	unsigned instruction = 0x9002;
	instruction += (rd & 0x1F) << 7;
	instruction += (r2 & 0x1F) << 2;
	execute_instruction16(instruction); 
};

//////////////////////////////////////////////////////////////////////////////////////
//	SUB
//////////////////////////////////////////////////////////////////////////////////////
void execute_sub32(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned instruction = 0x40000033;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	execute_instruction32(instruction);
};

void execute_sub16(unsigned rd, unsigned rs2)
{
	unsigned instruction = 0x8C01;
	instruction += (rd & 0x7) << 7;
	instruction += (rs2 & 0x7) << 2;
	execute_instruction16(instruction);
};
        
//////////////////////////////////////////////////////////////////////////////////////
//	SLT
//////////////////////////////////////////////////////////////////////////////////////
void execute_slti32(unsigned rd, unsigned rs1, unsigned imm)
{
	unsigned instruction = 0x2013;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += imm << 20;
	execute_instruction32(instruction);
};

void execute_sltiu32(unsigned rd, unsigned rs1, unsigned imm)
{
	unsigned instruction = 0x3013;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += imm << 20;
	execute_instruction32(instruction);
};                             

void execute_slt32(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned instruction = 0x2033;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	execute_instruction32(instruction);
};

void execute_sltu32(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned instruction = 0x3033;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	execute_instruction32(instruction);
};

void execute_sll32(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned instruction = 0x1033;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	execute_instruction32(instruction);
};

void execute_srl32(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned instruction = 0x5033;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	execute_instruction32(instruction);
};

void execute_sra32(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned instruction = 0x40005033;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	SLLI32
//////////////////////////////////////////////////////////////////////////////////////
void execute_slli32(unsigned rd, unsigned rs1, unsigned shamt)
{
	unsigned instruction = 0x1013;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (shamt & 0x1F) << 20;
	execute_instruction32(instruction);
};

void execute_srli32(unsigned rd, unsigned rs1, unsigned shamt)
{
	unsigned instruction = 0x5013;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (shamt & 0x1F) << 20;
	execute_instruction32(instruction);
};

void execute_srai32(unsigned rd, unsigned rs1, unsigned shamt)
{
	unsigned instruction = 0x40005013;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (shamt & 0x1F) << 20;
	execute_instruction32(instruction);
};

void execute_slli16(unsigned r1d, unsigned shamt)
{
	unsigned instruction = 0x0002;
	instruction += (r1d & 0x7) << 7;
	instruction += (shamt & 0x1f) << 2;
	instruction += ((shamt & 0x20) >> 5) << 12;
	execute_instruction16(instruction);
};
        
void execute_srli16(unsigned r1d, unsigned shamt)
{
	unsigned instruction = 0x8001;
	instruction += (r1d & 0x7) << 7;
	instruction += (shamt & 0x1f) << 2;
	instruction += ((shamt & 0x20) >> 5) << 12;
	execute_instruction16(instruction);
};
        
void execute_srai16(unsigned r1d, unsigned shamt)
{
	unsigned instruction = 0x8401;
	instruction += (r1d & 0x7) << 7;
	instruction += (shamt & 0x1f) << 2;
	instruction += ((shamt & 0x20) >> 5) << 12;
	execute_instruction16(instruction);
};
        
/////////////////////////////////////////////////////////////////////////////////////
//	MUL
//////////////////////////////////////////////////////////////////////////////////////
void execute_mul(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned instruction = 0x02000033;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	execute_instruction32(instruction);
};

void execute_mulh(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned instruction = 0x02001033;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	execute_instruction32(instruction);
};

void execute_mulhu(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned instruction = 0x02003033;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	execute_instruction32(instruction);
};

void execute_mulhsu(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned instruction = 0x02002033;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	DIV
//////////////////////////////////////////////////////////////////////////////////////
void execute_div(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned instruction = 0x02004033;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	execute_instruction32(instruction);
};

void execute_divu(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned instruction = 0x02005033;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	execute_instruction32(instruction);
};

void execute_rem(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned instruction = 0x02006033;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	execute_instruction32(instruction);
};

void execute_remu(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned instruction = 0x02007033;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	COMB
//////////////////////////////////////////////////////////////////////////////////////
void execute_andi32(unsigned rd, unsigned rs1, unsigned imm)
{
	unsigned instruction = 0x7013;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += imm << 20;
	execute_instruction32(instruction);
};

void execute_and32(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned instruction = 0x7033;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	execute_instruction32(instruction);
};

void execute_ori32(unsigned rd, unsigned rs1, unsigned imm)
{
	unsigned instruction = 0x6013;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += imm << 20;
	execute_instruction32(instruction);
};

void execute_or32(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned instruction = 0x6033;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	execute_instruction32(instruction);
};

void execute_xori32(unsigned rd, unsigned rs1, unsigned imm)
{
	unsigned instruction = 0x4013;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += imm << 20;
	execute_instruction32(instruction);
};

void execute_xor32(unsigned rd, unsigned rs1, unsigned rs2)
{
	unsigned instruction = 0x4033;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	execute_instruction32(instruction);
};

void execute_andi16(unsigned rd, unsigned imm)
{
	unsigned instruction = 0x8801;
	instruction += (rd & 0x7) << 7;
	instruction += (imm & 0x1F) << 2;
	instruction += ((imm & 0x20) >> 5) << 12;
	execute_instruction16(instruction);
};

void execute_and16(unsigned rd, unsigned rs2)
{
	unsigned instruction = 0x8C61;
	instruction += (rd & 0x7) << 7;
	instruction += (rs2 & 0x7) << 2;
	execute_instruction16(instruction);
};

void execute_or16(unsigned rd, unsigned rs2)
{
	unsigned instruction = 0x8C41;
	instruction += (rd & 0x7) << 7;
	instruction += (rs2 & 0x7) << 2;
	execute_instruction16(instruction);
};

void execute_xor16(unsigned rd, unsigned rs2)
{
	unsigned instruction = 0x8C21;
	instruction += (rd & 0x7) << 7;
	instruction += (rs2 & 0x7) << 2;
	execute_instruction16(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	Store Load
//////////////////////////////////////////////////////////////////////////////////////
void execute_auipc32(unsigned rd, unsigned imm)
{
	unsigned instruction = 0x17;
	instruction += (rd & 0x1F) << 7;
	instruction += ((imm & 0xFFFFF) >> 0) << 12;
	execute_instruction32(instruction);
};

void execute_lw32(unsigned rd, unsigned rs1, unsigned imm)
{
	unsigned instruction = 0x2003;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += ((imm & 0xFFF) >> 0) << 20;
	execute_instruction32(instruction);
};

void execute_lh32(unsigned rd, unsigned rs1, unsigned imm)
{
	unsigned instruction = 0x1003;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += ((imm & 0xFFF) >> 0) << 20;
	execute_instruction32(instruction);
};

void execute_lhu32(unsigned rd, unsigned rs1, unsigned imm)
{
	unsigned instruction = 0x5003;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += ((imm & 0xFFF) >> 0) << 20;
	execute_instruction32(instruction);
};

void execute_lb32(unsigned rd, unsigned rs1, unsigned imm)
{
	unsigned instruction = 0x0003;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += ((imm & 0xFFF) >> 0) << 20;
	execute_instruction32(instruction);
};

void execute_lbu32(unsigned rd, unsigned rs1, unsigned imm)
{
	unsigned instruction = 0x4003;
	instruction += (rd & 0x1F) << 7;
	instruction += (rs1 & 0x1F) << 15;
	instruction += ((imm & 0xFFF) >> 0) << 20;
	execute_instruction32(instruction);
};

void execute_sb32(unsigned rs1, unsigned rs2, unsigned imm)
{
	unsigned instruction = 0x0023;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	instruction += ((imm & 0x1F) >> 0) << 7;
	instruction += ((imm & 0xFE0) >> 5) << 25;
	execute_instruction32(instruction);
};

void execute_sh32(unsigned rs1, unsigned rs2, unsigned imm)
{
	unsigned instruction = 0x1023;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	instruction += ((imm & 0x1F) >> 0) << 7;
	instruction += ((imm & 0xFE0) >> 5) << 25;
	execute_instruction32(instruction);
};

void execute_sw32(unsigned rs1, unsigned rs2, unsigned imm)
{
	unsigned instruction = 0x2023;
	instruction += (rs1 & 0x1F) << 15;
	instruction += (rs2 & 0x1F) << 20;
	instruction += ((imm & 0x1F) >> 0) << 7;
	instruction += ((imm & 0xFE0) >> 5) << 25;
	execute_instruction32(instruction);
};

void execute_lw16(unsigned rd, unsigned rs1, unsigned imm)
{
	unsigned instruction = 0x4000;
	instruction += (rd & 0x7) << 2;
	instruction += (rs1 & 0x7) << 7;
	instruction += ((imm & 0x4) >> 2) << 6;
	instruction += ((imm & 0x38) >> 3) << 10;
	instruction += ((imm & 0x40) >> 6) << 5;
	execute_instruction16(instruction);
};

void execute_sw16(unsigned rs1, unsigned rs2, unsigned imm)
{
	unsigned instruction = 0xc000;
	instruction += (rs2 & 0x7) << 2;
	instruction += (rs1 & 0x7) << 7;
	instruction += ((imm & 0x4) >> 2) << 6;
	instruction += ((imm & 0x38) >> 3) << 10;
	instruction += ((imm & 0x40) >> 6) << 5;
	execute_instruction16(instruction);
};

void execute_lwsp16(unsigned rd, unsigned imm)
{
	unsigned instruction = 0x4002;
	instruction += (rd & 0x1F) << 7;
	instruction += ((imm & 0x1c) >> 2) << 4;
	instruction += ((imm & 0x20) >> 5) << 12;
	instruction += ((imm & 0xc0) >> 6) << 2;
	execute_instruction16(instruction);
};

void execute_swsp16(unsigned rs2, unsigned imm)
{
	unsigned instruction = 0xc002;
	instruction += (rs2 & 0x1F) << 2;
	instruction += ((imm & 0x3c) >> 2) << 9;
	instruction += ((imm & 0xc0) >> 6) << 7;
	execute_instruction16(instruction);
};

void execute_li16(unsigned rd, unsigned imm)
{
	unsigned instruction = 0x4001;
	instruction += (rd & 0x7) << 7;
	instruction += ((imm & 0x1f) >> 0) << 2;
	instruction += ((imm & 0x20) >> 5) << 12;
	execute_instruction16(instruction);
};

void execute_lui16(unsigned rd, unsigned imm)
{
	unsigned instruction = 0x6001;
	instruction += (rd & 0x7) << 7;
	instruction += ((imm & 0x1f) >> 0) << 2;
	instruction += ((imm & 0x20) >> 5) << 12;
	execute_instruction16(instruction);
};

void execute_mv16(unsigned rd, unsigned rs2)
{
	unsigned instruction = 0x8002;
	instruction += (rd & 0x1f) << 7;
	instruction += (rs2 & 0x1f) << 2;
	execute_instruction16(instruction);
};


//////////////////////////////////////////////////////////////////////////////////////
//	run_test_jump 32
//////////////////////////////////////////////////////////////////////////////////////
void run_test_jump32()
{
	fprintf(disasm, "-----------------------------------------\ntest jal32");
    write_r(14, 0xf);
    execute_addi32(14, 14, 0x10);
    write_r(15, 0x1f);    
    
	execute_jal32(14, 28);
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	write_failed();                
    write_r(15, 0xb4);
	execute_beq32(14, 15, 16);
	write_failed();
	
/*  moved to '16 because it doesn't work in pulp
    write_r(13, 0xec);
	execute_jalr32(13, 14, 28);
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	write_failed();                
    write_r(15, 0xb4);
	execute_beq32(14, 15, 16);
	write_failed();
*/	
}

void run_test_jump32_tmp()
{
	fprintf(disasm, "-----------------------------------------\ntest jal32");
    write_r(14, 0xf);
    execute_addi32(14, 14, 0x10);
    write_r(15, 0x1f); 
    
	execute_jal32(14, 28);
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	write_failed();                
    write_r(15, 0xb4);
	execute_beq32(14, 15, 16);
	write_failed();
	
    write_r(13, 0xec);
	execute_jalr32(13, 14, 28);
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	write_failed();                
    write_r(15, 0xb4);
	execute_beq32(14, 15, 16);
	write_failed();

}

//////////////////////////////////////////////////////////////////////////////////////
//	run_test_jump 16
//////////////////////////////////////////////////////////////////////////////////////
void run_test_jump16()
{
	fprintf(disasm, "-----------------------------------------\ntest jalr32 cont\n");
	
    write_r(13, 0xf8);
	execute_jalr32(13, 14, 28);
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	write_failed();                
    write_r(15, 0xfc);
	execute_beq32(14, 15, 16);
	write_failed();
	
	fprintf(disasm, "-----------------------------------------\ntest j16\n");
	
	execute_j16(26);
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	write_failed();                
	
	fprintf(disasm, "-----------------------------------------\ntest jal16\n");
	
	execute_jal16(26);
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	write_failed();                
    write_r(5, 0x148);
	execute_beq32(1, 5, 16);
	write_failed();
	
	fprintf(disasm, "-----------------------------------------\ntest jr16\n");
	
    write_r(3, 0x19a);
	execute_jr16(3, 26);
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	write_failed();                
	
	fprintf(disasm, "-----------------------------------------\ntest jalr16\n");
	
    write_r(3, 0x1bc);
	execute_jalr16(3, 26);
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	write_failed();                
    write_r(5, 0x1a4);
	execute_beq32(1, 5, 16);
	write_failed();
	
	

}
//////////////////////////////////////////////////////////////////////////////////////
//	run_test_add 32
//////////////////////////////////////////////////////////////////////////////////////
void run_test_add32()
{
	fprintf(disasm, "-----------------------------------------\ntest addi32: f + 10 = 1f;\n");
    write_r(14, 0xf);
    execute_addi32(14, 14, 0x10);
    write_r(15, 0x1f);
	execute_beq32(14, 15, 28); //pc + 2);
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	execute_instruction32(0x00000013); //          	nop
	write_failed();                
	
	fprintf(disasm, "-----------------------------------------\ntest addi32: f + f00 = ffff.ff0f;\n");
    write_r(14, 0xf);
    execute_addi32(14, 14, 0xf00);
    write_r(15, 0xffffff0f);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest addi32: (-2) + (-2) = (-4);\n");
    write_r(14, 0xFFFFFFFE);
    execute_addi32(14, 14, 0xFFE);
    write_r(15, 0xFFFFFFFC);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest addi32: 2 + (-2) = 0;\n");
    write_r(14, 0x00000002);
    execute_addi32(14, 14, 0xFFE);
    write_r(15, 0x00000000);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest addi32: 2 + (-4) = -2;\n");
    write_r(14, 0x00000002);
    execute_addi32(14, 14, 0xFFC);
    write_r(15, 0xFFFFFFFE);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest add32: f + 10 = 1f;\n");
    write_r(13, 0xf);
    write_r(14, 0x10);
    execute_add32(14, 14, 13);
    write_r(15, 0x1f);
	execute_beq32(14, 15, 16);
	write_failed();
}

//////////////////////////////////////////////////////////////////////////////////////
//	run_test_add 16
//////////////////////////////////////////////////////////////////////////////////////
void run_test_add16()
{

	fprintf(disasm, "-----------------------------------------\ntest addi16: f + 10 = 1f;\n");
    write_r(14, 0xf);
    execute_addi16(14, 0x10);
    write_r(15, 0x1f);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest addi16: f + 20 = ffffffef;\n");
    write_r(14, 0xf);
    execute_addi16(14, 0x20);
    write_r(15, 0xffffffef);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest addi16sp: f + 10 = 1f;\n");
    write_r(2, 0xf);
    execute_addi16sp(0x1f0);
    write_r(15, 0x1ff);
	execute_beq32(2, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest addi16sp: f + 20 = ffffffef;\n");
    write_r(2, 0x0f);
    execute_addi16sp(0x3f0);
    write_r(15, 0xffffffff);
	execute_beq32(2, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest addi4spn: f + 10 = 1f;\n");
    write_r(2, 0x7);
    execute_addi4spn(5, 0x10);
    write_r(15, 0x17);
	execute_beq32(13, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest add16: f + 10 = 1f;\n");
    write_r(13, 0xf0);
    write_r(14, 0xf);
    execute_add16(14, 13);
    write_r(15, 0xff);
	execute_beq32(14, 15, 16);
	write_failed();

}

//////////////////////////////////////////////////////////////////////////////////////
//	run_test_comb 32
//////////////////////////////////////////////////////////////////////////////////////
void run_test_comb32()
{
	fprintf(disasm, "-----------------------------------------\ntest andi32: ffffffff & 33 = 33;\n");
    write_r(14, 0xffffffff);
    execute_andi32(14, 14, 0x33);
    write_r(15, 0x33);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest andi32: ffffffff & fffff833 = fffff833;\n");
    write_r(14, 0xffffffff);
    execute_andi32(14, 14, 0x833);
    write_r(15, 0xfffff833);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest ori32: f | 33 = 3f;\n");
    write_r(14, 0xf);
    execute_ori32(14, 14, 0x33);
    write_r(15, 0x3f);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest ori32: f | 833 = fffff83f;\n");
    write_r(14, 0xf);
    execute_ori32(14, 14, 0x833);
    write_r(15, 0xfffff83f);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest xori32: f ^ 33 = 3C;\n");
    write_r(14, 0xf);
    execute_xori32(14, 14, 0x33);
    write_r(15, 0x3C);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest xori32: f ^ 833 = fffff83C;\n");
    write_r(14, 0xf);
    execute_xori32(14, 14, 0x833);
    write_r(15, 0xfffff83C);
	execute_beq32(14, 15, 16);
	write_failed();
 


	fprintf(disasm, "-----------------------------------------\ntest and32: f & 33 = 3;\n");
    write_r(13, 0x33);
    write_r(14, 0xf);
    execute_and32(14, 14, 13);
    write_r(15, 0x3);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest or32: f | 33 = 3f;\n");
    write_r(13, 0x33);
    write_r(14, 0xf);
    execute_or32(14, 14, 13);
    write_r(15, 0x3f);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest xor32: f ^ 33 = 3C;\n");
    write_r(13, 0x33);
    write_r(14, 0xf);
    execute_xor32(14, 14, 13);
    write_r(15, 0x3C);
	execute_beq32(14, 15, 16);
	write_failed();

}

//////////////////////////////////////////////////////////////////////////////////////
//	run_test_comb 16
//////////////////////////////////////////////////////////////////////////////////////
void run_test_comb16()
{
	fprintf(disasm, "-----------------------------------------\ntest andi16: ffffffff & 13 = 13;\n");
    write_r(12, 0xffffffff);
    execute_andi16(4, 0x13);
    write_r(5, 0x13);
	execute_beq32(12, 5, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest andi16: ffffffff & 33 = fffffff3;\n");
    write_r(12, 0xffffffff);
    execute_andi16(4, 0x33);
    write_r(5, 0xfffffff3);
	execute_beq32(12, 5, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest and16: f & 33 = 3;\n");
    write_r(12, 0xf);
    write_r(11, 0x33);
    execute_and16(4, 3);
    write_r(5, 0x3);
	execute_beq32(12, 5, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest or16: f | 33 = 3f;\n");
    write_r(12, 0xf);
    write_r(11, 0x33);
    execute_or16(4, 3);
    write_r(5, 0x3f);
	execute_beq32(12, 5, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest xor16: f ^ 33 = 3C;\n");
    write_r(12, 0xf);
    write_r(11, 0x33);
    execute_xor16(4, 3);
    write_r(5, 0x3C);
	execute_beq32(12, 5, 16);
	write_failed();

}

//////////////////////////////////////////////////////////////////////////////////////
//	run_test_sub 32
//////////////////////////////////////////////////////////////////////////////////////
void run_test_sub32()
{
	fprintf(disasm, "-----------------------------------------\ntest sub32: 1f - f = 10;\n");
    write_r(13, 0xf);
    write_r(14, 0x1f);
    execute_sub32(14, 14, 13);
    write_r(15, 0x10);
	execute_beq32(14, 15, 16);
	write_failed();

}

//////////////////////////////////////////////////////////////////////////////////////
//	run_test_sub 16
//////////////////////////////////////////////////////////////////////////////////////
void run_test_sub16()
{
	fprintf(disasm, "-----------------------------------------\ntest sub16: f + 10 = 1f;\n");
    write_r(11, 0xf);
    write_r(12, 0x1f);
    execute_sub16(4, 3);
    write_r(5, 0x10);
	execute_beq32(12, 5, 16);
	write_failed();

}
//////////////////////////////////////////////////////////////////////////////////////
//	run_test_shift 32
//////////////////////////////////////////////////////////////////////////////////////
void run_test_shift32()
{
	fprintf(disasm, "-----------------------------------------\ntest slli32: affeaffe << 4 = ffeaffe0;\n");
    write_r(14, 0xaffeaffe);
    execute_slli32(14, 14, 0x4);
    write_r(15, 0xffeaffe0);
	execute_beq32(14, 15, 16);
	write_failed();                
	
	fprintf(disasm, "-----------------------------------------\ntest srli32: f000affe >> 4 = 0f000aff;\n");
    write_r(14, 0xf000affe);
    execute_srli32(14, 14, 0x4);
    write_r(15, 0x0f000aff);         // logical: upper bits 0
	execute_beq32(14, 15, 16);
	write_failed();                

	fprintf(disasm, "-----------------------------------------\ntest srli32: f000affe >> 3 = ;\n");
    write_r(14, 0xf000affe);
    execute_srli32(14, 14, 0x3);
    write_r(15, 0x1e0015ff);         // logical: upper bits 0
	execute_beq32(14, 15, 16);
	write_failed();                

	fprintf(disasm, "-----------------------------------------\ntest srli32: f000affe >> 0 = f000affe;\n");
    write_r(14, 0xf000affe);
    execute_srli32(14, 14, 0x0);
    write_r(15, 0xf000affe);         // logical: upper bits 0
    //write_r(15, 0x0f000aff);         // logical: upper bits 0
	execute_beq32(14, 15, 16);
	write_failed();                

	fprintf(disasm, "-----------------------------------------\ntest srai32: affe >> 4 = aff;\n");
    write_r(14, 0x0000affe);
    execute_srai32(14, 14, 0x4);
    write_r(15, 0x00000aff);        // arith: upper bits 0
	execute_beq32(14, 15, 16);
	write_failed();                

	fprintf(disasm, "-----------------------------------------\ntest srai32: f000affe >> 4 = ff000aff;\n");
    write_r(14, 0xf000affe);
    execute_srai32(14, 14, 0x4);
    write_r(15, 0xff000aff);        // arith: upper bits 1
	execute_beq32(14, 15, 16);
	write_failed();                




	fprintf(disasm, "-----------------------------------------\ntest sll32: affe << 4 = aff;\n");
    write_r(14, 0xaffeaffe);
    write_r(13, 0x4);
    execute_sll32(14, 14, 13);
    write_r(15, 0xffeaffe0);
	execute_beq32(14, 15, 16);
	write_failed();                

	fprintf(disasm, "-----------------------------------------\ntest srl32: affe >> 4 = aff;\n");
    write_r(14, 0xf000affe);
    write_r(13, 0x4);
    execute_srl32(14, 14, 13);
    write_r(15, 0x0f000aff);         // logical: upper bits 0
	execute_beq32(14, 15, 16);
	write_failed();                

	fprintf(disasm, "-----------------------------------------\ntest sra32: affe >> 4 = aff;\n");
    write_r(14, 0xaffe);
    write_r(13, 0x4);
    execute_sra32(14, 14, 13);
    write_r(15, 0x00000aff);        // arith: upper bits 0
	execute_beq32(14, 15, 16);
	write_failed();                

	fprintf(disasm, "-----------------------------------------\ntest sra32: f000affe >> 4 = ff000aff;\n");
    write_r(14, 0xf000affe);
    write_r(13, 0x4);
    execute_sra32(14, 14, 13);
    write_r(15, 0xff000aff);        // arith: upper bits 1
	execute_beq32(14, 15, 16);
	write_failed();                

}	

//////////////////////////////////////////////////////////////////////////////////////
//	run_test_shift 16
//////////////////////////////////////////////////////////////////////////////////////
void run_test_shift16()
{
	fprintf(disasm, "-----------------------------------------\ntest slli16: f + 10 = 1f;\n");
    write_r(4, 0xaffeaffe);
    execute_slli16(4, 4);
    write_r(5, 0xffeaffe0);
	execute_beq32(4, 5, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest srli16: f + 10 = 1f;\n");
    write_r(12, 0xf000affe);
    execute_srli16(4, 4);
    write_r(5, 0x0f000aff);         // logical: upper bits 0
	execute_beq32(12, 5, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest srai16: f + 10 = 1f;\n");
    write_r(12, 0xaffe);
    execute_srai16(4, 4);
    write_r(5, 0x00000aff);         // arith: upper bits 0
	execute_beq32(12, 5, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest srai16: f + 10 = 1f;\n");
    write_r(12, 0xf000affe);
    execute_srai16(4, 4);
    write_r(5, 0xff000aff);         // arith: upper bits 1
	execute_beq32(12, 5, 16);
	write_failed();

}

//////////////////////////////////////////////////////////////////////////////////////
//	run_test_mul
//////////////////////////////////////////////////////////////////////////////////////
void run_test_mul()
{
	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mul: 5 * 3 = f;\n");
    write_r(13, 0x5);
    write_r(14, 0x3);
    execute_mul(14, 14, 13);
    write_r(15, 0xf);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mul: -5 * 3 = -f;\n");
    write_r(13, 0xfffffffb);
    write_r(14, 0x3);
    execute_mul(14, 14, 13);
    write_r(15, 0xfffffff1);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mul: 5 * -3 = -f;\n");
    write_r(13, 0x5);
    write_r(14, 0xfffffffd);
    execute_mul(14, 14, 13);
    write_r(15, 0xfffffff1);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mul: -5 * -3 = f;\n");
    write_r(13, 0xfffffffb);
    write_r(14, 0xfffffffd);
    execute_mul(14, 14, 13);
    write_r(15, 0xf);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mul: 12345 * 2 = 2468A;\n");
    write_r(13, 0x12345);
    write_r(14, 0x2);
    execute_mul(14, 14, 13);
    write_r(15, 0x2468A);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mul: 12345 * 10002 = 2347468A;\n");
    write_r(13, 0x12345);
    write_r(14, 0x00010002);
    execute_mul(14, 14, 13);
    write_r(15, 0x2347468A);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mul: 12345 * 10002 = 2347468A;\n");
    write_r(13, 0x00010002);
    write_r(14, 0x12345);
    execute_mul(14, 14, 13);
    write_r(15, 0x2347468A);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mulhu: 50000000h * 30000000h = 0f000000; carry = 0;\n");
    a = 0x50000000 >> 16;
    b = 0x30000000 >> 16;
    sx = (a * b);
    printf("mulhu 1: %X\n", sx);

    write_r(13, 0x50000000);
    write_r(14, 0x30000000);
    execute_mulhu(14, 14, 13);
    write_r(15, 0x0f000000);
	execute_beq32(14, 15, 16);
	write_failed(); 

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mulhu: 18000h * 18000h = 2; carry = 1;\n");
    a = 0x8000;
    b = 0x8000;
    sx = ((a * b) >> 16) + a + b;
    printf("mulhu 2: %X\n", sx); 
    
    write_r(13, 0x18000);
    write_r(14, 0x18000);
    execute_mulhu(14, 14, 13);
    write_r(15, 0x00000002);
	execute_beq32(14, 15, 16);
	write_failed(); 

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mulhu: 1F000h * 1F000h = 3; carry = 2;\n");
    a = 0xF0;
    b = 0xF0;
    sx = ((a * b) >> 8) + a + b;
    printf("mulhu 3: %X\n", sx); 
    
    write_r(13, 0x1F000);
    write_r(14, 0x1F000);
    execute_mulhu(14, 14, 13);
    write_r(15, 0x00000003);
	execute_beq32(14, 15, 16);
	write_failed(); 

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mulshu: 50000000h * 30000000h = 0f000000; no sign; carry = 0; neg_carry = 0;\n");
    write_r(13, 0x50000000);
    write_r(14, 0x30000000);
    execute_mulhsu(14, 14, 13);
    write_r(15, 0x0f000000);
	execute_beq32(14, 15, 16);
	write_failed(); 

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mulshu: b0...h * fd0...h = df0...; sign; carry = 0; neg_carry = 0;\n");
    a = 0xd0000000 >> 16 | 0xffff0000;
    b = 0xb0000000 >> 16; // | 0xffff0000;
    sx = (a * b);
    printf("mulhsu 1: %X\n", sx);

    write_r(13, 0xb0000000); 
    write_r(14, 0xd0000000);
    execute_mulhsu(14, 14, 13);
    write_r(15, 0xDF000000);
	execute_beq32(14, 15, 16);
	write_failed(); 

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mulshu: d0...h * fb0...h = bf0...; sign; carry = 0; neg_carry = 0;\n");
    a = 0xd0000000 >> 16; // | 0xffff0000;
    b = 0xb0000000 >> 16 | 0xffff0000;
    sx = (a * b);
    printf("mulhsu 2: %X\n", sx);

    write_r(13, 0xd0000000); 
    write_r(14, 0xb0000000);
    execute_mulhsu(14, 14, 13);
    write_r(15, 0xBF000000);
	execute_beq32(14, 15, 16);
	write_failed(); 

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mulshu: d0..1h * fb0..1h = bf0...; sign; carry = 0; neg_carry = 0;\n");
    a = 0xd0100000 >> 16; // | 0xffff0000;
    b = 0xb0100000 >> 16 | 0xffff0000;
    sx = (a * b);
    printf("mulhsu 2: %X\n", sx);
    a = 0xd0010000 >> 16; // | 0xffff0000;
    b = 0xb0010000 >> 16 | 0xffff0000;
    sx = (a * b);
    printf("mulhsu 2: %X\n", sx);
    //printf("mulhsu 2: %X\n", 0xd0000001);
    //printf("mulhsu 2: %X -> \n", 0xb0000001, 0x4fffffff);
    printf("mulhsu 2: b * d ........0000ffff\n");
    printf("mulhsu 2: a * d ....cfff3000....\n");
    printf("mulhsu 2: b * c ....00004fff....\n");
    printf("mulhsu 2: a * c 40ff3000........\n");
    printf("mulhsu 2: sum   40ffffff7fffffff carry = 0;\n");
    printf("mulhsu 2: sum   bf00000080000000\n");
    
    //printf("mulhsu 2: %X\n", 0xb + 0xd);

    write_r(13, 0xd0000001); 
    write_r(14, 0xb0000001);
    execute_mulhsu(14, 14, 13);
    write_r(15, 0xBF000000);
	execute_beq32(14, 15, 16);
	write_failed(); 

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mulhsu: 18000h * 18000h = 2; no sign; carry = 1; neg_carry = 0;\n");
    a = 0x8000;
    b = 0x8000;
    sx = ((a * b) >> 16) + a + b;
    printf("mulhsu 3: %X\n", sx); 
    
    write_r(13, 0x18000);                  	// carry data	
    write_r(14, 0x18000);                   // carry neg
    execute_mulhsu(14, 14, 13);
    write_r(15, 0x00000002);
	execute_beq32(14, 15, 16);
	write_failed(); 

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mulhsu: 1F000h * 1F000h = 3; no sign; carry = 2; neg_carry = 0;\n");
    a = 0xF0;
    b = 0xF0;
    sx = ((a * b) >> 8) + a + b;
    printf("mulhu 5: %X\n", sx); 
    
    write_r(13, 0x1F000);
    write_r(14, 0x1F000);
    execute_mulhsu(14, 14, 13);
    write_r(15, 0x00000003);
	execute_beq32(14, 15, 16);
	write_failed(); 

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mulh: 50000000h * 30000000h = 0x0f000000xxxxxxxx;\n");
    write_r(13, 0x50000000);
    write_r(14, 0x30000000);
    execute_mulh(14, 14, 13);
    write_r(15, 0x0f000000);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mulh: -5 * 30000000h = d0000000xxxxxxxx;\n");
    write_r(13, 0xfffffffb);
    write_r(14, 0x30000000);
    write_r(15, 0xffffffff);
    execute_mulh(14, 14, 13);
    //write_r(15, 0xd0000000);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mulh: 30000000h * -5 = d0000000xxxxxxxx;\n");
    write_r(14, 0xfffffffb);
    write_r(13, 0x30000000);
    execute_mulh(14, 14, 13);
    //write_r(15, 0xd0000000);
    write_r(15, 0xffffffff);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mulh: d0000000h * b0000000h = 0x0f000000xxxxxxxx;\n");
    a = 0xd0000000 >> 16 | 0xffff0000;
    b = 0xb0000000 >> 16 | 0xffff0000;
    sx = (a * b);
    printf("mulh 1: %X\n", sx);

    write_r(14, 0xd0000000);
    write_r(13, 0xb0000000);
    execute_mulh(14, 14, 13);
    write_r(15, 0x0f000000);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mulh: 80000001h * 80000001h = 0x40000001xxxxxxxx;\n");
    write_r(13, 0x80000001);
    write_r(14, 0x80000001);
    execute_mulhu(14, 14, 13);
    write_r(15, 0x40000001);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mul: 77777777h * 20000 = EEEE0000;\n");
	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mulh: 77777777h * 20000 = EEEE;\n");
    write_r(11, 0x77777777);
    write_r(12, 0x20000);
    execute_mul(13, 12, 11);
    execute_mulh(14, 12, 11);
    write_r(15, 0xEEEE0000);
	execute_beq32(13, 15, 16);
	write_failed(); 
    write_r(15, 0xEEEE);
	execute_beq32(14, 15, 16);
	write_failed();

/*
	fprintf(disasm, "-----------------------------------------\n");
	fprintf(disasm, "test mulhsu: -50000000h * 30000000h = 0f000000;\n");
    write_r(13, 0xb0000000);
    write_r(14, 0xd0000000);
    execute_mulhsu(14, 14, 13);
    write_r(15, 0x21000000);
	execute_beq32(14, 15, 16);
	write_failed();
*/

}
//////////////////////////////////////////////////////////////////////////////////////
//	run_test_div
//////////////////////////////////////////////////////////////////////////////////////
void run_test_div()
{
	fprintf(disasm, "-----------------------------------------\ntest div: f / 4 = 3;\n");
    write_r(13, 0x4);
    write_r(14, 0xf);
    execute_div(14, 14, 13);
    write_r(15, 0x3);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest div: -f / 4 = -3;\n");
    write_r(13, 0x4);
    write_r(14, 0xfffffff1);
    execute_div(14, 14, 13);
    write_r(15, 0xfffffffd);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest div: f / -4 = -3;\n");
    write_r(13, 0xfffffffc);
    write_r(14, 0xf);
    execute_div(14, 14, 13);
    write_r(15, 0xfffffffd);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest div: -f / -4 = 3;\n");
    write_r(13, 0xfffffffc);
    write_r(14, 0xfffffff1);
    execute_div(14, 14, 13);
    write_r(15, 0x3);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest div: affe / 0 = -1;\n");
    write_r(13, 0x00000000);
    write_r(14, 0x0000affe);
    execute_div(14, 14, 13);
    write_r(15, 0xffffffff);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest div: 80..00 / -1 = 80..00;\n");
    write_r(13, 0xffffffff);
    write_r(14, 0x80000000);
    execute_div(14, 14, 13);
    write_r(15, 0x80000000);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest divu: f / 4 = 3;\n");
    write_r(13, 0x4);
    write_r(14, 0xf);
    execute_divu(14, 14, 13);
    write_r(15, 0x3);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest divu: -f / 4 = -3;\n");
    write_r(13, 0x4);
    write_r(14, 0xfffffff1);
    execute_divu(14, 14, 13);
    write_r(15, 0x3ffffffc);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest divu: f / ffc = -3;\n");
    write_r(13, 0xfffffffc);
    write_r(14, 0xf);
    execute_divu(14, 14, 13);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest divu: f1 / fc = 3;\n");
    write_r(13, 0xfffffffc);
    write_r(14, 0xfffffff1);
    execute_divu(14, 14, 13);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest divu: affe / 0 = ff..ff;\n");
    write_r(13, 0x00000000);
    write_r(14, 0x0000affe);
    execute_divu(14, 14, 13);
    write_r(15, 0xffffffff);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest rem: ff rem 9 = 3;\n");
    write_r(13, 0x9);
    write_r(14, 0xff);
    execute_rem(14, 14, 13);
    write_r(15, 0x3);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest rem: 4 rem 4 = 0;\n");
    write_r(13, 0x4);
    write_r(14, 0x4);
    execute_rem(14, 14, 13);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest rem: -ff / 9 = -3;\n");
    write_r(13, 0x9);
    write_r(14, 0xffffff01);
    execute_rem(14, 14, 13);
    write_r(15, 0xfffffffd);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest rem: ff / -4 = -3;\n");
    write_r(13, 0xfffffff7);
    write_r(14, 0xff);
    execute_rem(14, 14, 13);
    write_r(15, 0x3);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest rem: -ff / -9 = 3;\n");
    write_r(13, 0xfffffff7);
    write_r(14, 0xffffff01);
    execute_rem(14, 14, 13);
    write_r(15, 0xfffffffd);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest rem: affe / 0 = affe;\n");
    write_r(13, 0x00000000);
    write_r(14, 0x0000affe);
    execute_rem(14, 14, 13);
    write_r(15, 0x0000affe);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest rem: 80..00 / -1 = 0;\n");
    write_r(13, 0xffffffff);
    write_r(14, 0x80000000);
    execute_rem(14, 14, 13);
    write_r(15, 0x00000000);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest renu: ff / 9 = 3;\n");
    write_r(13, 0x9);
    write_r(14, 0xff);
    execute_remu(14, 14, 13);
    write_r(15, 0x3);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest remu: -ff / 9 = -3;\n");
    write_r(13, 0x9);
    write_r(14, 0xffffff01);
    execute_remu(14, 14, 13);
    write_r(15, 0x1);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest remu: ff / -9 = -3;\n");
    write_r(13, 0xfffffff7);
    write_r(14, 0xff);
    execute_remu(14, 14, 13);
    write_r(15, 0xff);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest remu: -ff / -9 = 3;\n");
    write_r(13, 0xfffffff7);
    write_r(14, 0xffffff01);
    execute_remu(14, 14, 13);
    write_r(15, 0xffffff01);
	execute_beq32(14, 15, 16);
	write_failed();   

	fprintf(disasm, "-----------------------------------------\ntest remu: affe / 0 = affe;\n");
    write_r(13, 0x00000000);
    write_r(14, 0x0000affe);
    execute_remu(14, 14, 13);
    write_r(15, 0x0000affe);
	execute_beq32(14, 15, 16);
	write_failed();

}
//////////////////////////////////////////////////////////////////////////////////////
//	run_test_lesser
//////////////////////////////////////////////////////////////////////////////////////
void run_test_lesser()
{
	fprintf(disasm, "-----------------------------------------\ntest slti32: ff < ff => 0;\n");
    write_r(14, 0xff);
    execute_slti32(14, 14, 0xff);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slti32: ff < f => 0;\n");
    write_r(14, 0xff);
    execute_slti32(14, 14, 0xf);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slti32: ff < -ff0 => 0;\n");
    write_r(14, 0x0ff);
    execute_slti32(14, 14, 0xff0);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();
                                                        
	fprintf(disasm, "-----------------------------------------\ntest slti32: ff < -f00 => 0;\n");
    write_r(14, 0x0ff);
    execute_slti32(14, 14, 0xf00);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();
                                                        
	fprintf(disasm, "-----------------------------------------\ntest slti32: f < ff => 1;\n");
    write_r(14, 0x0f);
    execute_slti32(14, 14, 0xff);
    write_r(15, 0x1);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slti32: f < f => 0;\n");
    write_r(14, 0x0f);
    execute_slti32(14, 14, 0xf);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slti32: f < -ff0 => 0;\n");
    write_r(14, 0x0f);
    execute_slti32(14, 14, 0xf);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slti32: f < -f00 => 0;\n");
    write_r(14, 0x0f);
    execute_slti32(14, 14, 0xf);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slti32: -ff0 < ff => 1;\n");
    write_r(14, 0xfffffff0);
    execute_slti32(14, 14, 0xff);
    write_r(15, 0x1);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slti32: -ff0 < f => 1;\n");
    write_r(14, 0xfffffff0);
    execute_slti32(14, 14, 0xf);
    write_r(15, 0x1);
	execute_beq32(14, 15, 16);
	write_failed();


	fprintf(disasm, "-----------------------------------------\ntest slti32: -ff0 < -ff0 => 0;\n");
    write_r(14, 0xfffffff0);
    execute_slti32(14, 14, 0xff0);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slti32: -ff0 < -f00 => 0;\n");
    write_r(14, 0xfffffff0);
    execute_slti32(14, 14, 0xf00);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slti32: -f00 < ff => 1;\n");
    write_r(14, 0xffffff00);
    execute_slti32(14, 14, 0xff);
    write_r(15, 0x1);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slti32: -f00 < f => 1;\n");
    write_r(14, 0xffffff00);
    execute_slti32(14, 14, 0xf);
    write_r(15, 0x1);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slti32: -f00 < -ff0 => 1;\n");
    write_r(14, 0xffffff00);
    execute_slti32(14, 14, 0xff0);
    write_r(15, 0x1);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slti32: -f00 < -f00 => 0;\n");
    write_r(14, 0xffffff00);
    execute_slti32(14, 14, 0xf00);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();



	fprintf(disasm, "-----------------------------------------\ntest sltiu32: f < 1f => 1;\n");
    write_r(14, 0x0f);
    execute_sltiu32(14, 14, 0x1f);
    write_r(15, 0x1);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest sltiu32: fff < f => 0;\n");
    write_r(14, 0xfff);
    execute_sltiu32(14, 14, 0xf);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest sltiu32: fff < fffffff0 => 1;\n");
    write_r(14, 0xfff);
    execute_sltiu32(14, 14, 0xff0);
    write_r(15, 0x1);
	execute_beq32(14, 15, 16);
	write_failed();




	fprintf(disasm, "-----------------------------------------\ntest slt32: ff < ff => 0;\n");
    write_r(14, 0xff);
    write_r(13, 0xff);
    execute_slt32(14, 14, 13);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slt32: ff < f => 0;\n");
    write_r(14, 0xff);
    write_r(13, 0xf);
    execute_slt32(14, 14, 13);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slt32: ff < -ff0 => 0;\n");
    write_r(14, 0xff);
    write_r(13, 0xfffffff0);
    execute_slt32(14, 14, 13);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slt32: ff < -f00 => 0;\n");
    write_r(14, 0xff);
    write_r(13, 0xffffff00);
    execute_slt32(14, 14, 13);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();
                             
	fprintf(disasm, "-----------------------------------------\ntest slt32: f < ff => 1;\n");
    write_r(14, 0x0f);
    write_r(13, 0xff);
    execute_slt32(14, 14, 13);
    write_r(15, 0x1);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slt32: f < f => 0;\n");
    write_r(14, 0xf);
    write_r(13, 0xf);
    execute_slt32(14, 14, 13);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slt32: f < -ff0 => 0;\n");
    write_r(14, 0xf);
    write_r(13, 0xfffffff0);
    execute_slt32(14, 14, 13);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slt32: f < -f00 => 0;\n");
    write_r(14, 0xf);
    write_r(13, 0xffffff00);
    execute_slt32(14, 14, 13);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slt32: -ff0 < ff => 1;\n");
    write_r(14, 0xfffffff0);
    write_r(13, 0xff);
    execute_slt32(14, 14, 13);
    write_r(15, 0x1);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slt32: -ff0 < f => 1;\n");
    write_r(14, 0xfffffff0);
    write_r(13, 0xf);
    execute_slt32(14, 14, 13);
    write_r(15, 0x1);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slt32: -ff0 < -ff0 => 0;\n");
    write_r(14, 0xfffffff0);
    write_r(13, 0xfffffff0);
    execute_slt32(14, 14, 13);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slt32: -ff0 < -f00 => 0;\n");
    write_r(14, 0xfffffff0);
    write_r(13, 0xffffff00);
    execute_slt32(14, 14, 13);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slt32: -f00 < ff => 1;\n");
    write_r(14, 0xffffff00);
    write_r(13, 0xff);
    execute_slt32(14, 14, 13);
    write_r(15, 0x1);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slt32: -f00 < f => 1;\n");
    write_r(14, 0xffffff00);
    write_r(13, 0xf);
    execute_slt32(14, 14, 13);
    write_r(15, 0x1);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slt32: -f00 < -ff0 => 0;\n");
    write_r(14, 0xffffff00);
    write_r(13, 0xfffffff0);
    execute_slt32(14, 14, 13);
    write_r(15, 0x1);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slt32: -f00 < -f00 => 0;\n");
    write_r(14, 0xffffff00);
    write_r(13, 0xffffff00);
    execute_slt32(14, 14, 13);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();





	fprintf(disasm, "-----------------------------------------\ntest sltu32: f < 1f => 1;\n");
    write_r(13, 0x1f);
    write_r(14, 0x0f);
    execute_sltu32(14, 14, 13);
    write_r(15, 0x1);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest sltu32: 1f < f => 0;\n");
    write_r(13, 0xf);
    write_r(14, 0x1f);
    execute_sltu32(14, 14, 13);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

}

//////////////////////////////////////////////////////////////////////////////////////
//	run_test_branch 32
//////////////////////////////////////////////////////////////////////////////////////
void run_test_branch32()
{
	fprintf(disasm, "-----------------------------------------\ntest beq32: 1 = 1 => branch;\n");
    write_r(14, 0x0f);
    execute_slti32(14, 14, 0x1f);
    write_r(15, 0x1);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest bne32: f != 1 => branch;\n");
    write_r(14, 0xf);
    write_r(15, 0x1);
	execute_bne32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest blt32: 1 < f => branch;\n");
    write_r(14, 0x1);
    write_r(15, 0xf);
	execute_blt32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest blt32: -1 < f => branch;\n");
    write_r(14, 0xffffffff);
    write_r(15, 0xf);
	execute_blt32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest blt32: -f < -1 => branch;\n");
    write_r(14, 0xfffffff1);
    write_r(15, 0xffffffff);
	execute_blt32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest bge32: f == f => branch;\n");
    write_r(14, 0xf);
    write_r(15, 0xf);
	execute_bge32(14, 15, 16);
	write_failed();
                                                                                            
	fprintf(disasm, "-----------------------------------------\ntest bge32: -f == -f => branch;\n");
    write_r(14, 0xfffffff1);
    write_r(15, 0xfffffff1);
	execute_bge32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest bge32: f > 1 => branch;\n");
    write_r(14, 0xf);
    write_r(15, 0x1);
	execute_bge32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest bge32: f > -1 => branch;\n");
    write_r(14, 0xf);
    write_r(15, 0xffffffff);
	execute_bge32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest bge32: -1 > -f => branch;\n");
    write_r(14, 0xffffffff);
    write_r(15, 0xfffffffe);
	execute_bge32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slti32: f < 1f => 1;\n");
    write_r(14, 0x1);
    write_r(15, 0xf);
	execute_bltu32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slti32: f < 1f => 1;\n");
    write_r(14, 0xf);
    write_r(15, 0xf);
	execute_bgeu32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest slti32: f < 1f => 1;\n");
    write_r(14, 0xf);
    write_r(15, 0x1);
	execute_bgeu32(14, 15, 16);
	write_failed();    

}

//////////////////////////////////////////////////////////////////////////////////////
//	run_test_branch 16
//////////////////////////////////////////////////////////////////////////////////////
void run_test_branch16()
{
	fprintf(disasm, "-----------------------------------------\ntest beqz16: 0 = 1 => branch;\n");
    write_r(12, 0x0);
	execute_beqz16(4, 14);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest bnez16: f != 0 => branch;\n");
    write_r(12, 0xf);
	execute_bnez16(4, 14);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest auipc32\n");
    execute_auipc32(14, 0x1f);
    write_r(15, 0x0001f278);
	execute_beq32(14, 15, 16);
	write_failed();

}

//////////////////////////////////////////////////////////////////////////////////////
//	run_test_storeload 32
//////////////////////////////////////////////////////////////////////////////////////
void run_test_storeload32()
{
	fprintf(disasm, "-----------------------------------------\ntest sw32 lw32\n");
    write_r(12, 0x5);
    write_r(13, 0x0);
    execute_sw32(12, 13, 0x1f);
    execute_lw32(14, 12, 0x1f);
    write_r(15, 0x0);
	execute_beq32(14, 15, 16);
	write_failed();

    write_r(12, 0x1);
    write_r(13, 0x0affe);
    execute_sw32(12, 13, 0x1f);
    execute_lw32(14, 12, 0x1f);
    write_r(15, 0xaffe);
	execute_beq32(14, 15, 16);
	write_failed();

    write_r(12, 0x2);
    write_r(13, 0x0affe);
    execute_sw32(12, 13, 0x1f);
    execute_lw32(14, 12, 0x1f);
    write_r(15, 0xaffe);
	execute_beq32(14, 15, 16);
	write_failed();

    write_r(12, 0x3);
    write_r(13, 0x0affe);
    execute_sw32(12, 13, 0x1f);
    execute_lw32(14, 12, 0x1f);
    write_r(15, 0xaffe);
	execute_beq32(14, 15, 16);
	write_failed();

    write_r(12, 0x4);
    write_r(13, 0x0affe);
    execute_sw32(12, 13, 0x1f);
    execute_lw32(14, 12, 0x1f);
    write_r(15, 0xaffe);
	execute_beq32(14, 15, 16);
	write_failed();

    write_r(12, 0x1);
    execute_lw32(14, 12, 0x1f);
    write_r(15, 0xfefefefe);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest lh32\n");
    write_r(12, 0x5);
    write_r(13, 0xdeadbeaf);
    execute_sw32(12, 13, 0x1f);
    write_r(12, 0x1);
    write_r(13, 0xdeadbeaf);
    execute_sw32(12, 13, 0x1f);

    write_r(12, 0x1);
    execute_lh32(14, 12, 0x1f);
    write_r(15, 0xffffbeaf);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest lhu32\n");
    execute_lhu32(14, 12, 0x1f);
    write_r(15, 0x0000beaf);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest lb32\n");
    write_r(12, 0x1);
    execute_lb32(14, 12, 0x1f);
    write_r(15, 0xffffffaf);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest lbu32\n");
    execute_lbu32(14, 12, 0x1f);
    write_r(15, 0x000000af);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest sb32\n");
    write_r(12, 0x1);
    write_r(13, 0x00000000);
    execute_sb32(12, 13, 0x1f);
    execute_lw32(14, 12, 0x1f);
    write_r(15, 0xdeadbe00);
	execute_beq32(14, 15, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest sb32\n");
    write_r(12, 0x1);
    write_r(13, 0x00000000);
    execute_sh32(12, 13, 0x1f);
    execute_lw32(14, 12, 0x1f);
    write_r(15, 0xdead0000);
	execute_beq32(14, 15, 16);
	write_failed();

}

//////////////////////////////////////////////////////////////////////////////////////
//	run_test_storeload 16
//////////////////////////////////////////////////////////////////////////////////////
void run_test_storeload16()
{
	fprintf(disasm, "-----------------------------------------\ntest sw16 lw16\n");
    write_r(10, 0x5);
    write_r(11, 0xdeadaffe);
    execute_sw16(2, 3, 0x1f);
    execute_lw16(4, 2, 0x1f);
    write_r(5, 0xdeadaffe);
	execute_beq32(12, 5, 16);
	write_failed();

    write_r(10, 0x1);
    write_r(11, 0x0affedead);
    execute_sw16(2, 3, 0x1f);
    execute_lw16(4, 2, 0x1f);
    write_r(5, 0xaffedead);
	execute_beq32(12, 5, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest sw16sp lw16sp\n");
    write_r(2, 0x5);
    write_r(3, 0x01234567);
    execute_swsp16(3, 0x1f);
    execute_lwsp16(4, 0x1f);
    write_r(5, 0x01234567);
	execute_beq32(4, 5, 16);
	write_failed();
}
//////////////////////////////////////////////////////////////////////////////////////
//	run_test_load 16
//////////////////////////////////////////////////////////////////////////////////////
void run_test_load16()
{                   
	fprintf(disasm, "-----------------------------------------\ntest li16 \n");
    execute_li16(4, 0x1A);
    write_r(5, 0x1A);
	execute_beq32(4, 5, 16);
	write_failed();

    execute_li16(4, 0x3A);
    write_r(5, 0xfffffffA);
	execute_beq32(4, 5, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest lui16 \n");
    execute_lui16(4, 0x1A);
    write_r(5, 0x1A000);
	execute_beq32(4, 5, 16);
	write_failed();

    execute_lui16(4, 0x3A);
    write_r(5, 0xffffA000);
	execute_beq32(4, 5, 16);
	write_failed();

	fprintf(disasm, "-----------------------------------------\ntest mv16 \n");
    //execute_li16(4, 0x1A);
    write_r(4, 0xaffe);
    execute_mv16(5, 4);
	execute_beq32(4, 5, 16);
	write_failed();

}
//////////////////////////////////////////////////////////////////////////////////////
//	regression
//////////////////////////////////////////////////////////////////////////////////////
void run_regression(int testcase)
{   
	if (testcase == 0)
	{          
		run_test_add32();
		run_test_sub32();
		run_test_comb32(); 
		    
		write_passed();

		run_test_add16();
		run_test_sub16();
		run_test_comb16();     
	}
	if (testcase == 1)
	{          
		run_test_shift32();     
		run_test_mul();     
		run_test_div();     
		write_passed();
		run_test_shift16();     
	}
	if (testcase == 2)
	{          
		run_test_branch32();
		write_passed();
		run_test_branch16();
	}
	if (testcase == 3)
	{          
		run_test_lesser();     
		write_passed();
	}
	if (testcase == 4)
	{          
		run_test_storeload32();
		write_passed();
		run_test_storeload16();
		run_test_load16();
	}
	if (testcase == 5)
	{          
		run_test_jump32();
		write_passed();
		run_test_jump16();
	}
}

int main ()
{                                         
	for (int tc = 0; tc <= 5; tc += 1)
	{
		fprintf(disasm, "-----------------------------------------\n");
		fprintf(disasm, "testcase: %d\n", tc);
		fprintf(disasm, "-----------------------------------------\n");
		init();                           
		run_regression(tc);
		write_passed();
		write_hex(tc);
	}
	fclose(disasm);
}