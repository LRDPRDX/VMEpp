#ifndef V_PLUS_V1785N
#define V_PLUS_V1785N

//*******************************************************************
//
// Description :
//      Header for CAEN V1785N module.
//      V1785N is a 8 channel Dual Range Multievent Peak Sensing ADC
// Author :
//      Sikach Bogdan
// Location :
//      Budker Institute of Nuclear Physics. Room 210/2, Phone 51-57
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
// DR, SR, HR       = Data Reset, Software Reset or Hardware Reset (see the module's manual)
// Aux              = Auxiliary data
//*******************************************************************

// MEMORY MAP (BEGIN)
//****** Multi Event Buffer Definition ******
#define     V1785N_OUTPUT_BUFFER            0x0000UL//A32/D32 R DR/SR/HR
#define     V1785N_OUTPUT_BUFFER_START      V1785N_OUTPUT_BUFFER//Aux
#define     V1785N_OUTPUT_BUFFER_STOP       0x07FFUL//Aux, This is the last byte used by buffer
#define     V1785N_OUTPUT_BUFFER_SIZE       (V1785N_OUTPUT_BUFFER_STOP - V1785N_OUTPUT_BUFFER_START + 1UL)

//****** Firmware Revision Register Definition ******
#define     V1785N_FIRMWARE_REVISION        0x1000UL//A32/D16 R
#define     V1785N_FIRM_REV_MSB_MSK         0xFF00UL//Aux
#define     V1785N_FIRM_REV_LSB_MSK         0x00FFUL//Aux

//****** Geo Address Register Definition ******
#define     V1785N_GEO_ADDR                 0x1002UL//A32/D16 RW

//****** Multicast/Chained Block Transfer Address Register Definition ******
#define     V1785N_MCST_CBLT_ADDR           0x1004UL//A32/D16 RW HR

//****** Bit Set 1 Register Definition ******
#define     V1785N_BIT_SET_1                0x1006UL//A32/D16 RW SR/HR
#define     V1785N_BIT_SET_1_BERR_FLAG_BIT  0x0003U//Aux
#define     V1785N_BIT_SET_1_SEL_ADDR_BIT   0x0004U//Aux
#define     V1785N_BIT_SET_1_SOFT_RST_BIT   0x0007U//Aux

//****** Bit Clear 1 Register Definition ******
#define     V1785N_BIT_CLEAR_1              0x1008UL//A32/D16 RW SR/HR
//Bits for BIT_CLEAR_1 are the same as for BIT_SET_1

//****** Interrupt Level Register Definition ******
#define     V1785N_INTERRUPT_LEVEL          0x100AUL//A32/D16 RW SR/HR
#define     V1785N_INTERRUPT_LEVEL_MSK      0x0007UL//Aux

//****** Interrupt Vector Register Definition ******
#define     V1785N_INTERRUPT_VECTOR         0x100CUL//A32/D16 RW SR/HR
#define     V1785N_INTERRUPT_VECTOR_MSK     0x00FFUL//Aux

//****** Status (1) Register Definition ******
#define     V1785N_STATUS_REGISTER_1        0x100EUL//A32/D16 R  SR/HR
#define     V1785N_STS1_REG_DREADY_BIT      0x0000U//Aux
#define     V1785N_STS1_REG_GLB_DREADY_BIT  0x0001U//Aux
#define     V1785N_STS1_REG_BUSY_BIT        0x0002U//Aux
#define     V1785N_STS1_REG_GLB_BUSY_BIT    0x0003U//Aux
#define     V1785N_STS1_REG_AMNESIA_BIT     0x0004U//Aux
#define     V1785N_STS1_REG_PURGED_BIT      0x0005U//Aux
#define     V1785N_STS1_REG_TERM_ON_BIT     0x0006U//Aux
#define     V1785N_STS1_REG_TERM_OFF_BIT    0x0007U//Aux
#define     V1785N_STS1_REG_EVRDY_BIT       0x0008U//Aux

