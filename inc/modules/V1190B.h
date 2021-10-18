#ifndef V_PLUS_V1190B_H
#define V_PLUS_V1190B_H

//*******************************************************************
//
// Description :
//      Header for the CAEN V1190B module.
//      V1190B is a 64 channel VME Time to Digital Converter 
// Author :
//      Polina Sayfudinova & Sikach Bogdan
// Location :
//      Budker Institute of Nuclear Physics. Room 210/2, Phone 51-57
// Feedback :
//      p.saifudinova@g.nsu.ru, paradox1859@gmail.com
//
//*******************************************************************
//
//
//*******************************************************************
// Comments description
// D32, D16, ...    = Data size
// A32, A24, ...    = Address modifier (however, it's always A32 here)
// R, W, RW         = Read, Write or Read-Write access modes
// Aux              = Auxiliary data
//*******************************************************************

// MEMORY MAP (BEGIN)
#define V1190B_OUTPUT_BUFFER                0X0000UL
#define V1190B_OUTPUT_BUFFER_MAX            0x0FFCUL
#define V1190B_CONTROL_REGISTER             0x1000UL
#define V1190B_EN_LUT_READ_BIT              7U
#define V1190B_STATUS_REGISTER              0X1002UL
#define V1190B_INTERRUPT_LEVEL              0x100AUL
#define V1190B_INTERRUPT_LEVEL_MSK          0x0007UL
#define V1190B_INTERRUPT_VECTOR             0x100CUL
#define V1190B_INTERRUPT_VECTOR_MSK         0x00FFUL
#define V1190B_GEO_ADDRESS                  0x100EUL
#define V1190B_MCST_BASE_ADDRESS            0x1010UL
#define V1190B_MCST_CBLT_CTRL               0x1012UL
#define V1190B_MODULE_RESET                 0x1014UL
#define V1190B_SOFTWARE_CLEAR               0x1016UL
#define V1190B_SOFTWARE_EVENT_RESET         0x1018UL
#define V1190B_SOFTWARE_TRIGGER             0x101AUL
#define V1190B_EVENT_COUNTER                0x101CUL
#define V1190B_EVENT_STORED                 0x1020UL
#define V1190B_ALMOST_FULL_LEVEL            0x1022UL
#define V1190B_BLT_EVENT_NUMBER             0x1024UL
#define V1190B_FIRMWARE_REVISION            0x1026UL
#define V1190B_TESTREG                      0x1028UL
#define V1190B_OUT_PROG_CONTROL             0x102CUL
#define V1190B_MICRO                        0x102EUL
#define V1190B_MICRO_HANDSHAKE              0x1030UL
#define V1190B_WRITE_OK_MSK                 (1U<<0U)
#define V1190B_READ_OK_MSK                  (1U<<1U)
#define V1190B_SEL_FLASH                    0x1032UL
#define V1190B_FLASH                        0x1034UL
#define V1190B_COMPENSATION_SRAM_PAGE       0x1036UL
#define V1190B_EVENT_FIFO                   0x1038UL
#define V1190B_EVENT_FIFO_STORED            0x103CUL
#define V1190B_EVENT_FIFO_STATUS            0x103EUL
#define V1190B_DUMMY_32                     0x1200UL
#define V1190B_DUMMY_16                     0x1204UL

#define V1190V_MICRO_FIRMWARE_REVISION      0x0004UL
#define V1190B_MICRO_FIRMWARE_DAY           0x0005UL
#define V1190B_MICRO_FIRMWARE_MONTH         0x0006UL
#define V1190B_MICRO_FIRMWARE_YEAR          0x0007UL

