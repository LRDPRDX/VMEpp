#include "CAENVMElib.h"
#include "VException.h"
#include "modules/V1190B.h"

namespace vmeplus {

    void V1190B::Initialize() {}

    void V1190B::Release() {}

    void V1190B::AllocateBuffer() {}

    uint32_t V1190B::ReadBuffer() { return 0; }

    bool V1190B::GetEvent(VEvent *event) { return 0; }

    bool V1190B::GetEventAt(uint32_t index, VEvent *event) const { return 0; }

    void V1190B::WriteIRQLevel(uint16_t level) {}

    uint16_t V1190B::ReadIRQLevel() { return 0; }

    void V1190B::WriteIRQVector(uint16_t vector) {}

    uint16_t V1190B::ReadIRQVector() { return 0; }

    void V1190B::ISR(uint16_t vector) {}

    void V1190B::Reset() {}

    void V1190B::WriteOpcode(uint16_t opcode) {
        if (V1190B::ReadRegister16(V1190B_BASE_ADDRESS +V1190B_MICRO_HANDSHAKE) == 1) {
            V1190B::WriteRegister16(V1190B_BASE_ADDRESS +V1190B_MICRO, opcode);
        } else {

        }
    }

    uint16_t V1190B::ReadOpcode() {
        uint16_t opcode;
        if (V1190B::ReadRegister16(V1190B_BASE_ADDRESS +V1190B_MICRO_HANDSHAKE) == 1) {
            V1190B::ReadRegister16(V1190B_BASE_ADDRESS +V1190B_MICRO, opcode);
        } else {

        }
        return opcode;
    }
}
