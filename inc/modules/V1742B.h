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
#define     V1742B_OUTPUT_BUFFER                0x0000UL//A32/D32 R DR/SR/HR C
#define     V1742B_OUTPUT_BUFFER_START          V1742B_OUTPUT_BUFFER//Aux
#define     V1742B_OUTPUT_BUFFER_STOP           0x0FFCUL//Aux, This is the last byte occupied by buffer
#define     V1742B_OUTPUT_BUFFER_SIZE           (V1742B_OUTPUT_BUFFER_STOP - V1742B_OUTPUT_BUFFER_START + 1UL)//Aux

#define     V1742B_N_GROUPS                     0x4U//Aux
#define     V1742B_GROUP_SINGLE(BASE,G)         ((BASE) + (0x0100UL * ((G) % V1742B_N_GROUPS)))
#define     V1742B_GROUP_BCST(BASE)             (((BASE) & 0x00FFUL) | 0x8000UL)
#define     V1742B_GROUP_ADDRESS(BASE,G)        (((G) < V1742B_N_GROUPS) ? V1742B_GROUP_SINGLE(BASE,G) : V1742B_GROUP_BCST(BASE))//Aux

#define     V1742B_POST_TRIGGER(X)              V1742B_GROUP_ADDRESS(0x1014UL,(X))//A32/D32 RW G
#define     V1742B_POST_TRIGGER_VAL_MSK         0x03FFUL//Aux

#define     V1742B_DUMMY32(X)                   V1742B_GROUP_ADDRESS(0x1024UL,(X))//A32/D32 RW G

#define     V1742B_CHANNEL_THR(X)               V1742B_GROUP_ADDRESS(0x1080UL,(X))//A32/D32 RW G
#define     V1742B_CHANNEL_THR_VAL_MSK          0x0FFFUL//Aux 
#define     V1742B_CHANNEL_THR_IDX_MSK          0xF000UL//Aux 
#define     V1742B_CHANNEL_THR_IDX_SHFT         0x000CU//Aux

#define     V1742B_STATUS(X)                    V1742B_GROUP_ADDRESS(0x1088UL,(X))//A32/D32 R G

#define     V1742B_AMC_FRMW_REV(X)              V1742B_GROUP_ADDRESS(0x108CUL,(X))//A32/D32 R G
#define     V1742B_AMC_FRMW_REV_MIN_MSK         0x000000FFUL//Aux
#define     V1742B_AMC_FRMW_REV_MIN_SHFT        0x00U//Aux
#define     V1742B_AMC_FRMW_REV_MAJ_MSK         0x0000FF00UL//Aux
#define     V1742B_AMC_FRMW_REV_MAJ_SHFT        0x08U//Aux
#define     V1742B_AMC_FRMW_REV_DATE_MSK        0xFFFF0000UL//Aux
#define     V1742B_AMC_FRMW_REV_DATE_SHFT       0x10U//Aux

#define     V1742B_CHANNEL_DC_OFST(X)           V1742B_GROUP_ADDRESS(0x1098UL,(X))//A32/D32 RW G
#define     V1742B_CHANNEL_DC_OFST_VAL_MSK      0x0000FFFFUL//Aux
#define     V1742B_CHANNEL_DC_OFST_IDX_MSK      0x000F0000UL//Aux
#define     V1742B_CHANNEL_DC_OFST_IDX_SHFT     0x0010U//Aux

#define     V1742B_CHIP_TEMP(X)                 V1742B_GROUP_ADDRESS(0x10A0UL,(X))//A32/D32 R G
#define     V1742B_CHIP_TEMP_VAL_MSK            0x00FFUL//Aux

#define     V1742B_CHANNEL_SEL(X)               V1742B_GROUP_ADDRESS(0x10A4UL,(X))//A32/D32 RW G
#define     V1742B_CHANNEL_SEL_VAL_MSK          0x000FUL//Aux

#define     V1742B_CHANNEL_TRG_MASK(X)          V1742B_GROUP_ADDRESS(0x10A8UL,(X))//A32/D32 RW G
#define     V1742B_CHANNEL_TRG_MASK_MSK         0x00FFUL//Aux

