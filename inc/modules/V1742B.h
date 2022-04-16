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
#define     V1742B_CHANNEL_THR_IDX_MSK      0xF000UL//Aux 
#define     V1742B_CHANNEL_THR_IDX_SHFT     0x000CU//Aux

#define     V1742B_STATUS(X)                V1742B_GROUP_ADDRESS(0x1088UL,X)//A32/D32 R G

#define     V1742B_AMC_FRMW_REV(x)          V1742B_GROUP_ADDRESS(0x108CUL,X)//A32/D32 R G
#define     V1742B_AMC_FRMW_REV_MIN_MSK     0x000000FFUL//Aux
#define     V1742B_AMC_FRMW_REV_MAJ_MSK     0x0000FF00UL//Aux
#define     V1742B_AMC_FRMW_REV_DATE_MSK    0xFFFF0000UL//Aux

#define     V1742B_CHANNEL_DC_OFST(X)       V1742B_GROUP_ADDRESS(0x1098UL,X)//A32/D32 RW G
#define     V1742B_CHANNEL_DC_OFST_VAL_MSK  0x0000FFFFUL//Aux
#define     V1742B_CHANNEL_DC_OFST_IDX_MSK  0x000F0000UL//Aux
#define     V1742B_CHANNEL_DC_OFST_IDX_SHFT 0x0010U//Aux

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

#define     V1742B_LVDS_IO_DATA             0x8118UL//A32/D32 RW C
#define     V1742B_LVDS_IO_DATA_VAL_MSK     0xFFFFUL//Aux

#define     V1742B_FRNT_PANEL_IO_CNTL       0x811CUL//A32/D32 RW C 

#define     V1742B_GROUP_EN_MASK            0x8120UL//A32/D32 RW C
#define     V1742B_GROUP_EN_MASK_VAL_MSK    0x000FUL//Aux

#define     V1742B_ROC_FPGA_FWM_REV         0x8124UL//A32/D32 R C

#define     V1742B_SOFTWARE_CLK_SYNC        0x813CUL//A32/D32 W C

#define     V1742B_BOARD_INFO               0x8140UL//A32/D32 R C

#define     V1742B_EVENT_SIZE               0x814CUL//A32/D32 R C

//NOTE: this register is supported by Desktop (DT) boards only.
#define     V1742B_FAN_SPEED_CTRL           0x8168UL//A32/D32 RW C

#define     V1742B_RUN_DELAY                0x8170UL//A32/D32 RW C
#define     V1742B_RUN_DELAY_VAL_MSK        0x00FFUL//Aux

#define     V1742B_BOARD_FAIL_STATUS        0x8178UL//A32/D32 R C

#define     V1742B_FRNT_PANEL_LVDS          0x81A0UL//A32/D32 RW C

#define     V1742B_EXTD_VETO_DELAY          0x81C4UL//A32/D32 RW C 
#define     V1742B_EXTD_VETO_DELAY_VAL_MSK  0xFFFFUL//Aux

#define     V1742B_READOUT_CTRL             0xEF00UL//A32/D32 RW C
#define     V1742B_READOUT_CTRL_VAL_MSK     0x01FFUL//Aux
#define     V1742B_READOUT_CTRL_INT_MSK     0x0007UL//Aux

#define     V1742B_READOUT_STATUS           0xEF04UL//A32/D32 R C
#define     V1742B_READOUT_STATUS_VAL_MSK   0x000FUL//Aux

#define     V1742B_BOARD_ID                 0xEF08UL//A32/D32 RW C
#define     V1742B_BOARD_ID_GEO_MSK         0x001FUL//Aux

#define     V1742B_MCST_ADDR_CTRL           0xEF0CUL//A32/D32 RW C
#define     V1742B_MCST_ADDR_CTRL_ADDR_MSK  0x00FFUL//Aux
#define     V1742B_MCST_ADDR_CTRL_POS_MSK   0x0300UL//Aux

#define     V1742B_RELOC_ADDR               0xEF10UL//A32/D32 RW C
#define     V1742B_RELOC_ADDR_VAL_MSK       0xFFFFUL//Aux

#define     V1742B_INTRPT_STATUS            0xEF14UL//A32/D32 RW C

#define     V1742B_INTRPT_EVNT_NUM          0xEF18UL//A32/D32 RW C
#define     V1742B_INTRPT_EVNT_NUM_VAL_MSK  0x03FFUL//Aux