//****** Control (1) Register Definition ******
#define     V1785N_CONTROL_REGISTER_1       0x1010UL//A32/D16 RW SR/HR
#define     V1785N_CNTL_REG_BLKEND_BIT      0x0002U//Aux
#define     V1785N_CNTL_REG_PRG_RST_BIT     0x0004U//Aux
#define     V1785N_CNTL_REG_BERR_ENB_BIT    0x0005U//Aux
#define     V1785N_CNTL_REG_ALN64_BIT       0x0006U//Aux

//****** Address Decoder Register Definition ******
#define     V1785N_ADER_HIGH                0x1012UL//A32/D16 RW HR 
#define     V1785N_ADER_HIGH_MSK            0x10FFUL//Aux
#define     V1785N_ADER_LOW                 0x1014UL//A32/D16 RW HR
#define     V1785N_ADER_LOW_MSK             0x10FFUL//Aux

//****** Single Shot Register Definition ******
#define     V1785N_SINGLE_SHOT_RST          0x1016UL//A32/D16 W  

//****** Multicast/Chained Block Transfer Control Register Definition ******
#define     V1785N_MCST_CBLT_CTRL_REGISTER  0x101AUL//A32/D16 RW HR
#define     V1785_M_C_CTRL_REG_LAST_BIT     0x0000U//Aux
#define     V1785_M_C_CTRL_REG_FIRST_BIT    0x0001U//Aux

//****** Event Trigger Register Definition ******
#define     V1785N_EVENT_TRIGGER_REGISTER   0x1020UL//A32/D16 RW SR/HR
#define     V1785N_EVENT_TRIGGER_REG_MSK    0x001FUL//Aux

//****** Status (2) Register Definition ******
#define     V1785N_STATUS_REGISTER_2        0x1022UL//A32/D16 R  SR/HR
#define     V1785N_STS2_REG_BUFF_EMPTY_BIT  0x0001U//Aux
#define     V1785N_STS2_REG_BUFF_FULL_BIT   0x0002U//Aux
#define     V1785N_STS2_REG_DSEL0_BIT       0x0004U//Aux
#define     V1785N_STS2_REG_DSEL1_BIT       0x0005U//Aux
#define     V1785N_STS2_REG_CSEL0_BIT       0x0006U//Aux
#define     V1785N_STS2_REG_CSEL1_BIT       0x0007U//Aux

//****** Event Counter Register Definition ******
#define     V1785N_EVENT_COUNTER_L          0x1024UL//A32/D16 R  SR/HR/DR
#define     V1785N_EVENT_COUNTER_H          0x1026UL//A32/D16 R  SR/HR/DR
#define     V1785N_EVENT_COUNTER_H_MSK      0x00FFUL//Aux

//****** Increment Event Register Definition ******
#define     V1785N_INCREMENT_EVENT          0x1028UL//A32/D16 W

//****** Increment Offset Register Definition ******
#define     V1785N_INCREMENT_OFFSET         0x102AUL//A32/D16 W

//****** Load Test Register Definition ******
#define     V1785N_LOAD_TEST_REGISTER       0x102CUL//A32/D16 RW

//****** Fast Clear Register Definition ******
#define     V1785N_FCLR_WINDOW              0x102EUL//A32/D16 RW SR/HR
#define     V1785N_FCLR_WINDOW_MSK          0x03FFUL//Aux

//****** Bit Set 2 Register Definition ******
#define     V1785N_BIT_SET_2                0x1032UL//A32/D16 RW SR/HR
#define     V1785N_BIT_SET_2_MEM_TEST_BIT   0x0000U//Aux
#define     V1785N_BIT_SET_2_OFFLINE_BIT    0x0001U//Aux
#define     V1785N_BIT_SET_2_CLR_DATA_BIT   0x0002U//Aux
#define     V1785N_BIT_SET_2_OV_RNG_PRG_BIT 0x0003U//Aux
#define     V1785N_BIT_SET_2_LW_THR_PRG_BIT 0x0004U//Aux
#define     V1785N_BIT_SET_2_TEST_ACQ_BIT   0x0006U//Aux
#define     V1785N_BIT_SET_2_SLD_ENB_BIT    0x0007U//Aux
#define     V1785N_BIT_SET_2_STEP_TH_BIT    0x0008U//Aux
#define     V1785N_BIT_SET_2_AUTO_INC_BIT   0x000BU//Aux
#define     V1785N_BIT_SET_2_EMPTY_PRG_BIT  0x000CU//Aux
#define     V1785N_BIT_SET_2_SLD_SB_ENB_BIT 0x000DU//Aux
#define     V1785N_BIT_SET_2_ALL_TRG_BIT    0x000EU//Aux

