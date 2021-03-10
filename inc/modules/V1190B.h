#ifndef V_PLUS_V1190B_H
#define V_PLUS_V1190B_H

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
#define V1190B_SEL_FLASH                    0x1032UL
#define V1190B_FLASH                        0x1034UL
#define V1190B_COMPENSATION_SRAM_PAGE       0x1036UL
#define V1190B_EVENT_FIFO                   0x1038UL
#define V1190B_EVENT_FIFO_STORED            0x103CUL
#define V1190B_EVENT_FIFO_STATUS            0x103EUL
#define V1190B_DUMMY_32                     0x1200UL
#define V1190B_DUMMY_16                     0x1204UL

#include "VSlaveAcquisitor.h"
#include "VSlaveInterrupter.h"
#include "VEvent.h"

namespace vmeplus {

    class V1190B : public VSlaveInterrupter, public VSlaveAcquisitor {
    public:
        V1190B(std::string name, uint32_t address, uint32_t range);

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

        // Register
    public:
        void WriteRegister(uint32_t data);

        uint32_t ReadRegister();

        // OPCODE
    public:
        bool WriteOpcode(uint16_t opcode);

        bool ReadOpcode(uint16_t opcode);
    };
}

#endif
