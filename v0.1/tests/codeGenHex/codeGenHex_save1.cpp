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
//	check
//////////////////////////////////////////////////////////////////////////////////////
void check_z(bool set)
{
	fprintf(disasm, "\tcheck z %d\n", set);
	if (set)
	{
		execute_instruction16(0xd007);
	} else
	{
		execute_instruction16(0xd107);
	}
	write_failed();
};

void check_n(bool set)
{
	fprintf(disasm, "\tcheck n %d\n", set);
	if (set)
	{
		execute_instruction16(0xd407);
	} else
	{
		execute_instruction16(0xd507);
	}
	write_failed();
};

void check_c(bool set)
{
	fprintf(disasm, "\tcheck c %d\n", set);
	if (set)
	{
		execute_instruction16(0xd207);
	} else
	{
		execute_instruction16(0xd307);
	}
	write_failed();
};

void check_v(bool set)
{
	fprintf(disasm, "\tcheck v %d\n", set);
	if (set)
	{
		execute_instruction16(0xd607);
	} else
	{
		execute_instruction16(0xd707);
	}
	write_failed();
};

void check_flag (bool setflag)
{
	fprintf(disasm, "\tcheck flags setflag: %d\n", setflag);
	if (setflag)
	{
		if (c_valid) check_c(c);
		if (z_valid) check_z(z);
		if (n_valid) check_n(n);
		if (v_valid) check_v(v);
	} else
	{
		if (c_valid) check_c(!c);
		if (z_valid) check_z(!z);
		if (n_false_valid) check_n(!n);
	//	if (v_valid) check_v(!v);
	}
};

//////////////////////////////////////////////////////////////////////////////////////
//	set
//////////////////////////////////////////////////////////////////////////////////////
void set_flags_init()
{
	fprintf(disasm, "-----------------------------------------\nset_flags_init:\n");
	write_r(8, 0x0);
	write_r(9, 0x1);
	write_r(10, 0x2);
	write_r(7, 0xFFFFFFFF);
};