//****** Bit Clear 2 Register Definition ******
#define     V1785N_BIT_CLEAR_2              0x1034UL//A32/D16 W  SR/HR
//Bits for BIT_CLEAR_2 are the same as for BIT_SET_2

//****** Write Memory Test Address Register Definition ******
#define     V1785N_W_MEMORY_TEST_ADDR       0x1036UL//A32/D16 W  SR/HR
#define     V1785N_W_MEMORY_TEST_ADDR_MSK   0x07FFUL//A32/D16 W  SR/HR

//****** Memory Test Word (High and Low) Register Definition ******
#define     V1785N_MEMORY_TEST_WORD_HIGH    0x1038UL//A32/D16 W  SR/HR
#define     V1785N_MEMORY_TEST_WORD_LOW     0x103AUL//A32/D16 W

//****** Crate Select Register Definition ******
#define     V1785N_CRATE_SELECT             0x103CUL//A32/D16 RW SR/HR
#define     V1785N_CRATE_SELECT_MSK         0x00FFUL//Aux

//****** Test Event Write Register Definition ******
#define     V1785N_TEST_EVENT_WRITE         0x103EUL//A32/D16 W
#define     V1785N_EVENT_COUNTER_RST        0x1040UL//A32/D16 W 

//****** Read Memory Test Address Register Definition ******
#define     V1785N_R_MEMORY_TEST_ADDR       0x1064UL//A32/D16 W  SR/HR
#define     V1785N_R_MEMORY_TEST_ADDR_MSK   0x07FFUL//A32/D16 W  SR/HR

//****** Software Comm Register Definition ******
#define     V1785N_SW_COMM                  0x1068UL//A32/D16 W

//****** Slide Constant Register Definition ******
#define     V1785N_SLIDE_CONSTANT           0x106AUL//A32/D16 RW SR/HR
#define     V1785N_SLIDE_CONSTANT_MSK       0x00FFUL//Aux

//****** Group A & B Registers Definition ******
#define     V1785N_AAD                      0x1070UL//A32/D16 R
#define     V1785N_BAD                      0x1072UL//A32/D16 R
#define     V1785N_AD_MSK                   0x07FFUL//Aux

//****** Channel Thresholds Definition ******
#define     V1785N_THRESHOLDS_START         0x1080UL//Aux
#define     V1785N_THRESHOLDS_STOP          0x10BFUL//Aux, This is the last byte used by thresholds
#define     V1785N_HIGH_THRESHOLD(x)        (V1785N_THRESHOLDS_START + (0x0008UL * ((x) % 8UL)))
#define     V1785N_LOW_THRESHOLD(x)         (V1785N_THRESHOLDS_START + (0x0008UL * ((x) % 8UL)) + 0x0004UL)
#define     V1785N_THRESHOLD_MSK            0x00FFUL//Aux
#define     V1785N_THR_KILL_MSK             0x0100UL//Aux, kill bit
#define     V1785N_THR_KILL_BIT             0x0008U//Aux, kill bit

//****** Read Only Memory Definition ******
//ROM (The data read from ROM seems to be an 8-bit long)
#define     V1785N_OUI_MSB                  0x8026UL//A32/D16 R   
#define     V1785N_OUI                      0x802AUL//A32/D16 R
#define     V1785N_OUI_LSB                  0x802EUL//A32/D16 R
#define     V1785N_VERSION                  0x8032UL//A32/D16 R
#define     V1785N_BOARD_ID_MSB             0x8036UL//A32/D16 R
#define     V1785N_BOARD_ID                 0x803AUL//A32/D16 R
#define     V1785N_BOARD_ID_LSB             0x803EUL//A32/D16 R
#define     V1785N_REVISION                 0x804EUL//A32/D16 R
#define     V1785N_SERIAL_MSB               0x8F02UL//A32/D16 R
#define     V1785N_SERIAL_LSB               0x8F06UL//A32/D16 R
// MEMORY MAP (END)

