/*
        Name 1: Raiyan Chowdhury
        Name 2: Timberlon Gray
        UTEID 1: rac4444
        UTEID 2: tg22698 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 255
#define MAX_NUM_OF_LABELS 255
#define MAX_LABEL_LENGTH 20

enum {
	DONE, OK, EMPTY_LINE
};

int toNum( char * pStr )
{
	char * t_ptr;
	char * orig_pStr;
	int t_length,k;
	int lNum, lNeg = 0;
	long int lNumLong;

	orig_pStr = pStr;
	if( *pStr == '#' )                                /* decimal */
	{ 
		pStr++;
		if( *pStr == '-' )                                /* dec is negative */
		{
			lNeg = 1;
			pStr++;
		}
		t_ptr = pStr;
		t_length = strlen(t_ptr);
		for(k=0;k < t_length;k++)
		{
			if (!isdigit(*t_ptr))
			{
				printf("Error: invalid decimal operand, %s\n",orig_pStr);
				exit(4);
			}
			t_ptr++;
		}
		lNum = atoi(pStr);
		if (lNeg)
		lNum = -lNum;

		return lNum;
	}
	else if( *pStr == 'x' )        /* hex     */
	{
		pStr++;
		if( *pStr == '-' )                                /* hex is negative */
		{
		lNeg = 1;
		pStr++;
		}
		t_ptr = pStr;
		t_length = strlen(t_ptr);
		for(k=0;k < t_length;k++)
		{
			if (!isxdigit(*t_ptr))
			{
				printf("Error: invalid hex operand, %s\n",orig_pStr);
				exit(4);
			}
			t_ptr++;
		}
		lNumLong = strtol(pStr, NULL, 16);    /* convert hex string into integer */
		lNum = (lNumLong > INT_MAX)? INT_MAX : lNumLong;
		if( lNeg )
			lNum = -lNum;
		return lNum;
	}
	else
	{
		printf( "Error: invalid operand, %s\n", orig_pStr);
		exit(4);  /* This has been changed from error code 3 to error code 4, see clarification 12 */
	}
}

/*strcmp returns 0 if they are equal*/
int isOpcode(char* ptr) {
	if(!strcmp(ptr, "add")
	|| !strcmp(ptr, "and")
	|| !strcmp(ptr, "br")
	|| !strcmp(ptr, "brn")
	|| !strcmp(ptr, "brz")
	|| !strcmp(ptr, "brp")
	|| !strcmp(ptr, "brnz")
	|| !strcmp(ptr, "brnp")
	|| !strcmp(ptr, "brzp")
	|| !strcmp(ptr, "brnzp")
	|| !strcmp(ptr, "jmp")
	|| !strcmp(ptr, "jsr")
	|| !strcmp(ptr, "jsrr")
	|| !strcmp(ptr, "ldb")
	|| !strcmp(ptr, "ldw")
	|| !strcmp(ptr, "lea")
	|| !strcmp(ptr, "not")
	|| !strcmp(ptr, "ret")
	|| !strcmp(ptr, "rti")
	|| !strcmp(ptr, "lshf")
	|| !strcmp(ptr, "rshfl")
	|| !strcmp(ptr, "rshfa")
	|| !strcmp(ptr, "stb")
	|| !strcmp(ptr, "stw")
	|| !strcmp(ptr, "trap")
	|| !strcmp(ptr, "xor")
	|| !strcmp(ptr, "halt")
	|| !strcmp(ptr, "nop")) return 0;
	return -1;
}

int isPseudoOp(char* ptr) {
	if(!strcmp(ptr, ".orig")
	|| !strcmp(ptr, ".end")
	|| !strcmp(ptr, ".fill")) return 0;
	return -1;
}

int isValidLabel(char** pLabel) {
	/*returns 0 if invalid label, else returns 1*/
	char* ptr = *pLabel;
	if (!strcmp(ptr, "in") || !strcmp(ptr, "out") || !strcmp(ptr, "puts") || !strcmp(ptr, "getc")
		|| !strcmp(ptr, "r0") || !strcmp(ptr, "r1") || !strcmp(ptr, "r2") || !strcmp(ptr, "r3")
		|| !strcmp(ptr, "r4") || !strcmp(ptr, "r5") || !strcmp(ptr, "r6") || !strcmp(ptr, "r7")) return 0;;
	if (*ptr == 'x' || !isalpha(*ptr)) return 0;
	if (isOpcode(ptr) != -1) return 0;
	int ptr_length = strlen(ptr);
	if (ptr_length > 20) return 0;
	int i;
	for (i = 1; i < ptr_length; i++) {
		if (!isalnum(ptr[i])) return 0;
	}
	return 1;
}

