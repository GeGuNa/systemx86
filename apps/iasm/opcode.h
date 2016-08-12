#ifndef	OPCODE_H_
#define	OPCODE_H_

#define	OPCODE_AAA		0x37

#define	OPCODE_AAD		0xD50A
#define	OPCODE_AAD_IMM8	0xD5

#define	OPCODE_AAM		0xD40A
#define	OPCODE_AAM_IMM8	0xD4

#define	OPCODE_AAS		0x3F

#define	OPCODE_ADC_REG8_REG8	0x1200
#define	OPCODE_ADC_REG16_REG16	0x1300
#define	OPCODE_ADC_REG32_REG32	0x1300
#define	OPCODE_ADC_MEM8_REG8	0x1000
#define	OPCODE_ADC_MEM16_REG16	0x1100
#define	OPCODE_ADC_MEM32_REG32	0x1100
#define	OPCODE_ADC_REG8_MEM8	0x1200
#define	OPCODE_ADC_REG16_MEM16	0x1300
#define	OPCODE_ADC_REG32_MEM32	0x1300
#define	OPCODE_ADC_ACC8_IMM8	0x14
#define	OPCODE_ADC_ACC16_IMM16	0x15
#define	OPCODE_ADC_ACC32_IMM32	0x15
#define	OPCODE_ADC_REG_IMM8		0x8210
#define	OPCODE_ADC_MEM_IMM8		0x8210
#define	OPCODE_ADC_REG8_IMM8	0x8010
#define	OPCODE_ADC_REG16_IMM16	0x8110
#define	OPCODE_ADC_REG32_IMM32	0x8110
#define	OPCODE_ADC_MEM8_IMM8	0x8010
#define	OPCODE_ADC_MEM16_IMM16	0x8110
#define	OPCODE_ADC_MEM32_IMM32	0x8110

#define	OPCODE_ADD_REG8_REG8	0x0200
#define	OPCODE_ADD_REG16_REG16	0x0300
#define	OPCODE_ADD_REG32_REG32	0x0300
#define	OPCODE_ADD_MEM8_REG8	0x0000
#define	OPCODE_ADD_MEM16_REG16	0x0100
#define	OPCODE_ADD_MEM32_REG32	0x0100
#define	OPCODE_ADD_REG8_MEM8	0x0200
#define	OPCODE_ADD_REG16_MEM16	0x0300
#define	OPCODE_ADD_REG32_MEM32	0x0300
#define	OPCODE_ADD_ACC8_IMM8	0x04
#define	OPCODE_ADD_ACC16_IMM16	0x05
#define	OPCODE_ADD_ACC32_IMM32	0x05
#define	OPCODE_ADD_REG_IMM8		0x8200
#define	OPCODE_ADD_MEM_IMM8		0x8200
#define	OPCODE_ADD_REG8_IMM8	0x8000
#define	OPCODE_ADD_REG16_IMM16	0x8100
#define	OPCODE_ADD_REG32_IMM32	0x8100
#define	OPCODE_ADD_MEM8_IMM8	0x8000
#define	OPCODE_ADD_MEM16_IMM16	0x8100
#define	OPCODE_ADD_MEM32_IMM32	0x8100

#define	OPCODE_AND_REG8_REG8	0x2200
#define	OPCODE_AND_REG16_REG16	0x2300
#define	OPCODE_AND_REG32_REG32	0x2300
#define	OPCODE_AND_MEM8_REG8	0x2000
#define	OPCODE_AND_MEM16_REG16	0x2100
#define	OPCODE_AND_MEM32_REG32	0x2100
#define	OPCODE_AND_REG8_MEM8	0x2200
#define	OPCODE_AND_REG16_MEM16	0x2300
#define	OPCODE_AND_REG32_MEM32	0x2300
#define	OPCODE_AND_ACC8_IMM8	0x24
#define	OPCODE_AND_ACC16_IMM16	0x25
#define	OPCODE_AND_ACC32_IMM32	0x25
#define	OPCODE_AND_REG_IMM8		0x8220
#define	OPCODE_AND_MEM_IMM8		0x8220
#define	OPCODE_AND_REG8_IMM8	0x8020
#define	OPCODE_AND_REG16_IMM16	0x8120
#define	OPCODE_AND_REG32_IMM32	0x8120
#define	OPCODE_AND_MEM8_IMM8	0x8020
#define	OPCODE_AND_MEM16_IMM16	0x8120
#define	OPCODE_AND_MEM32_IMM32	0x8120