#define     V1742B_MAX_EVENT_BLT            0xEF1CUL//A32/D32 RW C
#define     V1742B_MAX_EVENT_BLT_VAL_MSK    0x03FFUL//Aux

#define     V1742B_SCRATCH                  0xEF20UL//A32/D32 RW C

#define     V1742B_SOFTWARE_RST             0xEF24UL//A32/D32 W C

#define     V1742B_SOFTWARE_CLR             0xEF28UL//A32/D32 W C

#define     V1742B_LUB                      0xF08BUL//Aux, The Last Used Byte

#include "VSlaveInterrupter.h"
#include "VSlaveAcquisitor.h"
#include "UConfigurable.h"


namespace vmepp
{
    class V1742B : public VSlaveInterrupter, public VSlaveAcquisitor, public UConfigurable<V1742B>
    {
        public :
            enum class RecordLength_t : uint8_t
            {
                s1024 = 0x00,
                s520  = 0x01,
                s256  = 0x02,
                s136  = 0x03,
            };

            enum class SamplingRate_t : uint8_t
            {
                M5000 = 0x00,
                M2500 = 0x01,
                M1000 = 0x02,
                M750  = 0x03,
            };

            enum class FanSpeed_t : uint8_t
            {
                LowAuto = 0x00,
                High    = 0x01,
            };

            enum class Group_t : uint8_t { G1 = 0, G2, G3, G4, };

            enum class TR_t : uint8_t { TR0 = 0, TR1 = 1, };

            enum StatusBit : uint32_t
            {
                MemoryFull      = 0x0001,
                MemoryEmpty     = 0x0002,
                BusySPI         = 0x0004,
                LockedPLLEven   = 0x0040,
                LockedPLLOdd    = 0x0080,
                BusyDRS4        = 0x0100,
                MezzRevision    = 0x0200,
                All             = 0x03C7,
            };

        protected :
            static uint8_t constexpr fChNumber      = 0x20U;   // 32 
            static uint8_t constexpr fGroupNumber   = 0x04; // 4

        public :
            static uint8_t constexpr GetChNumber() { return fChNumber; }
            static uint8_t constexpr GetGroupNumber() { return fGroupNumber; }

        protected :
            virtual void    Initialize() override;

        protected :
            void            WaitForSPI( Group_t group );

        public :
            V1742B( uint32_t baseAddress, uint32_t range = V1742B_LUB );
            ~V1742B() override = default;

        public :
            // Misc
            virtual void    Reset() override;
            void            WriteDummy32( Group_t group, uint32_t word );
            uint32_t        ReadDummy32( Group_t group );

            int16_t         ReadChipTemperature( Group_t group );

        public :
            // Info & ROM

        public :
            // Interrupts
            void            WriteIRQEvents( uint16_t n );
            uint16_t        ReadIRQEvents();
            void            WriteIRQLevel( uint16_t level ) override;
            uint16_t        ReadIRQLevel() override;
            void            WriteIRQVector( uint16_t level ) override;
            uint16_t        ReadIRQVector() override;
            void            ISR( uint16_t vector ) override;

        public :
            // Control & Status
            void            WriteReadoutCtrl( uint32_t value );
            uint32_t        ReadReadoutCtrl();

            uint32_t        ReadStatus( Group_t group );
            bool            ReadStatus( Group_t, StatusBit bit );

        public :
            // Trigger
            void WritePostTrigger( Group_t group, uint16_t n );
            uint16_t ReadPostTrigger( Group_t group );

            void WriteChannelThreshold( uint8_t ch, uint16_t threshold );
            uint16_t ReadChannelThreshold( uint8_t ch );

            void WriteEnableTrigger( uint8_t ch, bool enable = true );
            bool ReadEnableTriggerC( uint8_t ch );
            void WriteEnableTrigger( Group_t group, uint8_t mask );
            uint8_t ReadEnableTrigger( Group_t group );

            void WriteThresholdTR( TR_t tr, uint16_t threshold );
            uint16_t ReadThresholdTR( TR_t tr );

            void WriteOffsetTR( TR_t tr, uint16_t offset );
            uint16_t ReadOffsetTR( TR_t tr );

        public :
            // Channel
            void WriteChannelOffset( uint8_t ch, uint16_t offset );
            uint16_t ReadChannelOffset( uint8_t ch );
    };
}

#endif
