#include "CAENVMElib.h"
#include "modules/V1190B.h"

namespace vmeplus {
    V1190B::V1190B( uint32_t baseAddress, uint32_t range ) :
        VSlave( "V1190B", baseAddress, range ),
        VSlaveInterrupter( "V1190B", baseAddress, range ),
         VSlaveAcquisitor( "V1190B", baseAddress, range ) {}
 
    V1190B::~V1190B(){};

    void V1190B::Initialize()
    {
        PrintMessage( Message_t::INFO, "Inititalizing " + fName + "..." );
        PrintMessage( Message_t::INFO, "Inititalizing " + fName + "...OK" );
    }

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

    void V1190B::WriteMicro(uint16_t data) {
        //the VME (master) tests the WRITE_OK bit in the Micro Handshake Register
        // if the WO bit is set to 1, the VME can write a datum
        while (!(ReadRegister16(V1190B_MICRO_HANDSHAKE, V1190B_WRITE_OK_MSK))) {}
        WriteRegister16(V1190B_MICRO, data);
    }

    void V1190B::WriteMicro(Opcode opcode) {
        uint16_t code = static_cast<uint16_t>(opcode.command)<<8U | opcode.object;

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

    void V1190B::WriteSubstractionOfTriggerTime(bool data){
        if (data)
            WriteMicro(Opcode(Command::EN_SUB_TRG));
        else 
            WriteMicro(Opcode(Command::DIS_SUB_TRG));
    }

    void V1190B::ReadTriggerConfiguration(TriggerData& trigger){
        WriteMicro(Opcode(Command::READ_TRG_CONF));

        TriggerData(ReadMicro(), ReadMicro(), ReadMicro(), ReadMicro(), ReadMicro());
    }
}