#define	OPCODE_ARPL_REG16_REG16	0x6300
#define	OPCODE_ARPL_MEM16_REG16	0x6300

#define	OPCODE_BOUND_REG16_MEM1616	0x6200
#define	OPCODE_BOUND_REG32_MEM3232	0x6200

#define	OPCODE_BSF_REG16_REG16	0x0fbc00
#define	OPCODE_BSF_REG16_MEM16	0x0fbc00
#define	OPCODE_BSF_REG32_REG32	0x0fbc00
#define	OPCODE_BSF_REG32_MEM32	0x0fbc00

#define	OPCODE_BSR_REG16_REG16	0x0fbd00
#define	OPCODE_BSR_REG16_MEM16	0x0fbd00
#define	OPCODE_BSR_REG32_REG32	0x0fbd00
#define	OPCODE_BSR_REG32_MEM32	0x0fbd00





#define	OPCODE_CBW	0x98

#define	OPCODE_CLC	0xF8

#define	OPCODE_CLD	0xFC

#define	OPCODE_CLI	0xFA

#define	OPCODE_CMC	0xF5

#define	OPCODE_CMP_REG8_REG8	0x3A00
#define	OPCODE_CMP_REG16_REG16	0x3B00
#define	OPCODE_CMP_REG32_REG32	0x3B00
#define	OPCODE_CMP_MEM8_REG8	0x3800
#define	OPCODE_CMP_MEM16_REG16	0x3900
#define	OPCODE_CMP_MEM32_REG32	0x3900
#define	OPCODE_CMP_REG8_MEM8	0x3A00
#define	OPCODE_CMP_REG16_MEM16	0x3B00
#define	OPCODE_CMP_REG32_MEM32	0x3B00
#define	OPCODE_CMP_ACC8_IMM8	0x3C
#define	OPCODE_CMP_ACC16_IMM16	0x3D
#define	OPCODE_CMP_ACC32_IMM32	0x3D
#define	OPCODE_CMP_REG_IMM8		0x8238
#define	OPCODE_CMP_MEM_IMM8		0x8238
#define	OPCODE_CMP_REG8_IMM8	0x8038
#define	OPCODE_CMP_REG16_IMM16	0x8138
#define	OPCODE_CMP_REG32_IMM32	0x8138
#define	OPCODE_CMP_MEM8_IMM8	0x8038
#define	OPCODE_CMP_MEM16_IMM16	0x8138
#define	OPCODE_CMP_MEM32_IMM32	0x8138

#define	OPCODE_CMPSB	0xA6

#define	OPCODE_CMPSW	0xA7

#define	OPCODE_CWD	0x99

#define	OPCODE_DAA	0x27

#define	OPCODE_DAS	0x2F

#define	OPCODE_DEC_REG8		0xFE08
#define	OPCODE_DEC_REG16	0xFF08
#define	OPCODE_DEC_REG32	0xFF08
#define	OPCODE_DEC_MEM8		0xFE08
#define	OPCODE_DEC_MEM16	0xFF08
#define	OPCODE_DEC_MEM32	0xFF08

#define	OPCODE_DIV_REG8		0xF630
#define	OPCODE_DIV_REG16	0xF730
#define	OPCODE_DIV_REG32	0xF730
#define	OPCODE_DIV_MEM8		0xF630
#define	OPCODE_DIV_MEM16	0xF730
#define	OPCODE_DIV_MEM32	0xF730

#define	OPCODE_HLT	0xF4

#define	OPCODE_IDIV_REG8	0xF638
#define	OPCODE_IDIV_REG16	0xF738
#define	OPCODE_IDIV_REG32	0xF738
#define	OPCODE_IDIV_MEM8	0xF638
#define	OPCODE_IDIV_MEM16	0xF738
#define	OPCODE_IDIV_MEM32	0xF738

