#include "VException.h"
#include "modules/V1742B.h"


namespace vmepp
{
    //********************
    //****** V1742B ******
    //********************

    V1742B::V1742B( uint32_t baseAddress, uint32_t range ) :
        VSlave( baseAddress, range ),
        VSlaveInterrupter( baseAddress, range ),
        VSlaveAcquisitor( baseAddress, range )
    {
    }

    template<>
    const std::string UConfigurable<V1742B>::fName = "V1742B";

    //****** MISC + ******
    void V1742B::WriteDummy32( Group_t group, uint32_t word )
    {
        WriteRegister32( V1742B_DUMMY32(static_cast<uint8_t>(group)), word );
    }

    uint32_t V1742B::ReadDummy32( Group_t group )
    {
        return ReadRegister32( V1742B_DUMMY32( static_cast<uint8_t>(group) ) );
    }

    //****** CONTROL + ******
    void V1742B::WriteReadoutCtrl( uint32_t value )
    {
        WriteRegister32( V1742B_READOUT_CTRL, value, V1742B_READOUT_CTRL_VAL_MSK );
    }

    uint32_t V1742B::ReadReadoutCtrl()
    {
        return ReadRegister32( V1742B_READOUT_CTRL, V1742B_READOUT_CTRL_VAL_MSK );
    }
    //****** CONTROL - ******

    //****** INTERRUPTS + ******
    void V1742B::WriteIRQEvents( uint16_t n )
    {
        WriteRegister32( V1742B_INTRPT_EVNT_NUM, n, V1742B_INTRPT_EVNT_NUM_VAL_MSK );
    }

    uint16_t V1742B::ReadIRQEvents()
    {
        return ReadRegister32( V1742B_INTRPT_EVNT_NUM, V1742B_INTRPT_EVNT_NUM_VAL_MSK );
    }

    void V1742B::WriteIRQLevel( uint16_t level )
    {
        uint32_t readCtrl = ReadReadoutCtrl();
        WriteReadoutCtrl( readCtrl | (level & V1742B_READOUT_CTRL_INT_MSK) );
    }

    uint16_t V1742B::ReadIRQLevel()
    {
        return (ReadReadoutCtrl() & V1742B_READOUT_CTRL_INT_MSK);
    }

    void V1742B::WriteIRQVector( uint16_t vector )
    {
        WriteRegister32( V1742B_INTRPT_STATUS, vector );
    }

    uint16_t V1742B::ReadIRQVector()
    {
        return ReadRegister32( V1742B_INTRPT_STATUS );
    }

    void V1742B::ISR( uint16_t vector )
    {
        PrintMessage( Message_t::INFO, "V1742B ISR :: " + std::to_string( vector ) );
    }
    //****** INTERRUPTS - ******

    //****** TRIGGER + ******

    void V1742B::WritePostTrigger( Group_t group, uint16_t n )
    {
        WriteRegister32( V1742B_POST_TRIGGER(static_cast<uint8_t>(group)), n, V1742B_POST_TRIGGER_VAL_MSK );
    }

    uint16_t V1742B::ReadPostTrigger( Group_t group )
    {
        return ReadRegister32( V1742B_POST_TRIGGER(static_cast<uint8_t>(group)), V1742B_POST_TRIGGER_VAL_MSK );
    }

    void V1742B::WriteChannelThreshold( uint8_t ch, uint16_t threshold )
    {
        WriteRegister32( V1742B_CH)
    }
    //****** TRIGGER - ******
}