//WARNING: before writing this register, it is necessary to check that bit[2] = 0 at 0x1X88,
//otherwise the writing process will not run properly!
#define     V1742B_TR_TRG_THR(X)                V1742B_GROUP_ADDRESS(0x10D4UL,(X))//A32/D32 RW G
#define     V1742B_TR_TRG_THR_VAL_MSK           0xFFFFUL//Aux

#define     V1742B_TR_DC_OFST(X)                V1742B_GROUP_ADDRESS(0x10DCUL,(X))//A32/D32 RW G
#define     V1742B_TR_DC_OFST_VAL_MSK           0xFFFFUL//Aux

#define     V1742B_BOARD_CFG                    0x8000UL//A32/D32 R C
#define     V1742B_BOARD_CFG_SET                0x8004UL//A32/D32 W C
#define     V1742B_BOARD_CFG_MUST_SET           0x00000110UL//Aux
#define     V1742B_BOARD_CFG_MUST_CLR           0x00006000UL//Aux
#define     V1742B_BOARD_CFG_CLR                0x8008UL//A32/D32 W C
#define     V1742B_BOARD_CFG_TM_SHFT            0x03U//Aux
#define     V1742B_BOARD_CFG_TRG_POL_SHFT       0x06U//Aux
#define     V1742B_BOARD_CFG_TRG_DG_SHFT        0x0BU//Aux
#define     V1742B_BOARD_CFG_TRG_EN_SHFT        0x0CU//Aux
#define     V1742B_BOARD_CFG_MODE_SHFT          0x0DU//Aux
#define     V1742B_BOARD_CFG_TGIN_EN_SHFT       0x0FU//Aux
#define     V1742B_BOARD_CFG_TGIN_SIG_SHFT      0x11U//Aux
#define     V1742B_BOARD_CFG_TGOUT_SIG_SHFT     0x1CU//Aux
#define     V1742B_BOARD_CFG_TGOUT_SIG_MSK      0x30000000UL//Aux

//WARNING: this register must not be written while the acquisition is running.
#define     V1742B_CUSTOM_SIZE                  0x8020UL//A32/D32 RW C
#define     V1742B_CUSTOM_SIZE_VAL_MSK          0x0003UL//Aux

#define     V1742B_TEST_MODE_INIT_READ(X)       V1742B_GROUP_ADDRESS(0x107CUL,X)//A32/D32 R G 
#define     V1742B_TEST_MODE_INIT_WRITE         0x807CUL//A32/D32 W C
#define     V1742B_TEST_MODE_INIT_VAL_MSK       0x0FFFUL//Aux

//WARNING: This register must not be written while the acquisition is running.
#define     V1742B_DRS4_SAMP_FREQ_READ(X)       V1742B_GROUP_ADDRESS(0x10D8UL,X)//A32/D32 R G
#define     V1742B_DRS4_SAMP_FREQ_WRITE         0x80D8UL//A32/D32 W C
#define     V1742B_DRS4_SAMP_FREQ_VAL_MSK       0x0003UL//Aux

#define     V1742B_ACQ_CTRL                     0x8100UL//A32/D32 RW C
#define     V1742B_ACQ_CTRL_SRC_MSK             0x00000003UL//Aux
#define     V1742B_ACQ_CTRL_START_SHFT          0x02U//Aux
#define     V1742B_ACQ_CTRL_PLL_SHFT            0x06U//Aux
#define     V1742B_ACQ_CTRL_LVDS_BUSY_SHFT      0x08U//Aux
#define     V1742B_ACQ_CTRL_LVDS_VETO_SHFT      0x09U//Aux
#define     V1742B_ACQ_CTRL_LVDS_RNIN_SHFT      0x0BU//Aux
#define     V1742B_ACQ_CTRL_LVDS_VTIN_SHFT      0x0CU//Aux

#define     V1742B_ACQ_STATUS                   0x8104UL//A32/D32 R C

#define     V1742B_SOFT_TRG                     0x8108UL//A32/D32 W C

#define     V1742B_GLB_TRG_MASK                 0x810CUL//A32/D32 RW C
#define     V1742B_GLB_TRG_MASK_VAL_MSK         0xC0000000UL//Aux
#define     V1742B_GLB_TRG_MASK_VAL_SHFT        0x1EU//Aux 