#define	OPCODE_IMUL_REG8	0xF628
#define	OPCODE_IMUL_REG16	0xF728
#define	OPCODE_IMUL_REG32	0xF728
#define	OPCODE_IMUL_MEM8	0xF628
#define	OPCODE_IMUL_MEM16	0xF728
#define	OPCODE_IMUL_MEM32	0xF728

#define	OPCODE_INB_Imm8	0xE4
#define	OPCODE_INW_Imm8	0xE5
#define	OPCODE_IND_Imm8	0xE5
#define	OPCODE_INB_DX	0xEC
#define	OPCODE_INW_DX	0xED
#define	OPCODE_IND_DX	0xED

#define	OPCODE_INSB		0x6C
#define	OPCODE_INSW		0x6D
#define	OPCODE_INSD		0x6D

#define	OPCODE_INT_3		0xCC
#define	OPCODE_INT_IMM8		0xCD
#define	OPCODE_INTO			0xCE

#define	OPCODE_INC_REG8		0xFE00
#define	OPCODE_INC_REG16	0xFF00
#define	OPCODE_INC_REG32	0xFF00
#define	OPCODE_INC_MEM8		0xFE00
#define	OPCODE_INC_MEM16	0xFF00
#define	OPCODE_INC_MEM32	0xFF00

#define	OPCODE_IRET	0xCF

#define	OPCODE_LAHF	0x9F

#define	OPCODE_LDS_REG16_MEM32	0xC500

#define	OPCODE_LES_REG16_MEM32	0xC400

#define	OPCODE_LEA_REG16_MEM	0x8D00

#define	OPCODE_LGDT_MEM1632		0x0F0110

#define	OPCODE_LIDT_MEM1632		0x0F0118

#define	OPCODE_LODSB	0xAC

#define	OPCODE_LODSW	0xAD

#define	OPCODE_MOV_MEMOFF_ACC8	0xA2
#define	OPCODE_MOV_MEMOFF_ACC16	0xA3
#define	OPCODE_MOV_MEMOFF_ACC32	0xA3
#define	OPCODE_MOV_ACC8_MEMOFF	0xA0
#define	OPCODE_MOV_ACC16_MEMOFF	0xA1
#define	OPCODE_MOV_ACC32_MEMOFF	0xA1
#define	OPCODE_MOV_REG8_IMM8	0xB0
#define	OPCODE_MOV_REG16_IMM16	0xB8
#define	OPCODE_MOV_REG32_IMM32	0xB8
#define	OPCODE_MOV_MEM8_IMM8	0xC600
#define	OPCODE_MOV_MEM16_IMM16	0xC700
#define	OPCODE_MOV_MEM32_IMM32	0xC700
#define	OPCODE_MOV_REG8_REG8	0x8800
#define	OPCODE_MOV_REG16_REG16	0x8900
#define	OPCODE_MOV_REG32_REG32	0x8900
#define	OPCODE_MOV_REG8_MEM8	0x8A00
#define	OPCODE_MOV_REG16_MEM16	0x8B00
#define	OPCODE_MOV_REG32_MEM32	0x8B00
#define	OPCODE_MOV_MEM8_REG8	0x8800
#define	OPCODE_MOV_MEM16_REG16	0x8900
#define	OPCODE_MOV_MEM32_REG32	0x8900
#define	OPCODE_MOV_REG16_SEG	0x8C00
#define	OPCODE_MOV_SEG_REG16	0x8E00
#define	OPCODE_MOV_MEM16_SEG	0x8C00
#define	OPCODE_MOV_SEG_MEM16	0x8E00

#define	OPCODE_MOV_REG32_CR		0x0F2000
#define	OPCODE_MOV_CR_REG32		0x0F2200

#define	OPCODE_MOV_REG32_DR		0x0F2100
#define	OPCODE_MOV_DR_REG32		0x0F2300

#define	OPCODE_MOV_REG32_TR		0x0F2400
#define	OPCODE_MOV_TR_REG32		0x0F2600

#define	OPCODE_MOVSB	0xA4

#define	OPCODE_MOVSW	0xA4