//****** Read Only Memory Definition ******
//ROM (The data read from ROM seems to be an 8-bit long)
#define     V1190B_CHECKSUM                 0x4000UL//A32/D16 R
#define     V1190B_CKSM_LEN_MSB             0x4004UL//A32/D16 R
#define     V1190B_CKSM_LEN                 0x4008UL//A32/D16 R
#define     V1190B_CKSM_LEN_LSB             0x400CUL//A32/D16 R
#define     V1190B_CONSTANT_MSB             0x4010UL//A32/D16 R
#define     V1190B_CONSTANT                 0x4014UL//A32/D16 R
#define     V1190B_CONSTANT_LSB             0x4018UL//A32/D16 R
#define     V1190B_C_CODE                   0x401CUL//A32/D16 R
#define     V1190B_R_CODE                   0x4020UL//A32/D16 R
#define     V1190B_OUI_MSB                  0x4024UL//A32/D16 R   
#define     V1190B_OUI                      0x4028UL//A32/D16 R
#define     V1190B_OUI_LSB                  0x402CUL//A32/D16 R
#define     V1190B_VERSION                  0x4030UL//A32/D16 R
#define     V1190B_BOARD_ID_MSB             0x4034UL//A32/D16 R
#define     V1190B_BOARD_ID                 0x4038UL//A32/D16 R
#define     V1190B_BOARD_ID_LSB             0x403CUL//A32/D16 R
#define     V1190B_REVISION_MSB             0x4040UL//A32/D16 R
#define     V1190B_REVISION_M1              0x4044UL//A32/D16 R
#define     V1190B_REVISION_M2              0x4048UL//A32/D16 R
#define     V1190B_REVISION_LSB             0x404CUL//A32/D16 R
#define     V1190B_SERIAL_MSB               0x4080UL//A32/D16 R
#define     V1190B_SERIAL_LSB               0x4084UL//A32/D16 R

#define V1190B_COMPENSATION_SRAM            0x8000UL
#define V1190B_LUB                          0x8FFFUL            //the Last Used Byte
// MEMORY MAP (END)

#include <algorithm>
#include <vector>
#include <forward_list>

#include "VException.h"
#include "VSlaveAcquisitor.h"
#include "VSlaveInterrupter.h"
#include "VEvent.h"
#include "UConfigurable.h"

namespace vmepp {

    class V1190B;

    template<>
    struct UConfig<V1190B>
    {
    };
    /********************/
    /****** V1190B ******/
    /********************/
    class V1190B : public VSlaveInterrupter, public VSlaveAcquisitor<V1190B>, public UConfigurable<V1190B> {

        private :
            static uint8_t const fChNumber = 0x40;    // Number of channels 64
            static uint8_t const fTDCNumber = 2;      // Number of HPTDC chips

        public :
            static uint8_t constexpr GetChNumber() { return fChNumber; }
            static uint8_t constexpr GetTDCNumber() { return fTDCNumber; }

        public:
            enum class Command {
                //ACQUISITION MODE
                TRG_MATCH,                                      //set trigger matching
                CONT_STOR,                                      //set continuous storage
                READ_ACQ_MOD,                                   //read acquisition mode
                SET_KEEP_TOKEN,                                 //set keep_token
                CLEAR_KEEP_TOKEN,                               //clear keep token
                LOAD_DEF_CONFIG,                                //load default configuration
                SAVE_USER_CONFIG,                               //save User configuration
                LOAD_USER_CONFIG,                               //load User configuration
                AUTOLOAD_USER_CONF,                             //set auto load User configuration
                AUTOLOAD_DEF_CONF,                              //set auto load default configuration

                //TRIGGER
                SET_WIN_WIDTH = 0x10,                           //set window width
                SET_WIN_OFFS,                                   //set window offset
                SET_SW_MARGIN,                                  //set extra search margin
                SET_REJ_MARGIN,                                 //set reject margin
                EN_SUB_TRG,                                     //enable subtraction of trigger time
                DIS_SUB_TRG,                                    //disable subtraction of trigger time
                READ_TRG_CONF,                                  //read trigger configuration

                //TDC EDGE DETECTION & RESOLUTION
                SET_DETECTION = 0x22,                           //enable paired meas. leading/ trailing edge
                READ_DETECTION,                                 //read edge detection configuration
                SET_TR_LEAD_LSB,                                //set LSB of leading/trailing edge
                SET_PAIR_RES,                                   //set leading time and width res. when pair
                READ_RES,                                       //read resolution
                SET_DEAD_TIME = 0x28,                           //set channel dead time between hits
                READ_DEAD_TIME,                                 //read channel dead time between hits

                //TDC READOUT
                EN_HEAD_TRAILER = 0x30,                         //enable TDC header and TRAILER
                DIS_HEAD_TRAILER,                               //disable TDC header and TRAILER
                READ_HEAD_TRAILER,                              //read status TDC header and TRAILER
                SET_EVENT_SIZE,                                 //set maximum number of hits per event
                READ_EVENT_SIZE,                                //read maximum number of hits per event
                EN_ERROR_MARK,                                  //enable TDC error mark
                DIS_ERROR_MARK,                                 //disable TDC error mark
                EN_ERROR_BYPASS,                                //enable bypass TDC if error
                DIS_ERROR_BYPASS,                               //disable bypass TDC if error
                SET_ERROR_TYPES,                                //set TDC internal error type
                READ_ERROR_TYPES,                               //read TDC internal error type
                SET_FIFO_SIZE,                                  //set effective size of readout FIFO
                READ_FIFO_SIZE,                                 //read effective size of readout FIFO