#define     V1742B_FRNT_PANEL_TRG               0x8110UL//A32/D32 RW C
#define     V1742B_FRNT_PANEL_TRG_INDX_MSK      0x000FUL//Aux
#define     V1742B_FRNT_PANEL_TRG_LOGIC_MSK     0x0300UL//Aux

#define     V1742B_LVDS_IO_DATA                 0x8118UL//A32/D32 RW C
#define     V1742B_LVDS_IO_DATA_VAL_MSK         0xFFFFUL//Aux

#define     V1742B_FRNT_PNL_IO_CTRL             0x811CUL//A32/D32 RW C 
#define     V1742B_FRNT_PNL_IO_LEMO_SHFT        0x00U//Aux
#define     V1742B_FRNT_PNL_IO_TGOUT_EN_SHFT    0x01U//Aux
#define     V1742B_FRNT_PNL_IO_TGIN_CTRL_SHFT   0x0AU//Aux
#define     V1742B_FRNT_PNL_IO_TGIN_MEZZ_SHFT   0x0BU//Aux
#define     V1742B_FRNT_PNL_IO_TGOUT_FRC_SHFT   0x0EU//Aux
#define     V1742B_FRNT_PNL_IO_TGOUT_MOD_SHFT   0x0FU//Aux

#define     V1742B_GROUP_EN_MASK                0x8120UL//A32/D32 RW C
#define     V1742B_GROUP_EN_MASK_VAL_MSK        0x000FUL//Aux

#define     V1742B_ROC_FRMW_REV                 0x8124UL//A32/D32 R C
#define     V1742B_ROC_FRMW_REV_MIN_MSK         0x000000FFUL//Aux
#define     V1742B_ROC_FRMW_REV_MIN_SHFT        0x00U//Aux
#define     V1742B_ROC_FRMW_REV_MAJ_MSK         0x0000FF00UL//Aux
#define     V1742B_ROC_FRMW_REV_MAJ_SHFT        0x08U//Aux
#define     V1742B_ROC_FRMW_REV_DATE_MSK        0xFFFF0000UL//Aux
#define     V1742B_ROC_FRMW_REV_DATE_SHFT       0x10U//Aux

#define     V1742B_SOFTWARE_CLK_SYNC            0x813CUL//A32/D32 W C

#define     V1742B_BOARD_INFO                   0x8140UL//A32/D32 R C
#define     V1742B_BOARD_INFO_CODE_MSK          0x000000FFUL//Aux
#define     V1742B_BOARD_INFO_CODE_SHFT         0x0U//Aux
#define     V1742B_BOARD_INFO_MEM_MSK           0x0000FF00UL//Aux
#define     V1742B_BOARD_INFO_MEM_SHFT          0x8U//Aux
#define     V1742B_BOARD_INFO_GROUP_MSK         0x00FF0000UL//Aux
#define     V1742B_BOARD_INFO_GROUP_SHFT        0x10U//Aux

#define     V1742B_EVENT_SIZE                   0x814CUL//A32/D32 R C

//NOTE: this register is supported by Desktop (DT) boards only.
#define     V1742B_FAN_SPEED_CTRL               0x8168UL//A32/D32 RW C

#define     V1742B_RUN_DELAY                    0x8170UL//A32/D32 RW C
#define     V1742B_RUN_DELAY_VAL_MSK            0x00FFUL//Aux

#define     V1742B_BOARD_FAIL                   0x8178UL//A32/D32 R C
#define     V1742B_BOARD_FAIL_VAL_MSK           0x0010UL//Aux

#define     V1742B_FRNT_PANEL_LVDS              0x81A0UL//A32/D32 RW C

#define     V1742B_EXTD_VETO_DELAY              0x81C4UL//A32/D32 RW C 
#define     V1742B_EXTD_VETO_DELAY_VAL_MSK      0xFFFFUL//Aux

#define     V1742B_READOUT_CTRL                 0xEF00UL//A32/D32 RW C
#define     V1742B_READOUT_CTRL_VAL_MSK         0x01FFUL//Aux
#define     V1742B_READOUT_CTRL_INT_MSK         0x0007UL//Aux

#define     V1742B_READOUT_STATUS               0xEF04UL//A32/D32 R C
#define     V1742B_READOUT_STATUS_VAL_MSK       0x000FUL//Aux

