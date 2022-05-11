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
#define     V1742B_OUTPUT_BUFFER_STOP           0x0FFFUL//Aux, This is the last byte occupied by buffer
#define     V1742B_OUTPUT_BUFFER_SIZE           (V1742B_OUTPUT_BUFFER_STOP - V1742B_OUTPUT_BUFFER_START + 1UL)//Aux

#define     V1742B_N_GROUPS                     0x4U//Aux

#define     V1742B_POST_TRIGGER                 0x1014UL//A32/D32 RW G
#define     V1742B_POST_TRIGGER_VAL_MSK         0x03FFUL//Aux

#define     V1742B_DUMMY32                      0x1024UL//A32/D32 RW G

#define     V1742B_CHANNEL_THR                  0x1080UL//A32/D32 RW G
#define     V1742B_CHANNEL_THR_VAL_MSK          0x0FFFUL//Aux 
#define     V1742B_CHANNEL_THR_IDX_MSK          0xF000UL//Aux 
#define     V1742B_CHANNEL_THR_IDX_SHFT         0x000CU//Aux

#define     V1742B_STATUS                       0x1088UL//A32/D32 R G

#define     V1742B_AMC_FRMW_REV                 0x108CUL//A32/D32 R G
#define     V1742B_AMC_FRMW_REV_MIN_MSK         0x000000FFUL//Aux
#define     V1742B_AMC_FRMW_REV_MIN_SHFT        0x00U//Aux
#define     V1742B_AMC_FRMW_REV_MAJ_MSK         0x0000FF00UL//Aux
#define     V1742B_AMC_FRMW_REV_MAJ_SHFT        0x08U//Aux
#define     V1742B_AMC_FRMW_REV_DATE_MSK        0xFFFF0000UL//Aux
#define     V1742B_AMC_FRMW_REV_DATE_SHFT       0x10U//Aux

#define     V1742B_CHANNEL_DC_OFST              0x1098UL//A32/D32 RW G
#define     V1742B_CHANNEL_DC_OFST_VAL_MSK      0x0000FFFFUL//Aux
#define     V1742B_CHANNEL_DC_OFST_IDX_MSK      0x000F0000UL//Aux
#define     V1742B_CHANNEL_DC_OFST_IDX_SHFT     0x0010U//Aux

#define     V1742B_CHIP_TEMP                    0x10A0UL//A32/D32 R G
#define     V1742B_CHIP_TEMP_VAL_MSK            0x00FFUL//Aux

#define     V1742B_CHANNEL_SEL                  0x10A4UL//A32/D32 RW G
#define     V1742B_CHANNEL_SEL_VAL_MSK          0x000FUL//Aux

#define     V1742B_CHANNEL_TRG_MASK             0x10A8UL//A32/D32 RW G
#define     V1742B_CHANNEL_TRG_MASK_MSK         0x00FFUL//Aux

//WARNING: before writing this register, it is necessary to check that bit[2] = 0 at 0x1X88,
//otherwise the writing process will not run properly!
#define     V1742B_TR_TRG_THR                   0x10D4UL//A32/D32 RW G
#define     V1742B_TR_TRG_THR_VAL_MSK           0xFFFFUL//Aux

#define     V1742B_TR_DC_OFST                   0x10DCUL//A32/D32 RW G
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

#define     V1742B_TEST_MODE_INIT_READ          0x107CUL//A32/D32 R G 
#define     V1742B_TEST_MODE_INIT_WRITE         0x807CUL//A32/D32 W C
#define     V1742B_TEST_MODE_INIT_VAL_MSK       0x0FFFUL//Aux

//WARNING: This register must not be written while the acquisition is running.
#define     V1742B_DRS4_SAMP_FREQ_READ          0x10D8UL//A32/D32 R G
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