                //CHANNEL ENABLE
                EN_CHANNEL = 0x40,                              //enable channel nn
                DIS_CHANNEL,                                    //disable channel nn
                EN_ALL_CH,                                      //enable all channels
                DIS_ALL_CH,                                     //disable all channels
                WRITE_EN_PATTERN,                               //write enable pattern for channels
                READ_EN_PATTERN,                                //read enable pattern for channels
                WRITE_EN_PATTERN32,                             //write 32 bit enable pattern for channels
                READ_EN_PATTERN32,                              //read 32 bit enable pattern for channels

                //ADJUST
                SET_GLOB_OFFS = 0x50,                           //set global offset
                READ_GLOB_OFFS,                                 //read global offset
                SET_ADJUST_CH,                                  //set channel nn adjust
                READ_ADJUST_CH,                                 //read channel nn adjust
                SET_RC_ADJ,                                     //Set RC adjust of tdc 0n
                READ_RC_ADJ,                                    //Read RC adjust of tdc 0n
                SAVE_RC_ADJ,                                    //save RC adjust on EEPROM

                //MISCELLANEOUS
                READ_TDC_ID = 0x60,                             //read programmed ID of TDC 0n
                READ_MICRO_REV,                                 //read firmware revision of microcontroller
                RESET_DLL_PLL,                                  //reset DLL and PLL

                //ADVANCED
                WRITE_SETUP_REG = 0x70,                         //write word nn into the scan path setup
                READ_SETUP_REG,                                 //read word nn into the scan path setup
                UPDATE_SETUP_REG,                               //load the scan path setup
                DEFAULT_SETUP_REG,                              //reload the default scan path setup
                READ_ERROR_STATUS,                              //read errors in the TDC 0n status
                READ_DLL_LOCK,                                  //read the DLL LOCK bit of the TDC 0n
                READ_STATUS_STREAM,                             //read the TDC 0n status
                UPDATE_SETUP_TDC,                               //load the scan path setup on TDC nn

                //DEBUG AND TEST
                WRITE_EEPROM = 0xC0,                            //write 1 byte into the EEPROM
                READ_EEPROM,                                    //read 1 byte from the EEPROM
                REVISION_DATE_CONTROLLER_FW,                    //read the controller firmware revision/date
                WRITE_SPARE,                                    //write a 16 bit spare variable
                READ_SPARE,                                     //read a 16 bit spare variable
                ENABLE_TEST_MODE,                               //enable TDC test mode
                DISABLE_TEST_MODE,                              //disable TDC test mode
                SET_TDC_TEST_OUTPUT,                            //set TDC 0n signal test output
                SET_DLL_CLOCK,                                  //set DLL clock source
                READ_TDC_SETUP_SCAN_PATH                        //read all Setup Scan Path on TDC 0n
            };

            struct Opcode {
                Command command;
                uint8_t object;

                Opcode(Command command,
                       uint8_t object = 0x00) :
                        command(command),
                        object(object) {}
            };

            enum class TriggerMode_t {
                MATCHING, CONTINUOUS
            };

            enum class Config_t {
                DEFAULT, USER
            };

            enum class MaxHitsPerEvent : uint16_t {
                n0, n1, n2, n4, n8, n16, n32, n64, n128,
                NO_LIMIT = 0b1001,
                INVALID = 0b1010, // Actually, any number >= INVALID is meaningless
            };

            enum class FIFOSize : uint16_t {
                w2, w4, w8, w16, w32, w64, w128, w256
            };

            enum IError_t : uint16_t {
                // NB : it is not an enum class because we want to be able
                // to perform bitwise operations on these enumerations
                // without complex overloading of bitwise operators and static_cast
                VERNIER         = 0x0001,
                CRS_CNTR_PRTY   = 0x0002,
                SYNC            = 0x0004,
                L1_BUF_PRTY     = 0x0008,
                TRG_FIF0_PRTY   = 0x0010,
                TRG_MTCH        = 0x0020,
                ROUT_FIFO_PRTY  = 0x0040,
                ROUT_STATE      = 0x0080,
                SET_UP_PRTY     = 0x0100,
                CTRL_PRTY       = 0x0200,
                JTAG_PRTY       = 0x0400,
                ALL             = 0x07FF, // NB : Use ONLY fully qualified IError_t::ALL
            };