#define	OPCODE_MUL_REG8		0xF620
#define	OPCODE_MUL_REG16	0xF720
#define	OPCODE_MUL_REG32	0xF720
#define	OPCODE_MUL_MEM8		0xF620
#define	OPCODE_MUL_MEM16	0xF720
#define	OPCODE_MUL_MEM32	0xF720

#define	OPCODE_NEG_REG8		0xF618
#define	OPCODE_NEG_MEM8		0xF718
#define	OPCODE_NEG_REG16	0xF618
#define	OPCODE_NEG_MEM16	0xF718

#define	OPCODE_NOP	0x90

#define	OPCODE_NOT_REG8		0xF610
#define	OPCODE_NOT_MEM8		0xF710
#define	OPCODE_NOT_REG16	0xF610
#define	OPCODE_NOT_MEM16	0xF710

#define	OPCODE_OR_REG8_REG8		0x0A00
#define	OPCODE_OR_REG16_REG16	0x0B00
#define	OPCODE_OR_REG32_REG32	0x0B00
#define	OPCODE_OR_MEM8_REG8		0x0800
#define	OPCODE_OR_MEM16_REG16	0x0900
#define	OPCODE_OR_MEM32_REG32	0x0900
#define	OPCODE_OR_REG8_MEM8		0x0A00
#define	OPCODE_OR_REG16_MEM16	0x0B00
#define	OPCODE_OR_REG32_MEM32	0x0B00
#define	OPCODE_OR_ACC8_IMM8		0x0C
#define	OPCODE_OR_ACC16_IMM16	0x0D
#define	OPCODE_OR_ACC32_IMM32	0x0D
#define	OPCODE_OR_REG_IMM8		0x8208
#define	OPCODE_OR_MEM_IMM8		0x8208
#define	OPCODE_OR_REG8_IMM8		0x8008
#define	OPCODE_OR_REG16_IMM16	0x8108
#define	OPCODE_OR_REG32_IMM32	0x8108
#define	OPCODE_OR_MEM8_IMM8		0x8008
#define	OPCODE_OR_MEM16_IMM16	0x8108
#define	OPCODE_OR_MEM32_IMM32	0x8108

#define	OPCODE_OUTB_Imm8	0xE6
#define	OPCODE_OUTW_Imm8	0xE7
#define	OPCODE_OUTD_Imm8	0xE7
#define	OPCODE_OUTB_DX		0xEE
#define	OPCODE_OUTW_DX		0xEF
#define	OPCODE_OUTD_DX		0xEF

#define	OPCODE_OUTSB	0x6E
#define	OPCODE_OUTSW	0x6F
#define	OPCODE_OUTSD	0x6F

#define	OPCODE_POP_REG16	0x58
#define	OPCODE_POP_REG32	0x58
#define	OPCODE_POP_MEM16	0x8F00
#define	OPCODE_POP_MEM32	0x8F00
#define	OPCODE_POP_CS		0x0F
#define	OPCODE_POP_SS		0x17
#define	OPCODE_POP_DS		0x1F
#define	OPCODE_POP_ES		0x07
#define	OPCODE_POP_FS		0x0FA1
#define	OPCODE_POP_GS		0x0FA9

#define	OPCODE_POPA	0x61

#define	OPCODE_POPF	0x9D

#define	OPCODE_PUSH_REG16	0x50
#define	OPCODE_PUSH_REG32	0x50
#define	OPCODE_PUSH_MEM16	0xFF30
#define	OPCODE_PUSH_MEM32	0xFF30
#define	OPCODE_PUSH_IMM8	0x6A
#define	OPCODE_PUSH_IMM16	0x68
#define	OPCODE_PUSH_IMM32	0x68
#define	OPCODE_PUSH_CS		0x0E
#define	OPCODE_PUSH_SS		0x16
#define	OPCODE_PUSH_DS		0x1E
#define	OPCODE_PUSH_ES		0x06
#define	OPCODE_PUSH_FS		0x0FA0
#define	OPCODE_PUSH_GS		0x0FA8

#define	OPCODE_PUSHA	0x60

#define	OPCODE_PUSHF	0x9C