int readAndParse(FILE * pInfile, char * pLine, char ** pLabel, char ** pOpcode, char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4) {
	char * lRet, * lPtr;
	int i;
	if( !fgets( pLine, MAX_LINE_LENGTH, pInfile ) )
		return( DONE );
		   
	for( i = 0; i < strlen( pLine ); i++ )
		pLine[i] = tolower( pLine[i] );
		   
	/* convert entire line to lowercase */
	*pLabel = *pOpcode = *pArg1 = *pArg2 = *pArg3 = *pArg4 = pLine + strlen(pLine);

	/* ignore the comments */
	lPtr = pLine;

	while( *lPtr != ';' && *lPtr != '\0' && *lPtr != '\n' ) 
		lPtr++;

	*lPtr = '\0';
	if( !(lPtr = strtok( pLine, "\r\t\n ," ) ) ) 
		return( EMPTY_LINE );

	if( isOpcode( lPtr ) == -1 && lPtr[0] != '.' ) { /* found a label */ 
		*pLabel = lPtr;
		if( !( lPtr = strtok( NULL, "\r\t\n ," ) ) ) return( OK );
	}
		   
	*pOpcode = lPtr;
	if (!strcmp(*pOpcode, ".end")) return ( DONE );

	if( !( lPtr = strtok( NULL, "\r\t\n ," ) ) ) return( OK );
		   
	*pArg1 = lPtr;
		   
	if( !( lPtr = strtok( NULL, "\r\t\n ," ) ) ) return( OK );

	*pArg2 = lPtr;
	
	if( !( lPtr = strtok( NULL, "\r\t\n ," ) ) ) return( OK );

	*pArg3 = lPtr;

	if( !( lPtr = strtok( NULL, "\r\t\n ," ) ) ) return( OK );

	*pArg4 = lPtr;

	return( OK );
}

int regToInt(char** arg) {
	/*if (arg == NULL) exit(4);*/
	char* ptr = *arg;
	if (!strcmp(ptr, "")) exit(4);
	if (!strcmp(ptr, "r0")) return 0;
	if (!strcmp(ptr, "r1")) return 1;
	if (!strcmp(ptr, "r2")) return 2;
	if (!strcmp(ptr, "r3")) return 3;
	if (!strcmp(ptr, "r4")) return 4;
	if (!strcmp(ptr, "r5")) return 5;
	if (!strcmp(ptr, "r6")) return 6;
	if (!strcmp(ptr, "r7")) return 7;
	exit(4);
}

int getLabelAddress(char** label, char label_table[MAX_NUM_OF_LABELS][MAX_LABEL_LENGTH+1], int address_table[MAX_NUM_OF_LABELS]) {
	/*if (*label == NULL) exit(4);*/
	if (!strcmp(*label, "") || !isValidLabel(label)) exit(4);
	char* ptr = *label;
	int i;
	for (i = 0; i < MAX_NUM_OF_LABELS; i++) {
		if (!strcmp(ptr, label_table[i])) {/*If label found*/
			return address_table[i]; /*return corresponding address*/
		}
	}
	exit(1); /*If label is not found, return error code 1*/
}

int parse_add_and_xor (char** pOpcode, char** arg1, char** arg2, char** arg3, char** arg4) {
	int arg1int = regToInt(arg1);
	int arg2int = regToInt(arg2);
	int arg3int;
	/*if (*arg4 != NULL) exit(4);*/
	if (strcmp(*arg4, "")) exit(4);
	/* process offset or register and calculate asm output */
	if (!strcmp(*arg3, "")) exit(4);
	if ((strncmp(*arg3, "#", 1) == 0)
	 || (strncmp(*arg3, "x", 1) == 0)) {
		arg3int = toNum(*arg3);
		if ((arg3int < -16) || (arg3int > 15)) exit(3);
		arg3int &= 0x0000001f; /* mask to 5 bits */
		arg3int |= 32;  /* offset marker in bit 5*/
	} else {
		arg3int = regToInt(arg3);
	}
	/* creates asm instruction */
	int inst;
	if (!strcmp(*pOpcode, "add")) inst = 1 << 12;
	if (!strcmp(*pOpcode, "and")) inst = 5 << 12;
	if (!strcmp(*pOpcode, "xor")) inst = 9 << 12;
	inst &= 0xF000;
	inst |= (arg1int << 9); 
	inst |= (arg2int << 6);
	inst |= arg3int;
	return inst;
}