/****** ROM ******/
#define     V1742B_ROM_BOARD_VERSION            0xF030//A32/D32 R C

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
    /**
     * This class represents the V1742B module.
     * V1742B is a 32+2 channel 12 bit 5 GS/s Digitizer 
     */
    class V1742B : public VSlaveInterrupter, public VSlaveAcquisitor, public UConfigurable<V1742B>
    {
        protected :
            static uint8_t constexpr fChNumber      = 0x20U;                    /*!< number of channels (32) */
            static uint8_t constexpr fGroupNumber   = 0x04U;                    /*!< number of groups   (4) */
            static uint8_t constexpr fChInGroup     = fChNumber / fGroupNumber; /*!< number of channels per group (8) */
            static size_t  constexpr fNSamples      = 0x0400U;                  /*!< number of cells (samples) (1024) */

        public :
            /**
             * Get number of channels
             */
            static uint8_t constexpr GetChNumber()      { return fChNumber; }

            /**
             * Get number of groups
             */
            static uint8_t constexpr GetGroupNumber()   { return fGroupNumber; }

            /**
             * Get number of channels in one group (= number of channels / number of groups)
             */
            static uint8_t constexpr GetChInGroup()     { return fChInGroup; }

            /**
             * Get number of samples per channel.
             * NOTE: it always returns the maximum number of samples
             * despite the value set via the WriteRecordLength member function 
             */
            static size_t  constexpr GetNSamples()      { return fNSamples; }

        public :

            /**
             * Sampling rate
             */
            enum class SamplingRate_t : uint8_t
            {
                M5000 = CAEN_DGTZ_DRS4_5GHz,    /*!< 5   GS/sec, sample = 200 ps */
                M2500 = CAEN_DGTZ_DRS4_2_5GHz,  /*!< 2.5 GS/sec, sample = 400 ps */
                M1000 = CAEN_DGTZ_DRS4_1GHz,    /*!< 1   GS/sec, sample = 1 ns */
                M750,                           /*!< 750 MS/sec, sample = 1.33 ns */
            };


            //**************************
            //****** CORRECTION + ******
            //**************************
        public :
            /**
             * Channel specific data correction
             */
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

            /**
             * Group specific data correction
             */
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

            /**
             * Correction table
             */
            struct CorrectionTable
            {
                SamplingRate_t                                  freq;   /*< Sampling frequency */
                std::array<GroupCorrection, fGroupNumber>       table;  /*< Correction table */

                friend std::ostream& operator<<( std::ostream& lhs, const CorrectionTable& rhs )
                {
                    lhs.write( reinterpret_cast<char const*>( &(rhs.freq) ), sizeof( rhs.freq ) );
                    for( auto& g : rhs.table ) { lhs << g; }
                    return lhs;
                }

                friend std::istream& operator>>( std::istream& lhs, CorrectionTable& rhs )
                {
                    lhs.read( reinterpret_cast<char*>( &(rhs.freq) ), sizeof( rhs.freq ) );
                    for( auto& g : rhs.table ) { lhs >> g; }
                    return lhs;
                }
            };

            /**
             * Channel group
             */
            enum class Group_t : uint8_t
            {
                G0 = 0, /*!< Group 0, channels 0:7 */
                G1,     /*!< Group 1, channels 8:15 */
                G2,     /*!< Group 2, channels 16:23 */
                G3      /*!< Group 3, channels 24:31 */
            };

        protected :
            CorrectionTable fCorrectionTable;

            /**
             * Read correction table from a file.
             */
            void LoadCorrectionTable( const std::string& fileName );

        public :
            /**
             * Get current correction table. I.e. the last one loaded
             */
            const CorrectionTable& GetCorrectionTable();
            //**************************
            //****** CORRECTION - ******
            //**************************

        public :
            /**
             * Custom record length
             */
            enum class RecordLength_t : uint8_t
            {
                s1024 = 0x00, /*!< Record length is 1024 samples */
                s520  = 0x01, /*!< Record length is 520 samples */
                s256  = 0x02, /*!< Record length is 256 samples */
                s136  = 0x03, /*!< Record length is 136 samples */
            };

            /**
             * Fan speed. NOT USED in this module
             */
            enum class FanSpeed_t : uint8_t
            {
                LowAuto = 0x00, /*<! NOT USED in this module */
                High    = 0x01, /*<! NOT USED in this module */
            };

            /**
             * TRn channel
             */
            enum class TR_t : uint8_t
            {
                TR0 = 0, /*!< TR0 input connector on the front panel */
                TR1 = 1, /*!< TR0 input connector on the front panel */
            };

            /**
             * Bits of the `status` register
             */
            enum StatusBit : uint32_t
            {
                MemoryFull      = 0x0001, /*!< Memory full */
                MemoryEmpty     = 0x0002, /*!< Memory empty */
                BusySPI         = 0x0004, /*!< SPI bus is busy */
                LockedPLLEven   = 0x0040, /*!< The DRS4 PLL of the even group is locked */
                LockedPLLOdd    = 0x0080, /*!< The DRS4 PLL of the odd group is locked */
                BusyDRS4        = 0x0100, /*!< The DRS4 chip is busy */
                MezzRevision    = 0x0200, /*!< Mezzanine revision, (if set than revision is higher than or equal to 1) */
                All             = 0x03C7, /*!< All the above as a mask */
            };

            /**
             * Global trigger types
             */
            enum class GlobalTrigger_t : uint8_t
            {
                None            = 0x00, /*!< Global trigger disabled */
                ExternalOnly    = 0x01, /*!< Only the trigger on TRG-IN participates in the global trigger generation */
                SWOnly          = 0x02, /*!< Only the software trigger participates in the global trigger generation */
                All             = 0x03, /*!< Both: TRG-IN and software trigger are enabled */
            };

            /**
             * Board information
             */
            struct BoardInfo
            {
                uint8_t FAMILY_CODE;        /*!< Family code */
                uint8_t CHANNEL_MEM_SIZE;   /*!< Number of samples per channel */
                uint8_t GROUP_NUMBER;       /*!< Number of groups */

                BoardInfo( uint8_t code, uint8_t memSize, uint8_t groupN ) :
                    FAMILY_CODE( code ),
                    CHANNEL_MEM_SIZE( memSize ),
                    GROUP_NUMBER( groupN )
                { }
            };

            /**
             * Channel FPGA (AMC) firmware revision information
             */
            struct AMCFirmwareRev
            {
                uint8_t     MINOR;  /*!< AMC firmware minor revision number */
                uint8_t     MAJOR;  /*!< AMC firmware major revision number */
                uint16_t    DATE;   /*!< AMC firmware revision date (Y/M/DD) */

                AMCFirmwareRev( uint8_t min, uint8_t maj, uint16_t date ) :
                    MINOR( min ),
                    MAJOR( maj ),
                    DATE( date )
                { }
            };

            /**
             * Motherboard FPGA (ROC) firmware revision
             */
            struct ROCFirmwareRev
            {
                uint8_t     MINOR;  /*!< ROC firmware minor revision number */
                uint8_t     MAJOR;  /*!< ROC firmware major revision number */
                uint16_t    DATE;   /*!< ROC firmware revision date (Y/M/DD) */

                ROCFirmwareRev( uint8_t min, uint8_t maj, uint16_t date ) :
                    MINOR( min ),
                    MAJOR( maj ),
                    DATE( date )
                { }
            };

            /**
             * TRn trigger polarity
             */
            enum class TriggerPolarity_t : uint8_t
            {
                RisingEdge = 0x00,  /*!< Trigger on a rising edge */
                FallingEdge = 0x01, /*!< Trigger on a falling edge */
            };

            /**
             * Signal on TRG-IN connector is used either to gate or veto the acquisition
             */
            enum class TriggerIn_t : uint8_t
            {
                Gate = 0x00, /*!< Gate the acquisition */
                Veto = 0x01, /*!< Veto the acquisition */
            };

            enum class TriggerOut_t : uint8_t
            {
                NoSignal    = 0x00,
                AllTRn      = 0x01,
                AcceptedTRn = 0x02,
                BusyGroups  = 0x03,
            };

            /**
             * Logic level of the front panel LEMO connectors
             */
            enum class Level_t : uint8_t
            {
                NIM = 0x00, /*!< NIM Logic */
                TTL = 0x01, /*!< TTL Logic */
            };

            /**
             * Acquisition modes
             */
            enum class AcqMode_t : uint8_t
            {
                Output      = 0x00, /*!< Output mode */
                Transparent = 0x01, /*!< Transparent mode */
            };

            /**
             * Start source
             */
            enum class StartSource_t : uint8_t
            {
                SW          = 0x00, /*!< Start/Stop of a run is SW controlled */
                S_IN        = 0x01, /*!< Start/Stop of a run takes place on the active/inactive S-IN */
                FirstTrg    = 0x02, /*!< A run starts on the first trigger pulse (TRG-IN). The Stop of a run must be SW controlled */
                LVDS        = 0x03, /*!< LVDS controlled */
            };

            /**
             * Bits of the `Acquisition Status` register
             */
            enum class AcqStatusBit : uint32_t
            {
                AcqRun      = 0x00000004,   /*!< Acquisition is running */
                EventReady  = 0x00000008,   /*!< At least one event is available */
                EventFull   = 0x00000010,   /*!< Maximum number of storable events has been reached */
                PLLClockExt = 0x00000020,   /*!< External clock source is used by PLL */
                PLLGood     = 0x00000080,   /*!< PLL is locked */
                BoardReady  = 0x00000100,   /*!< Board ready for acquisition */
                StatusSIN   = 0x00008000,   /*!< The current logical level on the `S-IN` front panel connector*/
                StatusTRGIN = 0x00010000,   /*!< The current logical level on the `TRG-IN` front panel connector*/
                All         = 0x000181BC,   /*!< All the above as a mask */
            };

        protected :
            virtual void    Initialize() override;
            void            WriteSWReset();
            void            WriteSWClear();

            static uint32_t constexpr GroupAddress( uint32_t base )
            {
                return ((base & 0x00FF) | 0x8000);
            }

            static uint32_t constexpr GroupAddress( uint32_t base, Group_t g )
            {
                return (base | (0x0100 * static_cast<uint32_t>(g)));
            };

        protected :
            void            WaitForSPI( Group_t group );

        public :
            V1742B( uint32_t baseAddress, uint32_t range = V1742B_LUB );
            ~V1742B() override = default;

        public :
            // Misc
            virtual void    Reset() override;

            /**
             * Write a 32-bit word to a board register. Can be used for debug purposes to test
             * the local bus. NOTE: a write for group 0 and group 1 leads to the same settings,
             * as for group 2 and group 3.
             * @param group a group to write to
             * @param word a word to write
             * @see WriteDummy32( uint32_t word )
             * @see ReadDummy32( Group_t group )
             */
            void            WriteDummy32( Group_t group, uint32_t word );

            /**
             * Write a 32-bit word to a board register. Can be used for debug purposes to test
             * the local bus.
             * @param word a word to write to all the groups at once
             * @see WriteDummy32( Group_t group, uint32_t word )
             * @see ReadDummy32( Group_t group )
             */
            void            WriteDummy32( uint32_t word );

            /**
             * Read a 32-bit word from a board register. Can be used for debug purposes to test
             * the local bus.
             * @param group group to read from 
             * @see WriteDummy32( Group_t group, uint32_t word )
             * @see WriteDummy32( uint32_t word )
             */
            uint32_t        ReadDummy32( Group_t group );

            /**
             * Read DRS4 chip temperature
             * @param group chip number which temperature is to be read 
             * @return the temperature of the chip specified with the `group` parameter : from -64C to 127C
             */
            int16_t         ReadChipTemperature( Group_t group );

            void WriteLVDS( uint16_t mask );
            uint16_t ReadLVDS();

            void WriteTestModeEnable( bool enable = true );
            bool ReadTestModeEnable();

            void ApplyCorrection( UEvent<V1742B>& event ) const;

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
            void WritePostTrigger( uint16_t n );
            uint16_t ReadPostTrigger( Group_t group );

            /**
             * Set the trigger threshold of a channel.
             * NOTE : threshold value must be set in units of absolute scale
             * (NOT relative to the baseline).
             * @param ch channel index
             * @param threshold threshold value in LSB counts
             * @see GetChNumber()
             * @see ReadChannelThreshold( uint8_t ch )
             */
            void WriteChannelThreshold( uint8_t ch, uint16_t threshold );

            /**
             * Get the trigger threshold of a channel
             * @param ch channel index
             * @return threshold value in LSB counts
             * @see GetChNumber()
             * @see WriteChannelThreshold( uint8_t ch, uint16_t threshold )
             */
            uint16_t ReadChannelThreshold( uint8_t ch );

            void WriteEnableTrigger( uint8_t ch, bool enable );
            bool ReadEnableTriggerC( uint8_t ch );
            void WriteEnableTrigger( Group_t group, uint8_t mask );
            void WriteEnableTrigger( uint8_t mask );
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

            void WriteGlobalTrigger( GlobalTrigger_t trigger );
            GlobalTrigger_t ReadGlobalTrigger();

        public :
            // Acquisition
            virtual size_t HelperReadCycles() override;

            uint32_t GetBufferAddress() const override { return V1742B_OUTPUT_BUFFER_START; };

            void WriteGroupEnable( Group_t g, bool enable = true );
            void WriteGroupEnable( bool enable );

            void WriteChannelOffset( uint8_t ch, uint16_t offset );
            uint16_t ReadChannelOffset( uint8_t ch );

            void WriteRecordLength( RecordLength_t length );
            RecordLength_t ReadRecordLength();

            void WriteInitTestValue( uint16_t value );
            uint16_t ReadInitTestValue( Group_t group );

            void WriteSamplingRate( SamplingRate_t rate );
            SamplingRate_t ReadSamplingRate( Group_t group );

            void WriteSWTrigger();

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
            /**
             * Type synonym for the waveform data
             */
            typedef std::vector<uint16_t> Waveform;

            /**
             * Group data of an event of the V1742B module
             */
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
                friend void V1742B::ApplyCorrection( UEvent<V1742B>& event ) const;
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

            friend void V1742B::ApplyCorrection( UEvent<V1742B>& event ) const;
    };
}

#endif