#define	OPCODE_RCL_REG8_1	0xD010
#define	OPCODE_RCL_REG16_1	0xD110
#define	OPCODE_RCL_REG32_1	0xD110
#define	OPCODE_RCL_MEM8_1	0xD010
#define	OPCODE_RCL_MEM16_1	0xD110
#define	OPCODE_RCL_MEM32_1	0xD110
#define	OPCODE_RCL_REG8_CL	0xD210
#define	OPCODE_RCL_REG16_CL	0xD310
#define	OPCODE_RCL_REG32_CL	0xD310
#define	OPCODE_RCL_MEM8_CL	0xD210
#define	OPCODE_RCL_MEM16_CL	0xD310
#define	OPCODE_RCL_MEM32_CL	0xD310

#define	OPCODE_RCR_REG8_1	0xD018
#define	OPCODE_RCR_REG16_1	0xD118
#define	OPCODE_RCR_REG32_1	0xD118
#define	OPCODE_RCR_MEM8_1	0xD018
#define	OPCODE_RCR_MEM16_1	0xD118
#define	OPCODE_RCR_MEM32_1	0xD118
#define	OPCODE_RCR_REG8_CL	0xD218
#define	OPCODE_RCR_REG16_CL	0xD318
#define	OPCODE_RCR_REG32_CL	0xD318
#define	OPCODE_RCR_MEM8_CL	0xD218
#define	OPCODE_RCR_MEM16_CL	0xD318
#define	OPCODE_RCR_MEM32_CL	0xD318

#define	OPCODE_RDMSR	0x0F32

#define	OPCODE_RDTSC	0x0F31

#define	OPCODE_RET_NEAR			0xC3
#define	OPCODE_RET_IMM16NEAR	0xC2
#define	OPCODE_RET_FAR			0xCB
#define	OPCODE_RET_IMM16FAR		0xCA

#define OPCODE_RDPMC	0x0F33

#define	OPCODE_ROL_REG8_1	0xD000
#define	OPCODE_ROL_REG16_1	0xD100
#define	OPCODE_ROL_REG32_1	0xD100
#define	OPCODE_ROL_MEM8_1	0xD000
#define	OPCODE_ROL_MEM16_1	0xD100
#define	OPCODE_ROL_MEM32_1	0xD100
#define	OPCODE_ROL_REG8_CL	0xD200
#define	OPCODE_ROL_REG16_CL	0xD300
#define	OPCODE_ROL_REG32_CL	0xD300
#define	OPCODE_ROL_MEM8_CL	0xD200
#define	OPCODE_ROL_MEM16_CL	0xD300
#define	OPCODE_ROL_MEM32_CL	0xD300

#define	OPCODE_ROR_REG8_1	0xD008
#define	OPCODE_ROR_REG16_1	0xD108
#define	OPCODE_ROR_REG32_1	0xD108
#define	OPCODE_ROR_MEM8_1	0xD008
#define	OPCODE_ROR_MEM16_1	0xD108
#define	OPCODE_ROR_MEM32_1	0xD108
#define	OPCODE_ROR_REG8_CL	0xD208
#define	OPCODE_ROR_REG16_CL	0xD308
#define	OPCODE_ROR_REG32_CL	0xD308
#define	OPCODE_ROR_MEM8_CL	0xD208
#define	OPCODE_ROR_MEM16_CL	0xD308
#define	OPCODE_ROR_MEM32_CL	0xD308

#define	OPCODE_RSM	0x0FAA

#define	OPCODE_SETcc	0x0F90

#define	OPCODE_SAHF	0x9E

#define	OPCODE_SAL_REG8_1	0xD020
#define	OPCODE_SAL_REG16_1	0xD120
#define	OPCODE_SAL_REG32_1	0xD120
#define	OPCODE_SAL_MEM8_1	0xD020
#define	OPCODE_SAL_MEM16_1	0xD120
#define	OPCODE_SAL_MEM32_1	0xD120
#define	OPCODE_SAL_REG8_CL	0xD220
#define	OPCODE_SAL_REG16_CL	0xD320
#define	OPCODE_SAL_REG32_CL	0xD320
#define	OPCODE_SAL_MEM8_CL	0xD220
#define	OPCODE_SAL_MEM16_CL	0xD320
#define	OPCODE_SAL_MEM32_CL	0xD320

