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

    void V1742B::WaitForSPI( Group_t group )
    {
        while( not ReadStatus( group, StatusBit::BusySPI ) ) { }
    }

    //****** MISC + ******
    void V1742B::WriteDummy32( Group_t group, uint32_t word )
    {
        WriteRegister32( V1742B_DUMMY32(static_cast<uint8_t>(group)), word );
    }

    uint32_t V1742B::ReadDummy32( Group_t group )
    {
        return ReadRegister32( V1742B_DUMMY32( static_cast<uint8_t>(group) ) );
    }

    int16_t V1742B::ReadChipTemperature( Group_t group )
    {
        return ReadRegister32( V1742B_CHIP_TEMP(static_cast<uint8_t>(group)), V1742B_CHIP_TEMP_VAL_MSK );
    }

    //****** INFO & ROM + ******
    //****** INFO & ROM - ******

    //****** CONTROL & STATUS + ******
    void V1742B::WriteReadoutCtrl( uint32_t value )
    {
        WriteRegister32( V1742B_READOUT_CTRL, value, V1742B_READOUT_CTRL_VAL_MSK );
    }

    uint32_t V1742B::ReadReadoutCtrl()
    {
        return ReadRegister32( V1742B_READOUT_CTRL, V1742B_READOUT_CTRL_VAL_MSK );
    }

    uint32_t V1742B::ReadStatus( Group_t group )
    {
        return ReadRegister32( V1742B_STATUS( static_cast<uint8_t>(group) ), StatusBit::All );
    }

    bool V1742B::ReadStatus( Group_t group, StatusBit bit )
    {
        return ReadStatus( group ) & bit;
    }
    //****** CONTROL & STATUS - ******

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
        ch = ch / GetChNumber();
        Group_t group = static_cast<Group_t>( ch / GetGroupNumber() );
        ch = ch % GetGroupNumber();

        uint32_t thr = (uint32_t)threshold & V1742B_CHANNEL_THR_VAL_MSK;
        uint32_t index = (ch << V1742B_CHANNEL_THR_IDX_SHFT) & V1742B_CHANNEL_THR_IDX_MSK;

        WriteRegister32( V1742B_CHANNEL_THR(static_cast<uint8_t>(group)), index | thr );
    }

    uint16_t V1742B::ReadChannelThreshold( uint8_t ch )
    {
        ch = ch / GetChNumber();
        Group_t group = static_cast<Group_t>( ch / GetGroupNumber() );
        ch = ch % GetGroupNumber();
        uint32_t value = (uint32_t)ch & V1742B_CHANNEL_SEL_VAL_MSK;

        WriteRegister32( V1742B_CHANNEL_SEL(static_cast<uint8_t>(group)), value );

        return ReadRegister32( V1742B_CHANNEL_THR(static_cast<uint8_t>(group)), V1742B_CHANNEL_THR_VAL_MSK );
    }

    void V1742B::WriteEnableTrigger( uint8_t ch, bool enable )
    {
        ch = ch / GetChNumber();
        Group_t group = static_cast<Group_t>( ch / GetGroupNumber() );
        ch = ch % GetGroupNumber();

        uint32_t triggerMask = ReadRegister32( V1742B_CHANNEL_TRG_MASK(static_cast<uint8_t>(group)), V1742B_CHANNEL_TRG_MASK_MSK );

        if( enable ) triggerMask |= (1U << ch);
        else         triggerMask &= ~(1U << ch);

        WriteRegister32( V1742B_CHANNEL_TRG_MASK(static_cast<uint8_t>(group)), triggerMask, V1742B_CHANNEL_TRG_MASK_MSK );
    }

    void V1742B::WriteEnableTrigger( Group_t group, uint8_t mask )
    {
        WriteRegister32( V1742B_CHANNEL_TRG_MASK(static_cast<uint8_t>(group)), (uint32_t)mask, V1742B_CHANNEL_TRG_MASK_MSK );
    }

    bool V1742B::ReadEnableTriggerC( uint8_t ch )
    {
        ch = ch / GetChNumber();
        Group_t group = static_cast<Group_t>( ch / GetGroupNumber() );
        ch = ch % GetGroupNumber();

        uint32_t triggerMask = ReadRegister32( V1742B_CHANNEL_TRG_MASK(static_cast<uint8_t>(group)), V1742B_CHANNEL_TRG_MASK_MSK );

        return triggerMask & (1U << ch);
    }

    uint8_t V1742B::ReadEnableTrigger( Group_t group )
    {
        return ReadRegister32( V1742B_CHANNEL_TRG_MASK(static_cast<uint8_t>(group)), V1742B_CHANNEL_TRG_MASK_MSK );
    }

    void V1742B::WriteThresholdTR( TR_t tr, uint16_t threshold )
    {
        Group_t group = (tr == TR_t::TR0) ? Group_t::G1 : Group_t::G3;

        WaitForSPI( group );
        WriteRegister32( V1742B_TR_TRG_THR(static_cast<uint8_t>(group)), (uint32_t)threshold, V1742B_TR_TRG_THR_VAL_MSK );
    }

    uint16_t V1742B::ReadThresholdTR( TR_t tr )
    {
        Group_t group = (tr == TR_t::TR0) ? Group_t::G1 : Group_t::G3;
        return ReadRegister32( V1742B_TR_TRG_THR(static_cast<uint8_t>(group)), V1742B_TR_TRG_THR_VAL_MSK );
    }

    void V1742B::WriteOffsetTR( TR_t tr, uint16_t offset )
    {
        Group_t group = (tr == TR_t::TR0) ? Group_t::G1 : Group_t::G3;

        WaitForSPI( group );
        WriteRegister32( V1742B_TR_DC_OFST(static_cast<uint8_t>(group)), (uint32_t)offset, V1742B_TR_DC_OFST_VAL_MSK );
    }

    uint16_t V1742B::ReadOffsetTR( TR_t tr )
    {
        Group_t group = (tr == TR_t::TR0) ? Group_t::G1 : Group_t::G3;
        return ReadRegister32( V1742B_TR_DC_OFST(static_cast<uint8_t>(group)), V1742B_TR_DC_OFST_VAL_MSK );
    }
    //****** TRIGGER - ******

    //****** CHANNEL + ******
    void V1742B::WriteChannelOffset( uint8_t ch, uint16_t offset )
    {
        ch = ch / GetChNumber();
        Group_t group = static_cast<Group_t>( ch / GetGroupNumber() );
        ch = ch % GetGroupNumber();
        uint32_t offs = (uint32_t)offset & V1742B_CHANNEL_DC_OFST_VAL_MSK;
        uint32_t index = (ch << V1742B_CHANNEL_DC_OFST_IDX_SHFT) & V1742B_CHANNEL_DC_OFST_IDX_MSK;

        WaitForSPI( group );
        WriteRegister32( V1742B_CHANNEL_DC_OFST(static_cast<uint8_t>(group)), index | offs );
    }

    uint16_t V1742B::ReadChannelOffset( uint8_t ch )
    {
        ch = ch / GetChNumber();
        Group_t group = static_cast<Group_t>( ch / GetGroupNumber() );
        ch = ch % GetGroupNumber();
        uint32_t value = (uint32_t)ch & V1742B_CHANNEL_SEL_VAL_MSK;

        WriteRegister32( V1742B_CHANNEL_SEL(static_cast<uint8_t>(group)), value );

        return ReadRegister32( V1742B_CHANNEL_DC_OFST(static_cast<uint8_t>(group)) ) & V1742B_CHANNEL_DC_OFST_VAL_MSK;
    }
    //****** CHANNEL - ******
}