        public:
            V1190B(uint32_t address, uint32_t range = V1190B_LUB);
            ~V1190B() override = default;

        protected:
            void Initialize() override;

            /******************************/
            /****** DATA ACQUISITION ******/
            /******************************/
        public:
            struct EventFIFO
            {
                uint16_t eventCount;
                uint16_t wordCount;
            };
            EventFIFO ReadEventFIFO();
            uint16_t ReadEventFIFOStored();

            void WriteTestreg( uint32_t data );
            uint32_t ReadTestreg();
            void WriteEventCountReset();
            void WriteEventsBLT( uint8_t n );
            uint8_t ReadEventsBLT();
            uint32_t ReadEventCounter();
            uint16_t ReadEventsStored();
            void SendSWTrigger();

            bool GetEventAt(uint32_t index, UEvent<V1190B> &event) const override;
            uint32_t GetBufferAddress() const override { return 0; }

            /************************/
            /****** INTERRUPTS ******/
            /************************/
        public:
            void WriteIRQEvents(uint16_t n);
            uint16_t ReadIRQEvents();
            void WriteIRQLevel(uint16_t level) override;
            uint16_t ReadIRQLevel() override;
            void WriteIRQVector(uint16_t vector) override;
            uint16_t ReadIRQVector() override;
            void ISR(uint16_t vector) override;

            /****************************/
            /****** RESET & CONTROL******/
            /****************************/
        public:
            void Reset() override;
            void WriteSoftwareClear();
            void WriteSingleShotReset();

            enum class Control_t : uint16_t {
                BERR_EN,
                TERM,
                TERM_SW,
                EMPTY_EVENT,

                ALIGN64,
                COMPENSATION_EN,
                TEST_FIFO_EN,
                READ_COMPENSATION,

                EVENT_FIFO_EN,
                ETTT_EN,
                MB16_ADDR_EN = 12
            };

            enum class Status_t : uint16_t {
                DATA_READY,
                ALMOST_FULL,
                FULL,
                TRG_MATCH,

                HEADER_EN,
                TERM_ON,
                ERROR0,
                ERROR1,

                ERROR2,
                ERROR3,
                BERR_FLAG,
                PURG,

                RES0,
                RES1,
                PAIR,
                TRIGGER_LOST
            };

            void WriteControl( V1190B::Control_t bit, bool status = true );
            uint16_t ReadControl();
            bool ReadControl( V1190B::Control_t bit );

            bool ReadStatus( V1190B::Status_t bit );
            uint16_t ReadStatus();

            /*******************/
            /****** MICRO ******/
            /*******************/
        protected:
            void WriteMicro(uint16_t data);
            void WriteMicro(Opcode opcode);
            uint16_t ReadMicro();

            /******************************/
            /****** ACQUISITION MODE ******/
            /******************************/
        public :
            void WriteAcqMode(V1190B::TriggerMode_t mode);
            V1190B::TriggerMode_t ReadAcqMode();
            void WriteEnableKeepToken(bool status = true);
            void WriteLoadConfig(V1190B::Config_t config);
            void WriteAutoLoad(V1190B::Config_t config);
            void WriteSaveUserConfig();

            /*********************/
            /****** TRIGGER ******/
            /*********************/
        public:
            struct TriggerData {
                uint16_t winWidth;
                uint16_t winOffs;
                uint16_t swMargin;
                uint16_t rejMargin;
                uint16_t subTrigger;

                TriggerData(uint16_t winWidth = 0x0014,
                            uint16_t winOffs = 0xFFD8,
                            uint16_t swMargin = 0x0008,
                            uint16_t rejMargin = 0x0004,
                            uint16_t subTrigger = 0x0000) :
                        winWidth(winWidth),
                        winOffs(winOffs),
                        swMargin(swMargin),
                        rejMargin(rejMargin),
                        subTrigger(subTrigger) {}
            };

            void WriteWindowWidth(uint16_t data);
            void WriteWindowOffset(uint16_t data);
            void WriteExtraSearchMargin(uint16_t data);
            void WriteRejectMargin(uint16_t data);
            void WriteEnableSubTrigger(bool data);
            void ReadTriggerConfiguration(TriggerData &trigger);