#define	OPCODE_SAR_REG8_1	0xD038
#define	OPCODE_SAR_REG16_1	0xD138
#define	OPCODE_SAR_REG32_1	0xD138
#define	OPCODE_SAR_MEM8_1	0xD038
#define	OPCODE_SAR_MEM16_1	0xD138
#define	OPCODE_SAR_MEM32_1	0xD138
#define	OPCODE_SAR_REG8_CL	0xD238
#define	OPCODE_SAR_REG16_CL	0xD338
#define	OPCODE_SAR_REG32_CL	0xD338
#define	OPCODE_SAR_MEM8_CL	0xD238
#define	OPCODE_SAR_MEM16_CL	0xD338
#define	OPCODE_SAR_MEM32_CL	0xD338

#define	OPCODE_SHL_REG8_1	0xD020
#define	OPCODE_SHL_REG16_1	0xD120
#define	OPCODE_SHL_REG32_1	0xD120
#define	OPCODE_SHL_MEM8_1	0xD020
#define	OPCODE_SHL_MEM16_1	0xD120
#define	OPCODE_SHL_MEM32_1	0xD120
#define	OPCODE_SHL_REG8_CL	0xD220
#define	OPCODE_SHL_REG16_CL	0xD230
#define	OPCODE_SHL_REG32_CL	0xD230
#define	OPCODE_SHL_MEM8_CL	0xD220
#define	OPCODE_SHL_MEM16_CL	0xD230
#define	OPCODE_SHL_MEM32_CL	0xD230

#define	OPCODE_SHR_REG8_1	0xD028
#define	OPCODE_SHR_REG16_1	0xD128
#define	OPCODE_SHR_REG32_1	0xD128
#define	OPCODE_SHR_MEM8_1	0xD028
#define	OPCODE_SHR_MEM16_1	0xD128
#define	OPCODE_SHR_MEM32_1	0xD128
#define	OPCODE_SHR_REG8_CL	0xD228
#define	OPCODE_SHR_REG16_CL	0xD328
#define	OPCODE_SHR_REG32_CL	0xD328
#define	OPCODE_SHR_MEM8_CL	0xD228
#define	OPCODE_SHR_MEM16_CL	0xD328
#define	OPCODE_SHR_MEM32_CL	0xD328

#define	OPCODE_SBB_REG8_REG8	0x1A00
#define	OPCODE_SBB_REG16_REG16	0x1B00
#define	OPCODE_SBB_REG32_REG32	0x1B00
#define	OPCODE_SBB_MEM8_REG8	0x1800
#define	OPCODE_SBB_MEM16_REG16	0x1900
#define	OPCODE_SBB_MEM32_REG32	0x1900
#define	OPCODE_SBB_REG8_MEM8	0x1A00
#define	OPCODE_SBB_REG16_MEM16	0x1B00
#define	OPCODE_SBB_REG32_MEM32	0x1B00
#define	OPCODE_SBB_ACC8_IMM8	0x1C
#define	OPCODE_SBB_ACC16_IMM16	0x1D
#define	OPCODE_SBB_ACC32_IMM32	0x1D
#define	OPCODE_SBB_REG_IMM8		0x8218
#define	OPCODE_SBB_MEM_IMM8		0x8218
#define	OPCODE_SBB_REG8_IMM8	0x8018
#define	OPCODE_SBB_REG16_IMM16	0x8118
#define	OPCODE_SBB_REG32_IMM32	0x8118
#define	OPCODE_SBB_MEM8_IMM8	0x8018
#define	OPCODE_SBB_MEM16_IMM16	0x8118
#define	OPCODE_SBB_MEM32_IMM32	0x8118

#define	OPCODE_SCANSB	0xAE
#define	OPCODE_SCANSW	0xAF
#define	OPCODE_SCANSD	0xAF

#define	OPCODE_SGDT_MEM1632		0x0F0100

#define	OPCODE_SHLD_REG16_REG16_IMM8	0x0FA400
#define	OPCODE_SHLD_REG32_REG32_IMM8	0x0FA400
#define	OPCODE_SHLD_MEM16_REG16_IMM8	0x0FA400
#define	OPCODE_SHLD_MEM32_REG32_IMM8	0x0FA400
#define	OPCODE_SHLD_REG16_REG16_CL		0x0FA500
#define	OPCODE_SHLD_REG32_REG32_CL		0x0FA500
#define	OPCODE_SHLD_MEM16_REG16_CL		0x0FA500
#define	OPCODE_SHLD_MEM32_REG32_CL		0x0FA500

