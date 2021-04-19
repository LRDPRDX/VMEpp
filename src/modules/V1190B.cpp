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

    // TRIGGER
     void V1190B::WriteWindowWidth(uint16_t data){
        WriteMicro(Opcode(Command::SET_WIN_WIDTH));
        WriteMicro(data);
    }

    void V1190B::WriteWindowOffset(uint16_t data){
        WriteMicro(Opcode(Command::SET_WIN_OFFS));
        WriteMicro(data);
     }

    void V1190B::WriteExtraSearchMargin(uint16_t data){
        WriteMicro(Opcode(Command::SET_SW_MARGIN));
        WriteMicro(data);
     }

    void V1190B::WriteRejectMargin(uint16_t data){
        WriteMicro(Opcode(Command::SET_REJ_MARGIN));
        WriteMicro(data);
     }

    void V1190B::WriteEnableSubTrigger(bool data){
        if (data)
            WriteMicro(Opcode(Command::EN_SUB_TRG));
        else 
            WriteMicro(Opcode(Command::DIS_SUB_TRG));
    }

    void V1190B::ReadTriggerConfiguration(TriggerData& trigger){
        WriteMicro(Opcode(Command::READ_TRG_CONF));

        trigger.winWidth = ReadMicro();
        trigger.winOffs = ReadMicro();
        trigger.swMargin = ReadMicro();
        trigger.rejMargin = ReadMicro();
        trigger.subTrigger = ReadMicro();
    }

    // ADJUST
    void V1190B::WriteGlobalOffset(AdjustOffset adjustOffset) {
        WriteMicro(Opcode(Command::SET_GLOB_OFFS));
        WriteMicro(adjustOffset.coarseCounter & 0x0FFF);
        WriteMicro(adjustOffset.fineCounter & 0x001F);
    }

    void V1190B::ReadGlobalOffset(AdjustOffset& adjustOffset) {
        WriteMicro(Opcode(Command::READ_GLOB_OFFS));
        adjustOffset.coarseCounter = ReadMicro() & 0x0FFF;
        adjustOffset.fineCounter = ReadMicro() & 0x001F;
    }

    void V1190B::WriteAdjustChannel(uint16_t offset, uint8_t channel) {
        WriteMicro(Opcode(Command::SET_ADJUST_CH, channel % fChNumber));
        WriteMicro(offset & 0x00FF);
    }

    uint16_t V1190B::ReadAdjustChannel(uint8_t channel) {
        WriteMicro(Opcode(Command::READ_ADJUST_CH, channel % fChNumber));
        return ReadMicro() & 0x00FF;
    }

    // TDC EDGE DETECTION & RESOLUTION
    void V1190B::WriteDetection(EdgeDetect_t detect) {
        WriteMicro(Opcode(Command::SET_DETECTION));
        WriteMicro(static_cast<uint16_t>(detect) & 0x0003);
    }

    V1190B::EdgeDetect_t V1190B::ReadDetection() {
        WriteMicro(Opcode(Command::READ_DETECTION));
        return static_cast<EdgeDetect_t>(ReadMicro() & 0x0003);
    }

    void V1190B::WriteEdgeRes(TrLeadLSB lsb) {
        WriteMicro(Opcode(Command::SET_TR_LEAD_LSB));
        WriteMicro(static_cast<uint16_t>(lsb) & 0x0003);
    }

    void V1190B::WritePairRes(PairRes pairRes) {
        WriteMicro(Opcode(Command::SET_PAIR_RES));
        WriteMicro(((static_cast<uint16_t>(pairRes.width) << 8U) |
                    (static_cast<uint16_t>(pairRes.edgeTime))) & 0x0F07);
    }

    V1190B::TrLeadLSB V1190B::ReadEdgeRes() {
        WriteMicro(Opcode(Command::READ_RES));
        return static_cast<TrLeadLSB>(ReadMicro() & 0x0003);
    }

    void V1190B::ReadPairRes(PairRes& pairRes) {
        WriteMicro(Opcode(Command::READ_RES));
        uint16_t data = ReadMicro();
        pairRes.edgeTime = static_cast<ResLeadEdgeTime>(data & 0x0007);
        pairRes.width = static_cast<ResPulseWidth>((data & 0x0F00) >> 8U);
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