void set_flags (bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	fprintf(disasm, "\tset flags %d\n");
	z = this_z;
	c = this_c;
	n = this_n;
	v = this_v;

	z_valid = set_z;
	c_valid = set_c;
	n_valid = set_n;
      n_false_valid = set_n;
	v_valid = set_v;

	if (!c)		// carry
	{
		if (!z)	// carry zero:		FF + 1
		{
			execute_add_reg_t3(12, 7, 9, true, 0, 0);
			n_false_valid = false;
		} else
		if (!n)	// carry negative		FF + FF
		{
			execute_add_reg_t3(12, 7, 7, true, 0, 0);
		} else	// carry positive		FF + 2
		{
			execute_add_reg_t3(12, 7, 10, true, 0, 0);
		}
	} else
	{
		if (!z)	// no-carry zero		0
		{
			execute_add_reg_t3(12, 8, 8, true, 0, 0);
			n_false_valid = false;
		} else
		if (!n)	// no-carry negative	FF
		{
			execute_add_reg_t3(12, 7, 8, true, 0, 0);
		} else	// no-carry positive	1
		{
			execute_add_reg_t3(12, 8, 9, true, 0, 0);
		}
	}
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
//	ADC
//////////////////////////////////////////////////////////////////////////////////////
void execute_adc_reg_t1 (unsigned rdn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_adc_reg_t1: [%d] = [%d] + [%d] + c\n", rdn, rdn, rm);
	unsigned instruction = 0x4140;
	instruction += rdn & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_adc_reg_t2 (unsigned rd, unsigned rn, unsigned rm, bool setflag, unsigned type, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_adc_reg_t2: [%d] = [%d] + [%d] setflag: %d\n", rd, rn, rm, setflag);
	unsigned instruction = 0xeb400000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += type << 4;
	instruction += (imm5 & 0x3) << 6;
	instruction += (imm5 & 0x1C) << 10;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_adc_reg (int rn, int rm, int rd, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v,
                bool preset_c)
{
	// missing: DN

	fprintf(disasm, "-----------------------------------------\ntest_adc_reg:\n");
	unsigned result = a + b;
      if (preset_c) result += 1;

	set_flags(set_c, !preset_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	c = this_c;
	write_r(rd, a);
	execute_adc_reg_t1(rd, rm);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, !preset_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	c = this_c;
	execute_adc_reg_t2(rd, rn, rm, true, 0, 0);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, !preset_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	c = !preset_c; // this is not nec. true, but check_flag tests the inverse preset
	execute_adc_reg_t2(rd, rn, rm, false, 0, 0);
	check_flag(false);
	check_rd(rd, result, 3);
}

void test_adc_dis_reg (int rn, int rm, int rd, unsigned a, unsigned b, bool this_c, unsigned type, unsigned imm5)
{
	// missing: carry

	fprintf(disasm, "-----------------------------------------\ntest_adc_dis_reg:\n");
	unsigned result = a + b;

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
	execute_adc_reg_t2(rd, rn, rm, false, type, imm5);
//	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
//	c = !this_c;
	execute_adc_reg_t2(rd, rn, rm, true, type, imm5);
//	check_flag(true);
	check_rd(rd, result, 3);

}

void execute_adc_imm_t1 (unsigned rd, unsigned rn, bool setflag, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_adc_imm_t1: [%d] = [%d] + %x setflag: %d\n", rd, rn, imm8, setflag);
	unsigned instruction = 0xf1400000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += i << 26;
	instruction += imm3 << 12;
	instruction += imm8;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_adc_imm (int rn, int rd, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v,
                bool preset_c)
{
	fprintf(disasm, "-----------------------------------------\ntest_adc_imm:\n");
	unsigned result = a + (b & 0xff);
      if (preset_c) result += 1;
	if (result > 0)
		set_z = false;
	
	write_r(rd, ~result);
	set_flags(set_c, !preset_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	c = this_c;
	execute_adc_imm_t1(rd, rn, true, 0, 0, (b & 0xff));
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, !preset_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	c = !preset_c; // this is not nec. true, but check_flag tests the inverse preset
	execute_adc_imm_t1(rd, rn, false, 0, 0, (b & 0xff));
	check_flag(false);
	check_rd(rd, result, 3);
}

void test_adc_tei_imm (int rn, int rd, unsigned a, unsigned b, bool setflag, unsigned i, unsigned imm3, unsigned imm8, bool preset_c)
{
	bool set_c = true;
      bool this_c = false;
      bool set_z = false;
      bool this_z = false;
      bool set_n = false;
      bool this_n = false;
      bool set_v = false;
      bool this_v = false;

	fprintf(disasm, "-----------------------------------------\ntest_adc_tei_imm :\n");
	unsigned result = a + b;
      if (preset_c) result += 1;

	write_r(rd, ~result);
	set_flags(set_c, (!preset_c), set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_adc_imm_t1(rd, rn, false, i, imm3, imm8);
//	check_flag(false);
	check_rd(rd, result, 3);

//	write_r(rd, ~result);
//	set_flags(set_c, true, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
//	c = this_c;
//	execute_and_imm_t1(rd, rn, true, i, imm3, imm8);
//	check_flag(true);
//	check_rd(rd, result, 3);

}

//////////////////////////////////////////////////////////////////////////////////////
//	ADD
//////////////////////////////////////////////////////////////////////////////////////
void execute_add_reg_t1( unsigned rd, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_add_reg_t1: [%d] = [%d] + [%d]\n", rd, rn, rm);
	unsigned instruction = 0x1800;
	instruction += rd & 0x7;
	instruction += (rn & 0x7) << 3;
	instruction += (rm & 0x7) << 6;
	execute_instruction16(instruction);
};

void execute_add_sp_reg_t1 (unsigned rdm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_add_sp_reg_t1: [%d] = sp + [%d]\n", rdm, rdm);
	unsigned instruction = 0x4468;
	instruction += rdm & 0x7;
	execute_instruction16(instruction);
};

void execute_add_reg_t2 (unsigned rdn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_add_reg_t2: [%d] = [%d] + [%d]\n", rdn, rdn, rm);
	unsigned instruction = 0x4400;
	instruction += rdn & 0x7;
	instruction += (rdn & 0x8) << 4;
	instruction += rm << 3;
	execute_instruction16(instruction);
};

void execute_add_sp_reg_t2 (unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_add_sp_reg_t2: sp = sp + [%d]\n", rm);
	unsigned instruction = 0x4485;
	instruction += rm << 3;
	execute_instruction16(instruction);
};

void execute_add_reg_t3(unsigned rd, unsigned rn, unsigned rm, bool setflag, unsigned type, unsigned imm5)
{
	//fprintf(disasm, "-----------------------------------------\nexecute_add_reg_t3: [%d] = [%d] + [%d] setflag: %d\n", rd, rn, rm, setflag);
	unsigned instruction = 0xeb000000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += type << 4;
	instruction += (imm5 & 0x3) << 6;
	instruction += (imm5 & 0x1C) << 10;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void execute_add_sp_reg_t3 (unsigned rd, unsigned rn, unsigned rm, bool setflag, unsigned type, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_add_sp_reg_t3: [%d] = sp + [%d] tpye: %x imm5: %x setflag: %d\n", rd, rm, type, imm5, setflag);
	unsigned instruction = 0xeb0d0000;
	instruction += rd << 8;
	instruction += type << 4;
	instruction += (imm5 & 0x3) << 6;
	instruction += (imm5 & 0x1C) << 10;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_add_reg (int rn, int rm, int rd, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	// missing: DN

	fprintf(disasm, "-----------------------------------------\ntest_add_reg:\n");
	unsigned result = a + b;

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_add_reg_t1(rd, rn, rm);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	write_r(rd, a);
	execute_add_reg_t2(rd, rm);
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_add_reg_t3(rd, rn, rm, true, 0, 0);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_add_reg_t3(rd, rn, rm, false, 0, 0);
	check_flag(false);
	check_rd(rd, result, 3);

	/////////////////////////////////////////////////////
	result = sp + b;
	write_r(rd, b);

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_add_sp_reg_t1(rd);
	check_flag(false);
	check_rd(rd, result, 3);

	// changes sp itself which generates bugs
	//set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	//execute_add_sp_reg_t2(rm);
	//check_flag(false);
	//check_rd(13, result, 3);

	// missing: set/check flags correctly
	set_c = false;
	//set_z = false;
	this_z = false;
	set_n = false;
	set_v = false;

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_add_sp_reg_t3(rd, rn, rm, true, 0, 0);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_add_sp_reg_t3(rd, rn, rm, false, 0, 0);
	check_flag(false);
	check_rd(rd, result, 3);
}

void test_add_dis_reg (int rn, int rm, int rd, unsigned a, unsigned b, bool this_c, unsigned type, unsigned imm5)
{
	// missing: carry

	fprintf(disasm, "-----------------------------------------\ntest_add_dis_reg:\n");
	unsigned result = a + b;

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
	execute_add_reg_t3(rd, rn, rm, false, type, imm5);
//	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
//	c = !this_c;
	execute_add_reg_t3(rd, rn, rm, true, type, imm5);
//	check_flag(true);
	check_rd(rd, result, 3);

	/////////////////////////////////////////////////////
	result = sp + b;
	write_r(rd, b);

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
	execute_add_sp_reg_t3(rd, rn, rm, false, type, imm5);
//	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
//	c = !this_c;
	execute_add_sp_reg_t3(rd, rn, rm, true, type, imm5);
//	check_flag(true);
	check_rd(rd, result, 3);

}

void execute_add_imm_t1( unsigned rd, unsigned rn, unsigned imm3)
{
	fprintf(disasm, "-----------------------------------------\nexecute_add_imm_t1: [%d] = [%d] + %x\n", rd, rn, imm3);
	unsigned instruction = 0x1c00;
	instruction += rd & 0x7;
	instruction += (rn & 0x7) << 3;
	instruction += (imm3 & 0x7) << 6;
	execute_instruction16(instruction);
};

void execute_add_sp_imm_t1 (unsigned rd, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_add_sp_imm_t1: [%d] = sp + %x\n", rd, imm8);
	unsigned instruction = 0xa800;
	instruction += (rd & 0x7) << 8;
	instruction += imm8;
	execute_instruction16(instruction);
};

void execute_add_imm_t2 (unsigned rdn, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_add_imm_t2: [%d] = [%d] + %x\n", rdn, rdn, imm8);
	unsigned instruction = 0x3000;
	instruction += (rdn & 0x7) << 8;
	instruction += imm8;
	execute_instruction16(instruction);
};

void execute_add_imm_t3(unsigned rd, unsigned rn, bool setflag, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_add_imm_t3: [%d] = [%d] + %x setflag: %d\n", rd, rn, imm8, setflag);
	unsigned instruction = 0xf1000000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += i << 26;
	instruction += imm3 << 12;
	instruction += imm8;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void execute_add_sp_imm_t3 (unsigned rd, bool setflag, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_add_sp_imm_t3: [%d] = sp + %x setflag: %d\n", rd, imm8, setflag);
	unsigned instruction = 0xf10d0000;
	instruction += rd << 8;
	instruction += i << 26;
	instruction += imm3 << 12;
	instruction += imm8;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void execute_add_sp_imm_t4 (unsigned rd, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_add_sp_imm_t4: [%d] = sp + %x\n", rd, imm8);
	unsigned instruction = 0xf10d0000;
	instruction += rd << 8;
	instruction += i << 26;
	instruction += imm3 << 12;
	instruction += imm8;
	execute_instruction32(instruction);
};

void execute_add_imm_t4(unsigned rd, unsigned rn, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_add_imm_t4: [%d] = [%d] + %x\n", rd, rn, imm8);
	unsigned instruction = 0xf2000000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += i << 26;
	instruction += imm3 << 12;
	instruction += imm8;
	execute_instruction32(instruction);
};

void test_add_imm (int rn, int rd, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	fprintf(disasm, "-----------------------------------------\ntest_add_imm:\n");
	unsigned result = a + (b & 0x7);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_add_imm_t1(rd, rn, b & 0x7);
	check_flag(true);
	check_rd(rd, result, 3);

	result = a + (b & 0xFF);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	write_r(rd, a);
	execute_add_imm_t2(rd, b & 0xFF);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_add_imm_t3(rd, rn, true, 0, 0, b & 0xff);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_add_imm_t3(rd, rn, false, 0, 0, b & 0xff);
	check_flag(false);
	check_rd(rd, result, 3);

	result = a + (b & 0xFFF);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_add_imm_t4(rd, rn, ((b >> 11) & 0x1), ((b >> 8) & 0x7), b & 0xff);
	check_flag(false);
	check_rd(rd, result, 3);

//	/////////////////////////////////////////////////////
	result = sp + ((b & 0xFF) << 2);
//	write_r(rd, b);

	write_r(rd, ~result);
	set_flags(set_c, false, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_add_sp_imm_t1(rd, (b & 0xFF));
	check_rd(rd, result, 3);

//	// changes sp itself which generates bugs
//	//set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
//	//execute_add_sp_reg_t2(rm);
//	//check_flag(false);
//	//check_rd(13, result, 3);

	// missing: set/check flags correctly
	set_c = false;
	//set_z = false;
	this_z = false;
	set_n = false;
	set_v = false;

	result = sp + (b & 0xFF);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_add_sp_imm_t3(rd, true, 0, 0, b & 0xFF);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_add_sp_imm_t3(rd, false, 0, 0, b & 0xFF);
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_add_sp_imm_t4(rd, 0, 0, b & 0xFF);
	check_flag(false);
	check_rd(rd, result, 3);

}

void test_add_tei_imm (int rn, int rd, unsigned a, unsigned b, bool this_c, unsigned i, unsigned imm3, unsigned imm8)
{
	// missing: carry

	fprintf(disasm, "-----------------------------------------\ntest_add_tei_imm:\n");
	unsigned result = a + b;

	write_r(rd, ~result);
//	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_add_imm_t3(rd, rn, false, i, imm3, imm8);
//	check_flag(false);
	check_rd(rd, result, 3);

//	write_r(rd, ~result);
//	set_flags(set_c, true, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
//	c = this_c;
//	execute_and_imm_t1(rd, rn, true, i, imm3, imm8);
//	check_flag(true);
//	check_rd(rd, result, 3);

	result = sp + b;

	write_r(rd, ~result);
//	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_add_sp_imm_t3(rd, false, i, imm3, imm8);
//	check_flag(false);
	check_rd(rd, result, 3);

}

//////////////////////////////////////////////////////////////////////////////////////
//	AND
//////////////////////////////////////////////////////////////////////////////////////
void execute_and_reg_t1 (unsigned rdn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_and_reg_t1: [%d] = [%d] + [%d]\n", rdn, rdn, rm);
	unsigned instruction = 0x4000;
	instruction += rdn & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_and_reg_t2 (unsigned rd, unsigned rn, unsigned rm, bool setflag, unsigned type, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_and_reg_t2: [%d] = [%d] & [%d] type: %x imm5: %x setflag: %d\n", rd, rn, rm, type, imm5, setflag);
	unsigned instruction = 0xea000000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += rm;
	instruction += type << 4;
	instruction += (imm5 & 0x3) << 6;
	instruction += (imm5 & 0x1C) << 10;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_and_reg (int rn, int rm, int rd, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	// missing: carry
	set_c = false;

	fprintf(disasm, "-----------------------------------------\ntest_and_reg:\n");
	unsigned result = a & b;

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	write_r(rd, a);
	execute_and_reg_t1(rd, rm);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_and_reg_t2(rd, rn, rm, false, 0, 0);
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_and_reg_t2(rd, rn, rm, true, 0, 0);
	check_flag(true);
	check_rd(rd, result, 3);
}

void test_and_dis_reg (int rn, int rm, int rd, unsigned a, unsigned b, bool this_c, unsigned type, unsigned imm5)
{
	// missing: carry

	fprintf(disasm, "-----------------------------------------\ntest_and_dis_reg:\n");
	unsigned result = a & b;

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
	execute_and_reg_t2(rd, rn, rm, false, type, imm5);
//	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
//	c = !this_c;
	execute_and_reg_t2(rd, rn, rm, true, type, imm5);
//	check_flag(true);
	check_rd(rd, result, 3);
}

void execute_and_imm_t1 (unsigned rd, unsigned rn, bool setflag, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_and_imm_t1: [%d] = [%d] & %x setflag: %d\n", rd, rn, imm8, setflag);
	unsigned instruction = 0xf0000000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += i << 26;
	instruction += imm3 << 12;
	instruction += imm8;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_and_imm (int rn, int rd, unsigned a, unsigned i, unsigned imm3, unsigned imm8,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	fprintf(disasm, "-----------------------------------------\ntest_and_imm:\n");
	unsigned result = a & imm8;

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_and_imm_t1(rd, rn, false, 0, 0, imm8);
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, true, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	c = this_c;
	execute_and_imm_t1(rd, rn, true, 0, 0, imm8);
	check_flag(true);
	check_rd(rd, result, 3);

}

void test_and_tei_imm (int rn, int rd, unsigned a, unsigned b, bool this_c, unsigned i, unsigned imm3, unsigned imm8)
{
	// missing: carry

	fprintf(disasm, "-----------------------------------------\ntest_and_tei_imm:\n");
	unsigned result = a & b;

	write_r(rd, ~result);
//	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_and_imm_t1(rd, rn, false, i, imm3, imm8);
//	check_flag(false);
	check_rd(rd, result, 3);

//	write_r(rd, ~result);
//	set_flags(set_c, true, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
//	c = this_c;
//	execute_and_imm_t1(rd, rn, true, i, imm3, imm8);
//	check_flag(true);
//	check_rd(rd, result, 3);

}

//////////////////////////////////////////////////////////////////////////////////////
//	ASR
//////////////////////////////////////////////////////////////////////////////////////
void execute_asr_reg_t1 (unsigned rdn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_asr_reg_t1: [%d] = [%d] + [%d]\n", rdn, rdn, rm);
	unsigned instruction = 0x4100;
	instruction += rdn & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_asr_reg_t2 (unsigned rd, unsigned rn, unsigned rm, bool setflag)
{
	fprintf(disasm, "-----------------------------------------\nexecute_asr_reg_t2: [%d] = [%d] + [%d] setflag: %d\n", rd, rn, rm, setflag);
	unsigned instruction = 0xfa40f000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_asr_reg (int rn, int rm, int rd, unsigned a, unsigned b)
{
	// missing:		rm = 2^8 ??
	// missing: 	c when b==0
	bool set_c = true;
      bool this_c = true;
      bool set_z = true;
      bool this_z = false;
      bool set_n = true;
      bool this_n = false;
      bool set_v = false;
      bool this_v = false;

	// missing: carry
	//set_c = false;

	fprintf(disasm, "-----------------------------------------\ntest_asr_reg:\n");
	unsigned result = a >> b;
	if (((a >> 31) & 0x1) == 0x1)
		result += ~(0xffffffff >> b);

	if (b == 0)
		set_c = false;

	if (((a >> (b - 1)) & 0x1) == 0x1)
		this_c = true;
	else
		this_c = false;

	if (result == 0)
		this_z = true;
	else
		this_z = false;

	if (((result >> 31) & 0x1) == 0x1)
		this_n = true;
	else
		this_n = false;

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	write_r(rd, a);
	execute_asr_reg_t1(rd, rm);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_asr_reg_t2(rd, rn, rm, false);
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_asr_reg_t2(rd, rn, rm, true);
	check_flag(true);
	check_rd(rd, result, 3);

}

void execute_asr_imm_t1 (unsigned rd, unsigned rm, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_asr_imm_t1: [%d] = [%d] + %x\n", rd, rm, imm5);
	unsigned instruction = 0x1000;
	instruction += (imm5 & 0x1f) << 6;
	instruction += (rm & 0x7) << 3;
	instruction += rd & 0x7;
	execute_instruction16(instruction);
};

void execute_asr_imm_t2 (unsigned rd, unsigned rm, bool setflag, unsigned imm3, unsigned imm2)
{
	fprintf(disasm, "-----------------------------------------\nexecute_asr_imm_t2: [%d] = [%d] + %x setflag: %d\n", rd, rm, imm3, setflag);
	unsigned instruction = 0xea4f0020;
	instruction += (imm3 & 0x7) << 12;
	instruction += (imm2 & 0x3) << 6;
	instruction += rd << 8;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_asr_imm (int rn, int rd, unsigned a, unsigned b)
{
	// missing:		rm = 2^8 ??
	// missing: 	c when b==0
	bool set_c = true;
      bool this_c = true;
      bool set_z = true;
      bool this_z = false;
      bool set_n = true;
      bool this_n = false;
      bool set_v = false;
      bool this_v = false;

	// missing: carry
	//set_c = false;

	fprintf(disasm, "-----------------------------------------\ntest_asr_imm:\n");
	unsigned result = a >> b;
	if (((a >> 31) & 0x1) == 0x1)
		result += ~(0xffffffff >> b);

	if (b == 0)
		set_c = false;

	if (((a >> (b - 1)) & 0x1) == 0x1)
		this_c = true;
	else
		this_c = false;

	if (result == 0)
		this_z = true;
	else
		this_z = false;

	if (((result >> 31) & 0x1) == 0x1)
		this_n = true;
	else
		this_n = false;

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
//	write_r(rd, a);
	execute_asr_imm_t1(rd, rn, b & 0x1f);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_asr_imm_t2(rd, rn, false, ((b >> 2) & 0x7), (b & 0x3));
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_asr_imm_t2(rd, rn, true, ((b >> 2) & 0x7), (b & 0x3));
	check_flag(true);
	check_rd(rd, result, 3);

}

//////////////////////////////////////////////////////////////////////////////////////
//	B
//////////////////////////////////////////////////////////////////////////////////////
void execute_b_t2 (unsigned imm11)
{
	fprintf(disasm, "-----------------------------------------\nexecute_b_t2: %x\n", imm11);
	unsigned instruction = 0xe000;
	instruction += imm11 & 0x7ff;
	execute_instruction16(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	BIC
//////////////////////////////////////////////////////////////////////////////////////
void execute_bic_reg_t1 (unsigned rdn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_bic_reg_t1: [%d] = [%d] + [%d]\n", rdn, rdn, rm);
	unsigned instruction = 0x4380;
	instruction += rdn & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_bic_reg_t2 (unsigned rd, unsigned rn, unsigned rm, bool setflag, unsigned type, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_bic_reg_t2: [%d] = [%d] + [%d] setflag: %d\n", rd, rn, rm, setflag);
	unsigned instruction = 0xea200000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += type << 4;
	instruction += (imm5 & 0x3) << 6;
	instruction += (imm5 & 0x1C) << 10;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_bic_reg (int rn, int rm, int rd, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	// missing: carry
	set_c = false;

	fprintf(disasm, "-----------------------------------------\ntest_bic_reg:\n");
	unsigned result = a & ~b;

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	write_r(rd, a);
	execute_bic_reg_t1(rd, rm);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_bic_reg_t2(rd, rn, rm, false, 0, 0);
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_bic_reg_t2(rd, rn, rm, true, 0, 0);
	check_flag(true);
	check_rd(rd, result, 3);
}

void test_bic_dis_reg (int rn, int rm, int rd, unsigned a, unsigned b, bool this_c, unsigned type, unsigned imm5)
{
	// missing: carry

	fprintf(disasm, "-----------------------------------------\ntest_bic_dis_reg:\n");
	unsigned result = a & ~b;

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
	execute_bic_reg_t2(rd, rn, rm, false, type, imm5);
//	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
//	c = !this_c;
	execute_bic_reg_t2(rd, rn, rm, true, type, imm5);
//	check_flag(true);
	check_rd(rd, result, 3);
}

void execute_bic_imm_t1 (unsigned rd, unsigned rn, bool setflag, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_bic_imm_t1: [%d] = [%d] & i: %x imm3: %x imm8: %x setflag: %d\n", rd, rn, i, imm3, imm8, setflag);
	unsigned instruction = 0xf0200000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += i << 26;
	instruction += imm3 << 12;
	instruction += imm8;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_bic_imm (int rn, int rd, unsigned a, unsigned i, unsigned imm3, unsigned imm8,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	fprintf(disasm, "-----------------------------------------\ntest_bic_imm:\n");
	unsigned result = a & ~imm8;

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_bic_imm_t1(rd, rn, false, 0, 0, imm8);
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, true, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	c = this_c;
	execute_bic_imm_t1(rd, rn, true, 0, 0, imm8);
	check_flag(true);
	check_rd(rd, result, 3);

}

void test_bic_tei_imm (int rn, int rd, unsigned a, unsigned b, bool this_c, unsigned i, unsigned imm3, unsigned imm8)
{
	// missing: carry

	fprintf(disasm, "-----------------------------------------\ntest_and_tei_imm:\n");
	unsigned result = a & ~b;

	write_r(rd, ~result);
//	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_bic_imm_t1(rd, rn, false, i, imm3, imm8);
//	check_flag(false);
	check_rd(rd, result, 3);

//	write_r(rd, ~result);
//	set_flags(set_c, true, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
//	c = this_c;
//	execute_and_imm_t1(rd, rn, true, i, imm3, imm8);
//	check_flag(true);
//	check_rd(rd, result, 3);

}

//////////////////////////////////////////////////////////////////////////////////////
//	BFC
//////////////////////////////////////////////////////////////////////////////////////
void execute_bfc_t1 (unsigned rd, int msb, int lsb)
{
	fprintf(disasm, "-----------------------------------------\nexecute_bfc_t1: [%d] %d %d\n", rd, msb, lsb);
	unsigned instruction = 0xf36f0000;
	instruction += rd << 8;
	instruction += msb;
	instruction += (lsb & 0x3) << 6;
	instruction += ((lsb >> 2) & 0x7) << 12;
	execute_instruction32(instruction);
};

void test_bfc (int rd, unsigned a, int msb, int lsb)
{
	fprintf(disasm, "-----------------------------------------\ntest_bfc:\n");

	unsigned mask = 0x0;
	int n = 0;
	for (n = 0; n <= (msb - lsb); n++)
	{
		mask = mask << 1;
		mask += 1;
	}
	mask = mask << lsb;
	unsigned result = a & ~mask;

	execute_bfc_t1(rd, msb, lsb);
	check_rd(rd, result, 3);
}

//////////////////////////////////////////////////////////////////////////////////////
//	BFI
//////////////////////////////////////////////////////////////////////////////////////
void execute_bfi_t1 (unsigned rd, unsigned rn, int msb, int lsb)
{
	fprintf(disasm, "-----------------------------------------\nexecute_bfi_t1: [%d] %d %d\n", rd, msb, lsb);
	unsigned instruction = 0xf3600000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += msb;
	instruction += (lsb & 0x3) << 6;
	instruction += ((lsb >> 2) & 0x7) << 12;
	execute_instruction32(instruction);
};

void test_bfi (int rd, int rn, unsigned a, unsigned b, int msb, int lsb)
{
	fprintf(disasm, "-----------------------------------------\ntest_bfi:\n");

	unsigned mask = 0x0;
	int n = 0;
	for (n = 0; n <= (msb - lsb); n++)
	{
		mask = mask << 1;
		mask += 1;
	}
	unsigned result = (a & mask) << lsb;
	result += b & ~(mask << lsb);

	execute_bfi_t1(rd, rn, msb, lsb);
	check_rd(rd, result, 3);
}

//////////////////////////////////////////////////////////////////////////////////////
//	BX
//////////////////////////////////////////////////////////////////////////////////////
void execute_bx_t1 (unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_bx_t1: [%d]\n", rm);
	unsigned instruction = 0x4700;
	instruction += (rm & 0xf) << 3;
	execute_instruction16(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	CLZ
//////////////////////////////////////////////////////////////////////////////////////
void execute_clz_t1 (unsigned rd, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_clz_t1: [%d] = clz [%d]\n", rd, rm);
	unsigned instruction = 0xfab0f080;
	instruction += rm << 16;
	instruction += rm;
	instruction += rd << 8;
	execute_instruction32(instruction);
};

void test_clz (int rm, int rd, unsigned result)
{
	fprintf(disasm, "-----------------------------------------\ntest_clz:\n");

	execute_clz_t1(rd, rm);
	check_rd(rd, result, 3);
}

//////////////////////////////////////////////////////////////////////////////////////
//	CMN
//////////////////////////////////////////////////////////////////////////////////////
void execute_cmn_reg_t1 (unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_cmn_reg_t1: [%d] cmn [%d]\n", rn, rm);
	unsigned instruction = 0x42c0;
	instruction += rn & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_cmn_reg_t2 (unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_cmn_reg_t2: [%d] cmn [%d]\n", rn, rm);
	unsigned instruction = 0xeb100f00;
	instruction += rn << 16;
	instruction += rm;
	execute_instruction32(instruction);
};

void test_cmn_reg (int rn, int rm, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	// missing: DN

	fprintf(disasm, "-----------------------------------------\ntest_cmn_reg:\n");

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_cmn_reg_t1(rn, rm);
	check_flag(true);

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_cmn_reg_t2(rn, rm);
	check_flag(true);
}

void execute_cmn_imm_t1 (unsigned rn, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_cmn_imm_t1: [%d] cmn %x \n", rn, imm8);
	unsigned instruction = 0xf1100f00;
	instruction += rn << 16;
	instruction += i << 26;
	instruction += imm3 << 12;
	instruction += imm8;
	execute_instruction32(instruction);
};

void test_cmn_imm (int rn, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	fprintf(disasm, "-----------------------------------------\ntest_cmn_imm:\n");
	
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_cmn_imm_t1(rn, 0, 0, (b & 0xff));
	check_flag(true);
}

void test_cmn_tei_imm (int rn, int rd, unsigned a, unsigned b, bool setflag, unsigned i, unsigned imm3, unsigned imm8)
{
	bool set_c = false;
      bool this_c = false;
      bool set_z = true;
      bool this_z = false;
      bool set_n = true;
      bool this_n = false;
      bool set_v = false;
      bool this_v = false;

	fprintf(disasm, "-----------------------------------------\ntest_cmn_tei_imm:\n");
	unsigned result = a + b;

//	if (((a >> (b - 1)) & 0x1) == 0x1)
//		this_c = true;
//	else
//		this_c = false;

	if (result == 0)
		this_z = true;
	else
		this_z = false;

	if (((result >> 31) & 0x1) == 0x1)
		this_n = true;
	else
		this_n = false;

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_cmn_imm_t1(rn, i, imm3, imm8);
	check_flag(true);

}

//////////////////////////////////////////////////////////////////////////////////////
//	CMP
//////////////////////////////////////////////////////////////////////////////////////
void execute_cmp_reg_t1 (unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_cmp_reg_t1: [%d] cmp [%d]\n", rn, rm);
	unsigned instruction = 0x4280;
	instruction += rn & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_cmp_reg_t2 (unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_cmp_reg_t2: [%d] cmp [%d]\n", rn, rm);
	unsigned instruction = 0x4500;
	instruction += rn & 0x7;
	instruction += (rn & 0x8) << 4;
	instruction += (rm & 0xF) << 3;
	execute_instruction16(instruction);
};

void execute_cmp_reg_t3 (unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_cmp_reg_t3: [%d] cmp [%d]\n", rn, rm);
	unsigned instruction = 0xebb00f00;
	instruction += rn << 16;
	instruction += rm;
	execute_instruction32(instruction);
};

void test_cmp_reg (int rn, int rm, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	// missing: DN

	fprintf(disasm, "-----------------------------------------\ntest_cmp_reg:\n");

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_cmp_reg_t1(rn, rm);
	check_flag(true);

	write_r(11, a);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_cmp_reg_t2(11, rm);
	check_flag(true);

	write_r(11, b);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_cmp_reg_t2(rn, 11);
	check_flag(true);

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_cmp_reg_t3(rn, rm);
	check_flag(true);
}

void execute_cmp_imm_t1 (unsigned rn, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_cmp_imm_t1: [%d] cmp %x\n", rn, imm8);
	unsigned instruction = 0x2800;
	instruction += (rn & 0x7) << 8;
	instruction += imm8;
	execute_instruction16(instruction);
};

void execute_cmp_imm_t2 (unsigned rn, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_cmp_imm_t2: [%d] cmp %x \n", rn, imm8);
	unsigned instruction = 0xf1b00f00;
	instruction += rn << 16;
	instruction += i << 26;
	instruction += imm3 << 12;
	instruction += imm8;
	execute_instruction32(instruction);
};

void test_cmp_imm (int rn, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	fprintf(disasm, "-----------------------------------------\ntest_cmp_imm:\n");
	
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_cmp_imm_t1(rn, (b & 0xff));
	check_flag(true);

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_cmp_imm_t2(rn, ((b >> 11) & 0x1), ((b >> 8) & 0x7), (b & 0xff));
	check_flag(true);
}

void test_cmp_tei_imm (int rn, int rd, unsigned a, unsigned b, bool setflag, unsigned i, unsigned imm3, unsigned imm8)
{
	bool set_c = false;
      bool this_c = false;
      bool set_z = true;
      bool this_z = false;
      bool set_n = true;
      bool this_n = false;
      bool set_v = false;
      bool this_v = false;

	fprintf(disasm, "-----------------------------------------\ntest_cmp_tei_imm:\n");
	unsigned result = a - b;

//	if (((a >> (b - 1)) & 0x1) == 0x1)
//		this_c = true;
//	else
//		this_c = false;

	if (result == 0)
		this_z = true;
	else
		this_z = false;

	if (((result >> 31) & 0x1) == 0x1)
		this_n = true;
	else
		this_n = false;

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_cmp_imm_t2(rn, i, imm3, imm8);
	check_flag(true);

}

//////////////////////////////////////////////////////////////////////////////////////
//	DIV
//////////////////////////////////////////////////////////////////////////////////////
void execute_udiv_t1 (unsigned rd, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_udiv_t1: [%d] = [%d] * [%d] setflag: %d\n", rd, rn, rm);
	unsigned instruction = 0xfbb0f0f0;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += rm;
	execute_instruction32(instruction);
};

void execute_sdiv_t1 (unsigned rd, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_sdiv_t1: [%d] = [%d] * [%d] setflag: %d\n", rd, rn, rm);
	unsigned instruction = 0xfb90f0f0;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += rm;
	execute_instruction32(instruction);
};

void test_div_reg (int rn, int rm, int rd, unsigned a, unsigned b)
{
	fprintf(disasm, "-----------------------------------------\ntest_div_reg:\n");
	unsigned result = a / b;

	fprintf(disasm, "-----------------------------------------\nudiv: %8.8x / %8.8x = %8.8x\n", a, b, result);
	printf("%8.8x / %8.8x = %8.8x\n", a, b, result);

	write_r(rd, ~result);
	execute_udiv_t1(rd, rn, rm);
	check_rd(rd, result, 3);

	////////////////////////////////////////////  sdiv
	unsigned ua = a;
	unsigned ub = b;
	if (((a >> 31) & 0x1) == 0x1)
		ua = 0 - a;
	if (((b >> 31) & 0x1) == 0x1)
		ub = 0 - b;
	
	result = ua / ub;

	if (	(((a >> 31) & 0x1) == 0x1)	&
		(((b >> 31) & 0x1) == 0x0)	)
	{
		result = 0 - result;
      } else
	if (	(((a >> 31) & 0x1) == 0x0)	&
		(((b >> 31) & 0x1) == 0x1)	)
	{
		result = 0 - result;
	}

	fprintf(disasm, "-----------------------------------------\nsdiv: %8.8x / %8.8x = %8.8x\n", a, b, result);
	write_r(rd, ~result);
	execute_sdiv_t1(rd, rn, rm);
	check_rd(rd, result, 3);
}

//////////////////////////////////////////////////////////////////////////////////////
//	EOR
//////////////////////////////////////////////////////////////////////////////////////
void execute_eor_reg_t1 (unsigned rdn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_eor_reg_t1: [%d] = [%d] + [%d]\n", rdn, rdn, rm);
	unsigned instruction = 0x4040;
	instruction += rdn & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_eor_reg_t2 (unsigned rd, unsigned rn, unsigned rm, bool setflag, unsigned type, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_eor_reg_t2: [%d] = [%d] + [%d] setflag: %d\n", rd, rn, rm, setflag);
	unsigned instruction = 0xea800000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += type << 4;
	instruction += (imm5 & 0x3) << 6;
	instruction += (imm5 & 0x1C) << 10;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_eor_reg (int rn, int rm, int rd, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	// missing: carry
	set_c = false;

	fprintf(disasm, "-----------------------------------------\ntest_eor_reg:\n");
	unsigned result = a ^ b;

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	write_r(rd, a);
	execute_eor_reg_t1(rd, rm);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_eor_reg_t2(rd, rn, rm, false, 0, 0);
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_eor_reg_t2(rd, rn, rm, true, 0, 0);
	check_flag(true);
	check_rd(rd, result, 3);
}

void test_eor_dis_reg (int rn, int rm, int rd, unsigned a, unsigned b, bool this_c, unsigned type, unsigned imm5)
{
	// missing: carry

	fprintf(disasm, "-----------------------------------------\ntest_bic_eor_reg:\n");
	unsigned result = a ^ b;

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
	execute_eor_reg_t2(rd, rn, rm, false, type, imm5);
//	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
//	c = !this_c;
	execute_eor_reg_t2(rd, rn, rm, true, type, imm5);
//	check_flag(true);
	check_rd(rd, result, 3);
}

void execute_eor_imm_t1 (unsigned rd, unsigned rn, bool setflag, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_eor_imm_t1: [%d] = [%d] & %x setflag: %d\n", rd, rn, imm8, setflag);
	unsigned instruction = 0xf0800000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += i << 26;
	instruction += imm3 << 12;
	instruction += imm8;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_eor_imm (int rn, int rd, unsigned a, unsigned i, unsigned imm3, unsigned imm8,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	fprintf(disasm, "-----------------------------------------\ntest_eor_imm:\n");
	unsigned result = a ^ imm8;

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_eor_imm_t1(rd, rn, false, 0, 0, imm8);
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, true, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	c = this_c;
	execute_eor_imm_t1(rd, rn, true, 0, 0, imm8);
	check_flag(true);
	check_rd(rd, result, 3);

}

void test_eor_tei_imm (int rn, int rd, unsigned a, unsigned b, bool this_c, unsigned i, unsigned imm3, unsigned imm8)
{
	// missing: carry

	fprintf(disasm, "-----------------------------------------\ntest_eor_tei_imm:\n");
	unsigned result = a ^ b;

	write_r(rd, ~result);
//	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_eor_imm_t1(rd, rn, false, i, imm3, imm8);
//	check_flag(false);
	check_rd(rd, result, 3);

//	write_r(rd, ~result);
//	set_flags(set_c, true, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
//	c = this_c;
//	execute_eor_imm_t1(rd, rn, true, i, imm3, imm8);
//	check_flag(true);
//	check_rd(rd, result, 3);

}

//////////////////////////////////////////////////////////////////////////////////////
//	IT
//////////////////////////////////////////////////////////////////////////////////////
void execute_it_t1 (unsigned cond, unsigned mask)
{
	fprintf(disasm, "-----------------------------------------\nexecute_it_t1: cond: %x mask: %x\n", cond, mask);
	unsigned instruction = 0xbf00;
	instruction += (cond & 0xf) << 4;
	instruction += mask & 0xf;
	execute_instruction16(instruction);
}

//////////////////////////////////////////////////////////////////////////////////////
//	LDMDB / LDMEA
//////////////////////////////////////////////////////////////////////////////////////
void execute_ldmdb_t1 (unsigned rn, unsigned w, unsigned p, unsigned m, unsigned reglist)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldmdb_ldmea_t1: [%d] %x %x %x %x\n", rn, w, p, m, reglist);
	unsigned instruction = 0xe9100000;
	instruction += (w & 0x1) << 21;
	instruction += (rn & 0xf) << 16;
	instruction += (p & 0x1) << 15;
	instruction += (m & 0x1) << 14;
	instruction += reglist & 0x1fff;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	LDMIA / LDMFD
//////////////////////////////////////////////////////////////////////////////////////
void execute_ldmia_t1 (unsigned rn, unsigned reglist)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldmia_t1: [%d] %x\n", rn, reglist);
	unsigned instruction = 0xc800;
	instruction += (rn & 0x7) << 8;
	instruction += reglist & 0xff;
	execute_instruction16(instruction);
};

void execute_ldmia_t2 (unsigned rn, unsigned w, unsigned p, unsigned m, unsigned reglist)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldmia_t2: [%d] %x %x %x %x\n", rn, w, p, m, reglist);
	unsigned instruction = 0xe8900000;
	instruction += (w & 0x1) << 21;
	instruction += (rn & 0xf) << 16;
	instruction += (p & 0x1) << 15;
	instruction += (m & 0x1) << 14;
	instruction += reglist & 0x1fff;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	LDR
//////////////////////////////////////////////////////////////////////////////////////
void execute_ldr_imm_t1 (unsigned rt, unsigned rn, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldr_imm_t1: [%d] [%d] [%d]\n", rt, rn, imm5);
	unsigned instruction = 0x6800;
	instruction += rt & 0x7;
	instruction += (rn & 0x7) << 3;
	instruction += (imm5 & 0x1f) << 6;
	execute_instruction16(instruction);
};

void execute_ldr_imm_t2 (unsigned rt, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldr_imm_t2: [%d] [%d] [%d]\n", rt, imm8);
	unsigned instruction = 0x9800;
	instruction += (rt & 0x7) << 8;
	instruction += imm8 & 0xff;
	execute_instruction16(instruction);
};

void execute_ldr_imm_t3 (unsigned rt, unsigned rn, unsigned imm12)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldr_imm_t3: [%d] [%d] %x\n", rt, rn, imm12);
	unsigned instruction = 0xf8d00000;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += imm12;
	execute_instruction32(instruction);
};

void execute_ldr_imm_t4 (unsigned rt, unsigned rn, unsigned imm8, unsigned puw)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldr_imm_t4: [%d] [%d] %x %x\n", rt, rn, puw, imm8);
	unsigned instruction = 0xf8500800;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += puw << 8;
	instruction += imm8;
	execute_instruction32(instruction);
};

void execute_ldr_lit_t1 (unsigned rt, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldr_lit_t1: [%d] [%d] [%d]\n", rt, imm8);
	unsigned instruction = 0x4800;
	instruction += (rt & 0x7) << 8;
	instruction += imm8 & 0xff;
	execute_instruction16(instruction);
};

void execute_ldr_lit_t2 (unsigned rt, unsigned u, unsigned imm12)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldr_lit_t2: [%d] %x %x\n", rt, u, imm12);
	unsigned instruction = 0xf85f0000;
	instruction += u << 23;
	instruction += rt << 12;
	instruction += imm12;
	execute_instruction32(instruction);
};

void execute_ldr_reg_t1 (unsigned rt, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldr_reg_t1: [%d] [%d] [%d]\n", rt, rn, rm);
	unsigned instruction = 0x5800;
	instruction += rt & 0x7;
	instruction += (rn & 0x7) << 3;
	instruction += (rm & 0x7) << 6;
	execute_instruction16(instruction);
};

void execute_ldr_reg_t2 (unsigned rt, unsigned rn, unsigned rm, unsigned shift)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldr_imm_t2: [%d] [%d] %x %x\n", rt, rn, rm, shift);
	unsigned instruction = 0xf8500000;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += (shift & 0x3) << 4;
	instruction += rm;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	LDRB
//////////////////////////////////////////////////////////////////////////////////////
void execute_ldrb_imm_t1 (unsigned rt, unsigned rn, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrb_imm_t1: [%d] [%d] [%d]\n", rt, rn, imm5);
	unsigned instruction = 0x7800;
	instruction += rt & 0x7;
	instruction += (rn & 0x7) << 3;
	instruction += (imm5 & 0x1f) << 6;
	execute_instruction16(instruction);
};

void execute_ldrb_imm_t2 (unsigned rt, unsigned rn, unsigned imm12)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrb_imm_t2: [%d] [%d] %x\n", rt, rn, imm12);
	unsigned instruction = 0xf8900000;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += imm12;
	execute_instruction32(instruction);
};

void execute_ldrb_imm_t3 (unsigned rt, unsigned rn, unsigned imm8, unsigned puw)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrb_imm_t3: [%d] [%d] %x %x\n", rt, rn, imm8, puw);
	unsigned instruction = 0xf8100800;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += puw << 8;
	instruction += imm8;
	execute_instruction32(instruction);
};

void execute_ldrb_lit_t1 (unsigned rt, unsigned u, unsigned imm12)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrb_lit_t1: [%d] %x %x\n", rt, u, imm12);
	unsigned instruction = 0xf81f0000;
	instruction += u << 23;
	instruction += rt << 12;
	instruction += imm12;
	execute_instruction32(instruction);
};

void execute_ldrb_reg_t1 (unsigned rt, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrb_reg_t1: [%d] [%d] [%d]\n", rt, rn, rm);
	unsigned instruction = 0x5c00;
	instruction += rt & 0x7;
	instruction += (rn & 0x7) << 3;
	instruction += (rm & 0x7) << 6;
	execute_instruction16(instruction);
};

void execute_ldrb_reg_t2 (unsigned rt, unsigned rn, unsigned rm, unsigned shift)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrb_reg_t2: [%d] [%d] %x %x\n", rt, rn, rm, shift);
	unsigned instruction = 0xf8100000;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += (shift & 0x3) << 4;
	instruction += rm;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	LDRBT
//////////////////////////////////////////////////////////////////////////////////////
void execute_ldrbt_t1 (unsigned rt, unsigned rn, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrbt_t1: [%d] [%d] %x\n", rt, rn, imm8);
	unsigned instruction = 0xf8100e00;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += imm8;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	LDRD
//////////////////////////////////////////////////////////////////////////////////////
void execute_ldrd_imm_t1 (unsigned rt, unsigned rt2, unsigned rn, unsigned imm8, unsigned puw)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrd_imm_t1: [%d] [%d] [%d] %x %x\n", rt, rt2, rn, imm8, puw);
	unsigned instruction = 0xe8500000;
	instruction += (puw & 0x6) << 22;
	instruction += (puw & 0x1) << 21;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += rt2 << 8;
	instruction += imm8;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	LDREX
//////////////////////////////////////////////////////////////////////////////////////
void execute_ldrex_t1 (unsigned rt, unsigned rn, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrex_t1: [%d] [%d] %x\n", rt, rn, imm8);
	unsigned instruction = 0xe8500f00;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += imm8;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	LDREXB
//////////////////////////////////////////////////////////////////////////////////////
void execute_ldrexb_t1 (unsigned rt, unsigned rn)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrexb_t1: [%d] [%d] %x\n", rt, rn);
	unsigned instruction = 0xe8d00f4f;
	instruction += rn << 16;
	instruction += rt << 12;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	LDREXH
//////////////////////////////////////////////////////////////////////////////////////
void execute_ldrexh_t1 (unsigned rt, unsigned rn)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrexh_t1: [%d] [%d] %x\n", rt, rn);
	unsigned instruction = 0xe8d00f5f;
	instruction += rn << 16;
	instruction += rt << 12;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	LDRH
//////////////////////////////////////////////////////////////////////////////////////
void execute_ldrh_imm_t1 (unsigned rt, unsigned rn, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrh_imm_t1: [%d] [%d] [%d]\n", rt, rn, imm5);
	unsigned instruction = 0x8800;
	instruction += rt & 0x7;
	instruction += (rn & 0x7) << 3;
	instruction += (imm5 & 0x1f) << 6;
	execute_instruction16(instruction);
};

void execute_ldrh_imm_t2 (unsigned rt, unsigned rn, unsigned imm12)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrh_imm_t2: [%d] [%d] %x\n", rt, rn, imm12);
	unsigned instruction = 0xf8b00000;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += imm12;
	execute_instruction32(instruction);
};

void execute_ldrh_imm_t3 (unsigned rt, unsigned rn, unsigned imm8, unsigned puw)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrh_imm_t3: [%d] [%d] %x %x\n", rt, rn, imm8, puw);
	unsigned instruction = 0xf8300800;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += puw << 8;
	instruction += imm8;
	execute_instruction32(instruction);
};

void execute_ldrh_lit_t1 (unsigned rt, unsigned u, unsigned imm12)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrh_lit_t1: [%d] %x %x\n", rt, u, imm12);
	unsigned instruction = 0xf83f0000;
	instruction += u << 23;
	instruction += rt << 12;
	instruction += imm12;
	execute_instruction32(instruction);
};

void execute_ldrh_reg_t1 (unsigned rt, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrh_reg_t1: [%d] [%d] [%d]\n", rt, rn, rm);
	unsigned instruction = 0x5a00;
	instruction += rt & 0x7;
	instruction += (rn & 0x7) << 3;
	instruction += (rm & 0x7) << 6;
	execute_instruction16(instruction);
};

void execute_ldrh_reg_t2 (unsigned rt, unsigned rn, unsigned rm, unsigned shift)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrh_reg_t2: [%d] [%d] %x %x\n", rt, rn, rm, shift);
	unsigned instruction = 0xf8300000;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += (shift & 0x3) << 4;
	instruction += rm;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	LDRHT
//////////////////////////////////////////////////////////////////////////////////////
void execute_ldrht_t1 (unsigned rt, unsigned rn, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrht_t1: [%d] [%d] %x\n", rt, rn, imm8);
	unsigned instruction = 0xf8300e00;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += imm8;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	LDRSB
//////////////////////////////////////////////////////////////////////////////////////
void execute_ldrsb_imm_t1 (unsigned rt, unsigned rn, unsigned imm12)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrsb_imm_t1: [%d] [%d] %x\n", rt, rn, imm12);
	unsigned instruction = 0xf9900000;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += imm12;
	execute_instruction32(instruction);
};

void execute_ldrsb_imm_t2 (unsigned rt, unsigned rn, unsigned imm8, unsigned puw)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrsb_imm_t2: [%d] [%d] %x %x\n", rt, rn, imm8, puw);
	unsigned instruction = 0xf9100800;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += puw << 8;
	instruction += imm8;
	execute_instruction32(instruction);
};

void execute_ldrsb_lit_t1 (unsigned rt, unsigned u, unsigned imm12)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrsb_lit_t1: [%d] %x %x\n", rt, u, imm12);
	unsigned instruction = 0xf91f0000;
	instruction += u << 23;
	instruction += rt << 12;
	instruction += imm12;
	execute_instruction32(instruction);
};

void execute_ldrsb_reg_t1 (unsigned rt, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrsb_reg_t1: [%d] [%d] [%d]\n", rt, rn, rm);
	unsigned instruction = 0x5600;
	instruction += rt & 0x7;
	instruction += (rn & 0x7) << 3;
	instruction += (rm & 0x7) << 6;
	execute_instruction16(instruction);
};

void execute_ldrsb_reg_t2 (unsigned rt, unsigned rn, unsigned rm, unsigned shift)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrsb_reg_t2: [%d] [%d] %x %x\n", rt, rn, rm, shift);
	unsigned instruction = 0xf9100000;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += (shift & 0x3) << 4;
	instruction += rm;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	LDRSBT
//////////////////////////////////////////////////////////////////////////////////////
void execute_ldrsbt_t1 (unsigned rt, unsigned rn, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrsbt_t1: [%d] [%d] %x\n", rt, rn, imm8);
	unsigned instruction = 0xf9100e00;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += imm8;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	LDRSH
//////////////////////////////////////////////////////////////////////////////////////
void execute_ldrsh_imm_t1 (unsigned rt, unsigned rn, unsigned imm12)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrsh_imm_t1: [%d] [%d] %x\n", rt, rn, imm12);
	unsigned instruction = 0xf9b00000;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += imm12;
	execute_instruction32(instruction);
};

void execute_ldrsh_imm_t2 (unsigned rt, unsigned rn, unsigned imm8, unsigned puw)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrsh_imm_t2: [%d] [%d] %x %x\n", rt, rn, imm8, puw);
	unsigned instruction = 0xf9300800;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += puw << 8;
	instruction += imm8;
	execute_instruction32(instruction);
};

void execute_ldrsh_lit_t1 (unsigned rt, unsigned u, unsigned imm12)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrsh_lit_t1: [%d] %x %x\n", rt, u, imm12);
	unsigned instruction = 0xf93f0000;
	instruction += u << 23;
	instruction += rt << 12;
	instruction += imm12;
	execute_instruction32(instruction);
};

void execute_ldrsh_reg_t1 (unsigned rt, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrsh_reg_t1: [%d] [%d] [%d]\n", rt, rn, rm);
	unsigned instruction = 0x5e00;
	instruction += rt & 0x7;
	instruction += (rn & 0x7) << 3;
	instruction += (rm & 0x7) << 6;
	execute_instruction16(instruction);
};

void execute_ldrsh_reg_t2 (unsigned rt, unsigned rn, unsigned rm, unsigned shift)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrsh_reg_t2: [%d] [%d] %x %x\n", rt, rn, rm, shift);
	unsigned instruction = 0xf9300000;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += (shift & 0x3) << 4;
	instruction += rm;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	LDRSHT
//////////////////////////////////////////////////////////////////////////////////////
void execute_ldrsht_t1 (unsigned rt, unsigned rn, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrsht_t1: [%d] [%d] %x\n", rt, rn, imm8);
	unsigned instruction = 0xf9300e00;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += imm8;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	LDRT
//////////////////////////////////////////////////////////////////////////////////////
void execute_ldrt_t1 (unsigned rt, unsigned rn, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ldrt_t1: [%d] [%d] %x\n", rt, rn, imm8);
	unsigned instruction = 0xf8500e00;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += imm8;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	LSL
//////////////////////////////////////////////////////////////////////////////////////
void execute_lsl_reg_t1 (unsigned rdn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_lsl_reg_t1: [%d] = [%d] + [%d]\n", rdn, rdn, rm);
	unsigned instruction = 0x4080;
	instruction += rdn & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_lsl_reg_t2 (unsigned rd, unsigned rn, unsigned rm, bool setflag)
{
	fprintf(disasm, "-----------------------------------------\nexecute_lsl_reg_t2: [%d] = [%d] + [%d] setflag: %d\n", rd, rn, rm, setflag);
	unsigned instruction = 0xfa00f000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_lsl_reg (int rn, int rm, int rd, unsigned a, unsigned b)
{
	// missing:		rm = 2^8 ??
	// missing: 	c when b==0
	bool set_c = true;
      bool this_c = true;
      bool set_z = true;
      bool this_z = false;
      bool set_n = true;
      bool this_n = false;
      bool set_v = false;
      bool this_v = false;

	// missing: carry
	//set_c = false;

	fprintf(disasm, "-----------------------------------------\ntest_lsl_reg:\n");
	unsigned result = a << b;
	//if (((a >> 31) & 0x1) == 0x1)
	//	result += ~(0xffffffff >> b);

	if (b == 0)
		set_c = false;

	if (((a >> (32 - b)) & 0x1) == 0x1)
		this_c = true;
	else
		this_c = false;

	if (result == 0)
		this_z = true;
	else
		this_z = false;

	if (((result >> 31) & 0x1) == 0x1)
		this_n = true;
	else
		this_n = false;

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	write_r(rd, a);
	execute_lsl_reg_t1(rd, rm);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_lsl_reg_t2(rd, rn, rm, false);
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_lsl_reg_t2(rd, rn, rm, true);
	check_flag(true);
	check_rd(rd, result, 3);

}

void execute_lsl_imm_t1 (unsigned rd, unsigned rm, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_lsl_imm_t1: [%d] = [%d] + [%d]\n", rd, rm, imm5);
	unsigned instruction = 0x0000;
	instruction += (imm5 & 0x1f) << 6;
	instruction += (rm & 0x7) << 3;
	instruction += rd & 0x7;
	execute_instruction16(instruction);
};

void execute_lsl_imm_t2 (unsigned rd, unsigned rm, bool setflag, unsigned imm3, unsigned imm2)
{
	fprintf(disasm, "-----------------------------------------\nexecute_lsl_imm_t2: [%d] = [%d] + %x%x setflag: %d\n", rd, rm, imm3, imm2, setflag);
	unsigned instruction = 0xea4f0000;
	instruction += (imm3 & 0x7) << 12;
	instruction += rd << 8;
	instruction += (imm2 & 0x3) << 6;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_lsl_imm (int rn, int rd, unsigned a, unsigned b)
{
	// missing:		rm = 2^8 ??
	// missing: 	c when b==0
	bool set_c = true;
      bool this_c = true;
      bool set_z = true;
      bool this_z = false;
      bool set_n = true;
      bool this_n = false;
      bool set_v = false;
      bool this_v = false;

	// missing: carry
	//set_c = false;

	fprintf(disasm, "-----------------------------------------\ntest_lsl_imm:\n");
	unsigned result = a << b;
	//if (((a >> 31) & 0x1) == 0x1)
	//	result += ~(0xffffffff >> b);

	if (b == 0)
		set_c = false;

	if (((a >> (32 - b)) & 0x1) == 0x1)
		this_c = true;
	else
		this_c = false;

	if (result == 0)
		this_z = true;
	else
		this_z = false;

	if (((result >> 31) & 0x1) == 0x1)
		this_n = true;
	else
		this_n = false;

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_lsl_imm_t1(rd, rn, (b & 0x1f));
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_lsl_imm_t2(rd, rn, false, ((b >> 2) & 0x7), (b & 0x3));
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_lsl_imm_t2(rd, rn, true, ((b >> 2) & 0x7), (b & 0x3));
	check_flag(true);
	check_rd(rd, result, 3);

}

//////////////////////////////////////////////////////////////////////////////////////
//	LSR
//////////////////////////////////////////////////////////////////////////////////////
void execute_lsr_reg_t1 (unsigned rdn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_lsr_reg_t1: [%d] = [%d] + [%d]\n", rdn, rdn, rm);
	unsigned instruction = 0x40c0;
	instruction += rdn & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_lsr_reg_t2 (unsigned rd, unsigned rn, unsigned rm, bool setflag)
{
	fprintf(disasm, "-----------------------------------------\nexecute_lsr_reg_t2: [%d] = [%d] + [%d] setflag: %d\n", rd, rn, rm, setflag);
	unsigned instruction = 0xfa20f000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_lsr_reg (int rn, int rm, int rd, unsigned a, unsigned b)
{
	// missing:		rm = 2^8 ??
	// missing: 	c when b==0
	bool set_c = true;
      bool this_c = true;
      bool set_z = true;
      bool this_z = false;
      bool set_n = true;
      bool this_n = false;
      bool set_v = false;
      bool this_v = false;

	fprintf(disasm, "-----------------------------------------\ntest_lsr_reg:\n");
	unsigned result = a >> b;

	if (b == 0)
		set_c = false;

	if (((a >> (b - 1)) & 0x1) == 0x1)
		this_c = true;
	else
		this_c = false;

	if (result == 0)
		this_z = true;
	else
		this_z = false;

	if (((result >> 31) & 0x1) == 0x1)
		this_n = true;
	else
		this_n = false;

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	write_r(rd, a);
	execute_lsr_reg_t1(rd, rm);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_lsr_reg_t2(rd, rn, rm, false);
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_lsr_reg_t2(rd, rn, rm, true);
	check_flag(true);
	check_rd(rd, result, 3);

}

void execute_lsr_imm_t1 (unsigned rd, unsigned rm, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_lsr_imm_t1: [%d] = [%d] + %x\n", rd, rm, imm5);
	unsigned instruction = 0x0800;
	instruction += (imm5 & 0x1f) << 6;
	instruction += (rm & 0x7) << 3;
	instruction += rd & 0x7;
	execute_instruction16(instruction);
};

void execute_lsr_imm_t2 (unsigned rd, unsigned rm, bool setflag, unsigned imm3, unsigned imm2)
{
	fprintf(disasm, "-----------------------------------------\nexecute_lsr_imm_t2: [%d] = [%d] + %x%x setflag: %d\n", rd, rm, imm3, imm2, setflag);
	unsigned instruction = 0xea4f0010;
	instruction += (imm3 & 0x7) << 12;
	instruction += rd << 8;
	instruction += (imm2 & 0x3) << 6;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_lsr_imm (int rn, int rd, unsigned a, unsigned b)
{
	// missing:		rm = 2^8 ??
	// missing: 	c when b==0
	bool set_c = true;
      bool this_c = true;
      bool set_z = true;
      bool this_z = false;
      bool set_n = true;
      bool this_n = false;
      bool set_v = false;
      bool this_v = false;

	fprintf(disasm, "-----------------------------------------\ntest_lsr_imm:\n");
	unsigned result = a >> b;

	if (b == 0)
		set_c = false;

	if (((a >> (b - 1)) & 0x1) == 0x1)
		this_c = true;
	else
		this_c = false;

	if (result == 0)
		this_z = true;
	else
		this_z = false;

	if (((result >> 31) & 0x1) == 0x1)
		this_n = true;
	else
		this_n = false;

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_lsr_imm_t1(rd, rn, (b & 0x1f));
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_lsr_imm_t2(rd, rn, false, ((b >> 2) & 0x7), (b & 0x3));
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_lsr_imm_t2(rd, rn, true, ((b >> 2) & 0x7), (b & 0x3));
	check_flag(true);
	check_rd(rd, result, 3);

}

//////////////////////////////////////////////////////////////////////////////////////
//	MUL, MLA, MLS
//////////////////////////////////////////////////////////////////////////////////////
void execute_mul_t1 (unsigned rdn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_mul_t1: [%d] = [%d] + [%d] + c\n", rdn, rdn, rm);
	unsigned instruction = 0x4340;
	instruction += rdn & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_mul_t2 (unsigned rd, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_mul_t2: [%d] = [%d] * [%d] setflag: %d\n", rd, rn, rm);
	unsigned instruction = 0xfb00f000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += rm;
	execute_instruction32(instruction);
};

void execute_mla_t1 (unsigned rd, unsigned ra, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_mla_t1: [%d] = [%d] * [%d] setflag: %d\n", rd, rn, rm);
	unsigned instruction = 0xfb000000;
	instruction += rn << 16;
	instruction += ra << 12;
	instruction += rd << 8;
	instruction += rm;
	execute_instruction32(instruction);
};

void execute_mls_t1 (unsigned rd, unsigned ra, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_mls_t1: [%d] = [%d] * [%d] setflag: %d\n", rd, rn, rm);
	unsigned instruction = 0xfb000010;
	instruction += rn << 16;
	instruction += ra << 12;
	instruction += rd << 8;
	instruction += rm;
	execute_instruction32(instruction);
};

void test_mul (int ra, int rn, int rm, int rd, unsigned added, unsigned a, unsigned b)
{
	bool set_c = false;
      bool this_c = false;
      bool set_z = true;
      bool this_z = false;
      bool set_n = true;
      bool this_n = false;
      bool set_v = false;
      bool this_v = false;

	fprintf(disasm, "-----------------------------------------\ntest_mul:\n");
	unsigned result = a * b;
	if (result == 0x0)
		this_z = true;
	if (((result >> 31) & 0x1) == 0x1)
		this_n = true;

	//printf("%8.8x * %8.8x = %8.8x\n", a, b, result);

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	write_r(rd, a);
	execute_mul_t1(rd, rm);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_mul_t2(rd, rn, rm);
	check_flag(false);
	check_rd(rd, result, 3);

	unsigned result_add = result + added;
	write_r(rd, ~result_add);
	execute_mla_t1(rd, ra, rn, rm);
	check_rd(rd, result_add, 3);

	unsigned result_sub = added - result;
	write_r(rd, ~result_sub);
	execute_mls_t1(rd, ra, rn, rm);
	check_rd(rd, result_sub, 3);
}

//////////////////////////////////////////////////////////////////////////////////////
//	MOV
//////////////////////////////////////////////////////////////////////////////////////
void execute_mov_reg_t1 (unsigned rd, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_mov_reg_t1: [%d] = [%d] \n", rd, rm);
	unsigned instruction = 0x4600;
	instruction += rd & 0x7;
	instruction += (rd & 0x8) << 4;
	instruction += (rm & 0xf) << 3;
	execute_instruction16(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	ORN
//////////////////////////////////////////////////////////////////////////////////////
void execute_orn_reg_t1 (unsigned rd, unsigned rn, unsigned rm, bool setflag, unsigned type, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_orn_reg_t1: [%d] = [%d] + [%d] setflag: %d\n", rd, rn, rm, setflag);
	unsigned instruction = 0xea600000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += type << 4;
	instruction += (imm5 & 0x3) << 6;
	instruction += (imm5 & 0x1C) << 10;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_orn_reg (int rn, int rm, int rd, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	// missing: carry
	set_c = false;

	fprintf(disasm, "-----------------------------------------\ntest_orn_reg:\n");
	unsigned result = a | ~(b);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_orn_reg_t1(rd, rn, rm, false, 0, 0);
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_orn_reg_t1(rd, rn, rm, true, 0, 0);
	check_flag(true);
	check_rd(rd, result, 3);
}

void test_orn_dis_reg (int rn, int rm, int rd, unsigned a, unsigned b, bool this_c, unsigned type, unsigned imm5)
{
	// missing: carry

	fprintf(disasm, "-----------------------------------------\ntest_orn_dis_reg:\n");
	unsigned result = a | ~(b);

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
	execute_orn_reg_t1(rd, rn, rm, false, type, imm5);
//	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
//	c = !this_c;
	execute_orn_reg_t1(rd, rn, rm, true, type, imm5);
//	check_flag(true);
	check_rd(rd, result, 3);
}

void execute_orn_imm_t1 (unsigned rd, unsigned rn, bool setflag, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_orn_imm_t1: [%d] = [%d] & tei(i: %x imm3: %x imm8: %x) setflag: %d\n", rd, rn, i, imm3, imm8, setflag);
	unsigned instruction = 0xf0600000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += i << 26;
	instruction += imm3 << 12;
	instruction += imm8;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_orn_imm (int rn, int rd, unsigned a, unsigned i, unsigned imm3, unsigned imm8,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	fprintf(disasm, "-----------------------------------------\ntest_orn_imm:\n");
	unsigned result = a | ~imm8;

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_orn_imm_t1(rd, rn, false, 0, 0, imm8);
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, true, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	c = this_c;
	execute_orn_imm_t1(rd, rn, true, 0, 0, imm8);
	check_flag(true);
	check_rd(rd, result, 3);

}

void test_orn_tei_imm (int rn, int rd, unsigned a, unsigned b, bool this_c, unsigned i, unsigned imm3, unsigned imm8)
{
	// missing: carry

	fprintf(disasm, "-----------------------------------------\ntest_orn_tei_imm:\n");
	unsigned result = a | ~b;

	write_r(rd, ~result);
//	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_orn_imm_t1(rd, rn, false, i, imm3, imm8);
//	check_flag(false);
	check_rd(rd, result, 3);

//	write_r(rd, ~result);
//	set_flags(set_c, true, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
//	c = this_c;
//	execute_eor_imm_t1(rd, rn, true, i, imm3, imm8);
//	check_flag(true);
//	check_rd(rd, result, 3);

}

//////////////////////////////////////////////////////////////////////////////////////
//	ORR
//////////////////////////////////////////////////////////////////////////////////////
void execute_orr_reg_t1 (unsigned rdn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_orr_reg_t1: [%d] = [%d] + [%d]\n", rdn, rdn, rm);
	unsigned instruction = 0x4300;
	instruction += rdn & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_orr_reg_t2 (unsigned rd, unsigned rn, unsigned rm, bool setflag, unsigned type, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_orr_reg_t2: [%d] = [%d] + [%d] setflag: %d\n", rd, rn, rm, setflag);
	unsigned instruction = 0xea400000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += type << 4;
	instruction += (imm5 & 0x3) << 6;
	instruction += (imm5 & 0x1C) << 10;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_orr_reg (int rn, int rm, int rd, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	// missing: carry
	set_c = false;

	fprintf(disasm, "-----------------------------------------\ntest_orr_reg:\n");
	unsigned result = a | b;

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	write_r(rd, a);
	execute_orr_reg_t1(rd, rm);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_orr_reg_t2(rd, rn, rm, false, 0, 0);
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_orr_reg_t2(rd, rn, rm, true, 0, 0);
	check_flag(true);
	check_rd(rd, result, 3);
}

void test_orr_dis_reg (int rn, int rm, int rd, unsigned a, unsigned b, bool this_c, unsigned type, unsigned imm5)
{
	// missing: carry

	fprintf(disasm, "-----------------------------------------\ntest_orr_dis_reg:\n");
	unsigned result = a | b;

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
	execute_orr_reg_t2(rd, rn, rm, false, type, imm5);
//	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
//	c = !this_c;
	execute_orr_reg_t2(rd, rn, rm, true, type, imm5);
//	check_flag(true);
	check_rd(rd, result, 3);
}

void execute_orr_imm_t1 (unsigned rd, unsigned rn, bool setflag, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_orr_imm_t1: [%d] = [%d] & %x setflag: %d\n", rd, rn, imm8, setflag);
	unsigned instruction = 0xf0400000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += i << 26;
	instruction += imm3 << 12;
	instruction += imm8;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_orr_imm (int rn, int rd, unsigned a, unsigned i, unsigned imm3, unsigned imm8,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	fprintf(disasm, "-----------------------------------------\ntest_orr_imm:\n");
	unsigned result = a | imm8;

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_orr_imm_t1(rd, rn, false, 0, 0, imm8);
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, true, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	c = this_c;
	execute_orr_imm_t1(rd, rn, true, 0, 0, imm8);
	check_flag(true);
	check_rd(rd, result, 3);

}

void test_orr_tei_imm (int rn, int rd, unsigned a, unsigned b, bool this_c, unsigned i, unsigned imm3, unsigned imm8)
{
	// missing: carry

	fprintf(disasm, "-----------------------------------------\ntest_orr_tei_imm:\n");
	unsigned result = a | b;

	write_r(rd, ~result);
//	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_orr_imm_t1(rd, rn, false, i, imm3, imm8);
//	check_flag(false);
	check_rd(rd, result, 3);

//	write_r(rd, ~result);
//	set_flags(set_c, true, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
//	c = this_c;
//	execute_eor_imm_t1(rd, rn, true, i, imm3, imm8);
//	check_flag(true);
//	check_rd(rd, result, 3);

}

//////////////////////////////////////////////////////////////////////////////////////
//	POP
//////////////////////////////////////////////////////////////////////////////////////
void execute_pop_t1 (unsigned p, unsigned reglist)
{
	fprintf(disasm, "-----------------------------------------\nexecute_pop_t1: %x %x\n", p, reglist);
	unsigned instruction = 0xbc00;
	instruction += (p & 0x1) << 8;
	instruction += reglist & 0xff;
	execute_instruction16(instruction);
};

void execute_pop_t2 (unsigned p, unsigned m, unsigned reglist)
{
	fprintf(disasm, "-----------------------------------------\nexecute_pop_t2: %x %x %x\n", p, m, reglist);
	unsigned instruction = 0xe8bd0000;
	instruction += (p & 0x1) << 15;
	instruction += (m & 0x1) << 14;
	instruction += reglist & 0x1fff;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	PUSH
//////////////////////////////////////////////////////////////////////////////////////
void execute_push_t1 (unsigned m, unsigned reglist)
{
	fprintf(disasm, "-----------------------------------------\nexecute_push_t1: %x %x\n", m, reglist);
	unsigned instruction = 0xb400;
	instruction += (m & 0x1) << 8;
	instruction += reglist & 0xff;
	execute_instruction16(instruction);
};

void execute_push_t2 (unsigned m, unsigned reglist)
{
	fprintf(disasm, "-----------------------------------------\nexecute_push_t2: %x %x\n", m, reglist);
	unsigned instruction = 0xe92d0000;
	instruction += (m & 0x1) << 14;
	instruction += reglist & 0x1fff;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	RBIT
//////////////////////////////////////////////////////////////////////////////////////
void execute_rbit_t1 (unsigned rd, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_rbit_t1: [%d] = rbit [%d]\n", rd, rm);
	unsigned instruction = 0xfa90f0a0;
	instruction += rm << 16;
	instruction += rm;
	instruction += rd << 8;
	execute_instruction32(instruction);
};

void test_rbit (int rm, int rd, unsigned a)
{
	fprintf(disasm, "-----------------------------------------\ntest_rbit:\n");
	unsigned result = (a & 0x1) << 31;
	result += ((a & 0x2) >> 1) << 30;
	result += ((a & 0x4) >> 2) << 29;
	result += ((a & 0x8) >> 3) << 28;
	result += ((a & 0x10) >> 4) << 27;
	result += ((a & 0x20) >> 5) << 26;
	result += ((a & 0x40) >> 6) << 25;
	result += ((a & 0x80) >> 7) << 24;
	result += ((a & 0x100) >> 8) << 23;
	result += ((a & 0x200) >> 9) << 22;
	result += ((a & 0x400) >> 10) << 21;
	result += ((a & 0x800) >> 11) << 20;
	result += ((a & 0x1000) >> 12) << 19;
	result += ((a & 0x2000) >> 13) << 18;
	result += ((a & 0x4000) >> 14) << 17;
	result += ((a & 0x8000) >> 15) << 16;
	result += ((a & 0x10000) >> 16) << 15;
	result += ((a & 0x20000) >> 17) << 14;
	result += ((a & 0x40000) >> 18) << 13;
	result += ((a & 0x80000) >> 19) << 12;
	result += ((a & 0x100000) >> 20) << 11;
	result += ((a & 0x200000) >> 21) << 10;
	result += ((a & 0x400000) >> 22) << 9;
	result += ((a & 0x800000) >> 23) << 8;
	result += ((a & 0x1000000) >> 24) << 7;
	result += ((a & 0x2000000) >> 25) << 6;
	result += ((a & 0x4000000) >> 26) << 5;
	result += ((a & 0x8000000) >> 27) << 4;
	result += ((a & 0x10000000) >> 28) << 3;
	result += ((a & 0x20000000) >> 29) << 2;
	result += ((a & 0x40000000) >> 30) << 1;
	result += ((a & 0x80000000) >> 31) << 0;

	write_r(rd, ~result);
	execute_rbit_t1(rd, rm);
	check_rd(rd, result, 3);
}

//////////////////////////////////////////////////////////////////////////////////////
//	REV
//////////////////////////////////////////////////////////////////////////////////////
void execute_rev_t1 (unsigned rd, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_rev_t1: [%d] = rev [%d]\n", rd, rm);
	unsigned instruction = 0xba00;
	instruction += rd & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_rev_t2 (unsigned rd, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_rev_t2: [%d] = rev [%d]\n", rd, rm);
	unsigned instruction = 0xfa90f080;
	instruction += rm << 16;
	instruction += rm;
	instruction += rd << 8;
	execute_instruction32(instruction);
};

void execute_rev16_t1 (unsigned rd, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_rev16_t1: [%d] = rev16 [%d]\n", rd, rm);
	unsigned instruction = 0xba40;
	instruction += rd & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_rev16_t2 (unsigned rd, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_rev16_t2: [%d] = rev16 [%d]\n", rd, rm);
	unsigned instruction = 0xfa90f090;
	instruction += rm << 16;
	instruction += rm;
	instruction += rd << 8;
	execute_instruction32(instruction);
};

void execute_revsh_t1 (unsigned rd, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_revsh_t1: [%d] = revsh [%d]\n", rd, rm);
	unsigned instruction = 0xbac0;
	instruction += rd & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_revsh_t2 (unsigned rd, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_revsh_t2: [%d] = revsh [%d]\n", rd, rm);
	unsigned instruction = 0xfa90f0b0;
	instruction += rm << 16;
	instruction += rm;
	instruction += rd << 8;
	execute_instruction32(instruction);
};

void test_rev (int rm, int rd, unsigned a)
{
	fprintf(disasm, "-----------------------------------------\ntest_rev:\n");
	unsigned result = (a & 0xff) << 24;
	result += ((a & 0xff00) >> 8) << 16;
	result += ((a & 0xff0000) >> 16) << 8;
	result += (a & 0xff000000) >> 24;

	write_r(rd, ~result);
	execute_rev_t1(rd, rm);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	execute_rev_t2(rd, rm);
	check_rd(rd, result, 3);

	result = (a & 0xff) << 8;
	result += (a & 0xff00) >> 8;
	result += ((a & 0xff0000) >> 16) << 24;
	result += ((a & 0xff000000) >> 24) << 16;

	write_r(rd, ~result);
	execute_rev16_t1(rd, rm);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	execute_rev16_t2(rd, rm);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	result = (a & 0xff00) >> 8;
	if (((a & 0x80) >> 7) == 0x1) result += 0xffffff00;

	write_r(rd, ~result);
	execute_revsh_t1(rd, rm);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	execute_revsh_t2(rd, rm);
	check_rd(rd, result, 3);
}

//////////////////////////////////////////////////////////////////////////////////////
//	ROR
//////////////////////////////////////////////////////////////////////////////////////
void execute_ror_reg_t1 (unsigned rdn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ror_reg_t1: [%d] = [%d] + [%d]\n", rdn, rdn, rm);
	unsigned instruction = 0x41C0;
	instruction += rdn & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_ror_reg_t2 (unsigned rd, unsigned rn, unsigned rm, bool setflag)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ror_reg_t2: [%d] = [%d] + [%d] setflag: %d\n", rd, rn, rm, setflag);
	unsigned instruction = 0xfa60f000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_ror_reg (int rn, int rm, int rd, unsigned a, unsigned b)
{
	// missing:		rm = 2^8 ??
	// missing: 	c when b==0
	bool set_c = true;
      bool this_c = true;
      bool set_z = true;
      bool this_z = false;
      bool set_n = true;
      bool this_n = false;
      bool set_v = false;
      bool this_v = false;

	// missing: carry
	//set_c = false;

	fprintf(disasm, "-----------------------------------------\ntest_ror_reg:\n");
	unsigned result = a >> b;
	//if (((a >> 31) & 0x1) == 0x1)
	//	result += ~(0xffffffff >> b);
	result += a << (32 - b);

	if (b == 0)
		result = a;

	if (b == 0)
		set_c = false;
	else if (((a >> (b - 1)) & 0x1) == 0x1)
		this_c = true;
	else
		this_c = false;

	if (result == 0)
		this_z = true;
	else
		this_z = false;

	if (((result >> 31) & 0x1) == 0x1)
		this_n = true;
	else
		this_n = false;

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	write_r(rd, a);
	execute_ror_reg_t1(rd, rm);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_ror_reg_t2(rd, rn, rm, false);
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_ror_reg_t2(rd, rn, rm, true);
	check_flag(true);
	check_rd(rd, result, 3);

}

void execute_ror_imm_t1 (unsigned rd, unsigned rm, bool setflag, unsigned imm3, unsigned imm2)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ror_imm_t1: [%d] = [%d] + %x %x setflag: %d\n", rd, rm, imm3, imm2, setflag);
	unsigned instruction = 0xea4f0030;
	instruction += (imm3 & 0x7) << 12;
	instruction += rd << 8;
	instruction += (imm2 & 0x3) << 6;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_ror_imm (int rn, int rd, unsigned a, unsigned b)
{
	// missing:		rm = 2^8 ??
	// missing: 	c when b==0
	bool set_c = true;
      bool this_c = true;
      bool set_z = true;
      bool this_z = false;
      bool set_n = true;
      bool this_n = false;
      bool set_v = false;
      bool this_v = false;

	// missing: carry
	//set_c = false;

	fprintf(disasm, "-----------------------------------------\ntest_ror_imm:\n");
	unsigned result = a >> b;
	//if (((a >> 31) & 0x1) == 0x1)
	//	result += ~(0xffffffff >> b);
	result += a << (32 - b);

	if (b == 0)
		result = a;

	if (b == 0)
		set_c = false;
	else if (((a >> (b - 1)) & 0x1) == 0x1)
		this_c = true;
	else
		this_c = false;

	if (result == 0)
		this_z = true;
	else
		this_z = false;

	if (((result >> 31) & 0x1) == 0x1)
		this_n = true;
	else
		this_n = false;

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_ror_imm_t1(rd, rn, false, ((b >> 2) & 0x7), (b & 0x3));
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_ror_imm_t1(rd, rn, true, ((b >> 2) & 0x7), (b & 0x3));
	check_flag(true);
	check_rd(rd, result, 3);

}

//////////////////////////////////////////////////////////////////////////////////////
//	RRX
//////////////////////////////////////////////////////////////////////////////////////
void execute_rrx_reg_t1 (unsigned rd, unsigned rm, bool setflag)
{
	fprintf(disasm, "-----------------------------------------\nexecute_rrx_reg_t1: [%d] rrx [%d] setflag: %d\n", rd, rm, setflag);
	unsigned instruction = 0xea4f0030;
	instruction += rd << 8;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_rrx_reg (int rm, int rd, unsigned a)
{
	bool set_c = true;
      bool this_c = true;
      bool set_z = true;
      bool this_z = false;
      bool set_n = true;
      bool this_n = false;
      bool set_v = false;
      bool this_v = false;

	// missing: carry
	//set_c = false;

	fprintf(disasm, "-----------------------------------------\ntest_rrx_reg:\n");
	unsigned result = a >> 1;
	bool preset_c = true;
	result += 0x80000000;

	if (result == 0)
		this_z = true;
	else
		this_z = false;

	if (((result >> 31) & 0x1) == 0x1)
		this_n = true;
	else
		this_n = false;

	write_r(rd, ~result);
	set_flags(set_c, !preset_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_rrx_reg_t1(rd, rm, false);
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, !preset_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
      if (((a >> (0)) & 0x1) == 0x1)
		c = true;
	else
		c = false;
	execute_rrx_reg_t1(rd, rm, true);
	check_flag(true);
	check_rd(rd, result, 3);

	result = a >> 1;
	preset_c = false;

      if (((a >> (0)) & 0x1) == 0x1)
		this_c = true;
	else
		this_c = false;

	if (result == 0)
		this_z = true;
	else
		this_z = false;

	if (((result >> 31) & 0x1) == 0x1)
		this_n = true;
	else
		this_n = false;

	write_r(rd, ~result);
	set_flags(set_c, !preset_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_rrx_reg_t1(rd, rm, false);
	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, !preset_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
      if (((a >> (0)) & 0x1) == 0x1)
		c = true;
	else
		c = false;
	execute_rrx_reg_t1(rd, rm, true);
	check_flag(true);
	check_rd(rd, result, 3);

}

//////////////////////////////////////////////////////////////////////////////////////
//	RSB
//////////////////////////////////////////////////////////////////////////////////////
void execute_rsb_reg_t1 (unsigned rd, unsigned rn, unsigned rm, bool setflag, unsigned type, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_rsb_reg_t1: [%d] = [%d] - [%d]\n", rd, rn, rm);
	unsigned instruction = 0xebc00000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += type << 4;
	instruction += (imm5 & 0x3) << 6;
	instruction += (imm5 & 0x1C) << 10;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_rsb_reg (int rn, int rm, int rd, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	// missing: DN

	fprintf(disasm, "-----------------------------------------\ntest_rsb_reg:\n");
	unsigned result = b - a;

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_rsb_reg_t1(rd, rn, rm, true, 0, 0);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_rsb_reg_t1(rd, rn, rm, false, 0, 0);
	check_flag(false);
	check_rd(rd, result, 3);
}

void test_rsb_dis_reg (int rn, int rm, int rd, unsigned a, unsigned b, bool this_c, unsigned type, unsigned imm5)
{
	// missing: carry

	fprintf(disasm, "-----------------------------------------\ntest_rsb_dis_reg:\n");
	unsigned result = b - a;

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
	execute_rsb_reg_t1(rd, rn, rm, false, type, imm5);
//	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
//	c = !this_c;
	execute_rsb_reg_t1(rd, rn, rm, true, type, imm5);
//	check_flag(true);
	check_rd(rd, result, 3);

}

void execute_rsb_imm_t1 (unsigned rd, unsigned rn)
{
	fprintf(disasm, "-----------------------------------------\nexecute_rsb_imm_t1: [%d] = 0 - [%d]\n", rd, rn);
	unsigned instruction = 0x4240;
	instruction += rd & 0x7;
	instruction += (rn & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_rsb_imm_t2 (unsigned rd, unsigned rn, bool setflag, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_rsb_imm_t2: [%d] = [%d] + %x setflag: %d\n", rd, rn, imm8, setflag);
	unsigned instruction = 0xf1c00000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += (i & 0x1) << 26;
	instruction += (imm3 & 0x7) << 12;
	instruction += imm8;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_rsb_imm (int rn, int rm, int rd, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	fprintf(disasm, "-----------------------------------------\ntest_rsb_imm:\n");
	unsigned result = 0 - b; 

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_rsb_imm_t1(rd, rm);
//	check_flag(true);
	check_rd(rd, result, 3);

	result = (b & 0xff) - a; 

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_rsb_imm_t2(rd, rn, true, 0, 0, (b & 0xff));
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_rsb_imm_t2(rd, rn, false, 0, 0, (b & 0xff));
	check_flag(false);
	check_rd(rd, result, 3);

}

void test_rsb_tei_imm (int rn, int rd, unsigned a, unsigned b, bool this_c, unsigned i, unsigned imm3, unsigned imm8)
{
	// missing: carry

	fprintf(disasm, "-----------------------------------------\ntest_rsb_tei_imm:\n");
	unsigned result = b - a;

	write_r(rd, ~result);
//	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_rsb_imm_t2(rd, rn, false, i, imm3, imm8);
//	check_flag(false);
	check_rd(rd, result, 3);

//	write_r(rd, ~result);
//	set_flags(set_c, true, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
//	c = this_c;
//	execute_and_imm_t1(rd, rn, true, i, imm3, imm8);
//	check_flag(true);
//	check_rd(rd, result, 3);

}

//////////////////////////////////////////////////////////////////////////////////////
//	SBC
//////////////////////////////////////////////////////////////////////////////////////
void execute_sbc_reg_t1 (unsigned rdn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_sbc_reg_t1: [%d] = [%d] + [%d] + c\n", rdn, rdn, rm);
	unsigned instruction = 0x4180;
	instruction += rdn & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_sbc_reg_t2 (unsigned rd, unsigned rn, unsigned rm, bool setflag, unsigned type, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_sbc_reg_t2: [%d] = [%d] + [%d] setflag: %d\n", rd, rn, rm, setflag);
	unsigned instruction = 0xeb600000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += type << 4;
	instruction += (imm5 & 0x3) << 6;
	instruction += (imm5 & 0x1C) << 10;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_sbc_reg (int rn, int rm, int rd, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v,
                bool preset_c)
{
	// missing: DN

	fprintf(disasm, "-----------------------------------------\ntest_sbc_reg:\n");
	unsigned result = a - b;
      if (!preset_c) result -= 1;
      //result = 0xFFFFFFFF;

	set_flags(set_c, !preset_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	c = this_c;
	write_r(rd, a);
	execute_sbc_reg_t1(rd, rm);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, !preset_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	c = this_c;
	execute_sbc_reg_t2(rd, rn, rm, true, 0, 0);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, !preset_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	c = !preset_c; // this is not nec. true, but check_flag tests the inverse preset
	execute_sbc_reg_t2(rd, rn, rm, false, 0, 0);
	check_flag(false);
	check_rd(rd, result, 3);
}

void test_sbc_dis_reg (int rn, int rm, int rd, unsigned a, unsigned b, bool this_c, unsigned type, unsigned imm5)
{
	// missing: carry

	fprintf(disasm, "-----------------------------------------\ntest_sbc_dis_reg:\n");
	unsigned result = a - b;

	write_r(rd, ~result);
	set_flags(true, false, false, false, false, false, false, false);  // c, z, n, v
	execute_sbc_reg_t2(rd, rn, rm, false, type, imm5);
//	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(true, false, false, false, false, false, false, false);  // c, z, n, v
//	c = !this_c;
	execute_sbc_reg_t2(rd, rn, rm, true, type, imm5);
//	check_flag(true);
	check_rd(rd, result, 3);

}

void execute_sbc_imm_t1 (unsigned rd, unsigned rn, bool setflag, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_sbc_imm_t1: [%d] = [%d] + %x setflag: %d\n", rd, rn, imm8, setflag);
	unsigned instruction = 0xf1600000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += (i & 0x1) << 26;
	instruction += (imm3 & 0x7) << 12;
	instruction += imm8;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_sbc_imm (int rn, int rd, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v,
                bool preset_c)
{
	fprintf(disasm, "-----------------------------------------\ntest_sbc_imm:\n");
	unsigned result = a - (b & 0xff); 
      if (!preset_c) result -= 1;

	write_r(rd, ~result);
	set_flags(set_c, !preset_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	c = !this_c;
	execute_sbc_imm_t1(rd, rn, true, 0, 0, (b & 0xff));
	check_flag(true);
	check_rd(rd, result, 3);

	set_c = false;

	write_r(rd, ~result);
	set_flags(set_c, !preset_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	c = !this_c;
	execute_sbc_imm_t1(rd, rn, false, 0, 0, (b & 0xff));
	check_flag(false);
	check_rd(rd, result, 3);

}

void test_sbc_tei_imm (int rn, int rd, unsigned a, unsigned b, bool setflag, unsigned i, unsigned imm3, unsigned imm8, bool preset_c)
{
	bool set_c = true;
      bool this_c = false;
      bool set_z = false;
      bool this_z = false;
      bool set_n = false;
      bool this_n = false;
      bool set_v = false;
      bool this_v = false;

	fprintf(disasm, "-----------------------------------------\ntest_sbc_tei_imm:\n");
	unsigned result = a - b;
      if (!preset_c) result -= 1;

	write_r(rd, ~result);
	set_flags(set_c, (!preset_c), set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_sbc_imm_t1(rd, rn, false, i, imm3, imm8);
//	check_flag(false);
	check_rd(rd, result, 3);

//	write_r(rd, ~result);
//	set_flags(set_c, true, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
//	c = this_c;
//	execute_and_imm_t1(rd, rn, true, i, imm3, imm8);
//	check_flag(true);
//	check_rd(rd, result, 3);

}

//////////////////////////////////////////////////////////////////////////////////////
//	SBFX
//////////////////////////////////////////////////////////////////////////////////////
void execute_sbfx_t1 (unsigned rd, unsigned rn, int width, int lsb)
{
	fprintf(disasm, "-----------------------------------------\nexecute_sbfx_t1: [%d] = [%d] width: %d lsb: %d\n", rd, rn, width, lsb);
	unsigned instruction = 0xf3400000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += width - 1;
	instruction += (lsb & 0x3) << 6;
	instruction += ((lsb >> 2) & 0x7) << 12;
	execute_instruction32(instruction);
};

void test_sbfx (int rn, int rd, unsigned a, int width, int lsb)
{
	fprintf(disasm, "-----------------------------------------\ntest_sbfx:\n");

	unsigned mask = 0x0;
	int n = 0;
	for (n = 0; n < width; n++)
	{
		mask = mask << 1;
		mask += 1;
	}
	unsigned result = (a >> lsb) & mask;
	if (((a >> (lsb + width - 1)) & 0x1) == 0x1)
		result += ~mask;

	write_r(rd, ~result);
	execute_sbfx_t1(rd, rn, width, lsb);
	check_rd(rd, result, 3);
}

//////////////////////////////////////////////////////////////////////////////////////
//	SMULL, SMLAL
//////////////////////////////////////////////////////////////////////////////////////
void execute_smull_reg_t1 (unsigned rd, unsigned rd2, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_smull_reg_t1: [%d] = [%d] * [%d] setflag: %d\n", rd, rn, rm);
	unsigned instruction = 0xfb800000;
	instruction += rn << 16;
	instruction += rd2 << 12;
	instruction += rd << 8;
	instruction += rm;
	execute_instruction32(instruction);
};

void execute_smlal_reg_t1 (unsigned rd, unsigned rd2, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_smlal_reg_t1: [%d] = [%d] * [%d] setflag: %d\n", rd, rn, rm);
	unsigned instruction = 0xfbc00000;
	instruction += rn << 16;
	instruction += rd2 << 12;
	instruction += rd << 8;
	instruction += rm;
	execute_instruction32(instruction);
};

void test_smull_reg (int rn, int rm, int rd, int rd2, unsigned a, unsigned b)
{
	fprintf(disasm, "-----------------------------------------\ntest_smull_reg:\n");
	unsigned ua = a;
	unsigned ub = b;
	if (((a >> 31) & 0x1) == 0x1)
		ua = 0 - a;
	if (((b >> 31) & 0x1) == 0x1)
		ub = 0 - b;
	
	unsigned result_low = (ua * ub) & 0xffffffff;
	
	unsigned a0b0 = (ua & 0xffff) * (ub & 0xffff);
	unsigned mid0 = (((ua >> 16) & 0xffff) * (ub & 0xffff));
	unsigned mid1 = ((ua & 0xffff) * ((ub >> 16) & 0xffff));
	unsigned carry = ( ((a0b0 >> 16) & 0xffff) +
                         (mid0 & 0xffff) +
                         (mid1 & 0xffff) ) >> 16;
	unsigned result_high = (((ua >> 16) & 0xffff) * ((ub >> 16) & 0xffff)) +
                             ((mid0 >> 16) & 0xffff) +
                             ((mid1 >> 16) & 0xffff) + 
                             carry;

	if (a > 0)
	if (b > 0)
	if (	(((a >> 31) & 0x1) == 0x1)	&
		(((b >> 31) & 0x1) == 0x0)	)
	{
		result_low = 0 - result_low;
		result_high = (0 - result_high) - 1;
      } else
	if (	(((a >> 31) & 0x1) == 0x0)	&
		(((b >> 31) & 0x1) == 0x1)	)
	{
		result_low = 0 - result_low;
		result_high = (0 - result_high) - 1;
	}

	//printf("long_result %8.8x %8.8x\n", mid0, mid1);
	//printf("\nlong_result %8.8x %8.8x %8.8x %8.8x\n", a, b, result_high, result_low);

	unsigned result2_low = result_low * 2;
	mid0 = (result_low & 0xffff) * 2;
	mid1 = (((result_low >> 16) & 0xffff) * 2) + (mid0 >> 16);
	unsigned result2_high = (result_high * 2) + (mid1 >> 16);
	//printf("long_result %8.8x %8.8x %8.8x %8.8x\n", mid0, mid1, result2_high, result2_low);

	write_r(rd2, ~result_low);
	write_r(rd, ~result_high);

	execute_smull_reg_t1(rd, rd2, rn, rm);

	check_rd(rd2, result_low, 3);
	check_rd(rd, result_high, 3);

	
	execute_smlal_reg_t1(rd, rd2, rn, rm);

	check_rd(rd2, result2_low, 3);
	check_rd(rd, result2_high, 3);
}

//////////////////////////////////////////////////////////////////////////////////////
//	SSAT
//////////////////////////////////////////////////////////////////////////////////////
void execute_ssat_reg_t1 (unsigned rd, unsigned rn, unsigned sat, unsigned type, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ssat_reg_t1: [%d] = [%d] ssat [%d]\n", rd, rn, sat);
	unsigned instruction = 0xf3000000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += (type & 0x2) << 20;
	instruction += (imm5 & 0x3) << 6;
	instruction += (imm5 & 0x1C) << 10;
	instruction += sat - 1;
	execute_instruction32(instruction);
};

void test_ssat_reg (int rn, int rd, unsigned a, unsigned sat)
{
	fprintf(disasm, "-----------------------------------------\ntest_ssat_reg:\n");
	unsigned result = 0x0;

	unsigned mask = 0x0;
	int n = 0;
	for (n = 0; n < sat - 1; n++)
	{
		mask = mask << 1;
		mask += 1;
	}
	
	if (((a >> 31) & 0x1) == 0x1)
	{
		mask = ~mask;
		//printf("%8.8x\n", mask);
		if (a < mask)
			result = mask;
		else
			result = a;
	} else
	{
		if (a > mask)
			result = mask;
		else
			result = a;
	}
	//printf("%8.8x\n", result);

	write_r(rd, ~result);
	execute_ssat_reg_t1(rd, rn, sat, 0, 0);
	check_rd(rd, result, 3);

}

void test_ssat_dis_reg (int rn, int rd, unsigned a, unsigned sat, bool this_c, unsigned type, unsigned imm5)
{
	unsigned result = 0x0;

	unsigned mask = 0x0;
	int n = 0;
	for (n = 0; n < sat - 1; n++)
	{
		mask = mask << 1;
		mask += 1;
	}
	
	if (((a >> 31) & 0x1) == 0x1)
	{
		mask = ~mask;
		//printf("%8.8x\n", mask);
		if (a < mask)
			result = mask;
		else
			result = a;
	} else
	{
		if (a > mask)
			result = mask;
		else
			result = a;
	}
	//printf("%8.8x\n", result);

	write_r(rd, ~result);
	execute_ssat_reg_t1(rd, rn, sat, type, imm5);
	check_rd(rd, result, 3);
}

//////////////////////////////////////////////////////////////////////////////////////
//	STMDB 
//////////////////////////////////////////////////////////////////////////////////////
void execute_stmdb_t1 (unsigned rn, unsigned w, unsigned p, unsigned m, unsigned reglist)
{
	fprintf(disasm, "-----------------------------------------\nexecute_stmdb_stmea_t1: [%d] %x %x %x %x\n", rn, w, p, m, reglist);
	unsigned instruction = 0xe9000000;
	instruction += (w & 0x1) << 21;
	instruction += (rn & 0xf) << 16;
	instruction += (p & 0x1) << 15;
	instruction += (m & 0x1) << 14;
	instruction += reglist & 0x1fff;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	STMIA / STMFD
//////////////////////////////////////////////////////////////////////////////////////
void execute_stmia_t1 (unsigned rn, unsigned reglist)
{
	fprintf(disasm, "-----------------------------------------\nexecute_stmia_stmfd_t1: [%d] %x\n", rn, reglist);
	unsigned instruction = 0xc000;
	instruction += (rn & 0x7) << 8;
	instruction += reglist & 0xff;
	execute_instruction16(instruction);
};

void execute_stmia_t2 (unsigned rn, unsigned w, unsigned p, unsigned m, unsigned reglist)
{
	fprintf(disasm, "-----------------------------------------\nexecute_stmia_stmfd_t2: [%d] %x %x %x %x\n", rn, w, p, m, reglist);
	unsigned instruction = 0xe8800000;
	instruction += (w & 0x1) << 21;
	instruction += (rn & 0xf) << 16;
	instruction += (p & 0x1) << 15;
	instruction += (m & 0x1) << 14;
	instruction += reglist & 0x1fff;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	STR
//////////////////////////////////////////////////////////////////////////////////////
void execute_str_imm_t1 (unsigned rt, unsigned rn, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_str_imm_t1: [%d] [%d] [%d]\n", rt, rn, imm5);
	unsigned instruction = 0x6000;
	instruction += rt & 0x7;
	instruction += (rn & 0x7) << 3;
	instruction += (imm5 & 0x1f) << 6;
	execute_instruction16(instruction);
};

void execute_str_imm_t2 (unsigned rt, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_str_imm_t2: [%d] [%d] [%d]\n", rt, imm8);
	unsigned instruction = 0x9000;
	instruction += (rt & 0x7) << 8;
	instruction += imm8 & 0xff;
	execute_instruction16(instruction);
};

void execute_str_imm_t3 (unsigned rt, unsigned rn, unsigned imm12)
{
	fprintf(disasm, "-----------------------------------------\nexecute_str_imm_t3: [%d] [%d] %x\n", rt, rn, imm12);
	unsigned instruction = 0xf8c00000;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += imm12;
	execute_instruction32(instruction);
};

void execute_str_imm_t4 (unsigned rt, unsigned rn, unsigned imm8, unsigned puw)
{
	fprintf(disasm, "-----------------------------------------\nexecute_str_imm_t4: [%d] [%d] %x %x\n", rt, rn, puw, imm8);
	unsigned instruction = 0xf8400800;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += puw << 8;
	instruction += imm8;
	execute_instruction32(instruction);
};

void execute_str_reg_t1 (unsigned rt, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_str_reg_t1: [%d] [%d] [%d]\n", rt, rn, rm);
	unsigned instruction = 0x5000;
	instruction += rt & 0x7;
	instruction += (rn & 0x7) << 3;
	instruction += (rm & 0x7) << 6;
	execute_instruction16(instruction);
};

void execute_str_reg_t2 (unsigned rt, unsigned rn, unsigned rm, unsigned shift)
{
	fprintf(disasm, "-----------------------------------------\nexecute_str_imm_t2: [%d] [%d] %x %x\n", rt, rn, rm, shift);
	unsigned instruction = 0xf8400000;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += (shift & 0x3) << 4;
	instruction += rm;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	STRB
//////////////////////////////////////////////////////////////////////////////////////
void execute_strb_imm_t1 (unsigned rt, unsigned rn, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_strb_imm_t1: [%d] [%d] [%d]\n", rt, rn, imm5);
	unsigned instruction = 0x7000;
	instruction += rt & 0x7;
	instruction += (rn & 0x7) << 3;
	instruction += (imm5 & 0x1f) << 6;
	execute_instruction16(instruction);
};

void execute_strb_imm_t2 (unsigned rt, unsigned rn, unsigned imm12)
{
	fprintf(disasm, "-----------------------------------------\nexecute_strb_imm_t2: [%d] [%d] %x\n", rt, rn, imm12);
	unsigned instruction = 0xf8800000;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += imm12;
	execute_instruction32(instruction);
};

void execute_strb_imm_t3 (unsigned rt, unsigned rn, unsigned imm8, unsigned puw)
{
	fprintf(disasm, "-----------------------------------------\nexecute_strb_imm_t3: [%d] [%d] %x %x\n", rt, rn, imm8, puw);
	unsigned instruction = 0xf8000800;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += puw << 8;
	instruction += imm8;
	execute_instruction32(instruction);
};

void execute_strb_reg_t1 (unsigned rt, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_strb_reg_t1: [%d] [%d] [%d]\n", rt, rn, rm);
	unsigned instruction = 0x5400;
	instruction += rt & 0x7;
	instruction += (rn & 0x7) << 3;
	instruction += (rm & 0x7) << 6;
	execute_instruction16(instruction);
};

void execute_strb_reg_t2 (unsigned rt, unsigned rn, unsigned rm, unsigned shift)
{
	fprintf(disasm, "-----------------------------------------\nexecute_strb_reg_t2: [%d] [%d] %x %x\n", rt, rn, rm, shift);
	unsigned instruction = 0xf8000000;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += (shift & 0x3) << 4;
	instruction += rm;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	STRBT
//////////////////////////////////////////////////////////////////////////////////////
void execute_strbt_t1 (unsigned rt, unsigned rn, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_strbt_t1: [%d] [%d] %x\n", rt, rn, imm8);
	unsigned instruction = 0xf8000e00;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += imm8;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	STRD
//////////////////////////////////////////////////////////////////////////////////////
void execute_strd_imm_t1 (unsigned rt, unsigned rt2, unsigned rn, unsigned imm8, unsigned puw)
{
	fprintf(disasm, "-----------------------------------------\nexecute_strd_imm_t1: [%d] [%d] [%d] %x %x\n", rt, rt2, rn, imm8, puw);
	unsigned instruction = 0xe8400000;
	instruction += (puw & 0x6) << 22;
	instruction += (puw & 0x1) << 21;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += rt2 << 8;
	instruction += imm8;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	STREX
//////////////////////////////////////////////////////////////////////////////////////
void execute_strex_t1 (unsigned rt, unsigned rn, unsigned rd, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_strex_t1: [%d] [%d] %x\n", rt, rn, imm8);
	unsigned instruction = 0xe8400000;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += rd << 8;
	instruction += imm8;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	STREXB
//////////////////////////////////////////////////////////////////////////////////////
void execute_strexb_t1 (unsigned rt, unsigned rn, unsigned rd)
{
	fprintf(disasm, "-----------------------------------------\nexecute_strexb_t1: [%d] [%d] %x\n", rt, rn, rd);
	unsigned instruction = 0xe8c00f40;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += rd;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	STREXH
//////////////////////////////////////////////////////////////////////////////////////
void execute_strexh_t1 (unsigned rt, unsigned rn, unsigned rd)
{
	fprintf(disasm, "-----------------------------------------\nexecute_strexh_t1: [%d] [%d] [%d]\n", rt, rn, rd);
	unsigned instruction = 0xe8c00f50;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += rd;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	STRH
//////////////////////////////////////////////////////////////////////////////////////
void execute_strh_imm_t1 (unsigned rt, unsigned rn, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_strh_imm_t1: [%d] [%d] [%d]\n", rt, rn, imm5);
	unsigned instruction = 0x8000;
	instruction += rt & 0x7;
	instruction += (rn & 0x7) << 3;
	instruction += (imm5 & 0x1f) << 6;
	execute_instruction16(instruction);
};

void execute_strh_imm_t2 (unsigned rt, unsigned rn, unsigned imm12)
{
	fprintf(disasm, "-----------------------------------------\nexecute_strh_imm_t2: [%d] [%d] %x\n", rt, rn, imm12);
	unsigned instruction = 0xf8a00000;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += imm12;
	execute_instruction32(instruction);
};

void execute_strh_imm_t3 (unsigned rt, unsigned rn, unsigned imm8, unsigned puw)
{
	fprintf(disasm, "-----------------------------------------\nexecute_strh_imm_t3: [%d] [%d] %x %x\n", rt, rn, imm8, puw);
	unsigned instruction = 0xf8200800;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += puw << 8;
	instruction += imm8;
	execute_instruction32(instruction);
};

void execute_strh_reg_t1 (unsigned rt, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_strh_reg_t1: [%d] [%d] [%d]\n", rt, rn, rm);
	unsigned instruction = 0x5200;
	instruction += rt & 0x7;
	instruction += (rn & 0x7) << 3;
	instruction += (rm & 0x7) << 6;
	execute_instruction16(instruction);
};

void execute_strh_reg_t2 (unsigned rt, unsigned rn, unsigned rm, unsigned shift)
{
	fprintf(disasm, "-----------------------------------------\nexecute_strh_imm_t2: [%d] [%d] %x %x\n", rt, rn, rm, shift);
	unsigned instruction = 0xf8200000;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += (shift & 0x3) << 4;
	instruction += rm;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	STRHT
//////////////////////////////////////////////////////////////////////////////////////
void execute_strht_t1 (unsigned rt, unsigned rn, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_strht_t1: [%d] [%d] %x\n", rt, rn, imm8);
	unsigned instruction = 0xf8200e00;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += imm8;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	STRT
//////////////////////////////////////////////////////////////////////////////////////
void execute_strt_t1 (unsigned rt, unsigned rn, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_strt_t1: [%d] [%d] %x\n", rt, rn, imm8);
	unsigned instruction = 0xf8400e00;
	instruction += rn << 16;
	instruction += rt << 12;
	instruction += imm8;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	SUB
//////////////////////////////////////////////////////////////////////////////////////
void execute_sub_reg_t1 (unsigned rd, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_sub_reg_t1: [%d] = [%d] - [%d]\n", rd, rn, rm);
	unsigned instruction = 0x1a00;
	instruction += rd & 0x7;
	instruction += (rn & 0x7) << 3;
	instruction += (rm & 0x7) << 6;
	execute_instruction16(instruction);
};

void execute_sub_sp_reg_t1 (unsigned rd, unsigned rm, bool setflag, unsigned type, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_sub_sp_reg_t1: [%d] = 13 - [%d] setflag: %d\n", rd, rm, setflag);
	unsigned instruction = 0xebad0000;
	instruction += rd << 8;
	instruction += type << 4;
	instruction += (imm5 & 0x3) << 6;
	instruction += (imm5 & 0x1C) << 10;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void execute_sub_reg_t2 (unsigned rd, unsigned rn, unsigned rm, bool setflag, unsigned type, unsigned imm5)
{
	fprintf(disasm, "-----------------------------------------\nexecute_sub_reg_t2: [%d] = [%d] - [%d] setflag: %d\n", rd, rn, rm, setflag);
	unsigned instruction = 0xebA00000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += type << 4;
	instruction += (imm5 & 0x3) << 6;
	instruction += (imm5 & 0x1C) << 10;
	instruction += rm;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void test_sub_reg (int rn, int rm, int rd, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	// missing: DN

	fprintf(disasm, "-----------------------------------------\ntest_sub_reg:\n");
	unsigned result = a - b;

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_sub_reg_t1(rd, rn, rm);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_sub_reg_t2(rd, rn, rm, true, 0, 0);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_sub_reg_t2(rd, rn, rm, false, 0, 0);
	check_flag(false);
	check_rd(rd, result, 3);

	/////////////////////////////////////////////////////
	result = sp - b;
	write_r(rd, b);

	// missing: set/check flags correctly
	set_c = false;
	//set_z = false;
	this_z = false;
	set_n = false;
	set_v = false;

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_sub_sp_reg_t1(rd, rm, true, 0, 0);
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_sub_sp_reg_t1(rd, rm, false, 0, 0);
	check_flag(false);
	check_rd(rd, result, 3);
}

void test_sub_dis_reg (int rn, int rm, int rd, unsigned a, unsigned b, bool this_c, unsigned type, unsigned imm5)
{
	// missing: carry

	fprintf(disasm, "-----------------------------------------\ntest_sub_dis_reg:\n");
	unsigned result = a - b;

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
	execute_sub_reg_t2(rd, rn, rm, false, type, imm5);
//	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
//	c = !this_c;
	execute_sub_reg_t2(rd, rn, rm, true, type, imm5);
//	check_flag(true);
	check_rd(rd, result, 3);

	/////////////////////////////////////////////////////
	result = sp - b;
	write_r(rd, b);

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
	execute_sub_sp_reg_t1(rd, rm, false, type, imm5);
//	check_flag(false);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(true, true, false, false, false, false, false, false);  // c, z, n, v
//	c = !this_c;
	execute_sub_sp_reg_t1(rd, rm, true, type, imm5);
//	check_flag(true);
	check_rd(rd, result, 3);

}

void execute_sub_imm_t1 (unsigned rd, unsigned rn, unsigned imm3)
{
	fprintf(disasm, "-----------------------------------------\nexecute_sub_imm_t1: [%d] = [%d] - %x\n", rd, rn, imm3);
	unsigned instruction = 0x1e00;
	instruction += rd & 0x7;
	instruction += (rn & 0x7) << 3;
	instruction += (imm3 & 0x7) << 6;
	execute_instruction16(instruction);
};

void execute_sub_imm_t2 (unsigned rdn, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_sub_imm_t2: [%d] = [%d] - %x\n", rdn, rdn, imm8);
	unsigned instruction = 0x3800;
	instruction += (rdn & 0x7) << 8;
	instruction += imm8 & 0xff;
	execute_instruction16(instruction);
};

void execute_sub_imm_t3 (unsigned rd, unsigned rn, bool setflag, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_sub_imm_t3: [%d] = [%d] - %x setflag: %d\n", rd, rn, imm8, setflag);
	unsigned instruction = 0xf1a00000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += (i & 0x1) << 26;
	instruction += (imm3 & 0x7) << 12;
	instruction += imm8;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void execute_sub_imm_t4 (unsigned rd, unsigned rn, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_sub_imm_t4: [%d] = [%d] - %x\n", rd, rn, imm8);
	unsigned instruction = 0xf2a00000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += (i & 0x1) << 26;
	instruction += (imm3 & 0x7) << 12;
	instruction += imm8;
	execute_instruction32(instruction);
};

void execute_sub_sp_imm_t2 (unsigned rd, bool setflag, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_sub_sp_imm_t2: [%d] = sp - %x\n", rd, imm8);
	unsigned instruction = 0xf1ad0000;
	instruction += rd << 8;
	instruction += (i & 0x1) << 26;
	instruction += (imm3 & 0x7) << 12;
	instruction += imm8;
	if (setflag)
		instruction += 1 << 20;
	execute_instruction32(instruction);
};

void execute_sub_sp_imm_t3 (unsigned rd, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_sub_sp_imm_t3: [%d] = sp - %x\n", rd, imm8);
	unsigned instruction = 0xf2ad0000;
	instruction += rd << 8;
	instruction += (i & 0x1) << 26;
	instruction += (imm3 & 0x7) << 12;
	instruction += imm8;
	execute_instruction32(instruction);
};

void test_sub_imm (int rn, int rd, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	fprintf(disasm, "-----------------------------------------\ntest_sub_imm:\n");
	unsigned result = a - (b & 0x7); 

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_sub_imm_t1(rd, rn, (b & 0x7));
	check_flag(true);
	check_rd(rd, result, 3);

	result = a - (b & 0xff); 

	write_r(rd, a);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_sub_imm_t2(rd, (b & 0xff));
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_sub_imm_t3(rd, rn, true, 0, 0, (b & 0xff));
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_sub_imm_t3(rd, rn, false, 0, 0, (b & 0xff));
	check_flag(false);
	check_rd(rd, result, 3);

	result = a - (b & 0xfff); 

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_sub_imm_t4(rd, rn, ((b >> 11) & 0x1), ((b >> 8) & 0x7), (b & 0xff));
	check_flag(false);
	check_rd(rd, result, 3);

	/////////////////////////////////////////////////////
	result = sp - (b & 0xff);
//	write_r(rd, b);

	// sub_sp_t1 modifies sp itself, generates bugs

	// missing: set/check flags correctly
	set_c = false;
	//set_z = false;
	this_z = false;
	set_n = false;
	set_v = false;

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_sub_sp_imm_t2(rd, true, 0, 0, (b & 0xff));
	check_flag(true);
	check_rd(rd, result, 3);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_sub_sp_imm_t2(rd, false, 0, 0, (b & 0xff));
	check_flag(false);
	check_rd(rd, result, 3);

	result = sp - (b & 0xfff);

	write_r(rd, ~result);
	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_sub_sp_imm_t3(rd, ((b >> 11) & 0x1), ((b >> 8) & 0x7), (b & 0xff));
//	check_flag(false);
	check_rd(rd, result, 3);

}

void test_sub_tei_imm (int rn, int rd, unsigned a, unsigned b, bool this_c, unsigned i, unsigned imm3, unsigned imm8)
{
	// missing: carry

	fprintf(disasm, "-----------------------------------------\ntest_and_tei_imm:\n");
	unsigned result = a - b;

	write_r(rd, ~result);
//	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_sub_imm_t3(rd, rn, false, i, imm3, imm8);
//	check_flag(false);
	check_rd(rd, result, 3);

//	write_r(rd, ~result);
//	set_flags(set_c, true, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
//	c = this_c;
//	execute_and_imm_t1(rd, rn, true, i, imm3, imm8);
//	check_flag(true);
//	check_rd(rd, result, 3);

	result = sp - b;

	write_r(rd, ~result);
//	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_sub_sp_imm_t2(rd, false, i, imm3, imm8);
//	check_flag(false);
	check_rd(rd, result, 3);

}

//////////////////////////////////////////////////////////////////////////////////////
//	SXTB
//////////////////////////////////////////////////////////////////////////////////////
void execute_sxtb_t1 (unsigned rd, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_sxtb_t1: [%d] = [%d]\n", rd, rm);
	unsigned instruction = 0xb240;
	instruction += rd & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_sxtb_t2 (unsigned rd, unsigned rm, int rot)
{
	fprintf(disasm, "-----------------------------------------\nexecute_sxtb_t2: [%d] = [%d] rot: %d\n", rd, rm, rot);
	unsigned instruction = 0xfa4ff080;
	instruction += rd << 8;
	instruction += rm;
	instruction += (rot & 0x3) << 4;
	execute_instruction32(instruction);
};

void test_sxtb (int rn, int rd, unsigned a, int rot)
{
	fprintf(disasm, "-----------------------------------------\ntest_sxtb:\n");

	unsigned result = (a >> (8 * 0)) & 0xFF;
	if (((result >> 7) & 0x1) == 0x1)
		result += 0xFFFFFF00;

	write_r(rd, ~result);
	execute_sxtb_t1(rd, rn);
	check_rd(rd, result, 3);

	result = (a >> (8 * rot)) & 0xFF;
	if (((result >> 7) & 0x1) == 0x1)
		result += 0xFFFFFF00;

	write_r(rd, ~result);
	execute_sxtb_t2(rd, rn, rot);
	check_rd(rd, result, 3);
}

//////////////////////////////////////////////////////////////////////////////////////
//	SXTH
//////////////////////////////////////////////////////////////////////////////////////
void execute_sxth_t1 (unsigned rd, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_sxth_t1: [%d] = [%d]\n", rd, rm);
	unsigned instruction = 0xb200;
	instruction += rd & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_sxth_t2 (unsigned rd, unsigned rm, int rot)
{
	fprintf(disasm, "-----------------------------------------\nexecute_sxth_t2: [%d] = [%d] rot: %d\n", rd, rm, rot);
	unsigned instruction = 0xfa0ff080;
	instruction += rd << 8;
	instruction += rm;
	instruction += (rot & 0x3) << 4;
	execute_instruction32(instruction);
};

void test_sxth (int rn, int rd, unsigned a, int rot)
{
	fprintf(disasm, "-----------------------------------------\ntest_sxth:\n");

	unsigned result = (a >> (8 * 0)) & 0xFFFF;
	if (((result >> 15) & 0x1) == 0x1)
		result += 0xFFFF0000;

	write_r(rd, ~result);
	execute_sxth_t1(rd, rn);
	check_rd(rd, result, 3);

	result = (a >> (8 * rot)) & 0xFFFF;
	if (rot == 3)
		result += (a << 8) & 0xFF00;
	if (((result >> 15) & 0x1) == 0x1)
		result += 0xFFFF0000;

	write_r(rd, ~result);
	execute_sxth_t2(rd, rn, rot);
	check_rd(rd, result, 3);
}

//////////////////////////////////////////////////////////////////////////////////////
//	TBB
//////////////////////////////////////////////////////////////////////////////////////
void execute_tbb_t1 (unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_tbb_t1: [%d], [%d]\n", rn, rm);
	unsigned instruction = 0xe8d0f000;
	instruction += (rn & 0xf) << 16;
	instruction += rm;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	TBH
//////////////////////////////////////////////////////////////////////////////////////
void execute_tbh_t1 (unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_tbh_t1: [%d], [%d]\n", rn, rm);
	unsigned instruction = 0xe8d0f010;
	instruction += (rn & 0xf) << 16;
	instruction += rm;
	execute_instruction32(instruction);
};

//////////////////////////////////////////////////////////////////////////////////////
//	TEQ
//////////////////////////////////////////////////////////////////////////////////////
void execute_teq_reg_t1 (unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_teq_reg_t1: [%d] teq [%d]\n", rn, rm);
	unsigned instruction = 0xea900f00;
	instruction += rn << 16;
	instruction += rm;
	execute_instruction32(instruction);
};

void test_teq_reg (int rn, int rm, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	// missing: carry
	set_c = false;

	fprintf(disasm, "-----------------------------------------\ntest_teq_reg:\n");

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_teq_reg_t1(rn, rm);
	check_flag(true);
}

void execute_teq_imm_t1 (unsigned rn, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_teq_imm_t1: [%d] teq %d\n", rn, imm8);
	unsigned instruction = 0xf0900f00;
	instruction += rn << 16;
	instruction += i << 26;
	instruction += imm3 << 12;
	instruction += imm8;
	execute_instruction32(instruction);
};

void test_teq_imm (int rn, unsigned a, unsigned i, unsigned imm3, unsigned imm8,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	// missing: carry
	set_c = false;

	fprintf(disasm, "-----------------------------------------\ntest_teq_imm: [%d] %x i: %x imm3: %x imm8: %x\n", rn, a, i, imm3, imm8);

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_teq_imm_t1(rn, 0, 0, imm8);
	check_flag(true);
}

void test_teq_tei_imm (int rn, int rd, unsigned a, unsigned b, bool setflag, unsigned i, unsigned imm3, unsigned imm8)
{
	bool set_c = false;
      bool this_c = false;
      bool set_z = false;
      bool this_z = false;
      bool set_n = false;
      bool this_n = false;
      bool set_v = false;
      bool this_v = false;

	fprintf(disasm, "-----------------------------------------\ntest_teq_tei_imm:\n");
	unsigned result = a | b;

//	if (((a >> (b - 1)) & 0x1) == 0x1)
//		this_c = true;
//	else
//		this_c = false;

	if (result == 0)
		this_z = true;
	else
		this_z = false;

	if (((result >> 31) & 0x1) == 0x1)
		this_n = true;
	else
		this_n = false;

//	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_teq_imm_t1(rn, i, imm3, imm8);
//	check_flag(true);

}

//////////////////////////////////////////////////////////////////////////////////////
//	TST
//////////////////////////////////////////////////////////////////////////////////////
void execute_tst_reg_t1 (unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_tst_reg_t1: [%d] tst [%d]\n", rn, rm);
	unsigned instruction = 0x4200;
	instruction += rn & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_tst_reg_t2 (unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_tst_reg_t2: [%d] tst [%d]\n", rn, rm);
	unsigned instruction = 0xea100f00;
	instruction += rn << 16;
	instruction += rm;
	execute_instruction32(instruction);
};

void test_tst_reg (int rn, int rm, unsigned a, unsigned b,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	// missing: carry
	set_c = false;

	fprintf(disasm, "-----------------------------------------\ntest_tst_reg:\n");

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_tst_reg_t1(rn, rm);
	check_flag(true);

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_tst_reg_t2(rn, rm);
	check_flag(true);
}

void execute_tst_imm_t1 (unsigned rn, unsigned i, unsigned imm3, unsigned imm8)
{
	fprintf(disasm, "-----------------------------------------\nexecute_tst_imm_t1: [%d] teq %d\n", rn, imm8);
	unsigned instruction = 0xf0100f00;
	instruction += rn << 16;
	instruction += i << 26;
	instruction += imm3 << 12;
	instruction += imm8;
	execute_instruction32(instruction);
};

void test_tst_imm (int rn, unsigned a, unsigned i, unsigned imm3, unsigned imm8,
		    bool set_c, bool this_c, 
                bool set_z, bool this_z, 
                bool set_n, bool this_n, 
                bool set_v, bool this_v)
{
	// missing: carry
	set_c = false;

	fprintf(disasm, "-----------------------------------------\ntest_tst_imm:\n");

	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_tst_imm_t1(rn, 0, 0, imm8);
	check_flag(true);
}

void test_tst_tei_imm (int rn, int rd, unsigned a, unsigned b, bool setflag, unsigned i, unsigned imm3, unsigned imm8)
{
	bool set_c = false;
      bool this_c = false;
      bool set_z = false;
      bool this_z = false;
      bool set_n = false;
      bool this_n = false;
      bool set_v = false;
      bool this_v = false;

	fprintf(disasm, "-----------------------------------------\ntest_teq_tei_imm:\n");
	unsigned result = a & b;

//	if (((a >> (b - 1)) & 0x1) == 0x1)
//		this_c = true;
//	else
//		this_c = false;

	if (result == 0)
		this_z = true;
	else
		this_z = false;

	if (((result >> 31) & 0x1) == 0x1)
		this_n = true;
	else
		this_n = false;

//	set_flags(set_c, this_c, set_z, this_z, set_n, this_n, set_v, this_v);  // c, z, n, v
	execute_tst_imm_t1(rn, i, imm3, imm8);
//	check_flag(true);

}

//////////////////////////////////////////////////////////////////////////////////////
//	UBFX
//////////////////////////////////////////////////////////////////////////////////////
void execute_ubfx_t1 (unsigned rd, unsigned rn, int width, int lsb)
{
	fprintf(disasm, "-----------------------------------------\nexecute_ubfx_t1: [%d] = [%d] %d %d\n", rd, rn, width, lsb);
	unsigned instruction = 0xf3c00000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += width - 1;
	instruction += (lsb & 0x3) << 6;
	instruction += ((lsb >> 2) & 0x7) << 12;
	execute_instruction32(instruction);
};

void test_ubfx (int rn, int rd, unsigned a, int width, int lsb)
{
	fprintf(disasm, "-----------------------------------------\ntest_ubfx:\n");

	unsigned mask = 0x0;
	int n = 0;
	for (n = 0; n < width; n++)
	{
		mask = mask << 1;
		mask += 1;
	}
	unsigned result = (a >> lsb) & mask;
	//if (((a >> (lsb + width - 1)) & 0x1) == 0x1)
	//	result += ~mask;

	execute_ubfx_t1(rd, rn, width, lsb);
	check_rd(rd, result, 3);
}

//////////////////////////////////////////////////////////////////////////////////////
//	UMULL, UMLAL
//////////////////////////////////////////////////////////////////////////////////////
void execute_umul_reg_t1 (unsigned rd, unsigned rd2, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_umul_reg_t1: [%d] = [%d] * [%d] setflag: %d\n", rd, rn, rm);
	unsigned instruction = 0xfba00000;
	instruction += rn << 16;
	instruction += rd2 << 12;
	instruction += rd << 8;
	instruction += rm;
	execute_instruction32(instruction);
};

void execute_umlal_reg_t1 (unsigned rd, unsigned rd2, unsigned rn, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_umlal_reg_t1: [%d] = [%d] * [%d] setflag: %d\n", rd, rn, rm);
	unsigned instruction = 0xfbe00000;
	instruction += rn << 16;
	instruction += rd2 << 12;
	instruction += rd << 8;
	instruction += rm;
	execute_instruction32(instruction);
};

void test_umull_reg (int rn, int rm, int rd, int rd2, unsigned a, unsigned b)
{
	fprintf(disasm, "-----------------------------------------\ntest_umull_reg:\n");
	long long_result = a * b;
	unsigned result_low = (a * b) & 0xffffffff;
	
	unsigned a0b0 = (a & 0xffff) * (b & 0xffff);
	unsigned mid0 = (((a >> 16) & 0xffff) * (b & 0xffff));
	unsigned mid1 = ((a & 0xffff) * ((b >> 16) & 0xffff));
	unsigned carry = ( ((a0b0 >> 16) & 0xffff) +
                         (mid0 & 0xffff) +
                         (mid1 & 0xffff) ) >> 16;
	unsigned result_high = (((a >> 16) & 0xffff) * ((b >> 16) & 0xffff)) +
                             ((mid0 >> 16) & 0xffff) +
                             ((mid1 >> 16) & 0xffff) + 
                             carry;
	//printf("long_result %8.8x %8.8x\n", mid0, mid1);
	//printf("long_result %8.8x %8.8x %8.8x%8.8x\n", a, b, result_high, result_low);
	unsigned result2_low = result_low * 2;
	mid0 = (result_low & 0xffff) * 2;
	mid1 = (((result_low >> 16) & 0xffff) * 2) + (mid0 >> 16);
	unsigned result2_high = (result_high * 2) + (mid1 >> 16);
	//printf("long_result %8.8x %8.8x %8.8x%8.8x\n", mid0, mid1, result2_high, result2_low);
	
	write_r(rd2, ~result_low);
	write_r(rd, ~result_high);

	execute_umul_reg_t1(rd, rd2, rn, rm);

	check_rd(rd2, result_low, 3);
	check_rd(rd, result_high, 3);

	execute_umlal_reg_t1(rd, rd2, rn, rm);

	check_rd(rd2, result2_low, 3);
	check_rd(rd, result2_high, 3);
}

//////////////////////////////////////////////////////////////////////////////////////
//	USAT
//////////////////////////////////////////////////////////////////////////////////////
void execute_usat_reg_t1 (unsigned rd, unsigned rn, unsigned sat)
{
	fprintf(disasm, "-----------------------------------------\nexecute_usat_reg_t1: [%d] = [%d] usat [%d]\n", rd, rn, sat);
	unsigned instruction = 0xf3800000;
	instruction += rn << 16;
	instruction += rd << 8;
	instruction += sat - 1;
	execute_instruction32(instruction);
};

void test_usat_reg (int rn, int rd, unsigned a, unsigned sat)
{
	fprintf(disasm, "-----------------------------------------\ntest_usat_reg:\n");
	unsigned result = 0x0;

	unsigned mask = 0x0;
	int n = 0;
	for (n = 0; n < sat - 1; n++)
	{
		mask = mask << 1;
		mask += 1;
	}
	
	if (a > mask)
		result = mask;
	else
		result = a;

	//printf("%8.8x\n", result);

	write_r(rd, ~result);
	execute_usat_reg_t1(rd, rn, sat);
	check_rd(rd, result, 3);

}

//////////////////////////////////////////////////////////////////////////////////////
//	UXTB
//////////////////////////////////////////////////////////////////////////////////////
void execute_uxtb_t1 (unsigned rd, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_uxtb_t1: [%d] = [%d]\n", rd, rm);
	unsigned instruction = 0xb2c0;
	instruction += rd & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_uxtb_t2 (unsigned rd, unsigned rm, int rot)
{
	fprintf(disasm, "-----------------------------------------\nexecute_uxtb_t2: [%d] = [%d] rot: %d\n", rd, rm, rot);
	unsigned instruction = 0xfa5ff080;
	instruction += rd << 8;
	instruction += rm;
	instruction += (rot & 0x3) << 4;
	execute_instruction32(instruction);
};

void test_uxtb (int rn, int rd, unsigned a, int rot)
{
	fprintf(disasm, "-----------------------------------------\ntest_uxtb:\n");

	unsigned result = (a >> (8 * 0)) & 0xFF;
	//if (((result >> 7) & 0x1) == 0x1)
	//	result += 0xFFFFFF00;

	write_r(rd, ~result);
	execute_uxtb_t1(rd, rn);
	check_rd(rd, result, 3);

	result = (a >> (8 * rot)) & 0xFF;
	//if (((result >> 7) & 0x1) == 0x1)
	//	result += 0xFFFFFF00;

	write_r(rd, ~result);
	execute_uxtb_t2(rd, rn, rot);
	check_rd(rd, result, 3);
}

//////////////////////////////////////////////////////////////////////////////////////
//	UXTH
//////////////////////////////////////////////////////////////////////////////////////
void execute_uxth_t1 (unsigned rd, unsigned rm)
{
	fprintf(disasm, "-----------------------------------------\nexecute_uxth_t1: [%d] = [%d]\n", rd, rm);
	unsigned instruction = 0xb280;
	instruction += rd & 0x7;
	instruction += (rm & 0x7) << 3;
	execute_instruction16(instruction);
};

void execute_uxth_t2 (unsigned rd, unsigned rm, int rot)
{
	fprintf(disasm, "-----------------------------------------\nexecute_uxth_t2: [%d] = [%d] rot: %d\n", rd, rm, rot);
	unsigned instruction = 0xfa1ff080;
	instruction += rd << 8;
	instruction += rm;
	instruction += (rot & 0x3) << 4;
	execute_instruction32(instruction);
};

void test_uxth (int rn, int rd, unsigned a, int rot)
{
	fprintf(disasm, "-----------------------------------------\ntest_uxth:\n");

	unsigned result = (a >> (8 * 0)) & 0xFFFF;
	//if (((result >> 15) & 0x1) == 0x1)
	//	result += 0xFFFF0000;

	write_r(rd, ~result);
	execute_uxth_t1(rd, rn);
	check_rd(rd, result, 3);

	result = (a >> (8 * rot)) & 0xFFFF;
	if (rot == 3)
		result += (a << 8) & 0xFF00;
	//if (((result >> 15) & 0x1) == 0x1)
	//	result += 0xFFFF0000;

	write_r(rd, ~result);
	execute_uxth_t2(rd, rn, rot);
	check_rd(rd, result, 3);
}

//////////////////////////////////////////////////////////////////////////////////////
//	DP rand
//////////////////////////////////////////////////////////////////////////////////////
void run_test_dp_rand ()
{
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_rand:\n");
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
	unsigned a = 0x0;
	int n = 0;

	///////////////////////////////////////////// SXTB, SXTH
	int rot = 0;
	a = 0x01020304;
	write_r(rn, a);
	for (rot = 0; rot < 4; rot++)
	{
		test_sxtb(rn, rd, a, rot);
		test_sxth(rn, rd, a, rot);
		test_uxtb(rn, rd, a, rot);
		test_uxth(rn, rd, a, rot);
	}

	a = 0xA1A2A3A4;
	write_r(rn, a);
	for (rot = 0; rot < 4; rot++)
	{
		test_sxtb(rn, rd, a, rot);
		test_sxth(rn, rd, a, rot);
		test_uxtb(rn, rd, a, rot);
		test_uxth(rn, rd, a, rot);
	}


	///////////////////////////////////////////// SBFX
	int lsb = 0;
	int width = 0;
	a = 0xAAAAAAAA;
	write_r(rn, a);
	for (lsb = 0; lsb < 32; lsb++)
	{
		test_sbfx(rn, rd, a, (32 - lsb), lsb);
		test_ubfx(rn, rd, a, (32 - lsb), lsb);
	}
	for (width = 1; width <= (32 - lsb); width++)
	{
		test_sbfx(rn, rd, a, width, 0);
		test_ubfx(rn, rd, a, width, 0);
	}

	a = 0x55555555;
	write_r(rn, a);
	for (lsb = 0; lsb < 32; lsb++)
	{
		test_sbfx(rn, rd, a, (32 - lsb), lsb);
		test_ubfx(rn, rd, a, (32 - lsb), lsb);
	}
	for (width = 1; width <= (32 - lsb); width++)
	{
		test_sbfx(rn, rd, a, width, 0);
		test_ubfx(rn, rd, a, width, 0);
	}

	///////////////////////////////////////////// BFC
	for (lsb = 0; lsb < 32; lsb++)
	{
		a = 0xFFFFFFFF;
		write_r(rd, a);
		test_bfc(rd, a, lsb, lsb);
		a = 0x0;
		write_r(rd, a);
		test_bfc(rd, a, lsb, lsb);
	}

	///////////////////////////////////////////// BFI
	a = 0xFFFFFFFF;
	write_r(rn, a);
	for (lsb = 0; lsb < 32; lsb++)
	{
		write_r(rd, 0x0);
		test_bfi(rd, rn, a, 0x0, lsb, lsb);
		write_r(rd, 0xFFFFFFFF);
		test_bfi(rd, rn, a, 0xFFFFFFFF, lsb, lsb);
	}

	for (width = 0; width < 32; width++)
	{
		write_r(rd, 0x0);
		test_bfi(rd, rn, a, 0x0, width, 0);
		write_r(rd, 0xFFFFFFFF);
		test_bfi(rd, rn, a, 0xFFFFFFFF, width, 0);
	}

	///////////////////////////////////////////// CLZ
	fprintf(disasm, "-----------------------------------------\ntest_clz:\n");
	a = 0x0;
	for (n = 0; n < 33; n++)
	{
		if (n == 1)
			a = 0x1;
		else
			a *= 2;
		write_r(rm, a);
		if (n == 32)
			test_clz(rm, rd, 0);
		else 
			test_clz(rm, rd, 32 - n);
	}

	a = 0x0;
	for (n = 0; n < 33; n++)
	{
		if (n == 1)
			a = 0x1;
		else
            if (n > 1)
		{
			a = a << 1;
			a += 1;
		}
		write_r(rm, a);
		if (n == 32)
			test_clz(rm, rd, 0);
		else 
			test_clz(rm, rd, 32 - n);
	}

	///////////////////////////////////////////// RBIT
	fprintf(disasm, "-----------------------------------------\ntest_rbit:\n");
	a = 0xFFAA5500;
	write_r(rm, a);
	test_rbit(rm, rd, a);
	a = 0x0055AAFF;
	write_r(rm, a);
	test_rbit(rm, rd, a);

	///////////////////////////////////////////// REV
	fprintf(disasm, "-----------------------------------------\ntest_rev:\n");
	a = 0xFFAA5500;
	write_r(rm, a);
	test_rev(rm, rd, a);
	a = 0x0055AAFF;
	write_r(rm, a);
	test_rev(rm, rd, a);
}

//////////////////////////////////////////////////////////////////////////////////////
//	DP COMB reg
//////////////////////////////////////////////////////////////////////////////////////
void run_test_dp_comb_reg ()
{
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg:\n");
	set_flags_init();
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
	unsigned a = 0x0;
	unsigned b = 0x0;

	///////////////////////////////////////////// 0 + 0
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 0:\n");
	write_r(rn, a);
	write_r(rm, b);

	test_and_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_bic_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_eor_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_orn_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orr_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_teq_reg(rn, rm,     a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_tst_reg(rn, rm,     a, b, true, false, true, true, true, false, false, false); // c, z, n, v

	///////////////////////////////////////////// 0 + F
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 F:\n");
	b = 0xFFFFFFFF;
	write_r(rm, b);

	test_and_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_bic_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_eor_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orn_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_orr_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_teq_reg(rn, rm,     a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_tst_reg(rn, rm,     a, b, true, false, true, true, true, false, false, false); // c, z, n, v

	///////////////////////////////////////////// F + F = 1E
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg F F:\n");
	a = 0xFFFFFFFF;
	write_r(rn, a);

	test_and_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_bic_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_eor_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_orn_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orr_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_teq_reg(rn, rm,     a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_tst_reg(rn, rm,     a, b, true, false, true, false, true, true, false, false); // c, z, n, v

	///////////////////////////////////////////// F + 0
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg F 0:\n");
	b = 0x0;
	write_r(rm, b);

	test_and_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_bic_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_eor_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orn_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orr_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_teq_reg(rn, rm,     a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_tst_reg(rn, rm,     a, b, true, false, true, true, true, false, false, false); // c, z, n, v

	///////////////////////////////////////////// 5 + 0
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 5 0:\n");
	a = 0x55555555;
	write_r(rn, a);

	test_and_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_bic_reg(rn, rm, rd, a, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_eor_reg(rn, rm, rd, a, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_orn_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orr_reg(rn, rm, rd, a, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_teq_reg(rn, rm,     a, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_tst_reg(rn, rm,     a, b, true, false, true, true, true, false, false, false); // c, z, n, v

	///////////////////////////////////////////// 5 + 5 = A
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 5 5:\n");
	b = 0x55555555;
	write_r(rm, b);

	test_and_reg(rn, rm, rd, a, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_bic_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_eor_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_orn_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orr_reg(rn, rm, rd, a, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_teq_reg(rn, rm,     a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_tst_reg(rn, rm,     a, b, true, false, true, false, true, false, false, false); // c, z, n, v

	///////////////////////////////////////////// 0 + 5
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 5:\n");
	a = 0x0;
	write_r(rn, a);

	test_and_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_bic_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_eor_reg(rn, rm, rd, a, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_orn_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orr_reg(rn, rm, rd, a, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_teq_reg(rn, rm,     a, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_tst_reg(rn, rm,     a, b, true, false, true, true, true, false, false, false); // c, z, n, v

	///////////////////////////////////////////// A + 0
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg A 0:\n");
	a = 0xAAAAAAAA;
	write_r(rn, a);

	test_and_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_bic_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_eor_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orn_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orr_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_teq_reg(rn, rm,     a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_tst_reg(rn, rm,     a, b, true, false, true, true, true, false, false, false); // c, z, n, v

	///////////////////////////////////////////// A + A = 14
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg A A:\n");
	b = 0xAAAAAAAA;
	write_r(rm, b);

	test_and_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_bic_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_eor_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_orn_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orr_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_teq_reg(rn, rm,     a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_tst_reg(rn, rm,     a, b, true, false, true, false, true, true, false, false); // c, z, n, v

	///////////////////////////////////////////// 0 + A
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 A:\n");
	a = 0x0;
	write_r(rn, a);

	test_and_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_bic_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_eor_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orn_reg(rn, rm, rd, a, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_orr_reg(rn, rm, rd, a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_teq_reg(rn, rm,     a, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_tst_reg(rn, rm,     a, b, true, false, true, true, true, false, false, false); // c, z, n, v

}

//////////////////////////////////////////////////////////////////////////////////////
//	DP COMB imm
//////////////////////////////////////////////////////////////////////////////////////
void run_test_dp_comb_imm ()
{
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_imm:\n");
	set_flags_init();
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
	unsigned a = 0x0;
	unsigned b = 0x0;

	///////////////////////////////////////////// 0 + 0
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_imm 0 0:\n");
	write_r(rn, a);
	//write_r(rm, b);

	test_and_imm(rn, rd, a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_bic_imm(rn, rd, a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_eor_imm(rn, rd, a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_orn_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orr_imm(rn, rd, a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_teq_imm(rn,     a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_tst_imm(rn,     a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v

	///////////////////////////////////////////// 0 + F
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_imm 0 F:\n");
	b = 0xFF;
//	write_r(rm, b);

	test_and_imm(rn, rd, a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_bic_imm(rn, rd, a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_eor_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_orn_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orr_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_teq_imm(rn,     a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_tst_imm(rn,     a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v

	///////////////////////////////////////////// F + F = 1E
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_imm F F:\n");
	a = 0xFFFFFFFF;
	write_r(rn, a);

	test_and_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_bic_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_eor_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orn_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orr_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_teq_imm(rn,     a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_tst_imm(rn,     a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v

	///////////////////////////////////////////// F + 0
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_imm F 0:\n");
	b = 0x0;
//	write_r(rm, b);

	test_and_imm(rn, rd, a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_bic_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_eor_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orn_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orr_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_teq_imm(rn,     a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_tst_imm(rn,     a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v

	///////////////////////////////////////////// 5 + 0
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_imm 5 0:\n");
	a = 0x55555555;
	write_r(rn, a);

	test_and_imm(rn, rd, a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_bic_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_eor_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_orn_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orr_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_teq_imm(rn,     a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_tst_imm(rn,     a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v

	///////////////////////////////////////////// 5 + 5 = A
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_imm 5 5:\n");
	b = 0x55;
//	write_r(rm, b);

	test_and_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_bic_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_eor_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_eor_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_orn_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orr_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_teq_imm(rn,     a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_tst_imm(rn,     a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v

	///////////////////////////////////////////// 0 + 5
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_imm 0 5:\n");
	a = 0x0;
	write_r(rn, a);

	test_and_imm(rn, rd, a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_bic_imm(rn, rd, a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_eor_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_orn_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orr_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_teq_imm(rn,     a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_tst_imm(rn,     a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v

	///////////////////////////////////////////// A + 5
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_imm A 0:\n");
	a = 0xAAAAAAAA;
	write_r(rn, a);

	test_and_imm(rn, rd, a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_bic_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_eor_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orn_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orr_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_teq_imm(rn,     a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_tst_imm(rn,     a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v

	///////////////////////////////////////////// A + A = 14
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_imm A A:\n");
	b = 0xAA;
//	write_r(rm, b);

	test_and_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_bic_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_eor_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orn_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orr_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_teq_imm(rn,     a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_tst_imm(rn,     a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v

	///////////////////////////////////////////// 0 + A
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_imm 0 A:\n");
	a = 0x0;
	write_r(rn, a);

	test_and_imm(rn, rd, a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v//	test_bic_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_bic_imm(rn, rd, a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v
	test_eor_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_orn_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, true, false, false); // c, z, n, v
	test_orr_imm(rn, rd, a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_teq_imm(rn,     a, 0, 0, b, true, false, true, false, true, false, false, false); // c, z, n, v
	test_tst_imm(rn,     a, 0, 0, b, true, false, true, true, true, false, false, false); // c, z, n, v//	test_bic_reg(rn, rm, rd, a, b, true, false, true, true, true, false, false, false); // c, z, n, v

}

//////////////////////////////////////////////////////////////////////////////////////
//	DP MATH 1 reg
//////////////////////////////////////////////////////////////////////////////////////
void run_test_dp_math1_reg ()
{
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg:\n");
	set_flags_init();
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
	unsigned a = 0x0;
	unsigned b = 0x0;

	///////////////////////////////////////////// 0 + 0
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 0:\n");
	write_r(rn, a);
	write_r(rm, b);

	test_add_reg(rn, rm, rd, a, b, true, false, true, true, true, false, true, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, false, true, true, true, false, true, false, false); // 0, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, false, true, false, true, false, true, false, true); // 1, z, n, v
	test_cmn_reg(rn, rm,     a, b, true, false, true, true, true, false, true, false); // c, z, n, v

	///////////////////////////////////////////// 0 + 7 + C
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 7:\n");
	b = 0x7FFFFFFF;
	write_r(rm, b);

	test_add_reg(rn, rm, rd, a, b, true, false, true, false, true, false, true, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, false, true, false, true, false, true, false, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, true, true); // c, z, n, v
	test_cmn_reg(rn, rm,     a, b, true, false, true, false, true, false, true, false); // c, z, n, v

	///////////////////////////////////////////// 0 +- 8
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 8:\n");
	b = 0x80000000;
	write_r(rm, b);

	test_add_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false, true); // c, z, n, v
	test_cmn_reg(rn, rm,     a, b, true, false, true, false, true, true, true, false); // c, z, n, v

	///////////////////////////////////////////// 0 + F
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 F:\n");
	b = 0xFFFFFFFF;
	write_r(rm, b);

	test_add_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, true, true, true, true, false, true, false, true); // c, z, n, v
	test_cmn_reg(rn, rm,     a, b, true, false, true, false, true, true, true, false); // c, z, n, v

	///////////////////////////////////////////// F + F = 1E
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg F F:\n");
	a = 0xFFFFFFFF;
	write_r(rn, a);

	test_add_reg(rn, rm, rd, a, b, true, true, true, false, true, true, true, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, true, true, false, true, true, true, false, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, true, true, false, true, true, true, false, true); // c, z, n, v
	test_cmn_reg(rn, rm,     a, b, true, true, true, false, true, true, true, false); // c, z, n, v

	///////////////////////////////////////////// F + 7
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg F 7:\n");
	b = 0x7FFFFFFF;
	write_r(rm, b);

	test_add_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, false, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, false, true); // c, z, n, v
	test_cmn_reg(rn, rm,     a, b, true, true, true, false, true, false, true, false); // c, z, n, v

	///////////////////////////////////////////// F +- 8
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg F 8:\n");
	b = 0x80000000;
	write_r(rm, b);

	test_add_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, true); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, true, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, true, true, false, true, true, true, false, true); // c, z, n, v
	test_cmn_reg(rn, rm,     a, b, true, true, true, false, true, false, true, true); // c, z, n, v

	///////////////////////////////////////////// F + 0
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg F 0:\n");
	b = 0x0;
	write_r(rm, b);

	test_add_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, true, true, true, true, false, true, false, true); // c, z, n, v
	test_cmn_reg(rn, rm,     a, b, true, false, true, false, true, true, true, false); // c, z, n, v

	///////////////////////////////////////////// 7 + 0
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 7 0:\n");
	a = 0x7FFFFFFF;
	write_r(rn, a);

	test_add_reg(rn, rm, rd, a, b, true, false, true, false, true, false, true, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, false, true, false, true, false, true, false, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, true, true); // c, z, n, v
	test_cmn_reg(rn, rm,     a, b, true, false, true, false, true, false, true, false); // c, z, n, v

	///////////////////////////////////////////// 7 + 7
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 7 7:\n");
	b = 0x7FFFFFFF;
	write_r(rm, b);

	test_add_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, true); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, true, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, true, true); // c, z, n, v
	test_cmn_reg(rn, rm,     a, b, true, false, true, false, true, true, true, true); // c, z, n, v

	///////////////////////////////////////////// 7 +- 8
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg F 8:\n");
	b = 0x80000000;
	write_r(rm, b);

	test_add_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, true, true, true, true, false, true, false, true); // c, z, n, v
	test_cmn_reg(rn, rm,     a, b, true, false, true, false, true, true, true, false); // c, z, n, v

	///////////////////////////////////////////// 7 + F
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 7 F:\n");
	b = 0xFFFFFFFF;
	write_r(rm, b);

	test_add_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, false, false); // c, z, n, v
	test_adc_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, false, true); // c, z, n, v
	test_cmn_reg(rn, rm,     a, b, true, true, true, false, true, false, true, false); // c, z, n, v

	///////////////////////////////////////////// 5 + 0
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 5 0:\n");
	a = 0x55555555;
	write_r(rn, a);
	b = 0x0;
	write_r(rm, b);

	test_add_reg(rn, rm, rd, a, b, true, false, true, false, true, false, true, false); // c, z, n, v
	test_cmn_reg(rn, rm,     a, b, true, false, true, false, true, false, true, false); // c, z, n, v

	///////////////////////////////////////////// 5 + 5 = A
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 5 5:\n");
	b = 0x55555555;
	write_r(rm, b);

	test_add_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, true); // c, z, n, v
	test_cmn_reg(rn, rm,     a, b, true, false, true, false, true, true, true, true); // c, z, n, v

	///////////////////////////////////////////// 0 + 5
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 5:\n");
	a = 0x0;
	write_r(rn, a);

	test_add_reg(rn, rm, rd, a, b, true, false, true, false, true, false, true, false); // c, z, n, v
	test_cmn_reg(rn, rm,     a, b, true, false, true, false, true, false, true, false); // c, z, n, v

	///////////////////////////////////////////// A + 0
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg A 0:\n");
	a = 0xAAAAAAAA;
	write_r(rn, a);

	test_add_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false); // c, z, n, v
	test_cmn_reg(rn, rm,     a, b, true, false, true, false, true, true, true, false); // c, z, n, v

	///////////////////////////////////////////// A + A = 14
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg A A:\n");
	b = 0xAAAAAAAA;
	write_r(rm, b);

	test_add_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, true); // c, z, n, v
	test_cmn_reg(rn, rm,     a, b, true, true, true, false, true, false, true, true); // c, z, n, v

	///////////////////////////////////////////// 0 + A
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 A:\n");
	a = 0x0;
	write_r(rn, a);

	test_add_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false); // c, z, n, v
	test_cmn_reg(rn, rm,     a, b, true, false, true, false, true, true, true, false); // c, z, n, v

}

//////////////////////////////////////////////////////////////////////////////////////
//	DP MATH 1 imm
//////////////////////////////////////////////////////////////////////////////////////
void run_test_dp_math1_imm ()
{
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg:\n");
	set_flags_init();
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
	unsigned a = 0x0;
	unsigned b = 0x0;

	///////////////////////////////////////////// 0 + 0
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 0:\n");
	write_r(rn, a);
//	write_r(rm, b);

	test_add_imm(rn, rd, a, b, true, false, true, true, true, false, true, false); // c, z, n, v
	test_adc_imm(rn, rd, a, b, true, false, true, true, true, false, true, false, true); // c, z, n, v
	test_adc_imm(rn, rd, a, b, true, false, true, true, true, false, true, false, false); // c, z, n, v
	test_cmn_imm(rn,        b, true, false, true, true, true, false, true, false); // c, z, n, v

	///////////////////////////////////////////// 0 + F
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 F:\n");
	b = 0xFFF;
//	write_r(rm, b);

	test_add_imm(rn, rd, a, b, true, false, true, false, true, false, true, false); // c, z, n, v
	test_adc_imm(rn, rd, a, b, true, false, true, false, true, false, true, false, false); // c, z, n, v
	test_adc_imm(rn, rd, a, b, true, false, true, false, true, false, true, false, true); // c, z, n, v
	test_cmn_imm(rn,        b, true, false, true, false, true, false, true, false); // c, z, n, v

	///////////////////////////////////////////// F + F = 1E
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg F F:\n");
	a = 0xFFFFFFFF;
	write_r(rn, a);

	test_add_imm(rn, rd, a, b, true, true, true, false, true, false, true, false); // c, z, n, v
	test_adc_imm(rn, rd, a, b, true, true, true, false, true, false, true, false, false); // c, z, n, v
	test_adc_imm(rn, rd, a, b, true, true, true, false, true, false, true, false, true); // c, z, n, v
	test_cmn_imm(rn,        b, true, true, true, false, true, false, true, false); // c, z, n, v

	///////////////////////////////////////////// F + 0
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg F 0:\n");
	b = 0x0;
//	write_r(rm, b);

	test_add_imm(rn, rd, a, b, true, false, true, false, true, true, true, false); // c, z, n, v
	test_adc_imm(rn, rd, a, b, true, false, true, false, true, true, true, false, false); // c, z, n, v
	test_adc_imm(rn, rd, a, b, true, true, true, true, true, false, true, false, true); // c, z, n, v
	test_cmn_imm(rn,        b, true, false, true, false, true, true, true, false); // c, z, n, v

}

//////////////////////////////////////////////////////////////////////////////////////
//	DP MATH 2 reg
//////////////////////////////////////////////////////////////////////////////////////
void run_test_dp_math2_reg ()
{
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg:\n");
	set_flags_init();
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
	unsigned a = 0x0;
	unsigned b = 0x0;

	///////////////////////////////////////////// 0 + 0
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 0:\n");
	write_r(rn, a);
	write_r(rm, b);

	test_cmp_reg(rn, rm,     a, b, true, true, true, true, true, false, true, false); // c, z, n, v
	test_rsb_reg(rn, rm, rd, a, b, true, true, true, true, true, false, true, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, true, true, true, true, false, true, false, true); // c, z, n, v
	test_sub_reg(rn, rm, rd, a, b, true, true, true, true, true, false, true, false); // c, z, n, v

	///////////////////////////////////////////// 0 - 1
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 1:\n");
	b = 0x1;
	write_r(rm, b);

	test_cmp_reg(rn, rm,     a, b, true, false, true, false, true, true, true, false); // c, z, n, v
	test_rsb_reg(rm, rn, rd, b, a, true, false, true, false, true, true, true, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false, true); // c, z, n, v
	test_sub_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false); // c, z, n, v

	///////////////////////////////////////////// 0 + 7 + C
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 7:\n");
	b = 0x7FFFFFFF;
	write_r(rm, b);

	test_cmp_reg(rn, rm,     a, b, true, false, true, false, true, true, true, false); // c, z, n, v
	test_rsb_reg(rm, rn, rd, b, a, true, false, true, false, true, true, true, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false, true); // c, z, n, v
	test_sub_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false); // c, z, n, v

	///////////////////////////////////////////// 0 +- 8
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 8:\n");
	b = 0x80000000;
	write_r(rm, b);

	test_cmp_reg(rn, rm,     a, b, true, false, true, false, true, true, true, true); // c, z, n, v
	test_rsb_reg(rm, rn, rd, b, a, true, false, true, false, true, true, true, true); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, false, true, false, true, false, true, false, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, true, true); // c, z, n, v
	test_sub_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, true); // c, z, n, v

	///////////////////////////////////////////// 0 - F9
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 F9:\n");
	b = 0xFFFFFFF9;
	write_r(rm, b);

	test_cmp_reg(rn, rm,     a, b, true, false, true, false, true, false, true, false); // c, z, n, v
	test_rsb_reg(rm, rn, rd, b, a, true, false, true, false, true, false, true, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, false, true, false, true, false, true, false, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, false, true, false, true, false, true, false, true); // c, z, n, v
	test_sub_reg(rn, rm, rd, a, b, true, false, true, false, true, false, true, false); // c, z, n, v

	///////////////////////////////////////////// 0 + F
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 F:\n");
	b = 0xFFFFFFFF;
	write_r(rm, b);

	test_cmp_reg(rn, rm,     a, b, true, false, true, false, true, false, true, false); // c, z, n, v
	test_rsb_reg(rm, rn, rd, b, a, true, false, true, false, true, false, true, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, false, true, true, true, false, true, false, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, false, true, false, true, false, true, false, true); // c, z, n, v
	test_sub_reg(rn, rm, rd, a, b, true, false, true, false, true, false, true, false); // c, z, n, v

	///////////////////////////////////////////// F + F = 1E
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg F F:\n");
	a = 0xFFFFFFFF;
	write_r(rn, a);

	test_cmp_reg(rn, rm,     a, b, true, true, true, true, true, false, true, false); // c, z, n, v
	test_rsb_reg(rm, rn, rd, b, a, true, true, true, true, true, false, true, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, false, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, true, true, true, true, false, true, false, true); // c, z, n, v
	test_sub_reg(rn, rm, rd, a, b, true, true, true, true, true, false, true, false); // c, z, n, v

	///////////////////////////////////////////// F - F9
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 F9:\n");
	b = 0xFFFFFFF9;
	write_r(rm, b);

	test_cmp_reg(rn, rm,     a, b, true, true, true, false, true, false, true, false); // c, z, n, v
	test_rsb_reg(rm, rn, rd, b, a, true, true, true, false, true, false, true, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, false, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, false, true); // c, z, n, v
	test_sub_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, false); // c, z, n, v

	///////////////////////////////////////////// F +- 8
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg F 8:\n");
	b = 0x80000000;
	write_r(rm, b);

	test_cmp_reg(rn, rm,     a, b, true, true, true, false, true, false, true, false); // c, z, n, v
	test_rsb_reg(rm, rn, rd, b, a, true, true, true, false, true, false, true, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, false, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, false, true); // c, z, n, v
	test_sub_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, false); // c, z, n, v

	///////////////////////////////////////////// F +- 7
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg F 7:\n");
	b = 0x7FFFFFFF;
	write_r(rm, b);

	test_cmp_reg(rn, rm,     a, b, true, true, true, false, true, true, true, false); // c, z, n, v
	test_rsb_reg(rm, rn, rd, b, a, true, true, true, false, true, true, true, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, true, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, true, true, false, true, true, true, false, true); // c, z, n, v
	test_sub_reg(rn, rm, rd, a, b, true, true, true, false, true, true, true, false); // c, z, n, v

	///////////////////////////////////////////// F - 1
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg F 1:\n");
	b = 0x1;
	write_r(rm, b);

	test_cmp_reg(rn, rm,     a, b, true, true, true, false, true, true, true, false); // c, z, n, v
	test_rsb_reg(rm, rn, rd, b, a, true, true, true, false, true, true, true, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, true, true, false, true, true, true, false, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, true, true, false, true, true, true, false, true); // c, z, n, v
	test_sub_reg(rn, rm, rd, a, b, true, true, true, false, true, true, true, false); // c, z, n, v

	///////////////////////////////////////////// 7 - 1
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 7 1:\n");
	a = 0x7FFFFFFF;
	write_r(rn, a);
	b = 0x1;
	write_r(rm, b);

	test_cmp_reg(rn, rm,     a, b, true, true, true, false, true, false, true, false); // c, z, n, v
	test_rsb_reg(rm, rn, rd, b, a, true, true, true, false, true, false, true, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, false, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, false, true); // c, z, n, v
	test_sub_reg(rn, rm, rd, a, b, true, true, true, false, true, false, true, false); // c, z, n, v

	///////////////////////////////////////////// 7 - F9
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 7 F9:\n");
	b = 0xFFFFFFF9;
	write_r(rm, b);

	test_cmp_reg(rn, rm,     a, b, true, false, true, false, true, true, true, true); // c, z, n, v
	test_rsb_reg(rm, rn, rd, b, a, true, false, true, false, true, true, true, true); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, true, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, true, true); // c, z, n, v
	test_sub_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, true); // c, z, n, v

	///////////////////////////////////////////// 7 +- 8
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 7 8:\n");
	b = 0x80000000;
	write_r(rm, b);

	test_cmp_reg(rn, rm,     a, b, true, false, true, false, true, true, true, true); // c, z, n, v
	test_rsb_reg(rm, rn, rd, b, a, true, false, true, false, true, true, true, true); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, true, false); // c, z, n, v
	test_sbc_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, true, true); // c, z, n, v
	test_sub_reg(rn, rm, rd, a, b, true, false, true, false, true, true, true, true); // c, z, n, v

}

//////////////////////////////////////////////////////////////////////////////////////
//	DP MATH 2 imm
//////////////////////////////////////////////////////////////////////////////////////
void run_test_dp_math2_imm ()
{
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg:\n");
	set_flags_init();
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
	unsigned a = 0x0;
	unsigned b = 0x0;

	///////////////////////////////////////////// 0 - 0
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 0:\n");
	write_r(rn, a);
	write_r(rm, b);

	test_cmp_imm(rn,            b, true, true, true, true, true, false, true, false); // c, z, n, v
	test_rsb_imm(rn, rm, rd, a, b, true, true, true, true, true, false, true, false); // c, z, n, v
	test_sbc_imm(rn, rd, a, b, true, true, true, false, true, true, true, false, false); // c, z, n, v
	test_sbc_imm(rn, rd, a, b, true, false, true, true, true, false, true, false, true); // c, z, n, v
	test_sub_imm(rn, rd, a, b, true, true, true, true, true, false, true, false); // c, z, n, v

	///////////////////////////////////////////// 0 - F
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg 0 F:\n");
	b = 0xFFF;
	write_r(rm, b);

	test_cmp_imm(rn,            b, true, false, true, false, true, true, true, false); // c, z, n, v
	test_rsb_imm(rn, rm, rd, a, b, true, true, true, false, true, false, true, false); // c, z, n, v
	test_sbc_imm(rn, rd, a, b, true, true, true, false, true, true, true, false, false); // c, z, n, v
	test_sbc_imm(rn, rd, a, b, true, true, true, false, true, true, true, false, true); // c, z, n, v
	test_sub_imm(rn, rd, a, b, true, false, true, false, true, true, true, false); // c, z, n, v

	///////////////////////////////////////////// F - F = 1E
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg F F:\n");
	a = 0xFFFFFFFF;
	write_r(rn, a);

	test_cmp_imm(rn,            b, true, true, true, false, true, true, true, false); // c, z, n, v
	test_rsb_imm(rn, rm, rd, a, b, true, false, true, false, true, false, true, false); // c, z, n, v
	test_sbc_imm(rn, rd, a, b, true, false, true, false, true, true, true, false, false); // c, z, n, v
	test_sbc_imm(rn, rd, a, b, true, false, true, false, true, true, true, false, true); // c, z, n, v
	test_sub_imm(rn, rd, a, b, true, true, true, false, true, true, true, false); // c, z, n, v

	///////////////////////////////////////////// F - 0
	fprintf(disasm, "-----------------------------------------\nrun_test_dp_reg F 1:\n");
	b = 0x0;
	write_r(rm, b);

	test_cmp_imm(rn,            b, true, true, true, false, true, true, true, false); // c, z, n, v
	test_rsb_imm(rn, rm, rd, a, b, true, false, true, false, true, false, true, false); // c, z, n, v
	test_sbc_imm(rn, rd, a, b, true, false, true, false, true, true, true, false, false); // c, z, n, v
	test_sbc_imm(rn, rd, a, b, true, false, true, false, true, true, true, false, true); // c, z, n, v
	test_sub_imm(rn, rd, a, b, true, true, true, false, true, true, true, false); // c, z, n, v

}

//////////////////////////////////////////////////////////////////////////////////////
//	DP MATH 3 reg
//////////////////////////////////////////////////////////////////////////////////////
void run_test_dp_math3_reg ()
{
	fprintf(disasm, "-----------------------------------------\nrun_test_dp3_reg:\n");
	set_flags_init();
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
	unsigned a = 0x0;
	unsigned b = 0x0;

	a = 0xAAAAAAAA;
	write_r(rn, a);

	int sh = 0;
	for (sh = 0; sh < 32; sh += 1)
	{
		write_r(rm, sh);
		test_asr_reg(rn, rm, rd, a, sh); 
		test_ror_reg(rn, rm, rd, a, sh); 
		if (sh < 20)
			sh++;
	}
	test_rrx_reg(rn, rd, a); 

	a = 0x55555555;
	write_r(rn, a);
	for (sh = 0; sh < 32; sh += 1)
	{
		write_r(rm, sh);
		test_asr_reg(rn, rm, rd, a, sh); 
		test_ror_reg(rn, rm, rd, a, sh); 
		if (sh < 20)
			sh++;
	}
	test_rrx_reg(rn, rd, a); 

}

//////////////////////////////////////////////////////////////////////////////////////
//	DP MATH 3 imm
//////////////////////////////////////////////////////////////////////////////////////
void run_test_dp_math3_imm ()
{
	fprintf(disasm, "-----------------------------------------\nrun_test_dp3_imm:\n");
	set_flags_init();
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
	unsigned a = 0x0;
	unsigned b = 0x0;

	a = 0xAAAAAAAA;
	write_r(rn, a);

	int sh = 0;
	for (sh = 1; sh < 32; sh += 1)
	{
	//	write_r(rm, sh);
		test_asr_imm(rn, rd, a, sh); 
		test_ror_imm(rn, rd, a, sh); 
		if (sh < 20)
			sh++;
	}

	a = 0x55555555;
	write_r(rn, a);

	for (sh = 1; sh < 32; sh += 1)
	{
	//	write_r(rm, sh);
		test_asr_imm(rn, rd, a, sh); 
		test_ror_imm(rn, rd, a, sh); 
		if (sh < 20)
			sh++;
	}

}

//////////////////////////////////////////////////////////////////////////////////////
//	DP MATH 4 reg
//////////////////////////////////////////////////////////////////////////////////////
void run_test_dp_math4_reg ()
{
	fprintf(disasm, "-----------------------------------------\nrun_test_dp4_reg:\n");
	set_flags_init();
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
	unsigned a = 0x0;
	unsigned b = 0x0;

	a = 0xAAAAAAAA;
	write_r(rn, a);

	int sh = 0;
	for (sh = 0; sh < 32; sh += 1)
	{
		write_r(rm, sh);
		test_lsl_reg(rn, rm, rd, a, sh); 
		test_lsr_reg(rn, rm, rd, a, sh); 
		if (sh < 20)
			sh++;
	}

	a = 0x55555555;
	write_r(rn, a);
	for (sh = 0; sh < 32; sh += 1)
	{
		write_r(rm, sh);
		test_lsl_reg(rn, rm, rd, a, sh); 
		test_lsr_reg(rn, rm, rd, a, sh); 
		if (sh < 20)
			sh++;
	}

}

//////////////////////////////////////////////////////////////////////////////////////
//	DP MATH 4 reg
//////////////////////////////////////////////////////////////////////////////////////
void run_test_dp_math4_imm ()
{
	// missing sh = 0; lsl_imm_t1 => mov_reg_t2 => setflag = false ?
	fprintf(disasm, "-----------------------------------------\nrun_test_dp4_reg:\n");
	set_flags_init();
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
	unsigned a = 0x0;
	unsigned b = 0x0;

	a = 0xAAAAAAAA;
	write_r(rn, a);

	int sh = 0;
	for (sh = 1; sh < 32; sh += 1)
	{
		//write_r(rm, sh);
		test_lsl_imm(rn, rd, a, sh); 
		if (sh > 0)
			test_lsr_imm(rn, rd, a, sh); 
		if (sh < 20)
			sh++;
	}

	a = 0x55555555;
	write_r(rn, a);
	for (sh = 1; sh < 32; sh += 1)
	{
		//write_r(rm, sh);
		test_lsl_imm(rn, rd, a, sh); 
		if (sh > 0)
			test_lsr_imm(rn, rd, a, sh); 
		if (sh < 20)
			sh++;
	}

}

//////////////////////////////////////////////////////////////////////////////////////
//	DP MATH 5 reg
//////////////////////////////////////////////////////////////////////////////////////
void run_test_dp_math5_reg ()
{
	fprintf(disasm, "-----------------------------------------\nrun_test_dp5_reg:\n");
	set_flags_init();
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
	unsigned a = 0x0;
	unsigned b = 0x0;

	a = 0x1;

	int n = 0;
	for (n = 0; n < 32; n += 2)
	{
		write_r(rn, a);
		//printf("\n%8.8x\n", a);

		int sat = 0;
		for (sat = 1; sat < 32; sat += 3)
		{
			test_ssat_reg(rn, rd, a, sat); 
			test_usat_reg(rn, rd, a, sat); 
		}

		write_r(rn, ~a);
		//printf("\n%8.8x\n", ~a);

		for (sat = 1; sat < 32; sat += 3)
		{
			test_ssat_reg(rn, rd, ~a, sat); 
		}

		a = a << 1;
	}

}

//////////////////////////////////////////////////////////////////////////////////////
//	DP MATH 6 reg
//////////////////////////////////////////////////////////////////////////////////////
void run_test_dp_math6_reg ()
{
	fprintf(disasm, "-----------------------------------------\nrun_test_dp6_reg:\n");
	set_flags_init();
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
  	int ra = 4;
	unsigned a = 0x0;
	unsigned b = 0x0;

	a = 0x0;
	write_r(ra, 0xAAAAAAAA);

	int n = 0;
	for (n = 0; n < 32; n += 1)
	{
		write_r(rn, a);

		b = 0x0;

		int m = 0;
		for (m = 0; m <= n; m += 5)
		{
			write_r(rm, b);
			test_mul(ra, rn, rm, rd, 0xAAAAAAAA, a, b); 
			b = b << 1;
			b += 1;
			b = b << 1;
			b += 1;
			b = b << 1;
			b += 1;
		}

		a = a << 1;
		a += 1;
	}

}

//////////////////////////////////////////////////////////////////////////////////////
//	DP MATH 7 reg
//////////////////////////////////////////////////////////////////////////////////////
void run_test_dp_math7_reg ()
{
	fprintf(disasm, "-----------------------------------------\nrun_test_dp7_reg:\n");
	set_flags_init();
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
  	int rd2 = 4;
	unsigned a = 0x0;
	unsigned b = 0x0;

	a = 0x0;

	int n = 0;
	for (n = 0; n < 32; n += 1)
	{
		write_r(rn, a);

		b = 0x0;

		int m = 0;
		for (m = 0; m < n; m += 3)
		{
			write_r(rm, b);
			test_umull_reg(rn, rm, rd, rd2, a, b); 
			b = b << 1;
			b += 1;
			b = b << 1;
			b += 1;
		}

		a = a << 1;
		a += 1;
	}

}

//////////////////////////////////////////////////////////////////////////////////////
//	DP MATH 8 reg
//////////////////////////////////////////////////////////////////////////////////////
void run_test_dp_math8_reg ()
{
	fprintf(disasm, "-----------------------------------------\nrun_test_dp8_reg:\n");
	set_flags_init();
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
  	int rd2 = 4;
	unsigned a = 0x0;
	unsigned b = 0x0;

	a = 0x0;

	int n = 0;
	for (n = 0; n < 32; n += 1)
	{
		unsigned sa = a;
		if ((n & 0x1) == 0x1)
			sa = 0 - a;
		write_r(rn, sa);

		b = 0x0;

		int m = 0;
		for (m = 0; m < n; m += 3)
		{
			unsigned sb = b;
			if ((m & 0x1) == 0x1)
				sb = 0 - b;

			write_r(rm, sb);
			test_smull_reg(rn, rm, rd, rd2, sa, sb); 
			b = b << 1;
			b += 1;
			b = b << 1;
			b += 1;
		}

		a = a << 1;
		a += 1;
	}

}

//////////////////////////////////////////////////////////////////////////////////////
//	DP MATH 9 reg
//////////////////////////////////////////////////////////////////////////////////////
void run_test_dp_math9_reg ()
{
	fprintf(disasm, "-----------------------------------------\nrun_test_dp9_reg:\n");
	set_flags_init();
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
  	int ra = 4;
	unsigned a = 0x0;
	unsigned b = 0x0;

	a = 0x0;
	write_r(ra, 0xAAAAAAAA);

	int n = 0;
	for (n = 0; n < 32; n += 1)		// 42
	{
		write_r(rn, a);

		b = 0x1;

		int m = 0;
		for (m = 0; m <= n; m += 3)
		{
			write_r(rm, b);
			test_div_reg(rn, rm, rd, a, b); 
			b = b << 1;
			b += 1;
			b = b << 1;
			b += 1;
			b = b << 1;
			b += 1;
		}

		a = a << 1;
		if (n < 32)
			a += 1;
	}

}

//////////////////////////////////////////////////////////////////////////////////////
//	DecodeImmShift 1
//////////////////////////////////////////////////////////////////////////////////////
void run_test_DecodeImmShift1_reg ()
{
	// missing: TST, TEQ
	// missing: USAT, SSAT

	fprintf(disasm, "-----------------------------------------\nrun_test_DecodeImmShift1_reg:\n");
	set_flags_init();
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
	unsigned a = 0x0;
	unsigned b = 0x0;

	a = 0xFFFFFFFF;
	write_r(rn, a);
	b = 0x55555555;
	write_r(rm, b);

	int type = 0; 
	for (type = 0; type < 4; type ++)
	{
		int imm5 = 0;
		for (imm5 = 0; imm5 < 32; imm5 += 3)
		{
			if (type == 0)
			{
				unsigned bs = b << imm5;
				bool this_c = false;
  				//if (((b >> (32 - imm5)) & 0x1) == 0x1)
				//	this_c = true;
				test_and_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
				test_bic_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
				test_eor_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
				test_orr_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
				test_orn_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
				//if (bs > 0)
				//	test_ssat_dis_reg(rn,    rd, a, bs, this_c, type, imm5);
			} else
			if (type == 1)
			{
				unsigned bs = b >> imm5;
				if (imm5 == 0)
					bs = 0x0;
				test_and_dis_reg(rn, rm, rd, a, bs, true, type, imm5);
				test_bic_dis_reg(rn, rm, rd, a, bs, true, type, imm5);
				test_eor_dis_reg(rn, rm, rd, a, bs, true, type, imm5);
				test_orr_dis_reg(rn, rm, rd, a, bs, true, type, imm5);
				test_orn_dis_reg(rn, rm, rd, a, bs, true, type, imm5);
			} else
			if (type == 2)
			{
				unsigned bs = b >> imm5;
				if (imm5 == 0)
				{
	  				if (((b >> 31) & 0x1) == 0x1)
						bs = 0xffffffff;
					else
						bs = 0x0;
				} else
				{
  					if (((b >> 31) & 0x1) == 0x1)
						bs += ~(0xffffffff >> imm5);
				}
				test_and_dis_reg(rn, rm, rd, a, bs, true, type, imm5);
				test_bic_dis_reg(rn, rm, rd, a, bs, true, type, imm5);
				test_eor_dis_reg(rn, rm, rd, a, bs, true, type, imm5);
				test_orr_dis_reg(rn, rm, rd, a, bs, true, type, imm5);
				test_orn_dis_reg(rn, rm, rd, a, bs, true, type, imm5);
			} else
			{
				unsigned bs = b >> imm5;
				bs += b << (32 - imm5);
				if (imm5 == 0)
				{
					bs = b >> 1;
				}
				test_and_dis_reg(rn, rm, rd, a, bs, true, type, imm5);
				test_bic_dis_reg(rn, rm, rd, a, bs, true, type, imm5);
				test_eor_dis_reg(rn, rm, rd, a, bs, true, type, imm5);
				test_orr_dis_reg(rn, rm, rd, a, bs, true, type, imm5);
				test_orn_dis_reg(rn, rm, rd, a, bs, true, type, imm5);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//	DecodeImmShift 2
//////////////////////////////////////////////////////////////////////////////////////
void run_test_DecodeImmShift2_reg ()
{

	fprintf(disasm, "-----------------------------------------\nrun_test_DecodeImmShift2_reg:\n");
	set_flags_init();
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
	unsigned a = 0x0;
	unsigned b = 0x0;

	a = 0xFFFFFFFF;
	write_r(rn, a);
	b = 0x55555555;
	write_r(rm, b);

	int type = 0; 
	for (type = 0; type < 4; type ++)
	{
		int imm5 = 0;
		for (imm5 = 0; imm5 < 32; imm5 += 3)
		{
			if (type == 0)
			{
				unsigned bs = b << imm5;
				bool this_c = false;
  				//if (((b >> (32 - imm5)) & 0x1) == 0x1)
				//	this_c = true;
				test_add_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
				test_adc_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
				test_rsb_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
				test_sbc_dis_reg(rn, rm, rd, a, bs, false, type, imm5);
				test_sub_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
			} else
			if (type == 1)
			{
				unsigned bs = b >> imm5;
				if (imm5 == 0)
					bs = 0x0;
				bool this_c = false;
  				//if (((b >> (32 - imm5)) & 0x1) == 0x1)
				//	this_c = true;
				test_add_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
				test_adc_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
				test_rsb_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
				test_sbc_dis_reg(rn, rm, rd, a, bs, false, type, imm5);
				test_sub_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
			} else
			if (type == 2)
			{
				unsigned bs = b >> imm5;
				if (imm5 == 0)
				{
	  				if (((b >> 31) & 0x1) == 0x1)
						bs = 0xffffffff;
					else
						bs = 0x0;
				} else
				{
  					if (((b >> 31) & 0x1) == 0x1)
						bs += ~(0xffffffff >> imm5);
				}
				bool this_c = false;
  				//if (((b >> (32 - imm5)) & 0x1) == 0x1)
				//	this_c = true;
				test_add_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
				test_adc_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
				test_rsb_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
				test_sbc_dis_reg(rn, rm, rd, a, bs, false, type, imm5);
				test_sub_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
			} else
			{
				unsigned bs = b >> imm5;
				bs += b << (32 - imm5);
				if (imm5 == 0)
				{
					bs = b >> 1;
				}
				bool this_c = false;
  				//if (((b >> (32 - imm5)) & 0x1) == 0x1)
				//	this_c = true;
				test_add_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
				test_adc_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
				test_rsb_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
			//	test_sbc_dis_reg(rn, rm, rd, a, bs, true, type, imm5);
				test_sub_dis_reg(rn, rm, rd, a, bs, this_c, type, imm5);
			}
		}
	}
}

void test_all_tei_imm(int group, int rn, int rd, unsigned a, unsigned b, bool setflag, unsigned i, unsigned imm3, unsigned imm8)
{
	if (group == 1)
	{
		test_and_tei_imm(rn, rd, a, b, setflag, i, imm3, imm8);
		test_bic_tei_imm(rn, rd, a, b, setflag, i, imm3, imm8);
		test_eor_tei_imm(rn, rd, a, b, setflag, i, imm3, imm8);
		test_orr_tei_imm(rn, rd, a, b, setflag, i, imm3, imm8);
		test_orn_tei_imm(rn, rd, a, b, setflag, i, imm3, imm8);
		test_teq_tei_imm(rn, rd, a, b, setflag, i, imm3, imm8);
		test_tst_tei_imm(rn, rd, a, b, setflag, i, imm3, imm8);
	}

		test_adc_tei_imm(rn, rd, a, b, setflag, i, imm3, imm8, true);
		test_adc_tei_imm(rn, rd, a, b, setflag, i, imm3, imm8, false);
		test_add_tei_imm(rn, rd, a, b, setflag, i, imm3, imm8);
		test_cmn_tei_imm(rn, rd, a, b, setflag, i, imm3, imm8);
		test_cmp_tei_imm(rn, rd, a, b, setflag, i, imm3, imm8);
		test_rsb_tei_imm(rn, rd, a, b, setflag, i, imm3, imm8);
		test_sbc_tei_imm(rn, rd, a, b, setflag, i, imm3, imm8, true);
		test_sbc_tei_imm(rn, rd, a, b, setflag, i, imm3, imm8, false);
		test_sub_tei_imm(rn, rd, a, b, setflag, i, imm3, imm8);
}

//////////////////////////////////////////////////////////////////////////////////////
//	ThumbExpandImm 1
//////////////////////////////////////////////////////////////////////////////////////
void run_test_ThumbExpandImm1()
{

	fprintf(disasm, "-----------------------------------------\nrun_test_ThumbExpandImm1:\n");
	set_flags_init();
	
	int rn = 0;
	int rm = 1;
  	int rd = 2;
	unsigned a = 0x0;
	unsigned b = 0x0;

	int an = 0;
	int imm8n = 0;

	for (an = 0; an < 2; an++)
	{
		a = 0xFFFFFFFF;
		if (an == 1) a = 0x55555555;
		write_r(rn, a);

		for (imm8n = 0; imm8n < 2; imm8n++)
		{
			unsigned imm8 = 0xFF;
			if (imm8n == 1) imm8 = 0x55;

			int imm12_11_10 = 0; 
			for (imm12_11_10 = 0; imm12_11_10 < 4; imm12_11_10++)
			{
				int i = (imm12_11_10 >> 1) & 0x1;
				if (imm12_11_10 == 0)
				{
					int imm12_9_8 = 0;
					for (imm12_9_8 = 0; imm12_9_8 < 4; imm12_9_8++)
					{
						int imm3 = (imm12_11_10 & 0x1) << 2;
						imm3 += imm12_9_8;
						if (imm12_9_8 == 0)
						{
							b = imm8;
							test_all_tei_imm(1, rn, rd, a, b, true, i, imm3, imm8);
						} else
						if (imm12_9_8 == 1)
						{
							b = imm8;
							b += imm8 << 16;
							test_all_tei_imm(1, rn, rd, a, b, true, i, imm3, imm8);
						} else
						if (imm12_9_8 == 2)
						{
							b = imm8 << 8;
							b += imm8 << 24;
							test_all_tei_imm(1, rn, rd, a, b, true, i, imm3, imm8);
						} else
						if (imm12_9_8 == 3)
						{
							b = imm8;
							b += imm8 << 8;
							b += imm8 << 16;
							b += imm8 << 24;
							test_all_tei_imm(1, rn, rd, a, b, true, i, imm3, imm8);
						}
					}
				} else
				{
					unsigned unrot = imm8 & 0x7F;
					unrot += 0x80;
					int rotn = 0;
					for (rotn = 0; rotn < 4; rotn += 2)
					{
						int imm3 = (imm12_11_10 & 0x1) << 2;
						imm3 += rotn;
						int rot = (imm8 >> 7) & 0x1;
						rot += rotn << 1;
						rot += imm12_11_10 << 3;
						b = unrot >> rot;
						b += unrot << (32 - rot);
						test_all_tei_imm(1, rn, rd, a, b, true, i, imm3, imm8);
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//	ITBlock 
//////////////////////////////////////////////////////////////////////////////////////
void run_test_ITBlock()
{

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock:\n");
	set_flags_init();
	
	int rn = 4;
	int rm = 1;
  	int rd = 2;
	unsigned a = 0x0;
	unsigned b = 0x0;

	// it preparation

	write_r(0, 0x0);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: ADD reg\n");

	//////////////// FF + 1 sets c
	a = 0xFFFFFFFF;
	write_r(rn, a);
	b = 0x1;
	write_r(rm, b);
	unsigned result = a + b;

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_c = true => set to false, not modified => check for false

	set_flags(true, true, true, true, false, false, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_add_reg_t1(rd, rn, rm);
	check_c(false);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: ADD imm t1\n");

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_c = true => set to false, not modified => check for false

	set_flags(true, true, true, true, false, false, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_add_imm_t1(rd, rn, b);
	check_c(false);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: ADD imm t2\n");
	write_r(rd, a);

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_c = true => set to false, not modified => check for false

	set_flags(true, true, true, true, false, false, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_add_imm_t2(rd, b);
	check_c(false);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: ADC reg t1\n");
	write_r(rd, a);

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_c = true => set to false, not modified => check for false

	set_flags(true, true, true, true, false, false, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_adc_reg_t1(rd, b);
	check_c(false);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: SUB reg\n");

	//////////////// 0 - 0 = 0 + F + 1 sets c
	a = 0x0;
	write_r(rn, a);
	b = 0x0;
	write_r(rm, b);
	result = a - b;

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_c = true => set to false, not modified => check for false

	set_flags(true, true, true, true, false, false, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_sub_reg_t1(rd, rn, rm);
	check_c(false);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: SUB imm t1\n");

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_c = true => set to false, not modified => check for false

	set_flags(true, true, true, true, false, false, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_sub_imm_t1(rd, rn, b);
	check_c(false);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: SUB imm t2\n");
	write_r(rd, a);

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_c = true => set to false, not modified => check for false

	set_flags(true, true, true, true, false, false, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_sub_imm_t2(rd, b);
	check_c(false);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: SBC reg t1\n");
	write_r(rd, a);

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_c = false => set to true, not modified => check for true   DIFFERENT to previous !!!

	set_flags(true, false, true, true, false, false, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_sbc_reg_t1(rd, 0x5);
	check_c(true);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: RSB imm t1\n");

	//////////////// 0 - 0 = 0 + F + 1 sets c

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_c = true => set to false, not modified => check for false

	set_flags(true, true, true, true, false, false, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_rsb_imm_t1(rd, rn);
	check_c(false);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: AND reg t1\n");

	//////////////// F & F sets n
	a = 0xFFFFFFFF;
	write_r(rn, a);
	b = 0xFFFFFFFF;
	write_r(rm, b);

	write_r(rd, a);

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_n = true => set to false, not modified => check for false

	set_flags(true, true, true, true, true, true, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_and_reg_t1(rd, rm);
	check_n(false);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: BIC reg t1\n");

	//////////////// F & ~0 sets n
	b = 0x0;
	write_r(rm, b);

	write_r(rd, a);

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_n = true => set to false, not modified => check for false

	set_flags(true, true, true, true, true, true, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_bic_reg_t1(rd, rm);
	check_n(false);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: EOR reg t1\n");

	//////////////// F ^ 0 sets n
	write_r(rd, a);

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_n = true => set to false, not modified => check for false

	set_flags(true, true, true, true, true, true, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_eor_reg_t1(rd, rm);
	check_n(false);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: ORR reg t1\n");

	//////////////// F | 0 sets n
	write_r(rd, a);

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_n = true => set to false, not modified => check for false

	set_flags(true, true, true, true, true, true, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_orr_reg_t1(rd, rm);
	check_n(false);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: ASR reg t1\n");

	//////////////// F >> 1 sets n
	b = 0x1;
	write_r(rm, b);
	write_r(rd, a);

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_n = true => set to false, not modified => check for false

	set_flags(true, true, true, true, true, true, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_asr_reg_t1(rd, rm);
	check_n(false);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: ASR imm t1\n");

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_n = true => set to false, not modified => check for false

	set_flags(true, true, true, true, true, true, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_asr_imm_t1(rd, rn, b);
	check_n(false);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: ROR reg t1\n");
	write_r(rd, a);

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_n = true => set to false, not modified => check for false

	set_flags(true, true, true, true, true, true, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_ror_reg_t1(rd, rm);
	check_n(false);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: LSL reg t1\n");
	write_r(rd, a);

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_n = true => set to false, not modified => check for false

	set_flags(true, true, true, true, true, true, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_lsl_reg_t1(rd, rm);
	check_n(false);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: LSL imm t1\n");

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_n = true => set to false, not modified => check for false

	set_flags(true, true, true, true, true, true, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_lsl_imm_t1(rd, rn, b);
	check_n(false);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: LSR reg t1\n");
	write_r(rd, a);

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_n = false => set to true, not modified => check for true   DIFFERENT

	set_flags(true, true, true, true, true, false, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_lsr_reg_t1(rd, rm);
	check_n(true);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: LSR reg t1\n");

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_n = false => set to true, not modified => check for true   DIFFERENT

	set_flags(true, true, true, true, true, false, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_lsr_imm_t1(rd, rn, b);
	check_n(true);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: MUL t1\n");
	write_r(rd, a);

	// it block:	this_z = true, 0xbf08 => not modified
	// test modified  this_n = true => set to false, not modified => check for false

	set_flags(true, true, true, true, true, true, false, false);  // c, z, n, v
	execute_instruction16(0xbf08); 	//it 	eq  (z = 0)  

	execute_mul_t1(rd, rm);
	check_n(false);

	fprintf(disasm, "-----------------------------------------\nrun_test_ITBlock: z = 1\n");

	unsigned on = 0xFFFFFFFF;
	unsigned off = 0x0;
	write_r(5, 0x0);
	int cond = 0;
	for (cond = 0; cond <= 5; cond += 1)
	{
		int length = 0;
		int maxLength = 4;
		if (cond == 5) maxLength = 2;
		for (length = 1; length <= maxLength; length++)
		{	
			printf("\nlength: %x\n", length);
			int minMask = 1;
			int maxMask = 1;
			if (length == 2) minMask = 2;
			if (length == 2) maxMask = 3;
			if (length == 3) minMask = 4;
			if (length == 3) maxMask = 7;
			if (length == 4) minMask = 8;
			if (length == 4) maxMask = 15;
			unsigned mask = 0;
			for (mask = minMask; mask <= maxMask; mask += 1)
			{
				unsigned maskInv = 0x0;
				if ((mask & 0x1) > 0) maskInv += 8;
				if ((mask & 0x2) > 0) maskInv += 4;
				if ((mask & 0x4) > 0) maskInv += 2;
				if ((mask & 0x8) > 0) maskInv += 1;
				printf("cond: %x mask: %x maskInv: %x\n", cond, mask, maskInv);
				////////////////////////////////
				//	do execute
				////////////////////////////////
				int i = 0;
				for (i = 0; i <= length; i++)
				{
					write_r(i, on);
				}
	
				unsigned condLB = 0x0;
				if (cond == 0)
				{
					//set_flags(true, true, true, true, true, true, false, false);  // c, z, n, v
					// 0[5] + 0[5] = 0 => z set;
					execute_add_reg_t3(12, 5, 5, true, 0, 0);
				}
				if (cond == 1)
				{
					//set_flags(true, true, true, false, true, true, false, false);  // c, z, n, v
					// 0[5] + 8 = 8 => z clear;
					execute_add_imm_t3(12, 5, true, 0, 0, 8);
					condLB = 0x1;
				}
				if (cond == 2)
				{
					//set_flags(true, true, true, true, true, true, false, false);  // c, z, n, v
					// FF[0] + 1 = 100 => c set;
					execute_add_imm_t3(12, 0, true, 0, 0, 1);
				}
				if (cond == 3)
				{
					//set_flags(true, true, true, false, true, true, false, false);  // c, z, n, v
					// 0[5] + 8 = 8 => c clear;
					execute_add_imm_t3(12, 5, true, 0, 0, 8);
					condLB = 0x1;
				}
				if (cond == 4)
				{
					//set_flags(true, true, true, true, true, true, false, false);  // c, z, n, v
					// FF[0] + 0[5] = 0 => n set;
					execute_add_reg_t3(12, 0, 5, true, 0, 0);
				}
				if (cond == 5)
				{
					//set_flags(true, true, true, false, true, true, false, false);  // c, z, n, v
					// 0[5] + 8 = 8 => n clear;
					execute_add_imm_t3(12, 5, true, 0, 0, 8);
					condLB = 0x1;
				}

				execute_it_t1(cond, maskInv);
	
				for (i = length; i > 0; i--)
				{
					execute_and_reg_t1(i, 5);
				}

			 	execute_and_reg_t2(0, 0, 5, false, 0, 0);
	
				check_rd(length, off, 6);
				printf("check: %x off\n", length);

				if (length > 1)
				{
					if (condLB == 0x0)
					{
						if ((maskInv & 0x8) == 0x0)
						{
							check_rd(length - 1, off, 6);
							printf("check: %x off\n", length - 1);
						} else
						{
							check_rd(length - 1, on, 6);
							printf("check: %x on\n", length - 1);
						}
					} else
					{
						if ((maskInv & 0x8) > 0x0)
						{
							check_rd(length - 1, off, 6);
							printf("check: %x off\n", length - 1);
						} else
						{
							check_rd(length - 1, on, 6);
							printf("check: %x on\n", length - 1);
						}
					}
				}
				if (length > 2)
				{
					if (condLB == 0x0)
					{
						if ((maskInv & 0x4) == 0x0)
						{
							check_rd(length - 2, off, 6);
							printf("check: %x off\n", length - 2);
						} else
						{
							check_rd(length - 2, on, 6);
							printf("check: %x on\n", length - 2);
						}
					} else
					{
						if ((maskInv & 0x4) > 0x0)
						{
							check_rd(length - 2, off, 6);
							printf("check: %x off\n", length - 2);
						} else
						{
							check_rd(length - 2, on, 6);
							printf("check: %x on\n", length - 2);
						}
					}
				}
				if (length > 3)
				{
					if (condLB == 0x0)
					{
						if ((maskInv & 0x2) == 0x0)
						{
							check_rd(length - 3, off, 6);
							printf("check: %x off\n", length - 3);
						} else
						{
							check_rd(length - 3, on, 6);
							printf("check: %x on\n", length - 3);
						}
					} else
					{
						if ((maskInv & 0x2) > 0x0)
						{
							check_rd(length - 3, off, 6);
							printf("check: %x off\n", length - 3);
						} else
						{
							check_rd(length - 3, on, 6);
							printf("check: %x on\n", length - 3);
						}
					}
				}

				check_rd(0, off, 6);
				printf("check: %x off\n", 0);
	if (1)
	{
				////////////////////////////////
				//	do NOT execute
				////////////////////////////////
				for (i = 0; i <= length; i++)
				{
					write_r(i, on);
				}
	
				condLB = 0x0;
				if (cond == 1)
				{
					//set_flags(true, true, true, true, true, true, false, false);  // c, z, n, v
					// 0[5] + 0[5] = 0 => z set;
					execute_add_reg_t3(12, 5, 5, true, 0, 0);
				}
				if (cond == 0)
				{
					//set_flags(true, true, true, false, true, true, false, false);  // c, z, n, v
					// 0[5] + 8 = 8 => z clear;
					execute_add_imm_t3(12, 5, true, 0, 0, 8);
					condLB = 0x1;
				}
				if (cond == 3)
				{
					//set_flags(true, true, true, true, true, true, false, false);  // c, z, n, v
					// FF[0] + 1 = 100 => c set;
					execute_add_imm_t3(12, 0, true, 0, 0, 1);
				}
				if (cond == 2)
				{
					//set_flags(true, true, true, false, true, true, false, false);  // c, z, n, v
					// 0[5] + 8 = 8 => c clear;
					execute_add_imm_t3(12, 5, true, 0, 0, 8);
					condLB = 0x1;
				}
				if (cond == 5)
				{
					//set_flags(true, true, true, true, true, true, false, false);  // c, z, n, v
					// FF[0] + 0[5] = 0 => n set;
					execute_add_reg_t3(12, 0, 5, true, 0, 0);
				}
				if (cond == 4)
				{
					//set_flags(true, true, true, false, true, true, false, false);  // c, z, n, v
					// 0[5] + 8 = 8 => n clear;
					execute_add_imm_t3(12, 5, true, 0, 0, 8);
					condLB = 0x1;
				}


				execute_it_t1(cond, maskInv);
	
				for (i = length; i > 0; i--)
				{
					execute_and_reg_t1(i, 5);
				}
	
			 	execute_and_reg_t2(0, 0, 5, false, 0, 0);
	
				check_rd(length, on, 6);
				printf("check: %x on\n", length);

				if (length > 1)
				{
					if (condLB == 0x0)
					{
						if ((maskInv & 0x8) == 0x0)
						{
							check_rd(length - 1, off, 6);
							printf("check: %x off\n", length - 1);
						} else
						{
							check_rd(length - 1, on, 6);
							printf("check: %x on\n", length - 1);
						}
					} else
					{
						if ((maskInv & 0x8) > 0x0)
						{
							check_rd(length - 1, off, 6);
							printf("check: %x off\n", length - 1);
						} else
						{
							check_rd(length - 1, on, 6);
							printf("check: %x on\n", length - 1);
						}
					}
				}
				if (length > 2)
				{
					if (condLB == 0x0)
					{
						if ((maskInv & 0x4) == 0x0)
						{
							check_rd(length - 2, off, 6);
							printf("check: %x off\n", length - 2);
						} else
						{
							check_rd(length - 2, on, 6);
							printf("check: %x on\n", length - 2);
						}
					} else
					{
						if ((maskInv & 0x4) > 0x0)
						{
							check_rd(length - 2, off, 6);
							printf("check: %x off\n", length - 2);
						} else
						{
							check_rd(length - 2, on, 6);
							printf("check: %x on\n", length - 2);
						}
					}
				}
				if (length > 3)
				{
					if (condLB == 0x0)
					{
						if ((maskInv & 0x2) == 0x0)
						{
							check_rd(length - 3, off, 6);
							printf("check: %x off\n", length - 3);
						} else
						{
							check_rd(length - 3, on, 6);
							printf("check: %x on\n", length - 3);
						}
					} else
					{
						if ((maskInv & 0x2) > 0x0)
						{
							check_rd(length - 3, off, 6);
							printf("check: %x off\n", length - 3);
						} else
						{
							check_rd(length - 3, on, 6);
							printf("check: %x on\n", length - 3);
						}
					}
				}

				check_rd(0, off, 6);
				printf("check: %x off\n", 0);
	}
			}

		}
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////
//	Push Pop 
//////////////////////////////////////////////////////////////////////////////////////
void run_test_PushPop()
{
	fprintf(disasm, "-----------------------------------------\nrun_test_PushPop: single cycle, non consecutive, non dependent\n");

	fprintf(disasm, "-----------------------------------------\nrun_test_PushPop: execute_push/pop_t1\n");
	
	int n = 0;
	int sh = 1;
	int m = 0;
	
	for (n = 0; n <= 8; n++)
	{
		if (n == 8)
		{
			m = 1;
			n = 14;
		}
		printf("m: %d sh: %x\n", m, sh);

		write_r(n, 0x0);
		execute_push_t1(m, sh);
		write_r(n, 0xFFFFFFFF);
		execute_push_t1(m, sh);


		if (n == 14)					// missing pop P (15)
		{
			m = 0;
			n = 0;
			write_r(n, 0x55555555);
			execute_pop_t1(m, 1);
			check_rd(n, 0xFFFFFFFF, 3);
			execute_pop_t1(m, 1);
			check_rd(n, 0x0, 0);
			n = 8;
		} else
		{
			execute_pop_t1(m, sh);
			if (n < 2)
				check_rd(n, 0xFFFFFFFF, 3);
			else
				check_rd(n, 0xFFFFFFFF, 0);
			execute_pop_t1(m, sh);
			if (n < 2)
				check_rd(n, 0x0, 3);
			else
				check_rd(n, 0x0, 0);
		}

		sh = sh << 1;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_PushPop: execute_push/pop_t2\n");
	
	sh = 1;
	m = 0;
	
	for (n = 0; n <= 13; n++)
	{
		if (n == 13)
		{
			m = 1;
			n = 14;
		}
		printf("m: %d sh: %x\n", m, sh);

		write_r(n, 0x0);
		execute_push_t2(m, sh);
		write_r(n, 0xFFFFFFFF);
		execute_push_t2(m, sh);


		if (n == 14)					// missing pop P (15)
		{
			m = 0;
			n = 0;
			write_r(n, 0x55555555);
			execute_pop_t2(0, m, 1);
			check_rd(n, 0xFFFFFFFF, 3);
			execute_pop_t2(0, m, 1);
			check_rd(n, 0x0, 0);
			n = 13;
		} else
		{
			execute_pop_t2(0, m, sh);
			if (n < 2)
				check_rd(n, 0xFFFFFFFF, 3);
			else
				check_rd(n, 0xFFFFFFFF, 0);
			execute_pop_t2(0, m, sh);
			if (n < 2)
				check_rd(n, 0x0, 3);
			else
				check_rd(n, 0x0, 0);
		}

		sh = sh << 1;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_PushPop: single cycle, !!! consecutive, non dependent\n");
	
	sh = 1;
	
	for (n = 0; n <= 4; n++)
	{
		printf("sh: %x\n", sh);

		write_r(n, 0x0);
		write_r(n + 1, 0xFFFFFFFF);
		write_r(n + 2, 0x55555555);
		write_r(n + 3, 0xAAAAAAAA);
		execute_push_t1(0x0, sh);
		execute_push_t1(0x0, sh << 1);

		execute_pop_t1(0x0, sh);
		execute_pop_t1(0x0, sh << 1);

		execute_push_t1(0x0, sh << 2);
		execute_push_t1(0x0, sh << 3);

		execute_pop_t1(0x0, sh << 2);
		execute_pop_t1(0x0, sh << 3);

		if (n < 2)
			check_rd(n, 0xFFFFFFFF, 7);
		else
			check_rd(n, 0xFFFFFFFF, 0);
		if (n + 1 < 2)
			check_rd(n + 1, 0x0, 7);
		else
			check_rd(n + 1, 0x0, 0);
		if (n + 2 < 2)
			check_rd(n + 2, 0xAAAAAAAA, 7);
		else
			check_rd(n + 2, 0xAAAAAAAA, 0);
		if (n + 3 < 2)
			check_rd(n + 3, 0x55555555, 7);
		else
			check_rd(n + 3, 0x55555555, 0);

		sh = sh << 1;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_PushPop: execute_push/pop_t2\n");
	
	sh = 1;
	
	for (n = 0; n <= 9; n++)
	{
		printf("sh: %x\n", sh);

		write_r(n, 0x0);
		write_r(n + 1, 0xFFFFFFFF);
		write_r(n + 2, 0x55555555);
		write_r(n + 3, 0xAAAAAAAA);
		execute_push_t2(0x0, sh);
		execute_push_t2(0x0, sh << 1);

		execute_pop_t2(0x0, 0x0, sh);
		execute_pop_t2(0x0, 0x0, sh << 1);

		execute_push_t2(0x0, sh << 2);
		execute_push_t2(0x0, sh << 3);

		execute_pop_t2(0x0, 0x0, sh << 2);
		execute_pop_t2(0x0, 0x0, sh << 3);

		if (n < 2)
			check_rd(n, 0xFFFFFFFF, 7);
		else
			check_rd(n, 0xFFFFFFFF, 0);
		if (n + 1 < 2)
			check_rd(n + 1, 0x0, 7);
		else
			check_rd(n + 1, 0x0, 0);
		if (n + 2 < 2)
			check_rd(n + 2, 0xAAAAAAAA, 7);
		else
			check_rd(n + 2, 0xAAAAAAAA, 0);
		if (n + 3 < 2)
			check_rd(n + 3, 0x55555555, 7);
		else
			check_rd(n + 3, 0x55555555, 0);

		sh = sh << 1;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_PushPop: 2 cycles, non consecutive, non dependent\n");
	
	sh = 3;
	
	for (n = 0; n < 7; n++)
	{
		printf("sh: %x\n", sh);

		write_r(n, 0x0);
		write_r(n + 1, 0x1);
		execute_push_t1(0x0, sh);

		write_r(n, 0xFFFFFFFF);
		write_r(n + 1, 0xFFFFFFFE);
		execute_push_t1(0x0, sh);

		write_r(n, 0x55555555);
		write_r(n + 1, 0x55555555);

		execute_pop_t1(0x0, sh);
		if (n < 2)
			check_rd(n, 0xFFFFFFFF, 3);
		else
			check_rd(n, 0xFFFFFFFF, 0);
		if (n < 2)
			check_rd(n + 1, 0xFFFFFFFE, 3);
		else
			check_rd(n + 1, 0xFFFFFFFE, 0);
		execute_pop_t1(0x0, sh);
		if (n < 2)
			check_rd(n, 0x0, 3);
		else
			check_rd(n, 0x0, 0);
		if (n < 2)
			check_rd(n + 1, 0x1, 3);
		else
			check_rd(n + 1, 0x1, 0);

		sh = sh << 1;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_PushPop: execute_push/pop_t2\n");
	
	sh = 3;
	
	for (n = 0; n < 12; n++)
	{
		printf("sh: %x\n", sh);

		write_r(n, 0x0);
		write_r(n + 1, 0x1);
		execute_push_t2(0x0, sh);

		write_r(n, 0xFFFFFFFF);
		write_r(n + 1, 0xFFFFFFFE);
		execute_push_t2(0x0, sh);

		write_r(n, 0x55555555);
		write_r(n + 1, 0x55555555);

		execute_pop_t2(0x0, 0x0, sh);
		if (n < 2)
			check_rd(n, 0xFFFFFFFF, 3);
		else
			check_rd(n, 0xFFFFFFFF, 0);
		if (n < 2)
			check_rd(n + 1, 0xFFFFFFFE, 3);
		else
			check_rd(n + 1, 0xFFFFFFFE, 0);
		execute_pop_t2(0x0, 0x0, sh);
		if (n < 2)
			check_rd(n, 0x0, 3);
		else
			check_rd(n, 0x0, 0);
		if (n < 2)
			check_rd(n + 1, 0x1, 3);
		else
			check_rd(n + 1, 0x1, 0);

		sh = sh << 1;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_PushPop: 2 cycles, !!! consecutive, non dependent\n");
	
	sh = 3;
	
	for (n = 0; n < 1; n++)
	{
		printf("sh: %x\n", sh);

		write_r(n, 0x0);
		write_r(n + 1, 0x1);
		write_r(n + 2, 0x2);
		write_r(n + 3, 0x3);
		write_r(n + 4, 0x4);
		write_r(n + 5, 0x5);
		write_r(n + 6, 0x6);
		write_r(n + 7, 0x7);

		execute_push_t1(0x0, sh);
		execute_push_t1(0x0, sh << 2);

		execute_pop_t1(0x0, sh);
		execute_pop_t1(0x0, sh << 2);

		execute_push_t1(0x0, sh << 4);
		execute_push_t1(0x0, sh << 6);

		execute_pop_t1(0x0, sh << 6);		// missing: bug on the sam when flipped ? 
		execute_pop_t1(0x0, sh << 4);

		if (n < 2)
			check_rd(n, 0x2, 8);
		else
			check_rd(n, 0x2, 0);
		if (n + 1 < 2)
			check_rd(n + 1, 0x3, 8);
		else
			check_rd(n + 1, 0x3, 0);
		if (n + 2 < 2)
			check_rd(n + 2, 0x0, 8);
		else
			check_rd(n + 2, 0x0, 0);
		if (n + 3 < 2)
			check_rd(n + 3, 0x1, 8);
		else
			check_rd(n + 3, 0x1, 0);
		if (n + 4 < 2)
			check_rd(n + 4, 0x4, 8);
		else
			check_rd(n + 4, 0x4, 0);
		if (n + 5 < 2)
			check_rd(n + 5, 0x5, 8);
		else
			check_rd(n + 5, 0x5, 0);
		if (n + 6 < 2)
			check_rd(n + 6, 0x6, 8);
		else
			check_rd(n + 6, 0x6, 0);
		if (n + 7 < 2)
			check_rd(n + 7, 0x7, 8);
		else
			check_rd(n + 7, 0x7, 0);

		sh = sh << 1;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_PushPop: execute_push/pop_t2\n");
	
	sh = 3;
	
	for (n = 0; n < 6; n++)
	{
		printf("sh: %x\n", sh);

		write_r(n, 0x0);
		write_r(n + 1, 0x1);
		write_r(n + 2, 0x2);
		write_r(n + 3, 0x3);
		write_r(n + 4, 0x4);
		write_r(n + 5, 0x5);
		write_r(n + 6, 0x6);
		write_r(n + 7, 0x7);

		execute_push_t2(0x0, sh);
		execute_push_t2(0x0, sh << 2);

		execute_pop_t2(0x0, 0x0, sh);
		execute_pop_t2(0x0, 0x0, sh << 2);

		execute_push_t2(0x0, sh << 4);
		execute_push_t2(0x0, sh << 6);

		execute_pop_t2(0x0, 0x0, sh << 6);		// missing: bug on the sam when flipped ? 
		execute_pop_t2(0x0, 0x0, sh << 4);

		if (n < 1)
			check_rd(n, 0x2, 8);
		else
			check_rd(n, 0x2, 0);
		if (n < 1)
			check_rd(n + 1, 0x3, 8);
		else
			check_rd(n + 1, 0x3, 0);
		if (n < 1)
			check_rd(n + 2, 0x0, 8);
		else
			check_rd(n + 2, 0x0, 0);
		if (n < 1)
			check_rd(n + 3, 0x1, 8);
		else
			check_rd(n + 3, 0x1, 0);
		if (n < 1)
			check_rd(n + 4, 0x4, 8);
		else
			check_rd(n + 4, 0x4, 0);
		if (n < 1)
			check_rd(n + 5, 0x5, 8);
		else
			check_rd(n + 5, 0x5, 0);
		if (n < 1)
			check_rd(n + 6, 0x6, 8);
		else
			check_rd(n + 6, 0x6, 0);
		if (n < 1)
			check_rd(n + 7, 0x7, 8);
		else
			check_rd(n + 7, 0x7, 0);
		sh = sh << 1;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_PushPop: 3 cycles, non consecutive, non dependent\n");
	
	sh = 7;
	
	for (n = 0; n < 5; n++)
	{
		printf("sh: %x\n", sh);

		write_r(n, 0x0);
		write_r(n + 1, 0x1);
		write_r(n + 2, 0x2);
		execute_push_t1(0x0, sh);

		write_r(n, 0xFFFFFFFF);
		write_r(n + 1, 0xFFFFFFFE);
		write_r(n + 2, 0xFFFFFFFD);
		execute_push_t1(0x0, sh);

		write_r(n, 0x55555555);
		write_r(n + 1, 0x55555555);
		write_r(n + 2, 0x55555555);

		execute_pop_t1(0x0, sh);
		check_rd(n, 0xFFFFFFFF, 8);
		check_rd(n + 1, 0xFFFFFFFE, 8);
		check_rd(n + 2, 0xFFFFFFFD, 8);

		execute_pop_t1(0x0, sh);
		check_rd(n, 0x0, 8);
		check_rd(n + 1, 0x1, 8);
		check_rd(n + 2, 0x2, 8);

		sh = sh << 1;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_PushPop: execute_push/pop_t2\n");
	
	sh = 7;
	
	for (n = 0; n < 11; n++)
	{
		printf("sh: %x\n", sh);

		write_r(n, 0x0);
		write_r(n + 1, 0x1);
		write_r(n + 2, 0x2);
		execute_push_t2(0x0, sh);

		write_r(n, 0xFFFFFFFF);
		write_r(n + 1, 0xFFFFFFFE);
		write_r(n + 2, 0xFFFFFFFD);
		execute_push_t2(0x0, sh);

		write_r(n, 0x55555555);
		write_r(n + 1, 0x55555555);
		write_r(n + 2, 0x55555555);

		execute_pop_t2(0x0, 0x0, sh);
		check_rd(n, 0xFFFFFFFF, 8);
		check_rd(n + 1, 0xFFFFFFFE, 8);
		check_rd(n + 2, 0xFFFFFFFD, 8);

		execute_pop_t2(0x0, 0x0, sh);
		check_rd(n, 0x0, 8);
		check_rd(n + 1, 0x1, 8);
		check_rd(n + 2, 0x2, 8);

		sh = sh << 1;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_PushPop: single cycle, non consecutive, outgoing stall_from_dec\n");
	
	sh = 1;
	
	for (n = 0; n < 8; n++)
	{
		printf("sh: %x\n", sh);

		write_r(n, 0x0);
		execute_push_t1(0x0, sh);
		write_r(n, 0xFFFFFFFF);
		execute_push_t1(0x0, sh);

		write_r(n, 0x55555555);

		execute_pop_t1(0x0, sh);
		execute_eor_imm_t1(n + 1, n, false, 0, 0x3, 0xAA);
		if (n < 2)
			check_rd(n + 1, 0x55555555, 3);
		else
			check_rd(n + 1, 0x55555555, 0);

		execute_pop_t1(0x0, sh);
		execute_eor_imm_t1(n + 1, n, false, 0, 0x3, 0xAA);
		if (n < 2)
			check_rd(n + 1, 0xAAAAAAAA, 3);
		else
			check_rd(n + 1, 0xAAAAAAAA, 0);

		sh = sh << 1;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_PushPop: execute_push/pop_t2\n");
	
	sh = 1;
	
	for (n = 0; n < 12; n++)  // skip 12 because of n + 1 used
	{
		printf("sh: %x\n", sh);

		write_r(n, 0x0);
		execute_push_t2(0x0, sh);
		write_r(n, 0xFFFFFFFF);
		execute_push_t2(0x0, sh);

		write_r(n, 0x55555555);

		execute_pop_t2(0x0, 0x0, sh);
		execute_eor_imm_t1(n + 1, n, false, 0, 0x3, 0xAA);
		if (n < 2)
			check_rd(n + 1, 0x55555555, 3);
		else
			check_rd(n + 1, 0x55555555, 0);

		execute_pop_t2(0x0, 0x0, sh);
		execute_eor_imm_t1(n + 1, n, false, 0, 0x3, 0xAA);
		if (n < 2)
			check_rd(n + 1, 0xAAAAAAAA, 3);
		else
			check_rd(n + 1, 0xAAAAAAAA, 0);

		sh = sh << 1;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_PushPop: 2 cycles, non consecutive, outgoing stall_from_dec\n");
	
	sh = 3;
	
	for (n = 0; n < 7; n++)
	{
		printf("sh: %x\n", sh);

		write_r(n, 0x0);
		write_r(n + 1, 0x1);
		execute_push_t1(0x0, sh);

		write_r(n, 0xFFFFFFFF);
		write_r(n + 1, 0xFFFFFFFE);
		execute_push_t1(0x0, sh);

		write_r(n, 0x55555555);
		write_r(n + 1, 0x55555555);

		execute_pop_t1(0x0, sh);
		execute_eor_imm_t1(n + 2, n + 1, false, 0, 0x3, 0xAA);
		if (n < 2)
			check_rd(n, 0xFFFFFFFF, 3);
		else
			check_rd(n, 0xFFFFFFFF, 0);
		if (n < 2)
			check_rd(n + 2, 0x55555554, 3);
		else
			check_rd(n + 2, 0x55555554, 0);
		execute_pop_t1(0x0, sh);
		execute_eor_imm_t1(n + 2, n + 1, false, 0, 0x3, 0xAA);
		if (n < 2)
			check_rd(n, 0x0, 3);
		else
			check_rd(n, 0x0, 0);
		if (n < 2)
			check_rd(n + 2, 0xAAAAAAAB, 3);
		else
			check_rd(n + 2, 0xAAAAAAAB, 0);

		sh = sh << 1;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_PushPop: execute_push/pop_t2\n");
	
	sh = 3;
	
	for (n = 0; n < 11; n++)  // skip 11, 12 because of n + 1, 2 used
	{
		printf("sh: %x\n", sh);

		write_r(n, 0x0);
		write_r(n + 1, 0x1);
		execute_push_t2(0x0, sh);

		write_r(n, 0xFFFFFFFF);
		write_r(n + 1, 0xFFFFFFFE);
		execute_push_t2(0x0, sh);

		write_r(n, 0x55555555);
		write_r(n + 1, 0x55555555);

		execute_pop_t2(0x0, 0x0, sh);
		execute_eor_imm_t1(n + 2, n + 1, false, 0, 0x3, 0xAA);
		if (n < 2)
			check_rd(n, 0xFFFFFFFF, 3);
		else
			check_rd(n, 0xFFFFFFFF, 0);
		if (n < 2)
			check_rd(n + 2, 0x55555554, 3);
		else
			check_rd(n + 2, 0x55555554, 0);
		execute_pop_t2(0x0, 0x0, sh);
		execute_eor_imm_t1(n + 2, n + 1, false, 0, 0x3, 0xAA);
		if (n < 2)
			check_rd(n, 0x0, 3);
		else
			check_rd(n, 0x0, 0);
		if (n < 2)
			check_rd(n + 2, 0xAAAAAAAB, 3);
		else
			check_rd(n + 2, 0xAAAAAAAB, 0);

		sh = sh << 1;
	}

}
//////////////////////////////////////////////////////////////////////////////////////
//	LDM STM A 0
//////////////////////////////////////////////////////////////////////////////////////
void run_test_LDMSTM_A_0()
{
	int m = 0;
	int w = 1;
	int p = 0;
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: single cycle, non consecutive, non dependent\n");

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: execute_stmia/ldmia_t1\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = true\n");

	int rn = 0;
	int n = 0;
	int sh = 1;

	for (rn = 0; rn < 8; rn++)
	{
		sh = 1;
		for (n = 0; n < 8; n++)
		{
			if (rn != n)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x0);
				execute_stmia_t1(rn, sh);		// write 0
				check_rd(rn, 0x20072004, 8);		// check 4

				write_r(n, 0xAAAAAAAA);
				execute_stmia_t1(rn, sh);		// write 4
				check_rd(rn, 0x20072008, 8);		// check 8

				write_r(rn, 0x20072004);		// read 4
				execute_ldmia_t1(rn, sh);
				check_rd(n, 0xAAAAAAAA, 8);
				check_rd(rn, 0x20072008, 8);		// check 8

				write_r(rn, 0x20072000);		// read 0
				execute_ldmia_t1(rn, sh);
				check_rd(n, 0x0, 8);
				check_rd(rn, 0x20072004, 8);		// check 4

			}
			if (n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (rn > 0)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = false\n"); 

	// can't get it to work, bug in SAM ?

if (0)
{
	sh = 1;

	for (rn = 0; rn < 8; rn++)
	{
		sh = 1;
		for (n = 0; n < 8; n++)
		{
			if (rn == n)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x0);
				execute_stmia_t1(rn, sh);		// write 0
				check_rd(rn, 0x20072000, 8);		// check 4  <- problem

			if (0)
			{

				write_r(n, 0xAAAAAAAA);
				execute_stmia_t1(rn, sh);		// write 4
				check_rd(rn, 0x20072008, 8);		// check 8

				write_r(rn, 0x20072004);		// read 4
				execute_ldmia_t1(rn, sh);
				check_rd(n, 0xAAAAAAAA, 8);
				check_rd(rn, 0x20072008, 8);		// check 8

				write_r(rn, 0x20072000);		// read 0
				execute_ldmia_t1(rn, sh);
				check_rd(n, 0x0, 8);
				check_rd(rn, 0x20072004, 8);		// check 4
			}
			}
			if (n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (rn > 0)
			rn++;
	}
}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: execute_stmia/pop_t2\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = true\n");

if (0)
{
	rn = 0;
	n = 0;
	sh = 1;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 1;
		for (n = 0; n < 13; n++)
		{
			if (rn != n)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x0);
				execute_stmia_t2(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072004, 8);		// check 4

				write_r(n, 0xAAAAAAAA);
				execute_stmia_t2(rn, w, p, m, sh);	// write 4
				check_rd(rn, 0x20072008, 8);		// check 8

				write_r(rn, 0x20072004);		// read 4
				execute_ldmia_t2(rn, w, p, m, sh);
				check_rd(n, 0xAAAAAAAA, 8);
				check_rd(rn, 0x20072008, 8);		// check 8

				write_r(rn, 0x20072000);		// read 0
				execute_ldmia_t2(rn, w, p, m, sh);
				check_rd(n, 0x0, 8);
				check_rd(rn, 0x20072004, 8);		// check 4

			}
			if (1)
			{
				n++;
				sh = sh << 1;
				n++;
				sh = sh << 1;
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (1)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = false\n");

	sh = 1;
	w = 0;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 1;
		for (n = 0; n < 13; n++)
		{
			if (rn != n)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x0);
				execute_stmia_t2(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072000, 8);		// check 0

				write_r(n, 0xAAAAAAAA);
				execute_stmia_t2(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072000, 8);		// check 0

				execute_ldmia_t2(rn, w, p, m, sh);	// read 0
				check_rd(n, 0xAAAAAAAA, 8);
				check_rd(rn, 0x20072000, 8);		// check 0
			}
			if (1)
			{
				n++;
				sh = sh << 1;
				n++;
				sh = sh << 1;
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (1)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: single cycle, !!! consecutive, non dependent\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: execute_stmia/ldmia_t1\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = true\n");

	sh = 1;

	for (rn = 0; rn < 7; rn++)
	{
		int rn0 = rn;
		int rn1 = rn + 1;
		sh = 1;
		for (n = 0; n < 5; n++)
		{
			if (rn0 != n)
			if (rn0 != n + 1)
			if (rn0 != n + 2)
			if (rn0 != n + 3)
			if (rn1 != n)
			if (rn1 != n + 1)
			if (rn1 != n + 2)
			if (rn1 != n + 3)
			{
				unsigned sh0 = sh;
				unsigned sh1 = sh << 1;
				unsigned sh2 = sh << 2;
				unsigned sh3 = sh << 3;
				printf("rn0: %d rn1: %d sh0: %x sh1: %x sh2: %x sh3: %x\n", rn0, rn1, sh0, sh1, sh2, sh3);
	
				write_r(rn0, 0x20072000);
				write_r(rn1, 0x20072000);
				write_r(n, 0x0);
				write_r(n + 1, 0xFFFFFFFF);
				write_r(n + 2, 0xAAAAAAAA);
				write_r(n + 3, 0x55555555);
				execute_stmia_t1(rn0, sh0);		// write 0, 0
				execute_stmia_t1(rn0, sh1);		// write 4, F
				execute_ldmia_t1(rn1, sh1);		// read 0, F
				execute_ldmia_t1(rn1, sh0);		// read 4
				execute_stmia_t1(rn0, sh2);		// write 8, A
				execute_stmia_t1(rn0, sh3);		// write c, 5
				execute_ldmia_t1(rn1, sh3);		// read 8, 
				execute_ldmia_t1(rn1, sh2);		// read c

				check_rd(rn0, 0x20072010, 8);		// check 10
				check_rd(rn1, 0x20072010, 8);		// check 10
				check_rd(n + 1, 0x0, 8);
				check_rd(n + 0, 0xFFFFFFFF, 8);
				check_rd(n + 3, 0xAAAAAAAA, 8);
				check_rd(n + 2, 0x55555555, 8);
			}
			if (0) //n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (0) //rn > 0)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = false\n");

	// can't get it to work

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: execute_stmia/pop_t2\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = true\n");

	sh = 1;
	w = 1;

	for (rn = 0; rn < 12; rn++)
	{
		int rn0 = rn;
		int rn1 = rn + 1;
		sh = 1;
		for (n = 0; n < 10; n++)
		{
			if (rn0 != n)
			if (rn0 != n + 1)
			if (rn0 != n + 2)
			if (rn0 != n + 3)
			if (rn1 != n)
			if (rn1 != n + 1)
			if (rn1 != n + 2)
			if (rn1 != n + 3)
			{
				unsigned sh0 = sh;
				unsigned sh1 = sh << 1;
				unsigned sh2 = sh << 2;
				unsigned sh3 = sh << 3;
				printf("rn0: %d rn1: %d sh0: %x sh1: %x sh2: %x sh3: %x\n", rn0, rn1, sh0, sh1, sh2, sh3);
	
				write_r(rn0, 0x20072000);
				write_r(rn1, 0x20072000);
				write_r(n, 0x0);
				write_r(n + 1, 0xFFFFFFFF);
				write_r(n + 2, 0xAAAAAAAA);
				write_r(n + 3, 0x55555555);
				execute_stmia_t2(rn0, w, p, m, sh0);		// write 0, 0
				execute_stmia_t2(rn0, w, p, m, sh1);		// write 4, F
				execute_ldmia_t2(rn1, w, p, m, sh1);		// read 0, F
				execute_ldmia_t2(rn1, w, p, m, sh0);		// read 4
				execute_stmia_t2(rn0, w, p, m, sh2);		// write 8, A
				execute_stmia_t2(rn0, w, p, m, sh3);		// write c, 5
				execute_ldmia_t2(rn1, w, p, m, sh3);		// read 8, 
				execute_ldmia_t2(rn1, w, p, m, sh2);		// read c

				check_rd(rn0, 0x20072010, 8);		// check 10
				check_rd(rn1, 0x20072010, 8);		// check 10
				check_rd(n + 1, 0x0, 8);
				check_rd(n + 0, 0xFFFFFFFF, 8);
				check_rd(n + 3, 0xAAAAAAAA, 8);
				check_rd(n + 2, 0x55555555, 8);
			}
			if (0) //n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (1) //rn > 0)
			rn += 2;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = false\n");

	sh = 1;
	w = 0;

	for (rn = 0; rn < 12; rn++)
	{
		int rn0 = rn;
		int rn1 = rn + 1;
		sh = 1;
		for (n = 0; n < 10; n++)
		{
			if (rn0 != n)
			if (rn0 != n + 1)
			if (rn0 != n + 2)
			if (rn0 != n + 3)
			if (rn1 != n)
			if (rn1 != n + 1)
			if (rn1 != n + 2)
			if (rn1 != n + 3)
			{
				unsigned sh0 = sh;
				unsigned sh1 = sh << 1;
				unsigned sh2 = sh << 2;
				unsigned sh3 = sh << 3;
				printf("rn0: %d rn1: %d sh0: %x sh1: %x sh2: %x sh3: %x\n", rn0, rn1, sh0, sh1, sh2, sh3);
	
				write_r(rn0, 0x20072000);
				write_r(rn1, 0x20072000);
				write_r(n, 0x0);
				write_r(n + 1, 0xFFFFFFFF);
				write_r(n + 2, 0xAAAAAAAA);
				write_r(n + 3, 0x55555555);
				execute_stmia_t2(rn0, w, p, m, sh0);		// write 0, 0
				execute_stmia_t2(rn0, w, p, m, sh1);		// write 0, F
				execute_ldmia_t2(rn1, w, p, m, sh1);		// read 0, F
				execute_ldmia_t2(rn1, w, p, m, sh0);		// read 0, F
				execute_stmia_t2(rn0, w, p, m, sh2);		// write 0, A
				execute_stmia_t2(rn0, w, p, m, sh3);		// write 0, 5
				execute_ldmia_t2(rn1, w, p, m, sh3);		// read 0, 5
				execute_ldmia_t2(rn1, w, p, m, sh2);		// read 0, 5

				check_rd(rn0, 0x20072000, 8);		// check 0
				check_rd(rn1, 0x20072000, 8);		// check 0
				check_rd(n + 1, 0xFFFFFFFF, 8);
				check_rd(n + 0, 0xFFFFFFFF, 8);
				check_rd(n + 3, 0x55555555, 8);
				check_rd(n + 2, 0x55555555, 8);
			}
			if (0) //n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (1) //rn > 0)
			rn += 2;
	}
}

}
//////////////////////////////////////////////////////////////////////////////////////
//	LDM STM A 1
//////////////////////////////////////////////////////////////////////////////////////
void run_test_LDMSTM_A_1()
{
	int m = 0;
	int w = 1;
	int p = 0;

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: 2 cycles, non consecutive, non dependent\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: execute_stmia/ldmia_t1\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = true\n");

	int rn = 0;
	int n = 0;
	int sh = 3;
	w = 1;

	for (rn = 0; rn < 8; rn++)
	{
		sh = 3;
		for (n = 0; n < 7; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x0);
				write_r(n + 1, 0xAAAAAAAA);
				execute_stmia_t1(rn, sh);		// write 0
				check_rd(rn, 0x20072008, 8);		// check 4

				write_r(rn, 0x20072000);		// read 0
				execute_ldmia_t1(rn, sh);
				check_rd(rn, 0x20072008, 8);		// check 8
				check_rd(n + 1, 0xAAAAAAAA, 8);
				check_rd(n + 0, 0x0, 8);

			}
			if (n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (rn > 0)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = false\n"); 

	// can't get it to work, bug in SAM ?

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: execute_stmia/ldmia_t2\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = true\n");

	sh = 3;
	w = 1;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 3;
		for (n = 0; n < 12; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x0);
				write_r(n + 1, 0xAAAAAAAA);
				execute_stmia_t2(rn, w, p, m, sh);		// write 0
				check_rd(rn, 0x20072008, 8);		// check 4

				write_r(rn, 0x20072000);		// read 0
				execute_ldmia_t2(rn, w, p, m, sh);
				check_rd(rn, 0x20072008, 8);		// check 8
				check_rd(n + 1, 0xAAAAAAAA, 8);
				check_rd(n + 0, 0x0, 8);

			}
			if (n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (rn > 0)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = false\n");

	sh = 3;
	w = 0;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 3;
		for (n = 0; n < 12; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x0);
				write_r(n + 1, 0xAAAAAAAA);
				execute_stmia_t2(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072000, 8);		// check 0

				//write_r(rn, 0x20072000);		// read 0
				execute_ldmia_t2(rn, w, p, m, sh);
				check_rd(rn, 0x20072000, 8);		// check 0
				check_rd(n + 1, 0xAAAAAAAA, 8);
				check_rd(n + 0, 0x0, 8);

			}
			if (n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (rn > 0)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: 2 cycles, !!! consecutive, non dependent\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: execute_stmia/ldmia_t1\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = true\n");

	sh = 3;

	if (1)
	{
		int rn0 = 0;
		int rn1 = 1;
		unsigned sh0 = sh << 2;
		unsigned sh1 = sh << 4;
		unsigned sh2 = sh << 6;

		printf("rn0: %d rn1: %d sh0: %x sh1: %x sh2: %x\n", rn0, rn1, sh0, sh1, sh2);
	
		write_r(rn0, 0x20072000);
		write_r(rn1, 0x20072000);
		write_r(2, 0x2);
		write_r(3, 0x3);
		write_r(4, 0x4);
		write_r(5, 0x5);
		write_r(6, 0x6);
		write_r(7, 0x7);
		execute_stmia_t1(rn0, sh0);		// write 0, 2, 3
		execute_stmia_t1(rn0, sh1);		// write 8, 4, 5
		execute_ldmia_t1(rn1, sh1);		// read 0, 4, 5
		execute_ldmia_t1(rn1, sh0);		// read 8, 2, 3
		execute_stmia_t1(rn0, sh2);		// write 10, 6, 7
		execute_ldmia_t1(rn1, sh2);		// read 10, 6, 7

		check_rd(rn0, 0x20072018, 8);		// check 18
		check_rd(rn1, 0x20072018, 8);		// check 18
		check_rd(2, 0x4, 8);
		check_rd(3, 0x5, 8);
		check_rd(4, 0x2, 8);
		check_rd(5, 0x3, 8);
		check_rd(6, 0x6, 8);
		check_rd(7, 0x7, 8);
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = false\n");

	// can't get it to work

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: execute_stmia/pop_t2\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = true\n");

	sh = 3;
	w = 1;

	if (1)
	{
		int rn0 = 0;
		int rn1 = 1;
		unsigned sh0 = sh << 2;
		unsigned sh1 = sh << 4;
		unsigned sh2 = sh << 6;
		unsigned sh3 = sh << 8;

		printf("rn0: %d rn1: %d sh0: %x sh1: %x sh2: %x sh3: %x\n", rn0, rn1, sh0, sh1, sh2, sh3);
	
		write_r(rn0, 0x20072000);
		write_r(rn1, 0x20072000);
		write_r(2, 0x2);
		write_r(3, 0x3);
		write_r(4, 0x4);
		write_r(5, 0x5);
		write_r(6, 0x6);
		write_r(7, 0x7);
		write_r(8, 0x8);
		write_r(9, 0x9);
		execute_stmia_t2(rn0, w, p, m, sh0);		// write 0, 2, 3
		execute_stmia_t2(rn0, w, p, m, sh1);		// write 8, 4, 5
		execute_ldmia_t2(rn1, w, p, m, sh1);		// read 0, 4, 5
		execute_ldmia_t2(rn1, w, p, m, sh0);		// read 8, 2, 3
		execute_stmia_t2(rn0, w, p, m, sh2);		// write 10, 6, 7
		execute_stmia_t2(rn0, w, p, m, sh3);		// write 18, 8, 9
		execute_ldmia_t2(rn1, w, p, m, sh3);		// read 10, 8, 9
		execute_ldmia_t2(rn1, w, p, m, sh2);		// read 18, 6, 7

		check_rd(rn0, 0x20072020, 10);		// check 20
		check_rd(rn1, 0x20072020, 10);		// check 20
		check_rd(2, 0x4, 10);
		check_rd(3, 0x5, 10);
		check_rd(4, 0x2, 10);
		check_rd(5, 0x3, 10);
		check_rd(6, 0x8, 10);
		check_rd(7, 0x9, 10);
		check_rd(8, 0x6, 10);
		check_rd(9, 0x7, 10);
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = false\n");

	sh = 3;
	w = 0;

	if (1)
	{
		int rn0 = 0;
		int rn1 = 1;
		unsigned sh0 = sh << 2;
		unsigned sh1 = sh << 4;
		unsigned sh2 = sh << 6;
		unsigned sh3 = sh << 8;

		printf("rn0: %d rn1: %d sh0: %x sh1: %x sh2: %x sh3: %x\n", rn0, rn1, sh0, sh1, sh2, sh3);
	
		write_r(rn0, 0x20072000);
		write_r(rn1, 0x20072000);
		write_r(2, 0x2);
		write_r(3, 0x3);
		write_r(4, 0x4);
		write_r(5, 0x5);
		write_r(6, 0x6);
		write_r(7, 0x7);
		write_r(8, 0x8);
		write_r(9, 0x9);
		execute_stmia_t2(rn0, w, p, m, sh0);		// write 0, 2, 3
		execute_stmia_t2(rn0, w, p, m, sh1);		// write 0, 4, 5
		execute_ldmia_t2(rn1, w, p, m, sh1);		// read 0, 4, 5
		execute_ldmia_t2(rn1, w, p, m, sh0);		// read 0, 4, 5
		execute_stmia_t2(rn0, w, p, m, sh2);		// write 0, 6, 7
		execute_stmia_t2(rn0, w, p, m, sh3);		// write 0, 8, 9
		execute_ldmia_t2(rn1, w, p, m, sh3);		// read 0, 8, 9
		execute_ldmia_t2(rn1, w, p, m, sh2);		// read 0, 8, 9

		check_rd(rn0, 0x20072000, 10);		// check 0
		check_rd(rn1, 0x20072000, 10);		// check 0
		check_rd(2, 0x4, 10);
		check_rd(3, 0x5, 10);
		check_rd(4, 0x4, 10);
		check_rd(5, 0x5, 10);
		check_rd(6, 0x8, 10);
		check_rd(7, 0x9, 10);
		check_rd(8, 0x8, 10);
		check_rd(9, 0x9, 10);
	}

}
//////////////////////////////////////////////////////////////////////////////////////
//	LDM STM A 2
//////////////////////////////////////////////////////////////////////////////////////
void run_test_LDMSTM_A_2()
{
	int m = 0;
	int w = 1;
	int p = 0;

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: 3 cycles, non consecutive, non dependent\n");

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: execute_stmia/ldmia_t1\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = true\n");

	int rn = 0;
	int n = 0;
	int sh = 7;
	w = 1;

	for (rn = 0; rn < 8; rn++)
	{
		sh = 7;
		for (n = 0; n < 6; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			if (rn != n + 2)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x0);
				write_r(n + 1, 0xAAAAAAAA);
				write_r(n + 2, 0x55555555);
				execute_stmia_t1(rn, sh);		// write 0
				check_rd(rn, 0x2007200c, 8);		// check c

				write_r(rn, 0x20072000);		// read 0
				execute_ldmia_t1(rn, sh);
				check_rd(rn, 0x2007200c, 8);		// check c
				check_rd(n + 2, 0x55555555, 8);
				check_rd(n + 1, 0xAAAAAAAA, 8);
				check_rd(n + 0, 0x0, 8);

			}
			if (n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (rn > 0)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = false\n"); 

	// can't get it to work, bug in SAM ?

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: execute_stmia/ldmia_t2\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = true\n");

	sh = 7;
	w = 1;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 7;
		for (n = 0; n < 11; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			if (rn != n + 2)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x0);
				write_r(n + 1, 0xAAAAAAAA);
				write_r(n + 2, 0x55555555);
				execute_stmia_t2(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x2007200c, 8);		// check c

				write_r(rn, 0x20072000);		// read 0
				execute_ldmia_t2(rn, w, p, m, sh);
				check_rd(rn, 0x2007200c, 8);		// check c
				check_rd(n + 2, 0x55555555, 8);
				check_rd(n + 1, 0xAAAAAAAA, 8);
				check_rd(n + 0, 0x0, 8);

			}
			if (n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (rn > 0)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = false\n");

	sh = 7;
	w = 0;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 7;
		for (n = 0; n < 11; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			if (rn != n + 2)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x0);
				write_r(n + 1, 0xAAAAAAAA);
				write_r(n + 2, 0x55555555);
				execute_stmia_t2(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072000, 8);		// check 0

				//write_r(rn, 0x20072000);		// read 0
				execute_ldmia_t2(rn, w, p, m, sh);
				check_rd(rn, 0x20072000, 8);		// check 0
				check_rd(n + 2, 0x55555555, 8);
				check_rd(n + 1, 0xAAAAAAAA, 8);
				check_rd(n + 0, 0x0, 8);

			}
			if (n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (rn > 0)
			rn++;
	}

}
//////////////////////////////////////////////////////////////////////////////////////
//	LDM STM A 3
//////////////////////////////////////////////////////////////////////////////////////
void run_test_LDMSTM_A_3()
{
	int m = 0;
	int w = 1;
	int p = 0;

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: single cycle, non consecutive, outgoing stall_from_dec\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: execute_stmia/ldmia_t1\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = true\n");

	int rn = 0;
	int n = 0;
	int sh = 1;
	
	for (rn = 0; rn < 8; rn++)
	{
		sh = 1;
		for (n = 0; n < 8; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x0);
				execute_stmia_t1(rn, sh);		// write 0
				check_rd(rn, 0x20072004, 8);		// check 4

				write_r(n, 0xAAAAAAAA);
				execute_stmia_t1(rn, sh);		// write 4
				check_rd(rn, 0x20072008, 8);		// check 8

				write_r(rn, 0x20072004);		// read 4
				execute_ldmia_t1(rn, sh);
				execute_eor_imm_t1(n + 1, n, false, 0, 0x3, 0xFF);
				check_rd(n + 1, 0x55555555, 8);
				check_rd(rn, 0x20072008, 8);		// check 8

				write_r(rn, 0x20072000);		// read 0
				execute_ldmia_t1(rn, sh);
				execute_eor_imm_t1(n + 1, n, false, 0, 0x3, 0xAA);
				check_rd(n + 1, 0xAAAAAAAA, 8);
				check_rd(rn, 0x20072004, 8);		// check 4

			}
			if (n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (rn > 0)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = false\n"); 

	// can't get it to work, bug in SAM ?

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: execute_stmia/ldmia_t2\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = true\n");

	rn = 0;
	n = 0;
	sh = 1;
	w = 1;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 1;
		for (n = 0; n < 12; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x55555555);
				execute_stmia_t2(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072004, 8);		// check 4

				write_r(n, 0xAAAAAAAA);
				execute_stmia_t2(rn, w, p, m, sh);	// write 4
				check_rd(rn, 0x20072008, 8);		// check 8

				write_r(rn, 0x20072004);		// read 4
				execute_ldmia_t2(rn, w, p, m, sh);
				execute_eor_imm_t1(n + 1, n, false, 0, 0x3, 0xFF);
				check_rd(n + 1, 0x55555555, 8);
				check_rd(rn, 0x20072008, 8);		// check 8

				write_r(rn, 0x20072000);		// read 0
				execute_ldmia_t2(rn, w, p, m, sh);
				execute_eor_imm_t1(n + 1, n, false, 0, 0x3, 0xFF);
				check_rd(n + 1, 0xAAAAAAAA, 8);
				check_rd(rn, 0x20072004, 8);		// check 4

			}
			if (1)
			{
				n++;
				sh = sh << 1;
				n++;
				sh = sh << 1;
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (1)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = false\n");

	sh = 1;
	w = 0;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 1;
		for (n = 0; n < 12; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x0);
				execute_stmia_t2(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072000, 8);		// check 0

				write_r(n, 0xAAAAAAAA);
				execute_stmia_t2(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072000, 8);		// check 0

				execute_ldmia_t2(rn, w, p, m, sh);	// read 0
				execute_eor_imm_t1(n + 1, n, false, 0, 0x3, 0xFF);
				check_rd(n + 1, 0x55555555, 8);
				check_rd(rn, 0x20072000, 8);		// check 0
			}
			if (1)
			{
				n++;
				sh = sh << 1;
				n++;
				sh = sh << 1;
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (1)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: 2 cycles, non consecutive, outgoing stall_from_dec\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: execute_stmia/ldmia_t1\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = true\n");

	sh = 3;
	w = 1;

	for (rn = 0; rn < 8; rn++)
	{
		sh = 3;
		for (n = 0; n < 7; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			if (rn != n + 2)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x55555555);
				write_r(n + 1, 0xAAAAAAAA);
				execute_stmia_t1(rn, sh);		// write 0
				check_rd(rn, 0x20072008, 8);		// check 4

				write_r(rn, 0x20072000);		// read 0
				execute_ldmia_t1(rn, sh);
				execute_eor_imm_t1(n + 2, n, false, 0, 0x3, 0xFF);
				check_rd(rn, 0x20072008, 8);		// check 8
				check_rd(n + 1, 0xAAAAAAAA, 8);
				check_rd(n + 2, 0xAAAAAAAA, 8);

			}
			if (n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (rn > 0)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = false\n"); 

	// can't get it to work, bug in SAM ?

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: execute_stmia/ldmia_t2\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = true\n");

	sh = 3;
	w = 1;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 3;
		for (n = 0; n < 11; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			if (rn != n + 2)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x55555555);
				write_r(n + 1, 0xAAAAAAAA);
				execute_stmia_t2(rn, w, p, m, sh);		// write 0
				check_rd(rn, 0x20072008, 8);		// check 4

				write_r(rn, 0x20072000);		// read 0
				execute_ldmia_t2(rn, w, p, m, sh);
				execute_eor_imm_t1(n + 2, n, false, 0, 0x3, 0xFF);
				check_rd(rn, 0x20072008, 8);		// check 8
				check_rd(n + 1, 0xAAAAAAAA, 8);
				check_rd(n + 2, 0xAAAAAAAA, 8);

			}
			if (n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (rn > 0)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM A: wback = false\n");

	sh = 3;
	w = 0;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 3;
		for (n = 0; n < 11; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			if (rn != n + 2)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x55555555);
				write_r(n + 1, 0xAAAAAAAA);
				execute_stmia_t2(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072000, 8);		// check 0

				//write_r(rn, 0x20072000);		// read 0
				execute_ldmia_t2(rn, w, p, m, sh);
				execute_eor_imm_t1(n + 2, n, false, 0, 0x3, 0xFF);
				check_rd(rn, 0x20072000, 8);		// check 0
				check_rd(n + 1, 0xAAAAAAAA, 8);
				check_rd(n + 2, 0xAAAAAAAA, 8);

			}
			if (n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (rn > 0)
			rn++;
	}
}
//////////////////////////////////////////////////////////////////////////////////////
//	LDM STM B 0
//////////////////////////////////////////////////////////////////////////////////////
void run_test_LDMSTM_B_0()
{
	int m = 0;
	int w = 1;
	int p = 0;
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: single cycle, non consecutive, non dependent\n");

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: execute_stmdb/ldmdb_t1\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: wback = true\n");

	int rn = 0;
	int n = 0;
	int sh = 1;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 1;
		for (n = 0; n < 13; n++)
		{
			if (rn != n)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072008);
				write_r(n, 0x0);
				execute_stmdb_t1(rn, w, p, m, sh);	// write 4
				check_rd(rn, 0x20072004, 8);		// check 4

				write_r(n, 0xAAAAAAAA);
				execute_stmdb_t1(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072000, 8);		// check 0

				write_r(rn, 0x20072008);		// read 4
				execute_ldmdb_t1(rn, w, p, m, sh);
				check_rd(n, 0x0, 8);
				check_rd(rn, 0x20072004, 8);		// check 4

				//write_r(rn, 0x20072000);		// read 0
				execute_ldmdb_t1(rn, w, p, m, sh);
				check_rd(n, 0xAAAAAAAA, 8);
				check_rd(rn, 0x20072000, 8);		// check 0
			}
			if (1)
			{
				n++;
				sh = sh << 1;
				n++;
				sh = sh << 1;
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (1)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: wback = false\n");

	sh = 1;
	w = 0;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 1;
		for (n = 0; n < 13; n++)
		{
			if (rn != n)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x0);
				execute_stmdb_t1(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072000, 8);		// check 0

				write_r(n, 0xAAAAAAAA);
				execute_stmdb_t1(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072000, 8);		// check 0

				execute_ldmdb_t1(rn, w, p, m, sh);	// read 0
				check_rd(n, 0xAAAAAAAA, 8);
				check_rd(rn, 0x20072000, 8);		// check 0
			}
			if (1)
			{
				n++;
				sh = sh << 1;
				n++;
				sh = sh << 1;
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (1)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: single cycle, !!! consecutive, non dependent\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: execute_stmdb/ldmdb_t1\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: wback = true\n");

	sh = 1;
	w = 1;

	for (rn = 0; rn < 12; rn++)
	{
		int rn0 = rn;
		int rn1 = rn + 1;
		sh = 1;
		for (n = 0; n < 10; n++)
		{
			if (rn0 != n)
			if (rn0 != n + 1)
			if (rn0 != n + 2)
			if (rn0 != n + 3)
			if (rn1 != n)
			if (rn1 != n + 1)
			if (rn1 != n + 2)
			if (rn1 != n + 3)
			{
				unsigned sh0 = sh;
				unsigned sh1 = sh << 1;
				unsigned sh2 = sh << 2;
				unsigned sh3 = sh << 3;
				printf("rn0: %d rn1: %d sh0: %x sh1: %x sh2: %x sh3: %x\n", rn0, rn1, sh0, sh1, sh2, sh3);
	
				write_r(rn0, 0x20072010);
				write_r(rn1, 0x20072010);
				write_r(n, 0x0);
				write_r(n + 1, 0xFFFFFFFF);
				write_r(n + 2, 0xAAAAAAAA);
				write_r(n + 3, 0x55555555);
				execute_stmdb_t1(rn0, w, p, m, sh0);		// write 0, 0
				execute_stmdb_t1(rn0, w, p, m, sh1);		// write 4, F
				execute_ldmdb_t1(rn1, w, p, m, sh1);		// read 0, F
				execute_ldmdb_t1(rn1, w, p, m, sh0);		// read 4
				execute_stmdb_t1(rn0, w, p, m, sh2);		// write 8, A
				execute_stmdb_t1(rn0, w, p, m, sh3);		// write c, 5
				execute_ldmdb_t1(rn1, w, p, m, sh3);		// read 8, 
				execute_ldmdb_t1(rn1, w, p, m, sh2);		// read c

				check_rd(rn0, 0x20072000, 8);		// check 10
				check_rd(rn1, 0x20072000, 8);		// check 10
				check_rd(n + 1, 0x0, 8);
				check_rd(n + 0, 0xFFFFFFFF, 8);
				check_rd(n + 3, 0xAAAAAAAA, 8);
				check_rd(n + 2, 0x55555555, 8);
			}
			if (0) //n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (1) //rn > 0)
			rn += 2;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: wback = false\n");

	sh = 1;
	w = 0;

	for (rn = 0; rn < 12; rn++)
	{
		int rn0 = rn;
		int rn1 = rn + 1;
		sh = 1;
		for (n = 0; n < 10; n++)
		{
			if (rn0 != n)
			if (rn0 != n + 1)
			if (rn0 != n + 2)
			if (rn0 != n + 3)
			if (rn1 != n)
			if (rn1 != n + 1)
			if (rn1 != n + 2)
			if (rn1 != n + 3)
			{
				unsigned sh0 = sh;
				unsigned sh1 = sh << 1;
				unsigned sh2 = sh << 2;
				unsigned sh3 = sh << 3;
				printf("rn0: %d rn1: %d sh0: %x sh1: %x sh2: %x sh3: %x\n", rn0, rn1, sh0, sh1, sh2, sh3);
	
				write_r(rn0, 0x20072000);
				write_r(rn1, 0x20072000);
				write_r(n, 0x0);
				write_r(n + 1, 0xFFFFFFFF);
				write_r(n + 2, 0xAAAAAAAA);
				write_r(n + 3, 0x55555555);
				execute_stmdb_t1(rn0, w, p, m, sh0);		// write 0, 0
				execute_stmdb_t1(rn0, w, p, m, sh1);		// write 0, F
				execute_ldmdb_t1(rn1, w, p, m, sh1);		// read 0, F
				execute_ldmdb_t1(rn1, w, p, m, sh0);		// read 0, F
				execute_stmdb_t1(rn0, w, p, m, sh2);		// write 0, A
				execute_stmdb_t1(rn0, w, p, m, sh3);		// write 0, 5
				execute_ldmdb_t1(rn1, w, p, m, sh3);		// read 0, 5
				execute_ldmdb_t1(rn1, w, p, m, sh2);		// read 0, 5

				check_rd(rn0, 0x20072000, 8);		// check 0
				check_rd(rn1, 0x20072000, 8);		// check 0
				check_rd(n + 1, 0xFFFFFFFF, 8);
				check_rd(n + 0, 0xFFFFFFFF, 8);
				check_rd(n + 3, 0x55555555, 8);
				check_rd(n + 2, 0x55555555, 8);
			}
			if (0) //n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (1) //rn > 0)
			rn += 2;
	}

}
//////////////////////////////////////////////////////////////////////////////////////
//	LDM STM B 1
//////////////////////////////////////////////////////////////////////////////////////
void run_test_LDMSTM_B_1()
{
	int m = 0;
	int w = 1;
	int p = 0;

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: 2 cycles, non consecutive, non dependent\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: execute_stmdb/ldmdb_t1\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: wback = true\n");

	int rn = 0;
	int n = 0;
	int sh = 3;
	w = 1;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 3;
		for (n = 0; n < 12; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072008);
				write_r(n, 0x0);
				write_r(n + 1, 0xAAAAAAAA);
				execute_stmdb_t1(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072000, 8);		// check 4

				write_r(rn, 0x20072008);		// read 0
				execute_ldmdb_t1(rn, w, p, m, sh);
				check_rd(rn, 0x20072000, 8);		// check 8
				check_rd(n + 1, 0xAAAAAAAA, 8);
				check_rd(n + 0, 0x0, 8);

			}
			if (n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (rn > 0)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: wback = false\n");

	sh = 3;
	w = 0;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 3;
		for (n = 0; n < 12; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x0);
				write_r(n + 1, 0xAAAAAAAA);
				execute_stmdb_t1(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072000, 8);		// check 0

				//write_r(rn, 0x20072000);		// read 0
				execute_ldmdb_t1(rn, w, p, m, sh);
				check_rd(rn, 0x20072000, 8);		// check 0
				check_rd(n + 1, 0xAAAAAAAA, 8);
				check_rd(n + 0, 0x0, 8);

			}
			if (n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (rn > 0)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: 2 cycles, !!! consecutive, non dependent\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: execute_stmdb/ldmdb_t1\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: wback = true\n");

	sh = 3;
	w = 1;

	if (1)
	{
		int rn0 = 0;
		int rn1 = 1;
		unsigned sh0 = sh << 2;
		unsigned sh1 = sh << 4;
		unsigned sh2 = sh << 6;
		unsigned sh3 = sh << 8;

		printf("rn0: %d rn1: %d sh0: %x sh1: %x sh2: %x sh3: %x\n", rn0, rn1, sh0, sh1, sh2, sh3);
	
		write_r(rn0, 0x20072020);
		write_r(rn1, 0x20072020);
		write_r(2, 0x2);
		write_r(3, 0x3);
		write_r(4, 0x4);
		write_r(5, 0x5);
		write_r(6, 0x6);
		write_r(7, 0x7);
		write_r(8, 0x8);
		write_r(9, 0x9);
		execute_stmdb_t1(rn0, w, p, m, sh0);		// write 0, 2, 3
		execute_stmdb_t1(rn0, w, p, m, sh1);		// write 8, 4, 5
		execute_ldmdb_t1(rn1, w, p, m, sh1);		// read 0, 4, 5
		execute_ldmdb_t1(rn1, w, p, m, sh0);		// read 8, 2, 3
		execute_stmdb_t1(rn0, w, p, m, sh2);		// write 10, 6, 7
		execute_stmdb_t1(rn0, w, p, m, sh3);		// write 18, 8, 9
		execute_ldmdb_t1(rn1, w, p, m, sh3);		// read 10, 8, 9
		execute_ldmdb_t1(rn1, w, p, m, sh2);		// read 18, 6, 7

		check_rd(rn0, 0x20072000, 10);		// check 20
		check_rd(rn1, 0x20072000, 10);		// check 20
		check_rd(2, 0x4, 10);
		check_rd(3, 0x5, 10);
		check_rd(4, 0x2, 10);
		check_rd(5, 0x3, 10);
		check_rd(6, 0x8, 10);
		check_rd(7, 0x9, 10);
		check_rd(8, 0x6, 10);
		check_rd(9, 0x7, 10);
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: wback = false\n");

	sh = 3;
	w = 0;

	if (1)
	{
		int rn0 = 0;
		int rn1 = 1;
		unsigned sh0 = sh << 2;
		unsigned sh1 = sh << 4;
		unsigned sh2 = sh << 6;
		unsigned sh3 = sh << 8;

		printf("rn0: %d rn1: %d sh0: %x sh1: %x sh2: %x sh3: %x\n", rn0, rn1, sh0, sh1, sh2, sh3);
	
		write_r(rn0, 0x20072000);
		write_r(rn1, 0x20072000);
		write_r(2, 0x2);
		write_r(3, 0x3);
		write_r(4, 0x4);
		write_r(5, 0x5);
		write_r(6, 0x6);
		write_r(7, 0x7);
		write_r(8, 0x8);
		write_r(9, 0x9);
		execute_stmdb_t1(rn0, w, p, m, sh0);		// write 0, 2, 3
		execute_stmdb_t1(rn0, w, p, m, sh1);		// write 0, 4, 5
		execute_ldmdb_t1(rn1, w, p, m, sh1);		// read 0, 4, 5
		execute_ldmdb_t1(rn1, w, p, m, sh0);		// read 0, 4, 5
		execute_stmdb_t1(rn0, w, p, m, sh2);		// write 0, 6, 7
		execute_stmdb_t1(rn0, w, p, m, sh3);		// write 0, 8, 9
		execute_ldmdb_t1(rn1, w, p, m, sh3);		// read 0, 8, 9
		execute_ldmdb_t1(rn1, w, p, m, sh2);		// read 0, 8, 9

		check_rd(rn0, 0x20072000, 10);		// check 0
		check_rd(rn1, 0x20072000, 10);		// check 0
		check_rd(2, 0x4, 10);
		check_rd(3, 0x5, 10);
		check_rd(4, 0x4, 10);
		check_rd(5, 0x5, 10);
		check_rd(6, 0x8, 10);
		check_rd(7, 0x9, 10);
		check_rd(8, 0x8, 10);
		check_rd(9, 0x9, 10);
	}

}
//////////////////////////////////////////////////////////////////////////////////////
//	LDM STM B 2
//////////////////////////////////////////////////////////////////////////////////////
void run_test_LDMSTM_B_2()
{
	int m = 0;
	int w = 1;
	int p = 0;

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: 3 cycles, non consecutive, non dependent\n");

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: execute_stmdb/ldmdb_t1\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: wback = true\n");

	int rn = 0;
	int n = 0;
	int sh = 7;
	w = 1;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 7;
		for (n = 0; n < 11; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			if (rn != n + 2)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x2007200c);
				write_r(n, 0x0);
				write_r(n + 1, 0xAAAAAAAA);
				write_r(n + 2, 0x55555555);
				execute_stmdb_t1(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072000, 8);		// check c

				write_r(rn, 0x2007200c);		// read 0
				execute_ldmdb_t1(rn, w, p, m, sh);
				check_rd(rn, 0x20072000, 8);		// check c
				check_rd(n + 2, 0x55555555, 8);
				check_rd(n + 1, 0xAAAAAAAA, 8);
				check_rd(n + 0, 0x0, 8);

			}
			if (n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (rn > 0)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: wback = false\n");

	sh = 7;
	w = 0;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 7;
		for (n = 0; n < 11; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			if (rn != n + 2)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x0);
				write_r(n + 1, 0xAAAAAAAA);
				write_r(n + 2, 0x55555555);
				execute_stmdb_t1(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072000, 8);		// check 0

				//write_r(rn, 0x20072000);		// read 0
				execute_ldmdb_t1(rn, w, p, m, sh);
				check_rd(rn, 0x20072000, 8);		// check 0
				check_rd(n + 2, 0x55555555, 8);
				check_rd(n + 1, 0xAAAAAAAA, 8);
				check_rd(n + 0, 0x0, 8);

			}
			if (n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (rn > 0)
			rn++;
	}

}
//////////////////////////////////////////////////////////////////////////////////////
//	LDM STM B 3
//////////////////////////////////////////////////////////////////////////////////////
void run_test_LDMSTM_B_3()
{
	int m = 0;
	int w = 1;
	int p = 0;

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: single cycle, non consecutive, outgoing stall_from_dec\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: execute_stmdb/ldmdb_t1\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: wback = true\n");

	int rn = 0;
	int n = 0;
	int sh = 1;
	w = 1;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 1;
		for (n = 0; n < 12; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072008);
				write_r(n, 0x55555555);
				execute_stmdb_t1(rn, w, p, m, sh);	// write 8
				check_rd(rn, 0x20072004, 8);		// check 4

				write_r(n, 0xAAAAAAAA);
				execute_stmdb_t1(rn, w, p, m, sh);	// write 4
				check_rd(rn, 0x20072000, 8);		// check 0

				write_r(rn, 0x20072004);		// read 4
				execute_ldmdb_t1(rn, w, p, m, sh);
				execute_eor_imm_t1(n + 1, n, false, 0, 0x3, 0xFF);
				check_rd(n + 1, 0x55555555, 8);
				check_rd(rn, 0x20072000, 8);		// check 0

				write_r(rn, 0x20072008);		// read 4
				execute_ldmdb_t1(rn, w, p, m, sh);
				execute_eor_imm_t1(n + 1, n, false, 0, 0x3, 0xFF);
				check_rd(n + 1, 0xAAAAAAAA, 8);
				check_rd(rn, 0x20072004, 8);		// check 4

			}
			if (1)
			{
				n++;
				sh = sh << 1;
				n++;
				sh = sh << 1;
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (1)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: wback = false\n");

	sh = 1;
	w = 0;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 1;
		for (n = 0; n < 12; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x0);
				execute_stmdb_t1(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072000, 8);		// check 0

				write_r(n, 0xAAAAAAAA);
				execute_stmdb_t1(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072000, 8);		// check 0

				execute_ldmdb_t1(rn, w, p, m, sh);	// read 0
				execute_eor_imm_t1(n + 1, n, false, 0, 0x3, 0xFF);
				check_rd(n + 1, 0x55555555, 8);
				check_rd(rn, 0x20072000, 8);		// check 0
			}
			if (1)
			{
				n++;
				sh = sh << 1;
				n++;
				sh = sh << 1;
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (1)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: 2 cycles, non consecutive, outgoing stall_from_dec\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: execute_stmdb/ldmdb_t1\n");
	
	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: wback = true\n");

	sh = 3;
	w = 1;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 3;
		for (n = 0; n < 11; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			if (rn != n + 2)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072008);
				write_r(n, 0x55555555);
				write_r(n + 1, 0xAAAAAAAA);
				execute_stmdb_t1(rn, w, p, m, sh);		// write 0
				check_rd(rn, 0x20072000, 8);		// check 4

				write_r(rn, 0x20072008);		// read 0
				execute_ldmdb_t1(rn, w, p, m, sh);
				execute_eor_imm_t1(n + 2, n, false, 0, 0x3, 0xFF);
				check_rd(rn, 0x20072000, 8);		// check 8
				check_rd(n + 1, 0xAAAAAAAA, 8);
				check_rd(n + 2, 0xAAAAAAAA, 8);

			}
			if (n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (rn > 0)
			rn++;
	}

	fprintf(disasm, "-----------------------------------------\nrun_test_LDMSTM B: wback = false\n");

	sh = 3;
	w = 0;

	for (rn = 0; rn < 13; rn++)
	{
		sh = 3;
		for (n = 0; n < 11; n++)
		{
			if (rn != n)
			if (rn != n + 1)
			if (rn != n + 2)
			{
				printf("sh: %x\n", sh);
	
				write_r(rn, 0x20072000);
				write_r(n, 0x55555555);
				write_r(n + 1, 0xAAAAAAAA);
				execute_stmdb_t1(rn, w, p, m, sh);	// write 0
				check_rd(rn, 0x20072000, 8);		// check 0

				//write_r(rn, 0x20072000);		// read 0
				execute_ldmdb_t1(rn, w, p, m, sh);
				execute_eor_imm_t1(n + 2, n, false, 0, 0x3, 0xFF);
				check_rd(rn, 0x20072000, 8);		// check 0
				check_rd(n + 1, 0xAAAAAAAA, 8);
				check_rd(n + 2, 0xAAAAAAAA, 8);

			}
			if (n > 0)
			{
				n++;
				sh = sh << 1;
			}
			sh = sh << 1;
		}
		if (rn > 0)
			rn++;
	}

}
//////////////////////////////////////////////////////////////////////////////////////
//	Table Branch
//////////////////////////////////////////////////////////////////////////////////////
void run_test_TableBranch()
{
	fprintf(disasm, "-----------------------------------------\nrun_test_TableBranch:\n");
	
	int n = 0;
	int sh = 1;

	unsigned table = pc + 24;
	
	write_r(0, table);

	write_r(1, 0);

	execute_tbb_t1(0, 1);

	//execute_instruction32(0x000a0000);  // table start block1Size = 1
	execute_instruction32(0x00000000);  // table start
	execute_instruction32(0x000e0000);  // table start
	execute_instruction32(0x00000000);  // table start

	int block1Size = 1;

	for (n = 0; n < block1Size; n++)
	{
		write_failed();
	}

	int block2Size = 4;

	execute_b_t2((0x8 * block2Size) + 3);

	for (n = 0; n < block2Size; n++)
	{
		write_failed();
	}

	///////////////////////////////////////////	TBH
	
	table = pc + 20;
	
	write_r(0, table);

	write_r(1, 2);

	execute_tbh_t1(0, 1);

	//execute_instruction32(0x000a0000);  // table start block1Size = 1
	execute_instruction32(0x00000000);  // table start
	execute_instruction32(0x000e0000);  // table start
	execute_instruction32(0x00000000);  // table start

	block1Size = 1;

	for (n = 0; n < block1Size; n++)
	{
		write_failed();
	}

	block2Size = 4;

	execute_b_t2((0x8 * block2Size) + 3);

	for (n = 0; n < block2Size; n++)
	{
		write_failed();
	}

}

//////////////////////////////////////////////////////////////////////////////////////
//	LDR STR
//////////////////////////////////////////////////////////////////////////////////////
void run_test_LDRSTR()
{
//		missing:		lit
//					p = 0
//		
//		ldmdb
//		ldmea
//		ldmia			m3_fde
//		ldmfd
//		ldr 	imm		done
//		ldr	lit		m3_fde
//		ldr	reg		done (shift)
//		ldrb	imm		done
//		ldrb	lit		skip
//		ldrb	reg		done (shift)
//		ldrbt			done
//		ldrd 	imm		done (second)
//		ldrex			skip
//		ldrexb		skip
//		ldrexh		skip
//		ldrh 	imm		done
//		ldrh	lit		skip
//		ldrh	reg		done (shift)
//		ldrht 		done
//		ldrsb imm		done
//		ldrsb lit		skip
//		ldrsb reg		done (shift)
//		ldrsbt		done
//		ldrsh imm		skip
//		ldrsh lit		skip
//		ldrsh reg		done (shift)
//		ldrsht		done
//		ldrt			done
//
//		stmdb
//		stmfd
//		stmia
//		stmea
//		str	imm		done
//		str	reg		done (shift)
//		strb 	imm		done
//		strb	reg		done (shift)
//		strbt			done
//		strd 	imm		done (second)
//		strex			skip
//		strexb		skip
//		strexh		skip
//		strh	imm		done
//		strh	reg		done (shift)
//		strht			done
//		strt			done
//

	fprintf(disasm, "-----------------------------------------\nrun_test_LDRSTR:\n");

	int rn = 4;
	int rm = 1;
  	int rt1 = 0;
  	int rt2 = 2;
	int rt1_b = 7;
  	int rt2_b = 8;

	write_r(5, 0x0000ffff);
	
	unsigned base = 0x20078000;
	unsigned offset = 0x4;
	
	write_r(rn, base);
	write_r(rm, offset);

	unsigned result = 0x55555555;
	unsigned result2 = 0;
	unsigned hw_result = 0;
	unsigned byte_result = 0;

	int sh = 0;
	int shift = 0;
	int i = 0;
	int imm = 1;
	//for (sh = 0; sh < 4; sh++)
	//{
	//for (i = 0; i < sh; i++)
	//{
	//	imm = imm << 1;
	//}

	printf("sh: %x imm: %x\n", sh, imm);

	for (i = 0; i < 2; i++)
	{
		printf("--------------------------------\n");
		if (i == 1)
			result = 0xAAAAAAAA;

		fprintf(disasm, "-----------------------------------------\ntest str_reg, ldr_reg:\n");

		write_r(rt1, result);
		execute_str_reg_t1(rt1, rn, rm);
		execute_ldr_reg_t1(rt2, rn, rm);
		check_rd(rt2, result, 3);

		for (shift = 0; shift < 4; shift++)
		{
			result = ~result;
			write_r(rt1, result);
			execute_str_reg_t2(rt1, rn, rm, shift);
			printf("shift: %x result: %x\n", shift, result);
		}
		for (shift = 0; shift < 4; shift++)
		{
			result = ~result;
			execute_ldr_reg_t2(rt2, rn, rm, shift);
			check_rd(rt2, result, 3);
			//printf("shift: %x result: %x\n", shift, result);
		}

		fprintf(disasm, "-----------------------------------------\ntest str_imm, ldr_imm:\n");

		result += 1;
		for (imm = 1; imm <= 8; imm = imm << 1)
		{
			result = ~result;
			write_r(rt1, result);
			execute_str_imm_t1(rt1, rn, imm);
			printf("imm: %x result: %x\n", imm, result);
		}
		for (imm = 1; imm <= 8; imm = imm << 1)
		{
			result = ~result;
			execute_ldr_imm_t1(rt2, rn, imm);
			check_rd(rt2, result, 3);
		}

		result += 1;
		for (imm = 1; imm <= 8; imm = imm << 1)
		{
			result = ~result;
			write_r(rt1, result);
			execute_str_imm_t2(rt1, imm);
			printf("imm: %x result: %x\n", imm, result);
		}
		for (imm = 1; imm <= 8; imm = imm << 1)
		{
			result = ~result;
			execute_ldr_imm_t2(rt2, imm);
			check_rd(rt2, result, 3);
		}

		// missing: more steps, can't get it to work for all imm following
		result += 1;
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			result = ~result;
			write_r(rt1, result);
			execute_str_imm_t3(rt1, rn, imm);
			printf("imm: %x result: %x\n", imm, result);
		}
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			result = ~result;
			execute_ldr_imm_t3(rt2, rn, imm);
			check_rd(rt2, result, 3);
		}

		result += 1;
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			result = ~result;
			write_r(rt1, result);
			execute_str_imm_t4(rt1, rn, imm, 0x6);
			printf("imm: %x result: %x\n", imm, result);
		}
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			result = ~result;
			execute_ldr_imm_t4(rt2, rn, imm, 0x6);
			check_rd(rt2, result, 3);
		}

		result += 1;
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			result = ~result;
			write_r(rt1, result);
			execute_str_imm_t4(rt1, rn, imm, 0x4);
			printf("imm: %x result: %x\n", imm, result);
		}
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			result = ~result;
			execute_ldr_imm_t4(rt2, rn, imm, 0x4);
			check_rd(rt2, result, 3);
		}

		fprintf(disasm, "-----------------------------------------\ntest strb_reg, ldrb_reg:\n");

		result += 1;
	 	byte_result = result & 0xffffff00;
		write_r(rt1, result);
		execute_str_reg_t1(rt1, rn, rm);

		result = ~result;
		write_r(rt1, result);
		execute_strb_reg_t1(rt1, rn, rm);
		execute_ldr_reg_t1(rt2, rn, rm);
		byte_result += result & 0xff;
		check_rd(rt2, byte_result, 3);

		execute_ldrb_reg_t1(rt2, rn, rm);
		check_rd(rt2, result & 0xff, 3);

		//-----------------------------------------------

		result += 1;
		for (shift = 0; shift < 4; shift++)
		{
			write_r(rt1, result);
			execute_str_reg_t2(rt1, rn, rm, shift);
			result = ~result;
			write_r(rt1, result);
			execute_strb_reg_t2(rt1, rn, rm, shift);
			printf("shift: %x result: %x\n", shift, result);
		}
		for (shift = 0; shift < 4; shift++)
		{
	 		byte_result = result & 0xffffff00;
			result = ~result;
			byte_result += result & 0xff;
			execute_ldr_reg_t2(rt2, rn, rm, shift);
			check_rd(rt2, byte_result, 3);
		}
		for (shift = 0; shift < 4; shift++)
		{
			result = ~result;
			execute_ldrb_reg_t2(rt2, rn, rm, shift);
		 	execute_and_imm_t1(rt2, rt2, false, 0, 0, 0xff);
			check_rd(rt2, result & 0xff, 3);
		}

		fprintf(disasm, "-----------------------------------------\ntest strb_imm, ldrb_imm:\n");

		result += 1;
		for (imm = 0; imm <= 3; imm += 1)
		{
			write_r(rt1, result);
			execute_str_imm_t1(rt1, rn, imm);
			result = ~result;
			write_r(rt1, result);
			execute_strb_imm_t1(rt1, rn, imm * 4);
			printf("imm: %x result: %x\n", imm, result);
		}
		for (imm = 0; imm <= 3; imm += 1)
		{
	 		byte_result = result & 0xffffff00;
			result = ~result;
			byte_result += result & 0xff;
			execute_ldr_imm_t1(rt2, rn, imm);
			check_rd(rt2, byte_result, 3);
		}
		for (imm = 0; imm <= 3; imm += 1)
		{
			result = ~result;
			execute_ldrb_imm_t1(rt2, rn, imm * 4);
		 	execute_and_imm_t1(rt2, rt2, false, 0, 0, 0xff);
			check_rd(rt2, result & 0xff, 3);
		}

		//-----------------------------------------------

		result += 1;
		for (imm = 0; imm <= 3; imm += 1)
		{
			write_r(rt1, result);
			execute_str_imm_t1(rt1, rn, imm);
			result = ~result;
			write_r(rt1, result);
			execute_strb_imm_t2(rt1, rn, imm * 4);
			printf("imm: %x result: %x\n", imm, result);
		}
		for (imm = 0; imm <= 3; imm += 1)
		{
	 		byte_result = result & 0xffffff00;
			result = ~result;
			byte_result += result & 0xff;
			execute_ldr_imm_t1(rt2, rn, imm);
			check_rd(rt2, byte_result, 3);
		}
		for (imm = 0; imm <= 3; imm += 1)
		{
			result = ~result;
			execute_ldrb_imm_t2(rt2, rn, imm * 4);
		 	execute_and_imm_t1(rt2, rt2, false, 0, 0, 0xff);
			check_rd(rt2, result & 0xff, 3);
		}

		//-----------------------------------------------

		result += 1;
		for (imm = 0; imm <= 3; imm += 1)
		{
			write_r(rt1, result);
			execute_str_imm_t1(rt1, rn, imm);
			result = ~result;
			write_r(rt1, result);
			execute_strb_imm_t3(rt1, rn, imm * 4, 0x6);
			printf("imm: %x result: %x\n", imm, result);
		}
		for (imm = 0; imm <= 3; imm += 1)
		{
	 		byte_result = result & 0xffffff00;
			result = ~result;
			byte_result += result & 0xff;
			execute_ldr_imm_t1(rt2, rn, imm);
			check_rd(rt2, byte_result, 3);
		}
		for (imm = 0; imm <= 3; imm += 1)
		{
			result = ~result;
			execute_ldrb_imm_t3(rt2, rn, imm * 4, 0x6);
		 	execute_and_imm_t1(rt2, rt2, false, 0, 0, 0xff);
			check_rd(rt2, result & 0xff, 3);
		}

		// missing better test minus 
		result = ~result;
		write_r(rt1, result);
		execute_strb_imm_t3(rt1, rn, imm, 0x4);
		execute_ldrb_imm_t3(rt2, rn, imm, 0x4);
	 	execute_and_imm_t1(rt2, rt2, false, 0, 0, 0xff);
		check_rd(rt2, result & 0xff, 3);

		fprintf(disasm, "-----------------------------------------\ntest strbt, ldrbt:\n");

		result += 1;
		for (imm = 0; imm <= 3; imm += 1)
		{
			write_r(rt1, result);
			execute_str_imm_t1(rt1, rn, imm);
			result = ~result;
			write_r(rt1, result);
			execute_strbt_t1(rt1, rn, imm * 4);
			printf("imm: %x result: %x\n", imm, result);
		}
		for (imm = 0; imm <= 3; imm += 1)
		{
	 		byte_result = result & 0xffffff00;
			result = ~result;
			byte_result += result & 0xff;
			execute_ldr_imm_t1(rt2, rn, imm);
			check_rd(rt2, byte_result, 3);
		}
		for (imm = 0; imm <= 3; imm += 1)
		{
			result = ~result;
			execute_ldrbt_t1(rt2, rn, imm * 4);
		 	execute_and_imm_t1(rt2, rt2, false, 0, 0, 0xff);
			check_rd(rt2, result & 0xff, 3);
		}

		fprintf(disasm, "-----------------------------------------\ntest strh_reg, ldrh_reg:\n");

		result = ~result;
		write_r(rt1, result);
		execute_strh_reg_t1(rt1, rn, rm);
		execute_ldrh_reg_t1(rt2, rn, rm);
	 	execute_and_reg_t2(rt2, rt2, 5, false, 0, 0);
		check_rd(rt2, result & 0xffff, 3);

		//-----------------------------------------------

		result += 1;
		for (shift = 0; shift < 4; shift++)
		{
			write_r(rt1, result);
			execute_str_reg_t2(rt1, rn, rm, shift);
			result = ~result;
			write_r(rt1, result);
			execute_strh_reg_t2(rt1, rn, rm, shift);
			printf("shift: %x result: %x\n", shift, result);
		}
		for (shift = 0; shift < 4; shift++)
		{
	 		byte_result = result & 0xffff0000;
			result = ~result;
			byte_result += result & 0xffff;
			execute_ldr_reg_t2(rt2, rn, rm, shift);
			check_rd(rt2, byte_result, 3);
		}
		for (shift = 0; shift < 4; shift++)
		{
			result = ~result;
			execute_ldrh_reg_t2(rt2, rn, rm, shift);
		 	execute_and_reg_t2(rt2, rt2, 5, false, 0, 0);
			check_rd(rt2, result & 0xffff, 3);
		}

		fprintf(disasm, "-----------------------------------------\ntest strh_imm, ldrh_imm:\n");

		result += 1;
		for (imm = 0; imm <= 3; imm += 1)
		{
			write_r(rt1, result);
			execute_str_imm_t1(rt1, rn, imm);
			result = ~result;
			write_r(rt1, result);
			execute_strh_imm_t1(rt1, rn, imm * 2);
			printf("imm: %x result: %x\n", imm, result);
		}
		for (imm = 0; imm <= 3; imm += 1)
		{
	 		byte_result = result & 0xffff0000;
			result = ~result;
			byte_result += result & 0xffff;
			execute_ldr_imm_t1(rt2, rn, imm);
			check_rd(rt2, byte_result, 3);
		}
		for (imm = 0; imm <= 3; imm += 1)
		{
			result = ~result;
			execute_ldrh_imm_t1(rt2, rn, imm * 2);
		 	execute_and_reg_t2(rt2, rt2, 5, false, 0, 0);
			check_rd(rt2, result & 0xffff, 3);
		}

		//-----------------------------------------------

		result += 1;
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			write_r(rt1, result);
			execute_str_imm_t1(rt1, rn, imm);
			result = ~result;
			write_r(rt1, result);
			execute_strh_imm_t2(rt1, rn, imm * 4);
			printf("imm: %x result: %x\n", imm, result);
		}
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
	 		byte_result = result & 0xffff0000;
			result = ~result;
			byte_result += result & 0xffff;
			execute_ldr_imm_t1(rt2, rn, imm);
			check_rd(rt2, byte_result, 3);
		}
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			result = ~result;
			execute_ldrh_imm_t2(rt2, rn, imm * 4);
		 	execute_and_reg_t2(rt2, rt2, 5, false, 0, 0);
			check_rd(rt2, result & 0xffff, 3);
		}

		//-----------------------------------------------

		result += 1;
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			write_r(rt1, result);
			execute_str_imm_t1(rt1, rn, imm);
			result = ~result;
			write_r(rt1, result);
			execute_strh_imm_t3(rt1, rn, imm * 4, 0x6);
			printf("imm: %x result: %x\n", imm, result);
		}
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
	 		byte_result = result & 0xffff0000;
			result = ~result;
			byte_result += result & 0xffff;
			execute_ldr_imm_t1(rt2, rn, imm);
			check_rd(rt2, byte_result, 3);
		}
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			result = ~result;
			execute_ldrh_imm_t3(rt2, rn, imm * 4, 0x6);
		 	execute_and_reg_t2(rt2, rt2, 5, false, 0, 0);
			check_rd(rt2, result & 0xffff, 3);
		}

		//-----------------------------------------------

		result += 1;
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			write_r(rt1, result);
			execute_str_imm_t1(rt1, rn, imm);
			result = ~result;
			write_r(rt1, result);
			execute_strh_imm_t3(rt1, rn, imm, 0x4);
			printf("imm: %x result: %x\n", imm, result);
		}
		if (0)
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
	 		byte_result = result & 0xffff0000;
			result = ~result;
			byte_result += result & 0xffff;
			execute_ldr_imm_t1(rt2, rn, imm);
			check_rd(rt2, byte_result, 3);
		}
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			result = ~result;
			execute_ldrh_imm_t3(rt2, rn, imm, 0x4);
		 	execute_and_reg_t2(rt2, rt2, 5, false, 0, 0);
			check_rd(rt2, result & 0xffff, 3);
		}

		fprintf(disasm, "-----------------------------------------\ntest strht, ldrht:\n");

		result += 1;
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			write_r(rt1, result);
			execute_str_imm_t1(rt1, rn, imm);
			result = ~result;
			write_r(rt1, result);
			execute_strht_t1(rt1, rn, imm * 4);
			printf("imm: %x result: %x\n", imm, result);
		}
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
	 		byte_result = result & 0xffff0000;
			result = ~result;
			byte_result += result & 0xffff;
			execute_ldr_imm_t1(rt2, rn, imm);
			check_rd(rt2, byte_result, 3);
		}
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			result = ~result;
			execute_ldrht_t1(rt2, rn, imm * 4);
		 	execute_and_reg_t2(rt2, rt2, 5, false, 0, 0);
			check_rd(rt2, result & 0xffff, 3);
		}

		fprintf(disasm, "-----------------------------------------\ntest ldrsb_reg:\n");

		result = ~result;
		write_r(rt1, result);
		execute_strb_reg_t1(rt1, rn, rm);
		execute_ldrsb_reg_t1(rt2, rn, rm);
		byte_result = result & 0xff;
		if (((byte_result >> 7) & 0x1) == 0x1)
			byte_result += 0xffffff00;
		check_rd(rt2, byte_result, 3);

		//-----------------------------------------------

		result += 1;
		for (shift = 0; shift < 4; shift++)
		{
			write_r(rt1, result);
			execute_str_reg_t2(rt1, rn, rm, shift);
			result = ~result;
			write_r(rt1, result);
			execute_strb_reg_t2(rt1, rn, rm, shift);
			printf("shift: %x result: %x\n", shift, result);
		}
		for (shift = 0; shift < 4; shift++)
		{
			result = ~result;
			execute_ldrsb_reg_t2(rt2, rn, rm, shift);
		// 	execute_and_imm_t1(rt2, rt2, false, 0, 3, 0xff);	// workaround, remove !!!
			byte_result = result & 0xff;
			if (((byte_result >> 7) & 0x1) == 0x1)
				byte_result += 0xffffff00;
			check_rd(rt2, byte_result, 3);
		}

		fprintf(disasm, "-----------------------------------------\ntest ldrsb_imm:\n");

		result += 1;
		for (imm = 0; imm <= 3; imm += 1)
		{
			write_r(rt1, result);
			execute_str_imm_t1(rt1, rn, imm);
			result = ~result;
			write_r(rt1, result);
			execute_strb_imm_t1(rt1, rn, imm * 4);
			printf("imm: %x result: %x\n", imm, result);
		}
		for (imm = 0; imm <= 3; imm += 1)
		{
			result = ~result;
			execute_ldrsb_imm_t1(rt2, rn, imm * 4);
		 	//execute_and_imm_t1(rt2, rt2, false, 0, 0, 0xff);
			byte_result = result & 0xff;
			if (((byte_result >> 7) & 0x1) == 0x1)
				byte_result += 0xffffff00;
			check_rd(rt2, byte_result, 3);
		}

		//-----------------------------------------------

		result += 1;
		for (imm = 0; imm <= 3; imm += 1)
		{
			write_r(rt1, result);
			execute_str_imm_t1(rt1, rn, imm);
			result = ~result;
			write_r(rt1, result);
			execute_strb_imm_t2(rt1, rn, imm * 4);
			printf("imm: %x result: %x\n", imm, result);
		}
		for (imm = 0; imm <= 3; imm += 1)
		{
			result = ~result;
			execute_ldrsb_imm_t2(rt2, rn, imm * 4, 0x6);
		 	//execute_and_imm_t1(rt2, rt2, false, 0, 0, 0xff);
			byte_result = result & 0xff;
			if (((byte_result >> 7) & 0x1) == 0x1)
				byte_result += 0xffffff00;
			check_rd(rt2, byte_result, 3);
		}

		fprintf(disasm, "-----------------------------------------\ntest ldrsbt:\n");

		result += 1;
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			write_r(rt1, result);
			execute_str_imm_t1(rt1, rn, imm);
			result = ~result;
			write_r(rt1, result);
			execute_strbt_t1(rt1, rn, imm * 4);
			printf("imm: %x result: %x\n", imm, result);
		}
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			result = ~result;
			execute_ldrsbt_t1(rt2, rn, imm * 4);
		 	//execute_and_imm_t1(rt2, rt2, false, 0, 0, 0xff);
			byte_result = result & 0xff;
			if (((byte_result >> 7) & 0x1) == 0x1)
				byte_result += 0xffffff00;
			check_rd(rt2, byte_result, 3);
		}

		fprintf(disasm, "-----------------------------------------\ntest ldrsh_reg:\n");

		result = ~result;
		write_r(rt1, result);
		execute_strh_reg_t1(rt1, rn, rm);
		execute_ldrsh_reg_t1(rt2, rn, rm);
		hw_result = result & 0xffff;
		if (((hw_result >> 15) & 0x1) == 0x1)
			hw_result += 0xffff0000;
		check_rd(rt2, hw_result, 3);

		//-----------------------------------------------

		result += 1;
		for (shift = 0; shift < 4; shift++)
		{
			write_r(rt1, result);
			execute_str_reg_t2(rt1, rn, rm, shift);
			result = ~result;
			write_r(rt1, result);
			execute_strh_reg_t2(rt1, rn, rm, shift);
			printf("   shift: %x result: %x\n", shift, result);
		}
		for (shift = 0; shift < 4; shift++)
		{
			result = ~result;
			execute_ldrsh_reg_t2(rt2, rn, rm, shift);
		// 	execute_and_imm_t1(rt2, rt2, false, 0, 3, 0xff);	// workaround, remove !!!
			hw_result = result & 0xffff;
			if (((hw_result >> 15) & 0x1) == 0x1)
				hw_result += 0xffff0000;
			check_rd(rt2, hw_result, 3);
		}

		fprintf(disasm, "-----------------------------------------\ntest ldrsh_imm:\n");

		for (imm = 0; imm <= 3; imm += 1)
		{
			result = ~result;
			write_r(rt1, result);
			execute_str_imm_t1(rt1, rn, imm);
		}
		for (imm = 0; imm <= 3; imm += 1)
		{
			result = ~result;
			execute_ldrsh_imm_t1(rt2, rn, imm * 4);
			hw_result = result & 0xffff;
			if (((hw_result >> 15) & 0x1) == 0x1)
				hw_result += 0xffff0000;
			check_rd(rt2, hw_result, 3);
		}

		//-----------------------------------------------

		for (imm = 0; imm <= 3; imm += 1)
		{
			result = ~result;
			write_r(rt1, result);
			execute_str_imm_t1(rt1, rn, imm);
		}
		for (imm = 0; imm <= 3; imm += 1)
		{
			result = ~result;
			execute_ldrsh_imm_t2(rt2, rn, imm * 4, 0x6);
			hw_result = result & 0xffff;
			if (((hw_result >> 15) & 0x1) == 0x1)
				hw_result += 0xffff0000;
			check_rd(rt2, hw_result, 3);
		}

		fprintf(disasm, "-----------------------------------------\ntest ldrsht:\n");

		result += 1;
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			write_r(rt1, result);
			execute_str_imm_t1(rt1, rn, imm);
			result = ~result;
			write_r(rt1, result);
			execute_strht_t1(rt1, rn, imm * 4);
			printf("imm: %x result: %x\n", imm, result);
		}
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			result = ~result;
			execute_ldrsht_t1(rt2, rn, imm * 4);
			hw_result = result & 0xffff;
			if (((hw_result >> 15) & 0x1) == 0x1)
				hw_result += 0xffff0000;
			check_rd(rt2, hw_result, 3);
		}

		fprintf(disasm, "-----------------------------------------\ntest strt, ldrt:\n");

		result += 1;
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			result = ~result;
			write_r(rt1, result);
			execute_strt_t1(rt1, rn, imm);
		}
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			result = ~result;
			execute_ldrt_t1(rt2, rn, imm);
			check_rd(rt2, result, 3);
		}

		fprintf(disasm, "-----------------------------------------\ntest strex, ldrex:\n");

		// can't get it to work
		//result = ~result;
		//write_r(rt1, result);
		//execute_strex_t1(rt1, rn, 6, 0x8);
		//execute_ldrex_t1(rt2, rn, 0x8);
		//check_rd(rt2, result, 3);

		fprintf(disasm, "-----------------------------------------\ntest strd_imm, ldrd_imm:\n");

		result += 1;
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			result = ~result;
			write_r(rt1, result);
			result2 = result << 1;
			write_r(rt1_b, result2);
			execute_strd_imm_t1(rt1, rt1_b, rn, imm, 0x6);
		}
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			result = ~result;
			execute_ldrd_imm_t1(rt2, rt2_b, rn, imm, 0x6);
			check_rd(rt2, result, 3);
		// bug
		//	result2 = result << 1;
		//	check_rd(rt2_b, result2, 3);
		}

		//-----------------------------------------------

		result += 1;
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			result = ~result;
			write_r(rt1, result);
			result2 = result << 1;
			write_r(rt1_b, result2);
			execute_strd_imm_t1(rt1, rt1_b, rn, imm, 0x4);
		}
		for (imm = 4; imm <= 16; imm = imm << 2)
		{
			result = ~result;
			execute_ldrd_imm_t1(rt2, rt2_b, rn, imm, 0x4);
			check_rd(rt2, result, 3);
		// bug
		//	result2 = result << 1;
		//	check_rd(rt2_b, result2, 3);
		}
	}


	if (0)
	{
	//}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//	reset_problem
//////////////////////////////////////////////////////////////////////////////////////
void run_reset_problem()
{
	write_r(5, 0x2007042c);
	write_r(1, 0x200704cf);

	execute_sub_reg_t1(0, 1, 5);
	//write_r(0, 0x000000a3);
	check_rd(0, 0x000000a3, 3);

	execute_bic_imm_t1(4, 0, false, 0, 0, 3);
	
	write_r(3, 0x20070430);

	execute_add_reg_t1(2, 4, 3);

	execute_add_imm_t3(5, 5, false, 0, 0, 8);
  	// 803ea:	4295      	cmp	r5, r2
   	// 803ec:	d1f7      	bne.n	803de <Reset_Handler+0x9e>
	execute_instruction16(0x4295);
	execute_instruction16(0xd1fb);
}

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



	//run_test_slt();
	//run_test_dp_rand();
	//run_test_dp_comb_reg();
	//run_test_dp_comb_imm();
	//run_test_dp_math1_reg();
	//run_test_dp_math1_imm();
	//run_test_dp_math2_reg();
	//run_test_dp_math2_imm();
	//run_test_dp_math3_reg();
	//run_test_dp_math3_imm();
	//run_test_dp_math4_reg();
	//run_test_dp_math4_imm();
	//run_test_dp_math5_reg();
	//run_test_dp_math6_reg();
	//run_test_dp_math7_reg();
	//run_test_dp_math8_reg();
	//run_test_dp_math9_reg();                  // missing: I) a(31) = 1 II) b > a
	//run_test_DecodeImmShift1_reg();
	//run_test_DecodeImmShift2_reg();
	//run_test_ITBlock();
	//run_test_ThumbExpandImm1();
	//run_test_PushPop();
	//run_test_LDRSTR();				// missing double 
	//run_test_LDMSTM_A_0();			// missing wrback bug
	//run_test_LDMSTM_A_1();			// missing wrback bug
	//run_test_LDMSTM_A_2();			// missing wrback bug
	//run_test_LDMSTM_A_3();			// missing wrback bug
	//run_test_LDMSTM_B_0();			// missing wrback bug
	//run_test_LDMSTM_B_1();			// missing wrback bug
	//run_test_LDMSTM_B_2();			// missing wrback bug
	//run_test_LDMSTM_B_3();			// missing wrback bug
	//run_test_TableBranch();			// skip

	//run_reset_problem();
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