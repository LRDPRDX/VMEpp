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
#define     V1742B_POST_TRIGGER(x)          (0x1014UL + (0x0100UL * ((x) % V1742B_N_GROUPS)))//A32/D32 RW G
#define     V1742B_DUMMY32(x)               (0x1024UL + (0x0100UL * ((x) % V1742B_N_GROUPS)))//A32/D32 RW G
#define     V1742B_CHANNEL_THR(x)           (0x1080UL + (0x0100UL * ((x) % V1742B_N_GROUPS)))//A32/D32 RW G
#define     V1742B_CHANNEL_THR_VAL_MSK      0x0FFFUL//Aux 
#define     V1742B_CHANNEL_THR_INDX_MSK     0xF000UL//Aux 
#define     V1742_STATUS(x)                 (0x1088UL + (0x0100UL * ((x) % V1742B_N_GROUPS)))//A32/D32 R G

#endif