int parse_ldb_ldw_stb_stw (char** pOpcode, char** arg1, char** arg2, char** arg3, char** arg4) {
	int arg1int = regToInt(arg1);
	int arg2int = regToInt(arg2);
	int arg3int;
	/*if (*arg4 != NULL) exit(4);*/
	if (strcmp(*arg4, "")) exit(4);
	/* process offset */
	if (!strcmp(*arg3, "") 
	 || (strncmp(*arg3, "#", 1) && strncmp(*arg3, "x", 1))) { 
		exit(4);
	}
	arg3int = toNum(*arg3);
	if ((arg3int < -32) || (arg3int > 31)) exit(3);
	arg3int &= 0x0000003f; /* mask to 6 bits */

	/* creates asm instruction */
	int inst;
	if (!strcmp(*pOpcode, "ldb")) inst = 2 << 12;
	if (!strcmp(*pOpcode, "ldw")) inst = 6 << 12;
	if (!strcmp(*pOpcode, "stb")) inst = 3 << 12;
	if (!strcmp(*pOpcode, "stw")) inst = 7 << 12;
	inst += (arg1int << 9) + (arg2int << 6) + arg3int;
	return inst;
}

int parse_lshf_rshfl_rshfa (char** pOpcode, char** arg1, char** arg2, char** arg3, char** arg4) {
	int arg1int = regToInt(arg1);
	int arg2int = regToInt(arg2);
	int arg3int;
	/*if (*arg4 != NULL) exit(4);*/
	if (strcmp(*arg4, "")) exit(4);
	/* process offset */
	if ((!strcmp(*arg3, ""))
	 || (strncmp(*arg3, "#", 1) && strncmp(*arg3, "x", 1))) { 
		exit(4);
	}
	arg3int = toNum(*arg3);
	if ((arg3int < 0) || (arg3int > 15)) exit(3);
	arg3int &= 0x0000000f; /* mask to 4 bits */

	/* creates asm instruction */
	int inst = 0;
	/* Modify bits 5-4 based on instruction. LSHF has no change (00). */
	if (!strcmp(*pOpcode, "rshfl")) inst = 1 << 4;
	if (!strcmp(*pOpcode, "rshfa")) inst = 3 << 4;
	inst += (13 << 12) + (arg1int << 9) + (arg2int << 6) + arg3int;
	return inst;
}

int parse_branches (char** pOpcode, char** arg1, char** arg2, char** arg3, char** arg4, int c_adr, 
	char label_table[MAX_NUM_OF_LABELS][MAX_LABEL_LENGTH+1], int address_table[MAX_NUM_OF_LABELS]) {

	/*if (*arg2 != NULL) exit(4);*/
	if (strcmp(*arg2, "")) exit(4);
	/* process offset */
	int offset = getLabelAddress(arg1, label_table, address_table) - c_adr;
	offset >>= 1;
	if ((offset < -256) || (offset > 255)) exit(4);
	offset &= 0x000001ff; /* mask to 9 bits */

	/* creates asm instruction */
	int inst;
	if (!strcmp(*pOpcode, "brp")) inst = 1 << 9;
	if (!strcmp(*pOpcode, "brz")) inst = 2 << 9;
	if (!strcmp(*pOpcode, "brzp")) inst = 3 << 9;
	if (!strcmp(*pOpcode, "brn")) inst = 4 << 9;
	if (!strcmp(*pOpcode, "brnp")) inst = 5 << 9;
	if (!strcmp(*pOpcode, "brnz")) inst = 6 << 9;
	if (!strcmp(*pOpcode, "brnzp")
	 || !strcmp(*pOpcode, "br")) inst = 7 << 9;
	inst += offset;
	return inst;
}

int symbolExists(char* lLabel, char label_table[MAX_NUM_OF_LABELS][MAX_LABEL_LENGTH+1], int num) {
	int i;
	for (i = 0; i < num; i++) {
		if (!strcmp(label_table[i], lLabel)) return 1;
	}
	return 0;
}