#define     V1742B_BOARD_ID                     0xEF08UL//A32/D32 RW C
#define     V1742B_BOARD_ID_GEO_MSK             0x001FUL//Aux

#define     V1742B_MCST_ADDR_CTRL               0xEF0CUL//A32/D32 RW C
#define     V1742B_MCST_ADDR_CTRL_ADDR_MSK      0x00FFUL//Aux
#define     V1742B_MCST_ADDR_CTRL_POS_MSK       0x0300UL//Aux

#define     V1742B_RELOC_ADDR                   0xEF10UL//A32/D32 RW C
#define     V1742B_RELOC_ADDR_VAL_MSK           0xFFFFUL//Aux

#define     V1742B_INTRPT_STATUS                0xEF14UL//A32/D32 RW C

#define     V1742B_INTRPT_EVNT_NUM              0xEF18UL//A32/D32 RW C
#define     V1742B_INTRPT_EVNT_NUM_VAL_MSK      0x03FFUL//Aux

#define     V1742B_MAX_EVENT_BLT                0xEF1CUL//A32/D32 RW C
#define     V1742B_MAX_EVENT_BLT_VAL_MSK        0x03FFUL//Aux

#define     V1742B_SCRATCH                      0xEF20UL//A32/D32 RW C

#define     V1742B_SOFTWARE_RST                 0xEF24UL//A32/D32 W C

#define     V1742B_SOFTWARE_CLR                 0xEF28UL//A32/D32 W C

#define     V1742B_LUB                          0xF08BUL//Aux, The Last Used Byte

/****** Event Description ******/
// Header
#define     V1742B_HDR_EVENT_SIZE_MSK           0x0FFFFFFFUL//Aux
#define     V1742B_HDR_BOARD_ID_MSK             0xF8000000UL//Aux
#define     V1742B_HDR_BOARD_ID_SHFT            0x1BU//Aux
#define     V1742B_HDR_BOARD_FAIL_MSK           0x04000000UL//Aux
#define     V1742B_HDR_LVDS_MSK                 0x00FFFF00UL//Aux
#define     V1742B_HDR_LVDS_SHFT                0x8U//Aux
#define     V1742B_HDR_GROUP_MSK                0x0000000FUL//Aux
#define     V1742B_HDR_EVENT_CNT_MSK            0x00FFFFFFUL//Aux
#define     V1742B_HDR_EVENT_TTT_MSK            0xFFFFFFFFUL//Aux

// Group Description word
#define     V1742B_GRP_START_IDX_MSK            0x3FF00000UL//Aux
#define     V1742B_GRP_START_IDX_SHFT           0x14U//Aux
#define     V1742B_GRP_SAMP_MSK                 0x00030000UL//Aux
#define     V1742B_GRP_SAMP_SHFT                0x10U//Aux
#define     V1742B_GRP_TR_MSK                   0x00001000UL//Aux
#define     V1742B_GRP_CH_MSK                   0x00000FFFUL//Aux
#define     V1742B_GRP_TTT_MSK                  0x3FFFFFFFUL//Aux

// Filler
#define     V1742B_WORD_TYPE_FILLER             0xFFFFFFFFUL//Aux

#include "VSlaveInterrupter.h"
#include "VSlaveAcquisitor.h"
#include "VEvent.h"
#include "UConfigurable.h"
#include "UParser.h"

#include "CAENDigitizerType.h"

#include <vector>
#include <array>


namespace vmepp
{
    class V1742B : public VSlaveInterrupter, public VSlaveAcquisitor, public UConfigurable<V1742B>
    {
        protected :
            static uint8_t constexpr fChNumber      = 0x20U;                    // 32 
            static uint8_t constexpr fGroupNumber   = 0x04U;                    // 4
            static uint8_t constexpr fChInGroup     = fChNumber / fGroupNumber; // 8
            static size_t  constexpr fNSamples      = 0x0400U;                  // 1024

        public :
            static uint8_t constexpr GetChNumber()      { return fChNumber; }
            static uint8_t constexpr GetGroupNumber()   { return fGroupNumber; }
            static uint8_t constexpr GetChInGroup()     { return fChInGroup; }
            static size_t  constexpr GetNSamples()      { return fNSamples; }

