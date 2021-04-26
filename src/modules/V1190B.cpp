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

    // ACQUISITION MODE
    void V1190B::WriteAcqMode(V1190B::TriggerMode_t mode) {
        switch (mode) {
            case (TriggerMode_t::MATCHING) :
                WriteMicro(Opcode(Command::TRG_MATCH));
                break;
            case (TriggerMode_t::CONTINUOUS) :
                WriteMicro(Opcode(Command::CONT_STOR));
                break;
        }
    }

    V1190B::TriggerMode_t V1190B::ReadAcqMode() {
        WriteMicro(Opcode(Command::READ_ACQ_MOD));
        uint16_t data = ReadMicro();
        if (1U & data) return TriggerMode_t::MATCHING;     //check LSB
        else return TriggerMode_t::CONTINUOUS;
    }

    void V1190B::WriteEnableKeepToken(bool status) {
        if (status) WriteMicro(Opcode(Command::SET_KEEP_TOKEN));
        else WriteMicro(Opcode(Command::CLEAR_KEEP_TOKEN));
    }

    void V1190B::WriteLoadConfig(V1190B::Config_t config) {
        switch (config) {
            case (Config_t::DEFAULT) :
                WriteMicro(Opcode(Command::LOAD_DEF_CONFIG));
                break;
            case (Config_t::USER) :
                WriteMicro(Opcode(Command::LOAD_USER_CONFIG));
                break;
        }
    }

    void V1190B::WriteAutoLoad(V1190B::Config_t config) {
        switch (config) {
            case (Config_t::DEFAULT) :
                WriteMicro(Opcode(Command::AUTOLOAD_DEF_CONF));
                break;
            case (Config_t::USER) :
                WriteMicro(Opcode(Command::AUTOLOAD_USER_CONF));
                break;
        }
    }

    void V1190B::WriteSaveUserConfig() {
        WriteMicro(Opcode(Command::SAVE_USER_CONFIG));
    }

    // TRIGGER
    void V1190B::WriteWindowWidth(uint16_t data) {
        WriteMicro(Opcode(Command::SET_WIN_WIDTH));
        WriteMicro(data);
    }

    void V1190B::WriteWindowOffset(uint16_t data) {
        WriteMicro(Opcode(Command::SET_WIN_OFFS));
        WriteMicro(data);
    }

    void V1190B::WriteExtraSearchMargin(uint16_t data) {
        WriteMicro(Opcode(Command::SET_SW_MARGIN));
        WriteMicro(data);
    }

    void V1190B::WriteRejectMargin(uint16_t data) {
        WriteMicro(Opcode(Command::SET_REJ_MARGIN));
        WriteMicro(data);
    }

    void V1190B::WriteEnableSubTrigger(bool data) {
        if (data)
            WriteMicro(Opcode(Command::EN_SUB_TRG));
        else
            WriteMicro(Opcode(Command::DIS_SUB_TRG));
    }

    void V1190B::ReadTriggerConfiguration(TriggerData &trigger) {
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

    void V1190B::ReadGlobalOffset(AdjustOffset &adjustOffset) {
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

    void V1190B::ReadPairRes(PairRes &pairRes) {
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

    // TEST AND DEBUG
    const std::array<uint8_t, 39> V1190B::fEEPROM = {0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
                                                     0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
                                                     0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                                                         0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E,
                                                         0x1F, 0x20, 0x21, 0x22,0x23, 0x24, 0x25, 0x26, 0x27, 0x28};

    void V1190B::WriteEEPROM(uint16_t address, uint16_t data) {
        if (std::find(fEEPROM.begin(), fEEPROM.end(), address) == fEEPROM.end()) {
            WriteMicro(Opcode(Command::WRITE_EEPROM));
            WriteMicro(address);
            WriteMicro(data & 0x00FF);
        } else
            PrintMessage(Message_t::ERROR, "Invalid EEPROM address");
    }

    uint16_t V1190B::ReadEEPROM(uint16_t address) {
        WriteMicro(Opcode(Command::READ_EEPROM));
        WriteMicro(address);
        return ReadMicro() & 0x00FF;
    }

    void V1190B::ReadMicroFirmware(MicroFirmware &firmware) {
        WriteMicro(Opcode(Command::REVISION_DATE_CONTROLLER_FW));
        firmware.revision = ReadMicro();
        firmware.day = ReadMicro();
        firmware.month = ReadMicro();
        firmware.year = ReadMicro();
    }

    void V1190B::WriteSpare(uint16_t data) {
        WriteMicro(Opcode(Command::WRITE_SPARE));
        WriteMicro(data);
    }

    uint16_t V1190B::ReadSpare() {
        WriteMicro(Opcode(Command::READ_SPARE));
        return ReadMicro();
    }

    void V1190B::EnableTestMode(uint32_t data) {
        if (ReadAcqMode() == TriggerMode_t::MATCHING) {
            WriteMicro(Opcode(Command::ENABLE_TEST_MODE));
            WriteMicro(data & 0x0000FFFF);
            WriteMicro((data >> 16U) & 0x000000FF);
        } else
            PrintMessage(Message_t::ERROR, "To use this function change trigger mode to TRIGGER MATCHING");
    }

    void V1190B::DisableTestMode() {
        WriteMicro(Opcode(Command::DISABLE_TEST_MODE));
    }

    void V1190B::WriteTDCTestOutput(uint16_t data) {
        WriteMicro(Opcode(Command::SET_TDC_TEST_OUTPUT));
        WriteMicro(data & 0x000F);
    }

    void V1190B::WriteDLLClock(DLLClock dllClock) {
        WriteMicro(Opcode(Command::SET_DLL_CLOCK));
        WriteMicro(static_cast<uint16_t>(dllClock) & 0x0003);
    }
}
