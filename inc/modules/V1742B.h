#ifndef V_PLUS_V1742B
#define V_PLUS_V1742B

//*******************************************************************
//
// Description :
//      Header for the CAEN V1742B module.
//      V1742B is a 32+2 channel 12 bit 5 GS/s Digitizer 
// Author :
//      Sikach Bogdan
// Location :
//      Budker Institute of Nuclear Physics, Room 210/2, Phone 51-57
// Feedback :
//      paradox1859@gmail.com
//
//*******************************************************************
//
//
//*******************************************************************
// Comments description
// D32, D16, ...    = Data size
// A32, A24, ...    = Address modifier (however, it's always A32 here)
// R, W, RW         = Read, Write or Read-Write access modes
// G, C             = Group or Common register
// DR, SR, HR       = Data Reset, Software Reset or Hardware Reset (see the module's manual)
// Aux              = Auxiliary data
//*******************************************************************

// MEMORY MAP (BEGIN)
//****** Multi Event Buffer Definition ******
#define     V1742B_OUTPUT_BUFFER            0x0000UL//A32/D32 R DR/SR/HR C
#define     V1742B_OUTPUT_BUFFER_START      V1742B_OUTPUT_BUFFER//Aux
#define     V1742B_OUTPUT_BUFFER_STOP       0x0FFCUL//Aux, This is the last byte occupied by buffer
#define     V1742B_OUTPUT_BUFFER_SIZE       (V1742B_OUTPUT_BUFFER_STOP - V1742B_OUTPUT_BUFFER_START + 1UL)//Aux

#define     V1742B_N_GROUPS                 0x4U//Aux
#define     V1742B_GROUP_ADDRESS(BASE,G)    ((BASE) + (0x0100UL * ((G) % V1742B_N_GROUPS)))//Aux

#define     V1742B_POST_TRIGGER(X)          V1742B_GROUP_ADDRESS(0x1014UL,X)//A32/D32 RW G
#define     V1742B_POST_TRIGGER_VAL_MSK     0x03FFUL//Aux

#define     V1742B_DUMMY32(X)               V1742B_GROUP_ADDRESS(0x1024UL,X)//A32/D32 RW G

#define     V1742B_CHANNEL_THR(X)           V1742B_GROUP_ADDRESS(0x1080UL,X)//A32/D32 RW G
#define     V1742B_CHANNEL_THR_VAL_MSK      0x0FFFUL//Aux 
#define     V1742B_CHANNEL_THR_INDX_MSK     0xF000UL//Aux 

#define     V1742B_STATUS(X)                V1742B_GROUP_ADDRESS(0x1088UL,X)//A32/D32 R G

#define     V1742B_AMC_FRMW_REV(x)          V1742B_GROUP_ADDRESS(0x108CUL,X)//A32/D32 R G
#define     V1742B_AMC_FRMW_REV_MIN_MSK     0x000000FFUL//Aux
#define     V1742B_AMC_FRMW_REV_MAJ_MSK     0x0000FF00UL//Aux
#define     V1742B_AMC_FRMW_REV_DATE_MSK    0xFFFF0000UL//Aux

#define     V1742B_CHANNEL_DC_OFST(X)       V1742B_GROUP_ADDRESS(0x1098UL,X)//A32/D32 RW G
#define     V1742B_CHANNEL_DC_OFST_VAL_MSK  0x0000FFFFUL//Aux
#define     V1742B_CHANNEL_DC_OFST_INDX_MSK 0x000F0000UL//Aux

#define     V1742B_CHIP_TEMP(X)             V1742B_GROUP_ADDRESS(0x10A0UL,X)//A32/D32 R G
#define     V1742B_CHIP_TEMP_VAL_MSK        0x00FFUL//Aux

#define     V1742B_CHANNEL_SEL(X)           V1742B_GROUP_ADDRESS(0x10A4UL,X)//A32/D32 RW G
#define     V1742B_CHANNEL_SEL_VAL_MSK      0x000FUL//Aux

#define     V1742B_CHANNEL_TRG_MASK(X)      V1742B_GROUP_ADDRESS(0x10A8UL,X)//A32/D32 RW G
#define     V1742B_CHANNEL_TRG_MASK_MSK     0x00FFUL//Aux

//WARNING: before writing this register, it is necessary to check that bit[2] = 0 at 0x1X88,
//otherwise the writing process will not run properly!
#define     V1742B_TR_TRG_THR(X)            V1742B_GROUP_ADDRESS(0x10D4UL,X)//A32/D32 RW G
#define     V1742B_TR_TRG_THR_VAL_MSK       0xFFFFUL//Aux

#define     V1742B_TR_DC_OFST(X)            V1742B_GROUP_ADDRESS(0x10DCUL,X)//A32/D32 RW G
#define     V1742B_TR_DC_OFST_VAL_MSK       0xFFFFUL//Aux

#define     V1742B_BOARD_CFG                0x8000UL//A32/D32 R C
#define     V1742B_BOARD_CFG_SET            0x8004UL//A32/D32 W C
#define     V1742B_BOARD_CFG_CLR            0x8008UL//A32/D32 W C

//WARNING: this register must not be written while the acquisition is running.
#define     V1742B_CUSTOM_SIZE              0x8020UL//A32/D32 RW C
#define     V1742B_CUSTOM_SIZE_VAL_MSK      0x0003UL//Aux

#define     V1742B_TEST_MODE_INIT_READ(X)   V1742B_GROUP_ADDRESS(0x107CUL,X)//A32/D32 R G 
#define     V1742B_TEST_MODE_INIT_WRITE     0x807CUL//A32/D32 W C
#define     V1742B_TEST_MODE_INIT_VAL_MSK   0x0FFFUL//Aux

//WARNING: This register must not be written while the acquisition is running.
#define     V1742B_DRS4_SAMP_FREQ_READ(X)   V1742B_GROUP_ADDRESS(0x10D8UL,X)//A32/D32 R G           
#define     V1742B_DRS4_SAMP_FREQ_WRITE     0x80D8UL//A32/D32 W C
#define     V1742B_DRS4_SAMP_FREQ_VAL_MSK   0x0003UL//Aux

#define     V1742B_ACQ_CONTROL              0x8100UL//A32/D32 RW C

#define     V1742B_ACQ_STATUS               0x8104UL//A32/D32 R C

#define     V1742B_SOFT_TRG                 0x8108UL//A32/D32 W C

#define     V1742B_GLB_TRG_MASK             0x810CUL//A32/D32 RW C

#define     V1742B_FRNT_PANEL_TRG           0x8110UL//A32/D32 RW C
#define     V1742B_FRNT_PANEL_TRG_INDX_MSK  0x000FUL//Aux
#define     V1742B_FRNT_PANEL_TRG_LOGIC_MSK 0x0300UL//Aux

#define     V1742B_SCRATCH                  0xEF20UL//A32/D32 RW C

#define     V1742B_SOFTWARE_RST             0xEF24UL//A32/D32 W C

#define     V1742B_SOFTWARE_CLR             0xEF28UL//A32/D32 W C

#endif