#define	OPCODE_SHRD_REG16_REG16_IMM8	0x0FAC00
#define	OPCODE_SHRD_REG32_REG32_IMM8	0x0FAC00
#define	OPCODE_SHRD_MEM16_REG16_IMM8	0x0FAC00
#define	OPCODE_SHRD_MEM32_REG32_IMM8	0x0FAC00
#define	OPCODE_SHRD_REG16_REG16_CL		0x0FAD00
#define	OPCODE_SHRD_REG32_REG32_CL		0x0FAD00
#define	OPCODE_SHRD_MEM16_REG16_CL		0x0FAD00
#define	OPCODE_SHRD_MEM32_REG32_CL		0x0FAD00









#define	OPCODE_STC	0xF9

#define	OPCODE_STD	0xFD

#define	OPCODE_STI	0xFB

#define	OPCODE_STOSB	0xAA

#define	OPCODE_STOSW	0xAB

#define	OPCODE_SUB_REG8_REG8	0x2A00
#define	OPCODE_SUB_REG16_REG16	0x2B00
#define	OPCODE_SUB_REG32_REG32	0x2B00
#define	OPCODE_SUB_MEM8_REG8	0x2800
#define	OPCODE_SUB_MEM16_REG16	0x2900
#define	OPCODE_SUB_MEM32_REG32	0x2900
#define	OPCODE_SUB_REG8_MEM8	0x2A00
#define	OPCODE_SUB_REG16_MEM16	0x2B00
#define	OPCODE_SUB_REG32_MEM32	0x2B00
#define	OPCODE_SUB_ACC8_IMM8	0x2C
#define	OPCODE_SUB_ACC16_IMM16	0x2D
#define	OPCODE_SUB_ACC32_IMM32	0x2D
#define	OPCODE_SUB_REG_IMM8		0x8228
#define	OPCODE_SUB_MEM_IMM8		0x8228
#define	OPCODE_SUB_REG8_IMM8	0x8028
#define	OPCODE_SUB_REG16_IMM16	0x8128
#define	OPCODE_SUB_REG32_IMM32	0x8128
#define	OPCODE_SUB_MEM8_IMM8	0x8028
#define	OPCODE_SUB_MEM16_IMM16	0x8128
#define	OPCODE_SUB_MEM32_IMM32	0x8128

#define	OPCODE_TEST_REG8_REG8	0x8400
#define	OPCODE_TEST_REG16_REG16	0x8500
#define	OPCODE_TEST_REG32_REG32	0x8500
#define	OPCODE_TEST_MEM8_REG8	0x8400
#define	OPCODE_TEST_MEM16_REG16	0x8500
#define	OPCODE_TEST_MEM32_REG32	0x8500
#define	OPCODE_TEST_REG8_MEM8	0x8400
#define	OPCODE_TEST_REG16_MEM16	0x8500
#define	OPCODE_TEST_REG32_MEM32	0x8500
#define	OPCODE_TEST_ACC8_IMM8	0xA8
#define	OPCODE_TEST_ACC16_IMM16	0xA9
#define	OPCODE_TEST_ACC32_IMM32	0xA9
#define	OPCODE_TEST_REG_IMM8	0	/* 不需要 */
#define	OPCODE_TEST_MEM_IMM8	0	/* 不需要 */
#define	OPCODE_TEST_REG8_IMM8	0xF600
#define	OPCODE_TEST_REG16_IMM16	0xF700
#define	OPCODE_TEST_REG32_IMM32	0xF700
#define	OPCODE_TEST_MEM8_IMM8	0xF600
#define	OPCODE_TEST_MEM16_IMM16	0xF700
#define	OPCODE_TEST_MEM32_IMM32	0xF700

#define	OPCODE_WAIT	0x9B