        public :
            enum class SamplingRate_t : uint8_t
            {
                M5000 = CAEN_DGTZ_DRS4_5GHz,
                M2500 = CAEN_DGTZ_DRS4_2_5GHz,
                M1000 = CAEN_DGTZ_DRS4_1GHz,
                M750,
            };



            //**************************
            //****** CORRECTION + ******
            //**************************
        public :
            // Bad
            struct ChannelCorrection
            {
                std::array<int16_t, fNSamples> cell;
                std::array<int8_t, fNSamples>  sample;

                friend std::ostream& operator<<( std::ostream& lhs, const ChannelCorrection& rhs )
                {
                    WriteStdArrayToStream( rhs.cell, lhs );
                    WriteStdArrayToStream( rhs.sample, lhs );
                    return lhs;
                }

                friend std::istream& operator>>( std::istream& lhs, ChannelCorrection& rhs )
                {
                    ReadStdArrayFromStream( rhs.cell, lhs );
                    ReadStdArrayFromStream( rhs.sample, lhs );
                    return lhs;
                }
            };

            struct GroupCorrection
            {
                std::array<ChannelCorrection, fChInGroup>       channels;
                ChannelCorrection                               trN;
                std::array<float, fNSamples>                    time;

                friend std::ostream& operator<<( std::ostream& lhs, const GroupCorrection& rhs )
                {
                    for( auto& c : rhs.channels ) { lhs << c; }
                    lhs << rhs.trN;
                    WriteStdArrayToStream( rhs.time, lhs );

                    return lhs;
                }

                friend std::istream& operator>>( std::istream& lhs, GroupCorrection& rhs )
                {
                    for( auto& c : rhs.channels ) { lhs >> c; }
                    lhs >> rhs.trN;
                    ReadStdArrayFromStream( rhs.time, lhs );

                    return lhs;
                }
            };

            struct CorrectionTable
            {
                SamplingRate_t                                  freq; 
                std::array<GroupCorrection, fGroupNumber>       table;

                friend std::ostream& operator<<( std::ostream& lhs, const CorrectionTable& rhs )
                {
                    lhs.write( reinterpret_cast<char const*>( &(rhs.freq) ), sizeof( (rhs.freq) ) );
                    for( auto& g : rhs.table ) { lhs << g; }
                    return lhs;
                }

                friend std::istream& operator>>( std::istream& lhs, CorrectionTable& rhs )
                {
                    lhs.read( reinterpret_cast<char*>( &(rhs.freq) ), sizeof( (rhs.freq) );
                    for( auto& g : rhs.table ) { lhs >> g; }
                    return lhs;
                }
            };

            enum class Group_t : uint8_t { G1 = 0, G2, G3, G4, All };

        protected :
            CorrectionTable fCorrectionTable;

        public :
            void LoadCorrectionTable( const std::string& fileName );
            const CorrectionTable& GetCorrectionTable();
            //**************************
            //****** CORRECTION - ******
            //**************************

        public :
            enum class RecordLength_t : uint8_t
            {
                s1024 = 0x00,
                s520  = 0x01,
                s256  = 0x02,
                s136  = 0x03,
            };

            enum class FanSpeed_t : uint8_t
            {
                LowAuto = 0x00,
                High    = 0x01,
            };


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

            enum class GlobalTrigger_t : uint8_t
            {
                None            = 0x00,
                ExternalOnly    = 0x01,
                SWOnly          = 0x02,
                All             = 0x03,
            };

            struct BoardInfo
            {
                uint8_t FAMILY_CODE;
                uint8_t CHANNEL_MEM_SIZE;
                uint8_t GROUP_NUMBER;

                BoardInfo( uint8_t code, uint8_t memSize, uint8_t groupN ) :
                    FAMILY_CODE( code ),
                    CHANNEL_MEM_SIZE( memSize ),
                    GROUP_NUMBER( groupN )
                { }
            };

            struct AMCFirmwareRev
            {
                uint8_t     MINOR;
                uint8_t     MAJOR;
                uint16_t    DATE;

                AMCFirmwareRev( uint8_t min, uint8_t maj, uint16_t date ) :
                    MINOR( min ),
                    MAJOR( maj ),
                    DATE( date )
                { }
            };

            struct ROCFirmwareRev
            {
                uint8_t     MINOR;
                uint8_t     MAJOR;
                uint16_t    DATE;