            /********************/
            /****** ADJUST ******/
            /********************/
        public:
            struct AdjustOffset {
                uint16_t coarseCounter;
                uint16_t fineCounter;

                AdjustOffset(uint16_t coarseCounter = 0x0000,
                             uint16_t fineCounter = 0x0000) :
                        coarseCounter(coarseCounter),
                        fineCounter(fineCounter) {}
            };

            void WriteGlobalOffset(AdjustOffset adjustOffset);
            void ReadGlobalOffset(AdjustOffset &adjustOffset);
            void WriteAdjustChannel(uint16_t offset, uint8_t channel);
            uint16_t ReadAdjustChannel(uint8_t channel);

            /*********************************************/
            /****** TDC EDGE DETECTION & RESOLUTION ******/
            /*********************************************/
        public:
            enum class EdgeDetect_t {
                PAIR,
                TRAILING,
                LEADING,
                TRAILINGLEADING
            };

            enum class TrLeadLSB {
                ps800,
                ps200,
                ps100,
                UNUSED
            };

            enum class ResLeadEdgeTime {
                ps100,
                ps200,
                ps400,
                ps800,
                ps1600,
                ps3120,
                ps6250,
                ps12500
            };

            enum class ResPulseWidth {
                ps100,
                ps200,
                ps400,
                ps800,
                ps1600,
                ps3120,
                ps6250,
                ps12500,
                ns25,
                ns50,
                ns100,
                ns200,
                ns400,
                ns800,
                INVALID_1,
                INVALID_2
            };

            enum class DeadTime {
                ns5,
                ns10,
                ns30,
                ns100
            };

            struct PairRes {
                ResLeadEdgeTime edgeTime;
                ResPulseWidth width;

                PairRes(ResLeadEdgeTime edgeTime = ResLeadEdgeTime::ps100,
                        ResPulseWidth width = ResPulseWidth::ps100) :
                        edgeTime(edgeTime),
                        width(width) {}
            };

            void WriteDetection(EdgeDetect_t detect);
            EdgeDetect_t ReadDetection();
            void WriteEdgeRes(TrLeadLSB lsb);
            TrLeadLSB ReadEdgeRes();
            void WritePairRes(PairRes pairRes);
            void ReadPairRes(PairRes &pairRes);
            void WriteDeadTime(DeadTime time);
            DeadTime ReadDeadTime();

            /****************************/
            /****** TEST AND DEBUG ******/
            /****************************/
        private:
            static const std::array<uint8_t, 33> fEEPROM;

        public:
            void WriteEEPROM(uint16_t address, uint16_t data);
            uint16_t ReadEEPROM(uint16_t address);

            struct MicroFirmware {
                uint16_t revision;
                uint16_t day;
                uint16_t month;
                uint16_t year;

                MicroFirmware() {}
            };

            void ReadMicroFirmware(MicroFirmware &firmware);
            void WriteSpare(uint16_t data);
            uint16_t ReadSpare();
            void EnableTestMode(uint32_t data);
            void DisableTestMode();
            void WriteTDCTestOutput(uint16_t data);

            enum class DLLClock {
                DIRECT40MHz,
                PLL40MHz,
                PLL160MHz,
                PLL320MHz
            };

            void WriteDLLClock(DLLClock dllClock);

            /*********************/
            /****** CHANNEL ******/
            /*********************/
        public :
            enum class TDC {
                TDC0, TDC1
            };

            void WriteEnableChannel(uint8_t n, bool status = true);
            void WriteEnableAll(bool status = true);
            void WriteEnablePattern(V1190B::TDC tdc, uint32_t pattern);
            uint32_t ReadEnablePattern(V1190B::TDC tdc);

            /*************************/
            /****** TDC READOUT ******/
            /*************************/
        public :
            void WriteEnableHeaderTrailer(bool status = true);
            bool ReadEnableHeaderTrailer();
            void WriteMaxHitsPerEvent(V1190B::MaxHitsPerEvent n);
            V1190B::MaxHitsPerEvent ReadMaxHitsPerEvent();
            void WriteEnableErrMark(bool status = true );
            void WriteEnableBypass(bool status = true );
            void WriteErrorPattern(uint16_t);
            uint16_t ReadErrorPattern();
            void WriteEffSizeFIFO(V1190B::FIFOSize size);
            uint16_t ReadEffSizeFIFO();

