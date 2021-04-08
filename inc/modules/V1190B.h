#ifndef V_PLUS_V1190B_H
#define V_PLUS_V1190B_H

#define V1190B_OUTPUT_BUFFER                0X0000UL
#define V1190B_OUTPUT_BUFFER_MAX            0x0FFCUL
#define V1190B_CONTROL_REGISTER             0x1000UL
#define V1190B_STATUS_REGISTER              0X1002UL
#define V1190B_INTERRUPT_LEVEL              0x100AUL
#define V1190B_INTERRUPT_VECTOR             0x100CUL
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
#define V1190B_WRITE_OK_MSK                 (1<<0)              //the VME (master) tests the WRITE_OK bit in the Micro Handshake Register if the WO bit is set to 1, the VME can write a datum
#define V1190B_READ_OK_MSK                  (1<<1)              //the VME (master) tests the READ_OK bit in the Micro Handshake Register  if the RO bit is set to 1, the VME can read a datum
#define V1190B_SEL_FLASH                    0x1032UL
#define V1190B_FLASH                        0x1034UL
#define V1190B_COMPENSATION_SRAM_PAGE       0x1036UL
#define V1190B_EVENT_FIFO                   0x1038UL
#define V1190B_EVENT_FIFO_STORED            0x103CUL
#define V1190B_EVENT_FIFO_STATUS            0x103EUL
#define V1190B_DUMMY_32                     0x1200UL
#define V1190B_DUMMY_16                     0x1204UL

#define V1190B_LUB                          0x8FFFUL            //the Last Used Byte 

#include "VException.h"
#include "VSlaveAcquisitor.h"
#include "VSlaveInterrupter.h"
#include "VEvent.h"

namespace vmeplus {

    class V1190B : public VSlaveInterrupter, public VSlaveAcquisitor {

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
            EN_HEAD_TRAILER = 0x30,                                //enable TDC header and TRAILER
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

            Opcode(Command command, uint8_t object = 0x00):
                command(command), object(object) {}
        };

        struct TriggerData {
            uint16_t WinWidth;
            uint16_t WinOffs;
            uint16_t SwMargin;
            uint16_t RejMargin;
            uint16_t SubTrigger;
            
            TriggerData(uint16_t WinWidth, uint16_t WinOffs, uint16_t SwMargin,
                    uint16_t RejMargin, uint16_t SubTrigger):
                WinWidth(WinWidth), WinOffs(WinOffs), SwMargin(SwMargin),
                RejMargin(RejMargin), SubTrigger(SubTrigger) {}
        };

    public:
        V1190B( uint32_t address, uint32_t range = V1190B_LUB );

        virtual ~V1190B();

    protected:
        void Initialize() override;

        void Release() override;

        // Data acquisition
    public:
        void AllocateBuffer() override;

        uint32_t ReadBuffer() override;

        bool GetEvent(VEvent *event) override;

        bool GetEventAt(uint32_t index, VEvent *event) const override;

        // Interrupts
    public:
        void WriteIRQLevel(uint16_t level) override;

        uint16_t ReadIRQLevel() override;

        void WriteIRQVector(uint16_t vector) override;

        uint16_t ReadIRQVector() override;

        void ISR(uint16_t vector) override;

        // Misc
    public:
        void Reset() override;

        //MICRO
    public:
        void WriteMicro(uint16_t data);

        void WriteMicro(Opcode opcode);

        uint16_t ReadMicro();

    // Trigger functions
    public:
        void WriteWindowWidth(uint16_t data);
        void WriteWindowOffset(uint16_t data);
        void WriteExtraSearchMargin(uint16_t data);
        void WriteRejectMargin(uint16_t data);
        void WriteSubstractionOfTriggerTime(bool data);
        void ReadTriggerConfiguration(TriggerData& trigger);
    };
}

#endif