#define	OPCODE_XCHG_ACC16_REG16	0x90
#define	OPCODE_XCHG_REG16_ACC16	0x90
#define	OPCODE_XCHG_REG8_REG8	0x8600
#define	OPCODE_XCHG_REG16_REG16	0x8700
#define	OPCODE_XCHG_MEM8_REG8	0x8600
#define	OPCODE_XCHG_MEM16_REG16	0x8700
#define	OPCODE_XCHG_REG8_MEM8	0x8600
#define	OPCODE_XCHG_REG16_MEM16	0x8700

#define	OPCODE_XLAT	0xD7

#define	OPCODE_XOR_REG8_REG8	0x3200
#define	OPCODE_XOR_REG16_REG16	0x3300
#define	OPCODE_XOR_REG32_REG32	0x3300
#define	OPCODE_XOR_MEM8_REG8	0x3000
#define	OPCODE_XOR_MEM16_REG16	0x3100
#define	OPCODE_XOR_MEM32_REG32	0x3100
#define	OPCODE_XOR_REG8_MEM8	0x3200
#define	OPCODE_XOR_REG16_MEM16	0x3300
#define	OPCODE_XOR_REG32_MEM32	0x3300
#define	OPCODE_XOR_ACC8_IMM8	0x34
#define	OPCODE_XOR_ACC16_IMM16	0x35
#define	OPCODE_XOR_ACC32_IMM32	0x35
#define	OPCODE_XOR_REG_IMM8		0x8230
#define	OPCODE_XOR_MEM_IMM8		0x8230
#define	OPCODE_XOR_REG8_IMM8	0x8030
#define	OPCODE_XOR_REG16_IMM16	0x8130
#define	OPCODE_XOR_REG32_IMM32	0x8130
#define	OPCODE_XOR_MEM8_IMM8	0x8030
#define	OPCODE_XOR_MEM16_IMM16	0x8130
#define	OPCODE_XOR_MEM32_IMM32	0x8130

#define	OPCODE_CALL_MEMFAR	0xFF18
#define	OPCODE_CALL_NEAR	0xE8

#define	OPCODE_JCC_SHORT	0x70

#define	OPCODE_JCXZ_SHORT	0xE3

#define	OPCODE_JMP_SHORT	0xEB
#define	OPCODE_JMP_NEAR		0xE9

#define	OPCODE_LOOP_SHORT	0xE2

#define	OPCODE_LOOPZ_SHORT	0xE1

#define	OPCODE_LOOPNZ_SHORT	0xE0

#define	OPCODE_PREFIX_LOCK	0xF0

#define	OPCODE_PREFIX_REP	0xF3

#define	OPCODE_PREFIX_REPNZ	0xF2

#define	OPCODE_PREFIX_CS	0x2E
#define	OPCODE_PREFIX_DS	0x3E
#define	OPCODE_PREFIX_ES	0x26
#define	OPCODE_PREFIX_SS	0x36
#define	OPCODE_PREFIX_FS	0x64
#define	OPCODE_PREFIX_GS	0x65

#define	OPCODE_PREFIX_OPRDSZ_OVR	0x66
#define	OPCODE_PREFIX_ADDRSZ_OVR	0x67

/*
	cccc
*/
#define	CCCC_O		0x0

#define	CCCC_NO		0x1

#define	CCCC_C		0x2
#define	CCCC_B		0x2
#define	CCCC_NAE	0x2

#define	CCCC_NC		0x3
#define	CCCC_AE		0x3
#define	CCCC_NB		0x3

#define	CCCC_E		0x4
#define	CCCC_Z		0x4

#define	CCCC_NE		0x5
#define	CCCC_NZ		0x5

#define	CCCC_BE		0x6
#define	CCCC_NA		0x6

#define	CCCC_A		0x7
#define	CCCC_NBE	0x7

#define	CCCC_S		0x8

#define	CCCC_NS		0x9

#define	CCCC_P		0xA
#define	CCCC_PE		0xA

#define	CCCC_NP		0xB
#define	CCCC_PO		0xB

#define	CCCC_L		0xC
#define	CCCC_NGE	0xC

#define	CCCC_GE		0xD
#define	CCCC_NL		0xD

#define	CCCC_LE		0xE
#define	CCCC_NG		0xE

#define	CCCC_G		0xF
#define	CCCC_NLE	0xF

#endif
