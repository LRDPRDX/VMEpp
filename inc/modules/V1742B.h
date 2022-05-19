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
#include <map>


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
             * Get the number of samples per channel.
             * _NOTE:_ it always returns the maximum number of samples
             * despite the value set via the WriteRecordLength(RecordLength_t length) member function 
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

            typedef EnumIterator<SamplingRate_t, SamplingRate_t::M5000, SamplingRate_t::M750> RateIterator;

            friend std::ostream& operator<<( std::ostream& lhs, const SamplingRate_t rhs )
            {
                std::string s;
                switch( rhs )
                {
                    case( SamplingRate_t::M5000 ) : s = "5GHz"; break;
                    case( SamplingRate_t::M2500 ) : s = "2_5GHz"; break;
                    case( SamplingRate_t::M1000 ) : s = "1GHz"; break;
                    case( SamplingRate_t::M750 )  : s = "750MHz"; break;
                }
                return lhs << s;
            }

            //**************************
            //****** CORRECTION + ******
            //**************************
        public :
            /**
             * Channel-specific data correction
             */
            struct ChannelCorrection
            {
                std::array<int16_t, fNSamples> cell;
                std::array<int8_t, fNSamples>  sample;

                friend std::ostream& operator<<( std::ostream& lhs, const ChannelCorrection& rhs )
                {
                    WriteStdArrayToBinaryStream( rhs.cell, lhs );
                    WriteStdArrayToBinaryStream( rhs.sample, lhs );
                    return lhs;
                }

                friend std::istream& operator>>( std::istream& lhs, ChannelCorrection& rhs )
                {
                    ReadStdArrayFromBinaryStream( rhs.cell, lhs );
                    ReadStdArrayFromBinaryStream( rhs.sample, lhs );
                    return lhs;
                }
            };

            /**
             * Group-specific data correction
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
                    WriteStdArrayToBinaryStream( rhs.time, lhs );
                    return lhs;
                }

                friend std::istream& operator>>( std::istream& lhs, GroupCorrection& rhs )
                {
                    for( auto& c : rhs.channels ) { lhs >> c; }
                    lhs >> rhs.trN;
                    ReadStdArrayFromBinaryStream( rhs.time, lhs );
                    return lhs;
                }
            };

            /**
             * Frequency-specific correction
             */
            struct FreqCorrection
            {
                SamplingRate_t                                  freq;   /*< Sampling frequency */
                std::array<GroupCorrection, fGroupNumber>       table;  /*< Correction table */


                friend std::ostream& operator<<( std::ostream& lhs, const FreqCorrection& rhs )
                {
                    lhs.write( reinterpret_cast<char const*>( &(rhs.freq) ), sizeof( rhs.freq ) );
                    for( auto& g : rhs.table ) { lhs << g; }
                    return lhs;
                }

                friend std::istream& operator>>( std::istream& lhs, FreqCorrection& rhs )
                {
                    lhs.read( reinterpret_cast<char*>( &(rhs.freq) ), sizeof( rhs.freq ) );
                    for( auto& g : rhs.table ) { lhs >> g; }
                    return lhs;
                }
            };

            /**
             * Type synonym for the digitizer's correction tables
             */
            typedef std::map<SamplingRate_t, FreqCorrection> CorrectionTable;

            /**
             * Channel group, DRS4 chip index
             */
            enum class Group_t : uint8_t
            {
                G0,     /*!< Group 0, channels 0:7 */
                G1,     /*!< Group 1, channels 8:15 */
                G2,     /*!< Group 2, channels 16:23 */
                G3      /*!< Group 3, channels 24:31 */
            };

            typedef EnumIterator<Group_t, Group_t::G0, Group_t::G3> GroupIterator;


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
             * Bits of the `status` register.
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
             * Bits of the `error` register.
             */
            enum ErrorBit : uint32_t
            {
                LossLockPLL = 0x0010, /*!< PLL Lock Loss occurred */
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

                BoardInfo( uint8_t code = 0, uint8_t memSize = 0, uint8_t groupN = 0 ) :
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

                AMCFirmwareRev( uint8_t min = 0, uint8_t maj = 0, uint16_t date = 0 ) :
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

                ROCFirmwareRev( uint8_t min = 0, uint8_t maj = 0, uint16_t date = 0 ) :
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
             * @see WriteAcqMode( AcqMode_t mode )
             * @see ReadAcqMode()
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
            std::string                                 fPathToCorrectionTable;
            CorrectionTable                             fCorrectionTable;

            std::array<AMCFirmwareRev, fGroupNumber>    fAMCFirmware;
            ROCFirmwareRev                              fROCFirmware;
            BoardInfo                                   fBoardInfo;

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
            AMCFirmwareRev  ReadAMCFirmwareRev( Group_t group );
            ROCFirmwareRev  ReadROCFirmwareRev();

            /**
             * Read the specific information of the board, such as digitizer family, the
             * channel memory size and the channel density.
             * @return board info
             * @see BoardInfo
             */
            BoardInfo ReadBoardInfo();



        public :
            /**
             * A constructor.
             * @param baseAddress VME base address of the board. MUST match the one set
             * with the address rotary switches (see the manual to the board). 
             * @param range occupied size of the VME address space by the board.
             */
            V1742B( uint32_t baseAddress, uint32_t range = V1742B_LUB );
            ~V1742B() override = default;

        public :
            /** @name Misc
             * Miscellaneous member functions
             * @{
             */
            virtual void    Reset() override;

            /**
             * Write a 32-bit word to a board register. Can be used for debug purposes to test
             * the local bus. _NOTE:_ a write for group 0 and group 1 leads to the same settings,
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

            /**
             * Enable/disable the test mode. When the test mode is enabled, the input
             * samples are replaced by a sawtooth test signal.
             * @param enable enable status (true = enable, false = disable)
             * @see ReadTestModeEnable()
             */
            void WriteTestModeEnable( bool enable = true );

            /**
             * Get the enable status of the test mode. When the test mode is enabled, the input
             * samples are replaced by a sawtooth test signal.
             * @return enable status (true = enabled, false = disabled)
             * @see WriteTestModeEnable( bool enable )
             */
            bool ReadTestModeEnable();
             /** @} */

        public :
            /** @name Interrupts
             * The part of the interface with the device as with an _INTERRUPTER_ functional module.
             * @{
             */

            /** Set the number of events that causes an interrupt request.
             * If interrupts are enabled, the module generates a request
             * whenever it has stored in memory a number of events > `N`.
             * @param N interrupt event number
             * @see ReadIRQEvents()
             * @see WriteIRQLevel( uint16_t level )
             */
            void            WriteIRQEvents( uint16_t N );

            /** Get the number of events that causes an interrupt request.
             * If interrupts are enabled, the module generates a request
             * whenever it has stored in memory a number of events > `N`.
             * @return interrupt event number
             * @see WriteIRQEvents( uint16_t N )
             * @see WriteIRQLevel( uint16_t level )
             */
            uint16_t        ReadIRQEvents();

            /**
             * Set the VME interrupt level.
             * @param level interrupt level. _NOTE_: only the 3 least significant bits are used,
             * i.e. meaningful values are:
             * - 0 - VME interrupts are disabled
             * - 1, ..., 7 - VME interrupts are enabled
             * @see ReadIRQLevel()
             * @see WriteIRQVector( uint16_t vector )
             * @see ReadIRQVector()
             */
            void            WriteIRQLevel( uint16_t level ) override;

            /**
             * Get the VME interrupt level.
             * @return interrupt level. _NOTE_: only the 3 least significant bits are used
             * for the interrupt level,
             * i.e. meaningful values are:
             * - 0 - VME interrupts are disabled
             * - 1, ..., 7 - VME interrupts are enabled
             * @see WriteIRQLevel( uint16_t level )
             * @see WriteIRQVector( uint16_t vector )
             * @see ReadIRQVector()
             */
            uint16_t        ReadIRQLevel() override;

            /**
             * Set the VME interrupt status/ID (vector) that the module places on the VME
             * data bus during the interrupt acknowledge cycle.
             * @param vector interrupt vector.
             * @see WriteIRQLevel( uint16_t level )
             * @see ReadIRQLevel()
             * @see ReadIRQVector()
             */
            void            WriteIRQVector( uint16_t vector ) override;

            /**
             * Get the VME interrupt status/ID (vector) that the module places on the VME
             * data bus during the interrupt acknowledge cycle.
             * @return interrupt vector.
             * @see WriteIRQLevel( uint16_t level )
             * @see ReadIRQLevel()
             * @see WriteIRQVector( uint16_t vector )
             */
            uint16_t        ReadIRQVector() override;

            /**
             * _NOT YET IMPLEMENTED_
             */
            void            ISR( uint16_t vector ) override;
             /** @} */

        public :
            // Control & Status
            void            WriteReadoutCtrl( uint32_t value );
            uint32_t        ReadReadoutCtrl();

            /**
             * Read the status information common to all the channels of a group as a 32-bit word.
             * See the V1742B::StatusBit enumeration to interpretate each bit of that word.
             * @param group group index
             * @return status word
             * @see StatusBit
             * @see ReadStatus( Group_t group, StatusBit bit )
             */
            uint32_t        ReadStatus( Group_t group );

            /**
             * Read a given bit of the status information word common to all the channels of a
             * given group.
             * @param group group index
             * @param bit bit of interest in the status word
             * @return bit status (true = bit set, false = bit cleared) 
             * @see StatusBit
             * @see ReadStatus( Group_t group )
             */
            bool            ReadStatus( Group_t, StatusBit bit );

            uint32_t        ReadBoardConfiguration();

        public :
            /** @name Trigger
             * Trigger management
             * @{
             */

            /**
             * Set the post trigger value to a group.
             * The post trigger corresponds to the delay between the trigger arrival
             * and the DRS4 chip holding phase.
             * @param group group of channels to set the post trigger to
             * @param n post trigger value (1 unit = 8.5 ns)
             * @see ReadPostTrigger( Group_t group )
             * @see WritePostTrigger( uint16_t n )
             */
            void WritePostTrigger( Group_t group, uint16_t n );

            /**
             * Set the post trigger value to all the channels at once.
             * The post trigger corresponds to the delay between the trigger arrival
             * and the DRS4 chip holding phase.
             * @param n post trigger value (1 unit = 8.5 ns)
             * @see ReadPostTrigger( Group_t group )
             * @see WritePostTrigger( Group_t group, uint16_t n )
             */
            void WritePostTrigger( uint16_t n );

            /**
             * Get the post trigger value of a group.  
             * The post trigger corresponds to the delay between the trigger arrival
             * and the DRS4 chip holding phase.
             * @param group group of channels to get the post trigger of 
             * @return post trigger value (1 unit = 8.5 ns)
             */
            uint16_t ReadPostTrigger( Group_t group );

            /**
             * Set the trigger threshold of a channel.
             * _NOTE:_ threshold value must be set in units of absolute scale
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

            /**
             * Enable channels to participate in the trigger generation logic OR. The channel enabled with
             * this member function triggers when the input pulse crosses the threshold
             * (see WriteChannelThreshold).
             * Triggers of group 0 and group 1 enable the acquisition of all channels of group 0 and group 1
             * simultaneously, while triggers on group 2 and group 3 enable the acquisition of all channels
             * of group 2 and group 3 simultaneously.
             * @param ch channel index
             * @param enable enable flag (true = enable, false = disable)
             * @see WriteEnableTrigger( Group_t group, uint8_t mask )
             * @see WriteEnableTrigger( uint8_t mask )
             * @see ReadEnableTrigger( Group_t group )
             * @see ReadEnableTrigger( uint8_t ch )
             */
            void WriteEnableTrigger( uint8_t ch, bool enable );

            /**
             * Get the enable status of a channel. If this member function returns true it means the channel
             * triggers when the input pulse crosses the threshold (see WriteChannelThreshold).
             * @param ch channel index
             * @return enable flag (true = enabled, false = disabled)
             * @see WriteEnableTrigger( uint8_t ch, bool enable )
             * @see WriteEnableTrigger( Group_t group, uint8_t mask )
             * @see WriteEnableTrigger( uint8_t mask )
             * @see ReadEnableTrigger( Group_t group )
             */
            bool ReadEnableTrigger( uint8_t ch );

            /**
             * Enable channels to participate in the trigger generation logic OR. The channel enabled with
             * this member function triggers when the input pulse crosses the threshold
             * (see WriteChannelThreshold).
             * Triggers of group 0 and group 1 enable the acquisition of all channels of group 0 and group 1
             * simultaneously, while triggers on group 2 and group 3 enable the acquisition of all channels
             * of group 2 and group 3 simultaneously.
             * @param group channel group 
             * @param mask enable mask (i-th bit corresponds to the enable flag of the i-th channel of the group specified with the `group` argument)
             * @see WriteEnableTrigger( uint8_t ch, bool enable );
             * @see ReadEnableTrigger( uint8_t ch )
             * @see WriteEnableTrigger( uint8_t mask )
             * @see ReadEnableTrigger( Group_t group )
             */
            void WriteEnableTrigger( Group_t group, uint8_t mask );

            /**
             * Enable channels to participate in the trigger generation logic OR. The channel enabled with
             * this member function triggers when the input pulse crosses the threshold
             * (see WriteChannelThreshold).
             * Triggers of group 0 and group 1 enable the acquisition of all channels of group 0 and group 1
             * simultaneously, while triggers on group 2 and group 3 enable the acquisition of all channels
             * of group 2 and group 3 simultaneously.
             * @param mask enable mask (i-th bit corresponds to the enable flag of the i-th channel of each group)
             * @see WriteEnableTrigger( uint8_t ch, bool enable );
             * @see ReadEnableTrigger( uint8_t ch )
             * @see WriteEnableTrigger( Group_t group, uint8_t mask );
             * @see ReadEnableTrigger( Group_t group )
             */
            void WriteEnableTrigger( uint8_t mask );

            /**
             * Get the enable mask of a group.
             * triggers when the input pulse crosses the threshold (see WriteChannelThreshold).
             * @param group group index
             * @return enable mask (i-th bit corresponds to the enable flag of the i-th channel of the group specified with the `group` argument)
             * @see ReadEnableTrigger( uint8_t ch )
             * @see WriteEnableTrigger( uint8_t ch, bool enable )
             * @see WriteEnableTrigger( Group_t group, uint8_t mask )
             * @see WriteEnableTrigger( uint8_t mask )
             */
            uint8_t ReadEnableTrigger( Group_t group );

            /**
             * Set the trigger treshold of the TRn channel.
             * @param tr TR index
             * @param threshold threshold value in the DAC LSB counts
             * @see ReadThresholdTR( TR_t tr )
             */
            void WriteThresholdTR( TR_t tr, uint16_t threshold );

            /**
             * Get the trigger treshold of the TRn channel.
             * @param tr TR index
             * @return threshold threshold value in the DAC LSB counts
             * @see WriteThresholdTR( TR_t tr, uint16_t threshold )
             */
            uint16_t ReadThresholdTR( TR_t tr );

            /**
             * Set the DC offset for a TR channel.
             * @param tr TR index 
             * @param DC offset in the DAC LSB counts
             * @see ReadOffsetTR( TR_t tr )
             */
            void WriteOffsetTR( TR_t tr, uint16_t offset );

            /**
             * Get the DC offset for a TR channel.
             * @param tr TR index 
             * @return DC offset in the DAC LSB counts
             * @see WriteOffsetTR( TR_t tr, uint16_t offset )
             */
            uint16_t ReadOffsetTR( TR_t tr );

            void WriteVetoDelay( uint16_t value );
            uint16_t ReadVetoDelay();

            /**
             * Set the TRn trigger polarity.
             * @param pol trigger polarity
             * @see ReadTRPolarity()
             * @see TriggerPolarity_t
             */
            void WriteTRPolarity( TriggerPolarity_t pol );

            /**
             * Set the TRn trigger polarity.
             * @return trigger polarity
             * @see WriteTRPolarity( TriggerPolarity_t pol )
             * @see TriggerPolarity_t
             */
            TriggerPolarity_t ReadTRPolarity();

            /**
             * Enable/disable the readout of a TRn: when enabled, the signal TRn is digitized
             * and it is present in the data readout.
             * @param enable enable status (true = enable, false = disable)
             * @see ReadTRDigitize()
             */
            void WriteTRDigitize( bool enable = true );

            /**
             * Get the readout status of a TRn: when enabled, the signal TRn is digitized
             * and it is present in the data readout.
             * @return enable status (true = enabled, false = disabled)
             * @see WriteTRDigitize( bool enable )
             */
            bool ReadTRDigitize();

            /**
             * Enable/disable the TRn trigger. If enabled, the TRn signal is used as fast
             * trigger.
             * @param enable enable status (true = enable, false = disable)
             * @see ReadTREnable()
             */
            void WriteTREnable( bool enable = true );

            /**
             * Get the trigger status of a TRn. If enabled, the TRn signal is used as fast
             * trigger.
             * @return enable status (true = enabled, false = disabled)
             * @see WriteTREnable( bool enable )
             */
            bool ReadTREnable();

            /**
             * Enable/disable the signal on the TRG-IN to gate/veto the acquisition
             * @param enable enable status (true = enable, false = disable)
             * @see ReadTRGINEnable()
             * @see WriteTRGINSignal( TriggerIn_t trigger )
             * @see ReadTRGINSignal()
             */
            void WriteTRGINEnable( bool enable = true );

            /**
             * Check if the signal on the TRG-IN is enabled to gate/veto the acquisition
             * @return enable status (true = enabled, false = disabled)
             * @see WriteTRGINEnable( bool enable )
             * @see WriteTRGINSignal( TriggerIn_t trigger )
             * @see ReadTRGINSignal()
             */
            bool ReadTRGINEnable();

            /**
             * Choose the purpose of the signal on the TRG-IN :
             * - Veto the acquisition
             * - Gain the acquisition
             * @param signal signal intention 
             * @see WriteTRGINEnable( bool enable = true )
             * @see ReadTRGINEnable()
             * @see ReadTRGINSignal()
             * @see TriggerIn_t
             */
            void WriteTRGINSignal( TriggerIn_t signal );

            /**
             * Get the purpose of the signal on the TRG-IN :
             * - Veto the acquisition
             * - Gain the acquisition
             * @return signal intention 
             * @see WriteTRGINEnable( bool enable = true )
             * @see ReadTRGINEnable()
             * @see WriteTRGINSignal( TriggerIn_t signal )
             * @see TriggerIn_t
             */
            TriggerIn_t ReadTRGINSignal();

            void WriteTRGOUTSignal( TriggerOut_t trigger );
            TriggerOut_t ReadTRGOUTSignal();

            /**
             * Choose which signal can contribute to the gloabal trigger generation.
             * @param trigger trigger signal
             * @see ReadGlobalTrigger()
             * @see GlobalTrigger_t
             */
            void WriteGlobalTrigger( GlobalTrigger_t trigger );

            /**
             * Get information about the gloabal trigger generation.
             * @return trigger signal
             * @see WriteGlobalTrigger( GlobalTrigger_t trigger )
             * @see GlobalTrigger_t
             */
            GlobalTrigger_t ReadGlobalTrigger();

            /**
             * Force a software trigger generation which is propagated to all the enabled
             * channels of the board.
             * @see WriteGlobalTrigger( GlobalTrigger_t trigger )
             * @see ReadGlobalTrigger()
             */
            void WriteSWTrigger();
            /** @} */

        protected :
            virtual size_t HelperReadCycles() override;
            uint32_t GetBufferAddress() const override { return V1742B_OUTPUT_BUFFER_START; };

        public :
            /** @name Acquisition
             * Acquisition management
             * @{
             */

            /**
             * Enable/disable selected group to participate in the event readout.
             * **WARNING:** This function **MUST NOT** be called while the acquisition is running
             * @param g group index
             * @param enable enable status (true = enable, false = disable)
             * @see WriteGroupEnable( bool enable )
             */
            void WriteGroupEnable( Group_t g, bool enable = true );

            /**
             * Enable/disable all the groups to participate in the event readout.
             * **WARNING:** This function **MUST NOT** be called while the acquisition is running
             * @param enable enable status (true = enable, false = disable)
             * @see WriteGroupEnable( Group_t g, bool enable )
             */
            void WriteGroupEnable( bool enable );

            /**
             * Adjust the baseline position (i.e. 0 Volt) of the input signal on the ADC scale of a channel.
             * The ADC scale ranges from 0 to 2^{NBit} - 1. The DAC controlling the DC Offset
             * has 16 bits, i.e. it goes from 0 to 65535 independently from the NBit value and the board type.
             * Typically a DC Offset value of 32K (DAC mid‐scale) corresponds to about the ADC mid‐scale.
             * Increasing values of DC
             * Offset make the baseline decrease. The range of the DAC is about 5% (typ.) larger than
             * the ADC range, hence DAC settings close to 0 and 64K correspond to ADC respectively
             * over and under range
             * @param ch channel index
             * @param offset DC offset value in DAC LSB counts
             * @see ReadChannelOffset( uint8_t )
             */
            void WriteChannelOffset( uint8_t ch, uint16_t offset );


            /**
             * Get the baseline position (i.e. 0 Volt) of the input signal on the ADC scale of a channel.
             * The ADC scale ranges from 0 to 2^{NBit} - 1. The DAC controlling the DC Offset
             * has 16 bits, i.e. it goes from 0 to 65535 independently from the NBit value and the board type.
             * Typically a DC Offset value of 32K (DAC mid‐scale) corresponds to about the ADC mid‐scale.
             * Increasing values of DC
             * Offset make the baseline decrease. The range of the DAC is about 5% (typ.) larger than
             * the ADC range, hence DAC settings close to 0 and 64K correspond to ADC respectively
             * over and under range
             * @param ch channel index
             * @return offset DC offset value in DAC LSB counts
             * @see WriteChannelOffset( uint8_t ch, uint16_t offset )
             */
            uint16_t ReadChannelOffset( uint8_t ch );

            /**
             * Set the record length, which is the number of samples of the digitized waveform
             * in the acquisition window.
             * _NOTE:_ modifying the record length does NOT reduce the dead time.
             * **WARNING:** this function **MUST NOT** be called thile the acquisition is running !
             * @param length record length
             * @see ReadRecordLength()
             * @see RecordLength_t
             */
            void WriteRecordLength( RecordLength_t length );

            /**
             * Get the record length, which is the number of samples of the digitized waveform
             * in the acquisition window.
             * _NOTE:_ modifying the record length does NOT reduce the dead time.
             * @return record length
             * @see WriteRecordLength( RecordLength_t length )
             * @see RecordLength_t
             */
            RecordLength_t ReadRecordLength();

            /**
             * Set the initial value for the test mode.
             * @param value initial value
             * @see ReadInitTestValue( Group_t group )
             * @see WriteTestModeEnable( bool enable )
             * @see ReadTestModeEnable()
             */
            void WriteInitTestValue( uint16_t value );

            /**
             * Get the initial value for the test mode.
             * @param group group index 
             * @return initial value
             * @see WriteInitTestValue( uint16_t value )
             * @see WriteTestModeEnable( bool enable )
             * @see ReadTestModeEnable()
             */
            uint16_t ReadInitTestValue( Group_t group );

            /**
             * Set the sampling frequency of each DRS4 chip.
             * **WARNING:** this function **MUST NOT** be called while the acquisition is running.
             * @param rate sampling rate
             * @see ReadSamplingRate( Group_t group )
             * @see SamplingRate_t
             */
            void WriteSamplingRate( SamplingRate_t rate );

            /**
             * Set the sampling frequency of a DRS4 chip.
             * @param group group index
             * @return rate sampling rate
             * @see WriteSamplingRate( SamplingRate_t rate )
             * @see SamplingRate_t
             */
            SamplingRate_t ReadSamplingRate( Group_t group );


            /**
             * Get the curretn available event size in 32-bit words. The value is updated
             * after a complete readout of each event.
             * @return current event size
             */
            uint32_t ReadEventSize();

            /**
             * Write a 32-bit word to the board for test purposes.
             * @param value value to write
             * @see ReadScratch()
             */
            void WriteScratch( uint32_t value );

            /**
             * Read a 32-bit word (written via the WriteScratch( uint32_t value ) member function)
             * from the board.
             * @return written word
             * @see WriteScratch( uint32_t value )
             */
            uint32_t ReadScratch();

            /**
             * Set the maximum number of complete events which has to be transferred
             * for each block transfer (via VME BLT/CBLT cycles or Optical Link).
             * @param n number of events per BLT (1023 max)
             */
            void WriteMaxEventBLT( uint16_t n );

            /**
             * Get the maximum number of complete events which has to be transferred
             * for each block transfer (via VME BLT/CBLT cycles or Optical Link).
             * @return number of events per BLT (1023 max)
             */
            uint16_t ReadMaxEventBLT();

            /**
             * Choose the acquisition mode :
             * - Transparent: the input pulse is both sampled by the DRS4 capacitors at high
             *   frequency, and made available at the output for the ADC digital sampling at a
             *   smaller rate, about 30 MHz. Transparetn mode is the standard operating mode
             *   of the DRS4 chip, which continuously samples the input.
             * - Output: the input is no longer sampled and the capacitors hold the acquired
             *   samples and send them one at a time to the ADC at a frequency controlled by
             *   the FPGA (readout frequency). The Output mode starts when a trigger
             *   condition is met. Samples in the Output mode are those available in the readout
             *   for the user and they are correctly shaped.
             * @param mode acquisition mode
             * @see ReadAcqMode()
             * @see AcqMode_t
             */
            void WriteAcqMode( AcqMode_t mode );

            /**
             * Get the acquisition mode :
             * - Transparent: the input pulse is both sampled by the DRS4 capacitors at high
             *   frequency, and made available at the output for the ADC digital sampling at a
             *   smaller rate, about 30 MHz. Transparetn mode is the standard operating mode
             *   of the DRS4 chip, which continuously samples the input.
             * - Output: the input is no longer sampled and the capacitors hold the acquired
             *   samples and send them one at a time to the ADC at a frequency controlled by
             *   the FPGA (readout frequency). The Output mode starts when a trigger
             *   condition is met. Samples in the Output mode are those available in the readout
             *   for the user and they are correctly shaped.
             * @return acquisition mode
             * @see WriteAcqMode( AcqMode_t mode )
             * @see AcqMode_t
             */
            AcqMode_t ReadAcqMode();

            /**
             * Select Start/Stop acquisition source.
             * @param source acquisition start/stop source.
             * @see ReadStartSource()
             * @see StartSource_t
             */
            void WriteStartSource( StartSource_t source );

            /**
             * Get Start/Stop acquisition source.
             * @param source acquisition start/stop source.
             * @see ReadStartSource()
             * @see Source_t
             */
            StartSource_t ReadStartSource();

            /**
             * Depending on the mode selected with the
             * WriteStartSource( StartSource_t source ) member function either Start or Arm the
             * acquisition :
             * - StartSource_t::SW - starts the acquisition
             * - StartSource_t::S_IN - arms the acquisition
             * - StartSource_t::FirstTrg - arms the acquisition
             * - StartSource_t::LVDS - arms the acquisition
             * @see Stop()
             * @see WriteStartSource( StartSource_t source )
             * @see ReadStartSource()
             * @see StartSource_t
             */
            void Start();

            /**
             * Depending on the mode selected with the
             * WriteStartSource( StartSource_t source ) member function either Stop or Disarm the
             * acquisition :
             * - StartSource_t::SW - stops the acquisition
             * - StartSource_t::S_IN - disarms the acquisition
             * - StartSource_t::FirstTrg - disarms the acquisition
             * - StartSource_t::LVDS - disarms the acquisition
             * @see Start()
             * @see WriteStartSource( StartSource_t source )
             * @see ReadStartSource()
             * @see StartSource_t
             */
            void Stop();

            /**
             * Read the acquisition status information as a 32-bit word. See the V1742B::AcqStatusBit
             * enumeration to interpretate each bit of that word.
             * @return acquisition status
             * @see AcqStatusBit
             * @see ReadAcqStatus( AcqStatusBit bit )
             */
            uint32_t ReadAcqStatus();

            /**
             * Read a given bit of the acquisition status word.
             * @return bit status (true = bit set, false = bit cleared)
             * @see AcqStatusBit
             * @see ReadAcqStatus()
             */
            bool ReadAcqStatus( AcqStatusBit bit );

            /**
             * Monitor a set of board errors. In case of a failure,
             * the UEvent<V1742B>::GetBoardFail() function returns true. This function
             * allows to get the exact kind of error occurred. See the V1742B::ErrorBit
             * to interpretate each bit of the returned 32-bit word.
             * @return error word
             * @see ErrorBit
             */
            uint32_t ReadBoardFailure();
            /** @} */

        public :
            /** @name Front Panel
             * Interaction with the front panel connectors.
             * @{
             */

            /**
             * Set the electrical level of the front panel LEMO connectors:
             * - TRG-IN
             * - TRG-OUT
             * - S-IN
             * @param level logic family
             * @see ReadLEMOLevel()
             * @see Level_t
             */
            void WriteLEMOLevel( Level_t level );

            /**
             * Get the electrical level of the front panel LEMO connectors:
             * - TRG-IN
             * - TRG-OUT
             * - S-IN
             * @param level logic family
             * @see WriteLEMOLevel()
             * @see Level_t
             */
            Level_t ReadLEMOLevel();

            void WriteLVDS( uint16_t mask );
            uint16_t ReadLVDS();
            /** @} */

        protected :
            /**
             * Read correction table from a file.
             */
            void LoadCorrectionTable();

        public :
            /** @name Data Correction
             * Data correction
             * @{
             */

            /**
             * The DRS4 chip needs data corrections due to the unavoidable differences
             * in the chip constuction process. This function applies the default corrections
             * which are provided by CAEN in the memory flash of the board.
             * @param event event to apply the corrections to
             * @see SetPathToCorrectionTable( const std::string& path )
             */
            void ApplyCorrection( UEvent<V1742B>& event ) const;

            /**
             * Set path to a directory containing the correction tables.
             * This path may be either absolute or relative to the executable. 
             */
            void SetPathToCorrectionTable( const std::string& path ) { fPathToCorrectionTable = path; }

            /**
             * Get current correction table. I.e. the last one loaded
             */
            CorrectionTable GetCorrectionTable() const { return fCorrectionTable; }
            /** @} */

        public :
            // Config
            /**
             * Fill a given configuration instance with the current state of the board.
             * @param config configuration to fill
             * @see WriteConfig( const UConfig<V1742B>& config )
             */
            void    ReadConfig( UConfig<V1742B>& config ) override;

            /**
             * Program the board with a given configuration instance.
             * @param config configuration to write to the board 
             * @see ReadConfig( UConfig<V1742B>& config )
             */
            void    WriteConfig( const UConfig<V1742B>& config ) override;
    };

    template<>
    const std::string UConfigurable<V1742B>::fName;

    template <>
    struct UConfig<V1742B>
    {
        struct Interrupts
        {
            uint16_t    IRQ_EVENTS;
            uint16_t    IRQ_LEVEL;
            uint16_t    IRQ_VECTOR;

            template <class TArchive>
            void serialize( TArchive& ar )
            {
                ar( cereal::make_nvp( "irq_events", IRQ_EVENTS ),
                    cereal::make_nvp( "irq_level", IRQ_LEVEL ),
                    cereal::make_nvp( "irq_vector", IRQ_VECTOR ) );
            }
        };

        struct Trigger
        {
            struct Channel
            {
                uint16_t    THRESHOLD;
                uint16_t    OFFSET;
                bool        ENABLE_TRG;

                template <class TArchive>
                void serialize( TArchive& ar )
                {
                    ar( cereal::make_nvp( "threshold", THRESHOLD ),
                        cereal::make_nvp( "offset", OFFSET ),
                        cereal::make_nvp( "enable_trigger", ENABLE_TRG ) );
                }
            };

            uint16_t                                    POST_TRIGGER;
            std::array<Channel, V1742B::GetChNumber()>  CHANNELS;
            Channel                                     TR_0;
            Channel                                     TR_1;
            V1742B::GlobalTrigger_t                     GLB_TRG;
            V1742B::TriggerPolarity_t                   TRG_POL;

            template <class TArchive>
            void serialize( TArchive& ar )
            {
                ar( cereal::make_nvp( "post_trigger", POST_TRIGGER ),
                    cereal::make_nvp( "channels", CHANNELS ),
                    cereal::make_nvp( "tr_0", TR_0 ),
                    cereal::make_nvp( "tr_1", TR_1 ),
                    cereal::make_nvp( "global_trigger", GLB_TRG ),
                    cereal::make_nvp( "trigger_polarity", TRG_POL ) );
            }
        };

        struct Acquisition
        {
            std::array<bool, V1742B::GetGroupNumber()>  GR_ENABLE;
            V1742B::RecordLength_t                      REQ_LENGTH;
            V1742B::SamplingRate_t                      SAMP_RATE;
            uint16_t                                    MAX_EVENTS_BLT;
            V1742B::AcqMode_t                           ACQ_MODE;
            V1742B::StartSource_t                       START_SOURCE;

            template <class TArchive>
            void serialize( TArchive& ar )
            {
                ar( cereal::make_nvp( "group_readout", GR_ENABLE ),
                    cereal::make_nvp( "record_length", REQ_LENGTH ),
                    cereal::make_nvp( "sampling_rate", SAMP_RATE ),
                    cereal::make_nvp( "max_events_per_blt", MAX_EVENTS_BLT ),
                    cereal::make_nvp( "acquisition_mode", ACQ_MODE ),
                    cereal::make_nvp( "start_source", START_SOURCE ) );
            }
        };

        struct FrontPanel
        {
            V1742B::Level_t         LEMO_LVL;

            template <class TArchive>
            void serialize( TArchive& ar )
            {
                ar( cereal::make_nvp( "lemo_level", LEMO_LVL ) );
            }
        };

        Trigger     TRIGGER;
        Interrupts  INTERRUPTS;
        Acquisition ACQUISITION;
        FrontPanel  FRONT_PANEL;

        template <class TArchive>
        void serialize( TArchive& ar )
        {
            ar( cereal::make_nvp( "trigger", TRIGGER ),
                cereal::make_nvp( "interrupts", INTERRUPTS ),
                cereal::make_nvp( "acquisition", ACQUISITION ),
                cereal::make_nvp( "front_panel", FRONT_PANEL ) );
        }
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

            /**
             * Read if an error has occurred during the data readout.
             * @return error status (true = error, false = no error)
             * @see V1742B::ReadBoardFailure()
             */
            bool GetBoardFail() const       { return fHeader[1] & V1742B_HDR_BOARD_FAIL_MSK; }

            uint16_t GetLVDSPattern() const { return (fHeader[1] & V1742B_HDR_LVDS_MSK) >> V1742B_HDR_LVDS_SHFT; }
            uint8_t GetGroupMask() const    { return (fHeader[1] & V1742B_HDR_GROUP_MSK); }
            size_t GetEventCounter() const  { return (fHeader[2] & V1742B_HDR_EVENT_CNT_MSK); }
            uint32_t GetEventTTT() const    { return (fHeader[3] & V1742B_HDR_EVENT_TTT_MSK); }

            friend void V1742B::ApplyCorrection( UEvent<V1742B>& event ) const;
    };
}

#endif