                ROCFirmwareRev( uint8_t min, uint8_t maj, uint16_t date ) :
                    MINOR( min ),
                    MAJOR( maj ),
                    DATE( date )
                { }
            };

            enum class TriggerPolarity_t : uint8_t
            {
                RisingEdge = 0x00,
                FallingEdge = 0x01,
            };

            enum class TriggerIn_t : uint8_t
            {
                Gate = 0x00,
                Veto = 0x01,
            };

            enum class TriggerOut_t : uint8_t
            {
                NoSignal    = 0x00,
                AllTRn      = 0x01,
                AcceptedTRn = 0x02,
                BusyGroups  = 0x03,
            };

            enum class Level_t : uint8_t
            {
                NIM = 0x00,
                TTL = 0x01,
            };

            enum class AcqMode_t : uint8_t
            {
                Output      = 0x00,
                Transparent = 0x01,
            };

            enum class StartSource_t : uint8_t
            {
                SW          = 0x00,
                S_IN        = 0x01,
                FirstTrg    = 0x02,
                LVDS        = 0x03,
            };

            enum class AcqStatusBit : uint32_t
            {
                AcqRun      = 0x00000004,
                EventReady  = 0x00000008,
                EventFull   = 0x00000010,
                PLLClockExt = 0x00000020,
                PLLGood     = 0x00000080,
                BoardReady  = 0x00000100,
                StatusSIN   = 0x00008000,
                StatusTRGIN = 0x00010000,
                All         = 0x000181BC,
            };

        protected :
            virtual void    Initialize() override;
            void            WriteSWReset();
            void            WriteSWClear();

        public :
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

            void WriteLVDS( uint16_t mask );
            uint16_t ReadLVDS();

            void WriteTestModeEnable( bool enable = true );
            bool ReadTestModeEnable();

        public :
            // Info & ROM
            BoardInfo ReadBoardInfo();

            bool ReadPLLFailure();

            AMCFirmwareRev ReadAMCFirmwareRev( Group_t group );
            ROCFirmwareRev ReadROCFirmwareRev();

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

            uint32_t        ReadBoardConfiguration();

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

            void WriteVetoDelay( uint16_t value );
            uint16_t ReadVetoDelay();

            void WriteTRPolarity( TriggerPolarity_t pol );
            TriggerPolarity_t ReadTRPolarity();

            void WriteTRDigitize( bool enable = true );
            bool ReadTRDigitize();

            void WriteTREnable( bool enable = true );
            bool ReadTREnable();

            void WriteTRGINEnable( bool enable = true );
            bool ReadTRGINEnable();

            void WriteTRGINSignal( TriggerIn_t trigger );
            TriggerIn_t ReadTRGINSignal();

            void WriteTRGOUTSignal( TriggerOut_t trigger );
            TriggerOut_t ReadTRGOUTSignal();

        public :
            // Acquisition
            uint32_t GetBufferAddress() const override { return V1742B_OUTPUT_BUFFER_START; };

            void WriteGroupEnable( Group_t g, bool enable = true );

            void WriteChannelOffset( uint8_t ch, uint16_t offset );
            uint16_t ReadChannelOffset( uint8_t ch );

            void WriteRecordLength( RecordLength_t length );
            RecordLength_t ReadRecordLength();

            void WriteInitTestValue( uint16_t value );
            uint16_t ReadInitTestValue( Group_t group = Group_t::G1 );

            void WriteSamplingRate( SamplingRate_t rate );
            SamplingRate_t ReadSamplingRate( Group_t group = Group_t::G1 );

            void WriteSWTrigger();

            void WriteGlobalTrigger( GlobalTrigger_t trigger );
            GlobalTrigger_t ReadGlobalTrigger();

            uint32_t ReadEventSize();

            void WriteScratch( uint32_t value );
            uint32_t ReadScratch();

            void WriteMaxEventBLT( uint16_t n );
            uint16_t ReadMaxEventBLT();

            void WriteAcqMode( AcqMode_t mode );
            AcqMode_t ReadAcqMode();

            void WriteStartSource( StartSource_t source );
            StartSource_t ReadStartSource();

            void WriteStartStop( bool start );

            uint32_t ReadAcqStatus();
            bool ReadAcqStatus( AcqStatusBit bit );

