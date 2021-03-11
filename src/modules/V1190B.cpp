#include "modules/V1190B.h"

namespace vmeplus {


    void V1190B::Initialize() {}

    void V1190B::Release() {}

    void V1190B::AllocateBuffer() {}

    uint32_t V1190B::ReadBuffer() {return 0;}

    bool V1190B::GetEvent(VEvent *event) {return 0;}

    bool V1190B::GetEventAt(uint32_t index, VEvent *event) const {return 0;}

    void V1190B::WriteIRQLevel(uint16_t level) {}

    uint16_t V1190B::ReadIRQLevel() { return 0; }

    void V1190B::WriteIRQVector(uint16_t vector) {}

    uint16_t V1190B::ReadIRQVector() { return 0; }

    void V1190B::ISR(uint16_t vector) {}

    void V1190B::Reset() {}

    void WriteRegister(uint32_t data) {}

    uint32_t ReadRegister() { return 0;}

    void WriteOpcode(uint16_t opcode) {
    }

    uint16_t ReadOpcode(uint16_t opcode) {

        return 0; }
}