            /******************/
            /****** MISC ******/
            /******************/
        public :
            void WriteResetLoops();
            uint32_t ReadTDC_ID(V1190B::TDC tdc);
            uint16_t ReadMicroFWRev();

            /*****************/
            /****** ROM ******/
            /*****************/
        protected :
            //ROM Data
            std::string fFirmwareRevision;
            uint32_t fOUI;
            uint16_t fVersion;
            uint32_t fBoardID;
            uint32_t fRevision;

        protected :
            uint16_t ReadVersion();
            uint32_t ReadBoardID();
            uint32_t ReadOUI();
            uint32_t ReadRevision();
            uint16_t ReadSerialNumber();
            uint16_t ReadFirmRevNumber();//Actually not ROM

            /******************/
            /****** MISC ******/
            /******************/
        public :
            void WriteDummy32(uint32_t data);
            uint32_t ReadDummy32();
            void WriteDummy16(uint16_t data);
            uint16_t ReadDummy16();

            enum class ProgOUT {
                DATA_READY,
                FULL,
                ALM_FULL,
                ERROR
            };

            void WritePOut(V1190B::ProgOUT pout);
            ProgOUT ReadPOut();

            //State methods 
        public :
            //ROM
            std::string GetFirmRevision() const { return fFirmwareRevision; }//Not ROM
            uint32_t GetOUI() const { return fOUI; }
            uint16_t GetVersion() const { return fVersion; }
            uint32_t GetBoardID() const { return fBoardID; }
            uint32_t GetRevision() const { return fRevision; }

            /***********************/
            /****** LUT (INL) ******/
            /***********************/
        public:
            void EnableReadoutSRAM(bool status = true);
            void ReadCompensation(TrLeadLSB lsb, uint8_t channel, std::vector<int8_t> &data);

        public :
            void    ReadConfig( UConfig<V1190B>& config ) override;
            void    WriteConfig( const UConfig<V1190B>& config ) override;
    };// V1190B

    template<>
    const std::string UConfigurable<V1190B>::fName;

    /*************************/
    /****** V1190BEvent ******/
    /*************************/
    template<>
    class UEvent<V1190B> : public VEvent
    {
        public :
            enum Word_t
            {
                G_HEADER    = 0x40000000U,   // Global Header
                G_TRAILER   = 0x80000000U,   // Global Trailer
                T_HEADER    = 0x08000000U,   // TDC Header
                T_MEAS      = 0x00000000U,   // TDC Measurement
                T_ERROR     = 0x20000000U,   // TDC Error
                T_TRAILER   = 0x18000000U,   // TDC Trailer
                G_TTT       = 0x88000000U,   // Global Trigger Time Tag
                FILLER      = 0xC0000000U,   // FILLER
                MASK        = 0xf8000000U,
            };

            struct Hit
            {
                uint32_t value;

                uint32_t GetEdge() const    { return (value & 0x00007ffffU); }
                uint32_t GetWidth() const   { return ((value & 0x00007f000U) >> 12U); }
                uint32_t GetLead() const    { return (value & 0x000000fffU); }
                uint8_t  GetChannel() const { return ((value & 0x003f80000U) >> 19U); }
                bool     IsLeading() const  { return (value & (1U << 26U)); }

                Hit( uint32_t value ) :
                    value( value )
                {
                }
            };

        protected :
            uint32_t fGlobalHeader;
            uint32_t fETTT;
            uint32_t fErrors;
            uint32_t fGlobalTrailer;
            std::vector<Hit>   fHits;

        public :
            typedef typename std::vector<Hit>::const_iterator const_iterator;
            const_iterator cbegin() const noexcept  { return fHits.cbegin(); }
            const_iterator cend() const noexcept    { return fHits.cend(); }

        public :
            UEvent() { };
            ~UEvent() override = default;
            //UEvent( const UEvent& other );
            //UEvent& operator=( UEvent other );
            size_t GetNHits()           { return fHits.size(); }
            uint32_t GetEventCount()    { return ((fGlobalHeader & 0x07ffffe0U) >> 5U); }
            uint32_t GetETTT()          { return (fETTT & 0x07ffffffU); }
            uint32_t GetWordCount()     { return ((fGlobalTrailer & 0x001fffeU) >> 5U); }

        friend class V1190B;
    };// UEvent<V1990B>
}// vmepp

#endif