        public :
            // Front Panel
            void WriteLEMOLevel( Level_t level );
            Level_t ReadLEMOLevel();

        public :
            // Config
            void    ReadConfig( UConfig<V1742B>& config ) override;
            void    WriteConfig( const UConfig<V1742B>& config ) override;
    };

    template<>
    const std::string UConfigurable<V1742B>::fName;

    template <>
    class UConfig<V1742B>
    {
    };


    //*****************************
    //****** UEVENT<V1742B>  ******
    //*****************************
    template<>
    class UEvent<V1742B> : public VEvent
    {
        private :
            struct Helper12Bit 
            {
                bool        overlap;
                uint8_t     s1;
                uint8_t     s2;
                uint32_t    m1;
                uint32_t    m2;
                size_t      n;
            
                constexpr
                Helper12Bit( bool overlap,
                             uint8_t s1, uint8_t s2,
                             uint32_t m1, uint32_t m2,
                             size_t n ) :
                    overlap( overlap ),
                    s1( s1 ), s2( s2 ),
                    m1( m1 ), m2( m2 ),
                    n( n )
                { }
            };

            static constexpr uint8_t fNHelpers = 8;
            static const std::array<Helper12Bit, fNHelpers> fH;

            // Helper functions
            static uint16_t Read12BitWord( const VBuffer& b, size_t ib, size_t n )
            {
                return (                 (b[ib + fH[n].n]     & fH[n].m1) >> fH[n].s1) |
                       (fH[n].overlap ? ((b[ib + fH[n].n + 1] & fH[n].m2) << fH[n].s2) : 0);
            }

        public :
            typedef std::vector<uint16_t> Waveform; 

            struct Group
            {
                protected :
                    uint32_t                                        fHeader;
                    uint32_t                                        fTTT;
                    std::array<Waveform, V1742B::GetChInGroup()>    fData;
                    Waveform                                        fTR;

                public :
                    // Auxiliary functions for retreiving info from the group description word
                    uint16_t                GetStartIndex() const
                    {
                        return ((fHeader & V1742B_GRP_START_IDX_MSK) >> V1742B_GRP_START_IDX_SHFT);
                    };

                    V1742B::SamplingRate_t  GetSamplingRate() const
                    {
                        return static_cast<V1742B::SamplingRate_t>((fHeader & V1742B_GRP_SAMP_MSK) >> V1742B_GRP_SAMP_SHFT);
                    }

                    bool                    GetTRPresent() const { return (fHeader & V1742B_GRP_TR_MSK); }
                    uint16_t                GetChannelSize() const { return (fHeader & V1742B_GRP_CH_MSK); }
                    uint32_t                GetTTT() const { return (fTTT & V1742B_GRP_TTT_MSK); }

                    void                    Clear();

                    void                    Print() const;

                    const Waveform&         GetTR() const { return fTR; }
                    const Waveform&         GetChannel( uint8_t ch ) const { return fData.at( ch ); }

                friend class UEvent<V1742B>;
            };

        protected :
            std::array<DataWord_t, 4>                   fHeader;
            std::array<Group, V1742B::GetGroupNumber()> fData;

        public :
            bool Fill( size_t index, const VBuffer &buffer) override;
            void Print() const;
            const Group& GetGroup( V1742B::Group_t g ) const;

        public :
            size_t GetEventSize() const     { return fHeader[0] & V1742B_HDR_EVENT_SIZE_MSK; }
            uint8_t GetBoardID() const      { return (fHeader[1] & V1742B_HDR_BOARD_ID_MSK) >> V1742B_HDR_BOARD_ID_SHFT; }
            bool GetBoardFail() const       { return fHeader[1] & V1742B_HDR_BOARD_FAIL_MSK; }
            uint16_t GetLVDSPattern() const { return (fHeader[1] & V1742B_HDR_LVDS_MSK) >> V1742B_HDR_LVDS_SHFT; }
            uint8_t GetGroupMask() const    { return (fHeader[1] & V1742B_HDR_GROUP_MSK); }
            size_t GetEventCounter() const  { return (fHeader[2] & V1742B_HDR_EVENT_CNT_MSK); } 
            uint32_t GetEventTTT() const    { return (fHeader[3] & V1742B_HDR_EVENT_TTT_MSK); }
    };
}

#endif
