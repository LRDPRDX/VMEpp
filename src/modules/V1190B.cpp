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

    //CHANNEL
    void V1190B::WriteEnableChannel( uint8_t n, bool status )
    {
        if( status ) WriteMicro( Opcode( Command::EN_CHANNEL, n % fChNumber ) );
        else         WriteMicro( Opcode( Command::DIS_CHANNEL, n % fChNumber ) );
    }

    void V1190B::WriteEnableAll( bool status )
    {
        if( status ) WriteMicro( Opcode( Command::EN_ALL_CH ) );
        else         WriteMicro( Opcode( Command::DIS_ALL_CH ) );
    }


    void V1190B::WriteEnablePattern( V1190B::TDC tdc, uint32_t pattern )
    {
        switch( tdc )
        {
            case( TDC::TDC0 ) :
                WriteMicro( Opcode( Command::WRITE_EN_PATTERN32, 0x0000 ) );
                break;
            case( TDC::TDC1 ) :
                WriteMicro( Opcode( Command::WRITE_EN_PATTERN32, 0x0001 ) );
                break;
        }
        uint16_t lsb = (pattern & 0x0000ffff ); // ch 0 : 15
        uint16_t msb = (pattern >> 16U);        // ch 16 : 31
        WriteMicro( lsb );
        WriteMicro( msb );
    }

    uint32_t V1190B::ReadEnablePattern( V1190B::TDC tdc )
    {
        switch( tdc )
        {
            case( TDC::TDC0 ) :
                WriteMicro( Opcode( Command::READ_EN_PATTERN32, 0x0000 ) );
                break;
            case( TDC::TDC1 ) :
                WriteMicro( Opcode( Command::READ_EN_PATTERN32, 0x0001 ) );
                break;
        }
        uint32_t lsb = ReadMicro(); // ch 0 : 15
        uint32_t msb = ReadMicro(); // ch 16 : 31

        return ((msb << 16U) & 0xffff0000) | (lsb & 0x0000ffff);
    }

    // MISC
    uint32_t V1190B::ReadTDC_ID( V1190B::TDC tdc )
    {
        switch( tdc )
        {
            case( TDC::TDC0 ) :
                WriteMicro( Opcode( Command::READ_TDC_ID, 0x00 ) );
                break;
            case( TDC::TDC1 ) :
                WriteMicro( Opcode( Command::READ_TDC_ID, 0x01 ) );
                break;
        }
        // NOTE : there is a typo in the doc
        // After this OPCODE is sent *TWO* 16-bit
        // words must be read
        uint32_t lsb = ReadMicro();
        uint32_t msb = ReadMicro();

        return ((msb << 16U) & 0xffff0000) | (lsb & 0x0000ffff);
    }

    uint16_t V1190B::ReadMicroFWRev()
    {
        WriteMicro( Opcode( Command::READ_MICRO_REV ) );
        return (ReadMicro() & 0x00FF );
    } 

    void V1190B::WriteResetLoops()
    {
        WriteMicro( Opcode( Command::RESET_DLL_PLL ) );
    }
}