int main (int argc, char* argv[]) {
	char *prgName = NULL;
	FILE* infile = NULL;
	FILE* infile2 = NULL;
	FILE* outfile = NULL;

	char label_table[MAX_NUM_OF_LABELS][MAX_LABEL_LENGTH+1];
	int address_table[MAX_NUM_OF_LABELS];
	int num_of_labels = 0;
	int start_address;
	int current_address;
	int i = 0;

	prgName = argv[0];

	infile = fopen(argv[1], "r");
	infile2 = fopen(argv[1], "r");
	outfile = fopen(argv[2], "w");

	if (!infile) {
		printf("Error: Cannot open file %s\n", argv[1]);
		exit(4);
	}
	if (!outfile) {
		printf("Error: Cannot open file %s\n", argv[2]);
		exit(4);
	}

	/* code */

	char lLine[MAX_LINE_LENGTH+1], *lLabel, *lOpcode, *lArg1, *lArg2, *lArg3, *lArg4;
	int lRet;

	/*figure out where .ORIG designates the start of the program*/
	do {
		lRet = readAndParse(infile, lLine, &lLabel, &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4);
		if (lRet != DONE && lRet != EMPTY_LINE) {
			if (!strcmp(lOpcode, ".orig")) {
				if (!strcmp(lArg1, "")) exit(4);
				current_address = toNum(lArg1);
				start_address = toNum(lArg1);
				if ( (start_address % 2 != 0) || start_address < 0 || start_address > 0xFFFF) exit(3); 
			} else {
				exit(4); /*if first instruction line is not .ORIG, error code 4*/
			}
		}
	} while (strcmp(lOpcode, ".orig"));

	/*create symbol table*/
	do {
		lRet = readAndParse(infile, lLine, &lLabel, &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4);
		if (lRet != DONE && lRet != EMPTY_LINE) {
			/*stuff*/
			if (current_address > 0xFFFF) exit(4); /*out of bounds instruction*/
			/*if (*lLabel != 0)*/
			if (strcmp(lLabel, "")) {	
				if (!isValidLabel(&lLabel)) exit(4);
				if (symbolExists(lLabel, label_table, num_of_labels)) exit(4);
				strcpy(label_table[i], lLabel);
				num_of_labels++;
				address_table[i] = current_address;/*address of current label*/
				i++;
			}
			current_address += 2;
		}
	} while (lRet != DONE);

	if (strcmp(lOpcode, ".end")) exit(4); /*no .END at end of assembly program*/

	/*symbol table test*/
	/*int k = 0;
	for (k = 0; k < i; k++){
		printf("%s %x\n", label_table[k], address_table[k]);
	}*/

	/*assemble and generate output file*/
	int instr;
	current_address = start_address;
	do {
		lRet = readAndParse(infile2, lLine, &lLabel, &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4);
		if (lRet != DONE && lRet != EMPTY_LINE) {
			if (!strcmp(lOpcode, "")) exit(2); /*missing opcode*/
			if (lOpcode[0] != '.' && isOpcode(lOpcode) == -1) exit(2); /*invalid opcode*/
			if (lOpcode[0] == '.' && isPseudoOp(lOpcode) == -1) exit(4);

			if (!strcmp(lOpcode, ".orig")) {
				if (!strcmp(lArg1, "")) exit(4);
				instr = toNum(lArg1);
				if ( (instr % 2 != 0) || instr < 0 || instr > 0xFFFF) exit(3);
				fprintf(outfile, "0x%.4X\n", instr);
			}
			else if (!strcmp(lOpcode, ".fill")) {
				if (!strcmp(lArg1, "")) exit(4);
				instr = toNum(lArg1);
				if (instr < -32768 || instr > 0xFFFF) exit(3);
				fprintf(outfile, "0x%.4X\n", instr & 0x0000FFFF);
			}
			else if (!strcmp(lOpcode, "add")
				  || !strcmp(lOpcode, "and")
				  || !strcmp(lOpcode, "xor")) {
				instr = parse_add_and_xor(&lOpcode, &lArg1, &lArg2, &lArg3, &lArg4);
				fprintf(outfile, "0x%.4X\n", instr);
			}
			else if (!strcmp(lOpcode, "ldb")
				  || !strcmp(lOpcode, "ldw")
				  || !strcmp(lOpcode, "stb")
				  || !strcmp(lOpcode, "stw")) {
				instr = parse_ldb_ldw_stb_stw(&lOpcode, &lArg1, &lArg2, &lArg3, &lArg4);
				fprintf(outfile, "0x%.4X\n", instr);
			}
			else if (!strcmp(lOpcode, "lshf")
				  || !strcmp(lOpcode, "rshfl")
				  || !strcmp(lOpcode, "rshfa")) {
			 	instr = parse_lshf_rshfl_rshfa(&lOpcode, &lArg1, &lArg2, &lArg3, &lArg4);
				fprintf(outfile, "0x%.4X\n", instr);
			}
			else if (!strcmp(lOpcode, "br")
				  || !strcmp(lOpcode, "brn")
				  || !strcmp(lOpcode, "brz")
				  || !strcmp(lOpcode, "brp")
				  || !strcmp(lOpcode, "brnz")
				  || !strcmp(lOpcode, "brnp")
				  || !strcmp(lOpcode, "brzp")
				  || !strcmp(lOpcode, "brnzp")) {
			 	instr = parse_branches(&lOpcode, &lArg1, &lArg2, &lArg3, &lArg4, current_address, label_table, address_table);
				fprintf(outfile, "0x%.4X\n", instr);
			}
			else if (!strcmp(lOpcode, "ret")) {
				if (strcmp(lArg1, "")) exit(4);
				instr = 0x0000C1C0;
				fprintf(outfile, "0x%.4X\n", instr);
			}
			else if (!strcmp(lOpcode, "rti")) {
				if (strcmp(lArg1, "")) exit(4);
				instr = 0x00008000;
				fprintf(outfile, "0x%.4X\n", instr);
			}
			else if (!strcmp(lOpcode, "jmp")) {
				int arg1int = regToInt(&lArg1);
				if (strcmp(lArg2, "")) exit(4);
				instr = (12 << 12) + (arg1int << 6);
				fprintf(outfile, "0x%.4X\n", instr);
			}
			else if (!strcmp(lOpcode, "jsrr")) {
				int arg1int = regToInt(&lArg1);
				if (strcmp(lArg2, "")) exit(4);
				instr = (4 << 12) + (arg1int << 6);
				fprintf(outfile, "0x%.4X\n", instr);
			}
			else if (!strcmp(lOpcode, "not")) {
				int arg1int = regToInt(&lArg1);
				int arg2int = regToInt(&lArg2);
				if (strcmp(lArg3, "")) exit(4);
				instr = (9 << 12) + (arg1int << 9) + (arg2int << 6) + 0x0000003F;
				fprintf(outfile, "0x%.4X\n", instr);
			}
			else if (!strcmp(lOpcode, "lea")) {
				int arg1int = regToInt(&lArg1);
				if (strcmp(lArg3, "")) exit(4);
				/* process offset */
				int offset = getLabelAddress(&lArg2, label_table, address_table) - current_address;
				offset >>= 1;
				if ((offset < -256) || (offset > 255)) exit(4);
				offset &= 0x000001ff; /* mask to 9 bits */
				instr = 0x0000E000 + (arg1int << 9) + offset;
				fprintf(outfile, "0x%.4X\n", instr);
			}
			else if (!strcmp(lOpcode, "jsr")) {
				if (strcmp(lArg2, "")) exit(4);
				/* process offset */
				int offset = getLabelAddress(&lArg1, label_table, address_table) - current_address;
				offset >>= 1;
				if ((offset < -1024) || (offset > 1023)) exit(4);
				offset &= 0x000007ff; /* mask to 11 bits */
				instr = 0x00004800 + offset;
				fprintf(outfile, "0x%.4X\n", instr);
			}
			else if (!strcmp(lOpcode, "trap")) {
				if (strcmp(lArg2, "")) exit(4);
				if (strncmp(lArg1, "x", 1)) exit(4);
				int arg1int = toNum(lArg1);
				if ((arg1int < 0) || (arg1int > 255)) exit(3);
				instr = 0x0000F000 + arg1int;
				fprintf(outfile, "0x%.4X\n", instr);
			}
			else if (!strcmp(lOpcode, "halt")) {
				if (strcmp(lArg1, "")) exit(4);
				instr = 0x0000F025; /* TRAP x25 */
				fprintf(outfile, "0x%.4X\n", instr);
			}
			else if (!strcmp(lOpcode, "nop")) {
				if (strcmp(lArg1, "")) exit(4);
				instr = 0x00000000;
				fprintf(outfile, "0x%.4X\n", instr);
			}
			else exit(2);

			current_address += 2;
		} 

		
	} while (lRet != DONE);

	/* end code */

	fclose(infile);
	fclose(infile2);
	fclose(outfile);

	exit(0);

}