#include "CAENVMElib.h"
#include "modules/V1190B.h"

namespace vmeplus {
    V1190B::V1190B(uint32_t baseAddress, uint32_t range) :
            VSlave("V1190B", baseAddress, range),
            VSlaveInterrupter("V1190B", baseAddress, range),
            VSlaveAcquisitor("V1190B", baseAddress, range) {}

    V1190B::~V1190B() {};

    void V1190B::Initialize() {
        PrintMessage(Message_t::INFO, "Inititalizing " + fName + "...");
        PrintMessage(Message_t::INFO, "Inititalizing " + fName + "...OK");
    }

    void V1190B::Release() {}

    void V1190B::AllocateBuffer() {}

    uint32_t V1190B::ReadBuffer() { return 0; }

    bool V1190B::GetEvent(VEvent *event) { return 0; }

    bool V1190B::GetEventAt(uint32_t index, VEvent *event) const { return 0; }

    //INTERRUPTER
    void V1190B::WriteIRQLevel(uint16_t level) {
        WriteRegister16(V1190B_INTERRUPT_LEVEL, level, V1190B_INTERRUPT_LEVEL_MSK);
    }

    uint16_t V1190B::ReadIRQLevel() {
        return ReadRegister16(V1190B_INTERRUPT_LEVEL, V1190B_INTERRUPT_LEVEL_MSK);
    }

    void V1190B::WriteIRQVector(uint16_t vector) {
        WriteRegister16(V1190B_INTERRUPT_VECTOR, vector, V1190B_INTERRUPT_VECTOR_MSK);
    }

    uint16_t V1190B::ReadIRQVector() {
        return ReadRegister16(V1190B_INTERRUPT_VECTOR, V1190B_INTERRUPT_VECTOR_MSK);
    }

    void V1190B::ISR(uint16_t vector) {}

    void V1190B::Reset() {}

    void V1190B::WriteMicro(uint16_t data) {
        //the VME (master) tests the WRITE_OK bit in the Micro Handshake Register
        // if the WO bit is set to 1, the VME can write a datum
        while (!(ReadRegister16(V1190B_MICRO_HANDSHAKE, V1190B_WRITE_OK_MSK))) {}
        WriteRegister16(V1190B_MICRO, data);
    }

    void V1190B::WriteMicro(Opcode opcode) {
        uint16_t code = static_cast<uint16_t>(opcode.command) << 8U | opcode.object;

        //the VME (master) tests the WRITE_OK bit in the Micro Handshake Register
        // if the WO bit is set to 1, the VME can write a datum
        while (!(ReadRegister16(V1190B_MICRO_HANDSHAKE, V1190B_WRITE_OK_MSK))) {}
        WriteRegister16(V1190B_MICRO, code);
    }

    uint16_t V1190B::ReadMicro() {
        //a valid datum can be read via VME only if the READ_OK (RO) bit in the Micro
        //Handshake Register is set to 1
        while (!(ReadRegister16(V1190B_MICRO_HANDSHAKE, V1190B_READ_OK_MSK))) {}
        return ReadRegister16(V1190B_MICRO);
    }

    void V1190B::WriteDetection(EdgeDetect detect) {
        WriteMicro(Opcode(Command::SET_DETECTION));
        WriteMicro(static_cast<uint16_t>(detect) & 0x0003);
    }

    V1190B::EdgeDetect V1190B::ReadDetection() {
        WriteMicro(Opcode(Command::READ_DETECTION));
        return static_cast<EdgeDetect>(ReadMicro() & 0x0003);
    }

    void V1190B::WriteLSB(EdgeLSB lsb) {
        EdgeDetect edgeDetect = ReadDetection();
        if ((edgeDetect == EdgeDetect::TRAILING) or (edgeDetect == EdgeDetect::LEADING) or (edgeDetect == EdgeDetect::TRAILINGLEADING)) {
            WriteMicro(Opcode(Command::SET_TR_LEAD_LSB));
            WriteMicro(static_cast<uint16_t>(lsb) & 0x0003);
        }
    }

    void V1190B::WritePairRes(PairRes pairRes) {
        if (ReadDetection() == EdgeDetect::PAIR) {
            WriteMicro(Opcode(Command::SET_PAIR_RES));
            WriteMicro(((static_cast<uint16_t>(pairRes.width) << 8U) |
                        (static_cast<uint16_t>(pairRes.edgeTime) & 0x0007)) & 0x0F07);
        }
    }

    uint16_t V1190B::ReadTDCRes() {
        EdgeDetect edgeDetect = ReadDetection();
        if ((edgeDetect == EdgeDetect::TRAILING) or (edgeDetect == EdgeDetect::LEADING) or (edgeDetect == EdgeDetect::TRAILINGLEADING)) {
            WriteMicro(Opcode(Command::READ_RES));
            return (ReadMicro() & 0x0003);
        }
        if (edgeDetect == EdgeDetect::PAIR) {
            WriteMicro(Opcode(Command::READ_RES));
            return (ReadMicro() & 0x0F07);
        }
    }

    void V1190B::WriteDeadTime(DeadTime time) {
        WriteMicro(Opcode(Command::SET_DEAD_TIME));
        WriteMicro(static_cast<uint16_t>(time) & 0x0003);
    }

    V1190B::DeadTime V1190B::ReadDeadTime() {
        WriteMicro(Opcode(Command::READ_DEAD_TIME));
        return static_cast<DeadTime>(ReadMicro() & 0x0003);
    }
}