//****** MISC ******
#define     V1785N_LUB                      0xFFFFUL//Aux, The Last Used Byte

#define     V1785N_N_CHANNELS               0x08U//Aux, The number of available channels
#define     V1785N_MAX_CHANNEL_DATA         (2U * V1785N_N_CHANNELS)
#define     V1785N_MAX_BLT_SIZE             (0x0100U)//256 B
#define     V1785N_MAX_MBLT_SIZE            (0x0800U)//2 KiB
#define     V1785N_MAX_DATA_SIZE            (18U * 32U * 4U)//2302 bytes 

//Determine the word type in the buffer
#define     V1785N_WORD_TYPE_DATA           0x00000000UL//Aux
#define     V1785N_WORD_TYPE_DT_CH_MSK      0x001C0000UL//Aux
#define     V1785N_WORD_TYPE_DT_RG_MSK      0x00020000UL//Aux
#define     V1785N_WORD_TYPE_DT_UN_MSK      0x00002000UL//Aux
#define     V1785N_WORD_TYPE_DT_OV_MSK      0x00001000UL//Aux
#define     V1785N_WORD_TYPE_DT_ADC_MSK     0x00000FFFUL//Aux

#define     V1785N_WORD_TYPE_HEADER         0x02000000UL//Aux
#define     V1785N_WORD_TYPE_HDR_CH_MSK     0x00003F00UL//Aux   
#define     V1785N_WORD_TYPE_HDR_CR_MSK     0x00FF0000UL//Aux   

#define     V1785N_WORD_TYPE_EOB            0x04000000UL//Aux
#define     V1785N_WORD_TYPE_EOB_CNT_MSK    0x00FFFFFFUL//Aux

#define     V1785N_WORD_TYPE_INVALID        0x06000000UL//Aux
#define     V1785N_WORD_TYPE_MSK            0x07000000UL//Aux

//#include "VBoard.h"
#include "VSlaveAcquisitor.h"
#include "VSlaveInterrupter.h"
#include "VEvent.h"
#include "UConfigurable.h"

#include <string>

namespace vmeplus
{
    class V1785NEvent;

    //****** V1785N Part ******
    class V1785N : public VSlaveAcquisitor, public VSlaveInterrupter, public UConfigurable
    {
        public :
            enum class Range_t : uint8_t { HIGH, LOW };

        protected :
            //ROM Data
            std::string     fFirmwareRevision;
            uint32_t        fOUI;
            uint16_t        fVersion;
            uint32_t        fBoardID;
            uint16_t        fRevision;

        protected :
            virtual void    Initialize() override;

        protected :
            //Auxiliary control functions
            virtual void    ClearBit16( uint32_t address, uint16_t bit ) override;
            virtual void    SetBit16( uint32_t address, uint16_t bit ) override;

        public :
            V1785N( uint32_t baseAddress, uint32_t range = V1785N_LUB );
            virtual         ~V1785N();

        protected : 
            //Read ROM
            uint16_t        ReadVersion();
            uint32_t        ReadBoardID();
            uint32_t        ReadOUI();
            uint16_t        ReadRevision();
            uint16_t        ReadSerialMSB();
            uint16_t        ReadSerialLSB();
            uint16_t        ReadSerialNumber();
            uint16_t        ReadFirmRevNumber();//Actually not ROM

        public :
            //Channels settings
            void            EnableAll( bool alive = true );
            uint16_t        ReadLowThreshold( uint16_t ch );
            void            WriteLowThreshold( uint16_t ch, uint16_t th, bool kill = true );
            uint16_t        ReadHighThreshold( uint16_t ch );
            void            WriteHighThreshold( uint16_t ch, uint16_t th, bool kill = true );
            bool            ReadIfEnabledLow( uint16_t ch );
            bool            ReadIfEnabledHigh( uint16_t ch );

