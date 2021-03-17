#include <string>

#include "CAENVMElib.h"
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

    void V1190B::WriteMicro(uint16_t opcode) {
        uint16_t writeOkMsk = 0x0001;
        while (V1190B::ReadRegister16(V1190B_BASE_ADDRESS + V1190B_MICRO_HANDSHAKE, writeOkMsk) != writeOkMsk) {}
        V1190B::WriteRegister16(V1190B_BASE_ADDRESS + V1190B_MICRO, opcode);
    }

    void V1190B::WriteMicro(Opcode opcode) {
        uint16_t code = std::stoi(std::to_string(static_cast<uint8_t>(opcode.command)) + std::to_string(opcode.obj));
        uint16_t writeOkMsk = 0x0001;
        while (V1190B::ReadRegister16(V1190B_BASE_ADDRESS + V1190B_MICRO_HANDSHAKE, writeOkMsk) != writeOkMsk) {}
        V1190B::WriteRegister16(V1190B_BASE_ADDRESS + V1190B_MICRO, code);
    }

    uint16_t V1190B::ReadMicro() {
        uint16_t readOkMsk = 0x0002;
        while (V1190B::ReadRegister16(V1190B_BASE_ADDRESS + V1190B_MICRO_HANDSHAKE, readOkMsk) != readOkMsk) {}
        return ReadRegister16(V1190B_BASE_ADDRESS + V1190B_MICRO);
    }
}
