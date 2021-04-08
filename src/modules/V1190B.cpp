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

    // ACQUISITION MODE
    void V1190B::WriteAcqMode( V1190B::TriggerMode_t mode )
    {
        switch( mode )
        {
            case( TriggerMode_t::MATCHING ) :
                WriteMicro( Opcode( Command::TRG_MATCH ) );
                break;
            case( TriggerMode_t::CONTINUOUS ) :
                WriteMicro( Opcode( Command::CONT_STOR ) );
                break;
        }
    }

    V1190B::TriggerMode_t V1190B::ReadAcqMode()
    {
        WriteMicro( Opcode( Command::READ_ACQ_MOD ) );
        uint16_t data = ReadMicro();
        if( 1U & data ) return TriggerMode_t::MATCHING;     //check LSB
        else            return TriggerMode_t::CONTINUOUS;
    }

    void V1190B::WriteEnableKeepToken( bool status )
    {
        if( status ) WriteMicro( Opcode( Command::SET_KEEP_TOKEN ) );
        else         WriteMicro( Opcode( Command::CLEAR_KEEP_TOKEN ) );
    }

    void V1190B::WriteLoadConfig( V1190B::Config_t config )
    {
        switch( config )
        {
            case( Config_t::DEFAULT ) :
                WriteMicro( Opcode( Command::LOAD_DEF_CONFIG ) );
                break;
            case( Config_t::USER ) :
                WriteMicro( Opcode( Command::LOAD_USER_CONFIG ) );
                break;
        }
    }

    void V1190B::WriteAutoLoad( V1190B::Config_t config )
    {
        switch( config )
        {
            case( Config_t::DEFAULT ) :
                WriteMicro( Opcode( Command::AUTOLOAD_DEF_CONF ) );
                break;
            case( Config_t::USER ) :
                WriteMicro( Opcode( Command::AUTOLOAD_USER_CONF ) );
                break;
        }
    }
}