        public :
            //Misc
            virtual void    Reset() override;
            void            SendSoftReset();
            void            SendSingleShotReset();

        public :
            //Interrupts
            void            WriteIRQEvents( uint16_t n );
            uint16_t        ReadIRQEvents();
            void            WriteIRQLevel( uint16_t level ) override;
            uint16_t        ReadIRQLevel() override;
            void            WriteIRQVector( uint16_t level ) override;
            uint16_t        ReadIRQVector() override;
            void            ISR( uint16_t vector ) override;

            //Monitor state registers
            void            PrintControl();
            void            PrintBitSet();
            void            PrintStatus();

            //Data acquisition
            void            AllocateBuffer() override;
            //void            FreeBuffer() override;
            uint32_t        ReadBuffer() override;
            //void            ScanBuffer() override;
            //bool            GetEvent( VEvent *event ) override;
            bool            GetEventAt( uint32_t index, VEvent *event ) const override;
            void            ClearData();

        //State methods 
        public :
            //ROM
            std::string     GetFirmRevision() const { return fFirmwareRevision; }//Not ROM
            uint32_t        GetOUI() const { return fOUI; }
            uint16_t        GetVersion() const { return fVersion; }
            uint32_t        GetBoardID() const { return fBoardID; }
            uint16_t        GetRevision() const { return fRevision; }

            //Data Acquisition
            //uint32_t        GetNEventsRead() const { return fNEventsRead; }

            //Printing
            virtual void    Print() const override;

        public :
            virtual void    ReadConfig( nlohmann::json &config ) override;
            virtual void    WriteConfig( const nlohmann::json &config ) override;
    };

    //****** V1785NEvent Part ******
    class V1785NEvent : public VEvent
    {
        protected :
            uint32_t    fHeader;
            uint32_t    fData[V1785N_MAX_CHANNEL_DATA];
            uint32_t    fEOB;

        public :
            V1785NEvent();
            ~V1785NEvent();
            V1785NEvent( const V1785NEvent &other );
            V1785NEvent& operator=( V1785NEvent other );
            friend void swap( V1785NEvent &first, V1785NEvent &second )
            {
                using std::swap;
                swap( first.fHeader, second.fHeader );
                swap( first.fData, second.fData );
                swap( first.fEOB, second.fEOB );
            }

        public :
            uint16_t GetMemoChannels() const
            {
                return (fHeader  & V1785N_WORD_TYPE_HDR_CH_MSK) >> 8U;
            }

            uint16_t GetEventCounter() const
            {
                return fEOB & V1785N_WORD_TYPE_EOB_CNT_MSK;
            }

            uint16_t GetChannelNumber( uint16_t i ) const
            {
                i %= (V1785N_N_CHANNELS * 2U);
                return (fData[i] & V1785N_WORD_TYPE_DT_CH_MSK) >> 18U;
            }   

            bool GetChannelUnder( uint16_t i ) const
            {
                i %= (V1785N_N_CHANNELS * 2U);
                return (fData[i] & V1785N_WORD_TYPE_DT_UN_MSK);
            }

            bool GetChannelOver( uint16_t i ) const
            {
                i %= (V1785N_N_CHANNELS * 2U);
                return (fData[i] & V1785N_WORD_TYPE_DT_OV_MSK);
            }

            uint32_t GetChannelData( uint16_t i ) const
            {
                i %= (V1785N_N_CHANNELS * 2U);
                return (fData[i] & V1785N_WORD_TYPE_DT_ADC_MSK);
            }

            V1785N::Range_t GetChannelRange( uint16_t i ) const
            {
                i %= (V1785N_N_CHANNELS * 2U);
                return (fData[i] & V1785N_WORD_TYPE_DT_RG_MSK) ? V1785N::Range_t::HIGH : V1785N::Range_t::LOW;
            }

            friend class V1785N;
    };

}
#endif
