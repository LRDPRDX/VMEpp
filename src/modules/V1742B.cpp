#include "VException.h"
#include "modules/V1742B.h"

#include <iomanip>
#include <sstream>
#include <type_traits>


namespace vmepp
{
    //********************
    //****** V1742B ******
    //********************
    V1742B::V1742B( uint32_t baseAddress, uint32_t range ) :
        VSlave( baseAddress, range ),
        VSlaveInterrupter( baseAddress, range ),
        VSlaveAcquisitor( baseAddress, range ),

        fPathToCorrectionTable( "./correction.x742_corr" )
    {
    }

    template<>
    const std::string UConfigurable<V1742B>::fName = "V1742B";

    void V1742B::WaitForSPI( Group_t group )
    {
        while( ReadStatus( group, StatusBit::BusySPI ) ) { }
    }

    void V1742B::LoadCorrectionTable()
    {
        PrintMessage( Message_t::INFO, "Loading correction tables from " + fPathToCorrectionTable + " ..." );
        std::ifstream f( fPathToCorrectionTable, std::ios_base::in | std::ios_base::binary );

        while( f.good() )
        {
            FreqCorrection fCorr;
            f >> fCorr;
            fCorrectionTable[fCorr.freq] = fCorr;
        }
        for( auto rate : RateIterator() )
        {
            if( fCorrectionTable.find( rate ) == fCorrectionTable.end() )
            {
                std::stringstream ss;
                ss << rate;
                PrintMessage( Message_t::WARNING, "Correction table for the rate " + ss.str() + " has NOT been loaded." );
            }
        }
    }

    void V1742B::WriteSWReset()
    {
        WriteRegister32( V1742B_SOFTWARE_RST, 1 );
    }

    void V1742B::WriteSWClear()
    {
        WriteRegister32( V1742B_SOFTWARE_CLR, 1 );
    }

    void V1742B::Initialize()
    {
        PrintMessage( Message_t::INFO, "Initialization " + fName + "...\n" );

        //Correction tables
        LoadCorrectionTable();
        //Static info
        for( uint8_t g = 0; g < fGroupNumber; ++g )
        {
            fAMCFirmware[g] = ReadAMCFirmwareRev( static_cast<Group_t>( g ) );
        }
        fROCFirmware = ReadROCFirmwareRev();
        fBoardInfo = ReadBoardInfo();

        //WriteSWReset();
        WriteSWClear();
        // Misc
        WriteDummy32( 0x12345678 );
        WriteTestModeEnable( false );

        // Interrupts
        WriteIRQEvents( 0 );
        WriteIRQLevel( 0 );
        WriteIRQVector( 0 );

        // Control & Status
        WriteRegister32( V1742B_BOARD_CFG_SET, V1742B_BOARD_CFG_MUST_SET );
        WriteRegister32( V1742B_BOARD_CFG_CLR, V1742B_BOARD_CFG_MUST_CLR );

        // Trigger
        WritePostTrigger( 0 );

        for( uint8_t ch = 0; ch < GetChNumber(); ++ch )
        {
            WriteChannelThreshold( ch, 0 );
            WriteChannelOffset( ch, 0x7FFF );
        }

        WriteEnableTrigger( 0 );

        WriteThresholdTR( TR_t::TR0, 0 );
        WriteThresholdTR( TR_t::TR1, 0 );
        WriteOffsetTR( TR_t::TR0, 0x7FFF );
        WriteOffsetTR( TR_t::TR1, 0x7FFF );
        WriteTriggerPolarity( TriggerPolarity_t::RisingEdge );
        WriteReadoutEnableTR( false );
        WriteEnableTriggerTR( false );

        WriteTRGINEnable( false );
        WriteTRGINSignal( TrgInSignal_t::Gate );

        WriteTRGOUTSignal( TrgOutSignal_t::NoSignal );

        //WriteVetoDelay( 0 );

        WriteGlobalTrigger( GlobalTrigger_t::SWOnly );

        // Acquisition
        WriteRecordLength( RecordLength_t::s1024 );
        WriteInitTestValue( 0 );
        WriteSamplingRate( SamplingRate_t::M5000 );
        WriteScratch( 0x12345678 );
        WriteMaxEventBLT( 1024 );
        WriteAcqMode( AcqMode_t::Transparent );
        WriteStartSource( StartSource_t::SW );
        WriteReadoutEnableGroup( false );

        // Front Panel
        WriteLEMOLevel( Level_t::TTL );

        WriteTRGINSignal( TrgInSignal_t::Gate );
        WriteTRGINEnable( false );
        WriteTRGINToMezz( false );
        WriteTRGINSync( TrgInSync_t::Edge );

        WriteTRGOUTSignal( TrgOutSignal_t::NoSignal );
        WriteTRGOUTGeneration( TrgOutSource_t::None );

        // Print Some Info
        for( uint8_t g = 0; g < GetGroupNumber(); ++g )
        {
            std::cout << "Temperature of Chip #" << (int)g << " is " << ReadChipTemperature( static_cast<Group_t>( g )) << " C\n";
            std::cout << "Mezzanine Revision: " << ReadStatus( static_cast<Group_t>( g ), StatusBit::MezzRevision ) << "\n";
        }

        PrintMessage( Message_t::INFO, "Initialization " + fName + " Done!\n" );
    }

    void V1742B::Reset()
    {
        PrintMessage( Message_t::INFO, "Reset is Not Implemented Yet !" );
    }

    //****** MISC + ******
    void V1742B::WriteDummy32( Group_t group, uint32_t word )
    {
        WriteRegister32( GroupAddress( V1742B_DUMMY32, group ), word );
    }

    void V1742B::WriteDummy32( uint32_t word )
    {
        WriteRegister32( GroupAddress( V1742B_DUMMY32 ), word );
    }

    uint32_t V1742B::ReadDummy32( Group_t group )
    {
        return ReadRegister32( GroupAddress( V1742B_DUMMY32, group ) );
    }

    int16_t V1742B::ReadChipTemperature( Group_t group )
    {
        return ReadRegister32( GroupAddress( V1742B_CHIP_TEMP, group ), V1742B_CHIP_TEMP_VAL_MSK );
    }

    void V1742B::WriteLVDS( uint16_t mask )
    {
        WriteRegister32( V1742B_LVDS_IO_DATA, (uint32_t)(mask), V1742B_LVDS_IO_DATA_VAL_MSK );
    }

    uint16_t V1742B::ReadLVDS()
    {
        return ReadRegister32( V1742B_LVDS_IO_DATA, V1742B_LVDS_IO_DATA_VAL_MSK );
    }

    void V1742B::WriteTestModeEnable( bool enable )
    {
        uint32_t value = (1U << V1742B_BOARD_CFG_TM_SHFT);
        if( enable )
        {
            WriteRegister32( V1742B_BOARD_CFG_SET, value );
        }
        else
        {
            WriteRegister32( V1742B_BOARD_CFG_CLR, value );
        }
    }

    bool V1742B::ReadTestModeEnable()
    {
        return ReadRegister32( V1742B_BOARD_CFG, (1U << V1742B_BOARD_CFG_TM_SHFT) );
    }

    void V1742B::ApplyCorrection( UEvent<V1742B>& event ) const
    {
        for( uint8_t ng = 0; ng < fGroupNumber; ++ng )
        {
            if( event.GetGroupMask() & (1U << ng) )
            {
                SamplingRate_t rate = event.fData[ng].GetSamplingRate();
                const auto tbIt = fCorrectionTable.find( rate );
                if( tbIt == fCorrectionTable.end() )
                {
                    PrintMessage( Message_t::WARNING, "Correction process for the group #" + std::to_string(ng) + " stopped: " + "sampling frequencies don't match" );
                    continue;
                }
                auto start = event.fData[ng].GetStartIndex();
                for( uint8_t ch = 0; ch < fChInGroup; ++ch )
                {
                    for( size_t i = 0; i < fNSamples; ++i )
                    {
                        event.fData[ng].fData[ch][i] -= tbIt->second.table[ng].channels[ch].cell[(i+start) % fNSamples];
                        event.fData[ng].fData[ch][i] -= tbIt->second.table[ng].channels[ch].sample[i];
                    }
                }
                if( event.fData[ng].GetTRPresent() )
                {
                    for( size_t i = 0; i < fNSamples; ++i )
                    {
                        event.fData[ng].fTR[i] -= tbIt->second.table[ng].trN.cell[(i + start) % fNSamples];
                        event.fData[ng].fTR[i] -= tbIt->second.table[ng].trN.sample[i];
                    }
                }
            }
        }
    }
    //****** MISC - ******

    //****** INFO & ROM + ******
    V1742B::AMCFirmwareRev V1742B::ReadAMCFirmwareRev( Group_t group )
    {
        uint32_t value = ReadRegister32( GroupAddress( V1742B_AMC_FRMW_REV, group ) );
        uint8_t minor = (value & V1742B_AMC_FRMW_REV_MIN_MSK) >> V1742B_AMC_FRMW_REV_MIN_SHFT;
        uint8_t major = (value & V1742B_AMC_FRMW_REV_MAJ_MSK) >> V1742B_AMC_FRMW_REV_MAJ_SHFT;
        uint16_t date = (value & V1742B_AMC_FRMW_REV_DATE_MSK) >> V1742B_AMC_FRMW_REV_DATE_SHFT;

        return AMCFirmwareRev( minor, major, date );
    }

    V1742B::ROCFirmwareRev V1742B::ReadROCFirmwareRev()
    {
        uint32_t value = ReadRegister32( V1742B_ROC_FRMW_REV );
        uint8_t minor = (value & V1742B_ROC_FRMW_REV_MIN_MSK) >> V1742B_ROC_FRMW_REV_MIN_SHFT;
        uint8_t major = (value & V1742B_ROC_FRMW_REV_MAJ_MSK) >> V1742B_ROC_FRMW_REV_MAJ_SHFT;
        uint16_t date = (value & V1742B_ROC_FRMW_REV_DATE_MSK) >> V1742B_ROC_FRMW_REV_DATE_SHFT;

        return ROCFirmwareRev( minor, major, date );
    }

    V1742B::BoardInfo V1742B::ReadBoardInfo()
    {
        uint32_t value = ReadRegister32( V1742B_BOARD_INFO );
        uint8_t code = (value & V1742B_BOARD_INFO_CODE_MSK) >> V1742B_BOARD_INFO_CODE_SHFT;
        uint8_t mem  = (value & V1742B_BOARD_INFO_MEM_MSK) >> V1742B_BOARD_INFO_MEM_SHFT;
        uint8_t group = (value & V1742B_BOARD_INFO_GROUP_MSK) >> V1742B_BOARD_INFO_GROUP_SHFT;

        return BoardInfo( code, mem, group );
    }

    uint32_t V1742B::ReadBoardFailure()
    {
        return ReadRegister32( V1742B_BOARD_FAIL, V1742B_BOARD_FAIL_VAL_MSK );
    }
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
        return ReadRegister32( GroupAddress( V1742B_STATUS, group ), StatusBit::All );
    }

    bool V1742B::ReadStatus( Group_t group, StatusBit bit )
    {
        return (ReadStatus( group ) & bit);
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
        WriteRegister32( GroupAddress( V1742B_POST_TRIGGER, group ), n, V1742B_POST_TRIGGER_VAL_MSK );
    }

    void V1742B::WritePostTrigger( uint16_t n )
    {
        WriteRegister32( GroupAddress( V1742B_POST_TRIGGER ), n, V1742B_POST_TRIGGER_VAL_MSK );
    }

    uint16_t V1742B::ReadPostTrigger( Group_t group )
    {
        return ReadRegister32( GroupAddress( V1742B_POST_TRIGGER, group ), V1742B_POST_TRIGGER_VAL_MSK );
    }

    void V1742B::WriteChannelThreshold( uint8_t ch, uint16_t threshold )
    {
        ch %= fChNumber;
        Group_t group = static_cast<Group_t>( ch / fChInGroup );
        ch %= fChInGroup;

        uint32_t thr = (uint32_t)threshold & V1742B_CHANNEL_THR_VAL_MSK;
        uint32_t index = (ch << V1742B_CHANNEL_THR_IDX_SHFT) & V1742B_CHANNEL_THR_IDX_MSK;

        WriteRegister32( GroupAddress( V1742B_CHANNEL_THR, group ), index | thr );
    }

    uint16_t V1742B::ReadChannelThreshold( uint8_t ch )
    {
        ch %= fChNumber;
        Group_t group = static_cast<Group_t>( ch / fChInGroup );
        ch %= fChInGroup;
        uint32_t value = (uint32_t)ch & V1742B_CHANNEL_SEL_VAL_MSK;

        WriteRegister32( GroupAddress( V1742B_CHANNEL_SEL, group ), value );

        return ReadRegister32( GroupAddress( V1742B_CHANNEL_THR, group ), V1742B_CHANNEL_THR_VAL_MSK );
    }

    void V1742B::WriteEnableTrigger( uint8_t ch, bool enable )
    {
        ch %= fChNumber;
        Group_t group = static_cast<Group_t>( ch / fChInGroup );
        ch %= fChInGroup;

        uint32_t triggerMask = ReadRegister32( GroupAddress( V1742B_CHANNEL_TRG_MASK, group ), V1742B_CHANNEL_TRG_MASK_MSK );

        if( enable ) triggerMask |= (1U << ch);
        else         triggerMask &= ~(1U << ch);

        WriteRegister32( GroupAddress( V1742B_CHANNEL_TRG_MASK, group ), triggerMask, V1742B_CHANNEL_TRG_MASK_MSK );
    }

    void V1742B::WriteEnableTrigger( Group_t group, uint8_t mask )
    {
        WriteRegister32( GroupAddress( V1742B_CHANNEL_TRG_MASK, group ), (uint32_t)mask, V1742B_CHANNEL_TRG_MASK_MSK );
    }

    void V1742B::WriteEnableTrigger( uint8_t mask )
    {
        WriteRegister32( GroupAddress( V1742B_CHANNEL_TRG_MASK ), (uint32_t)mask, V1742B_CHANNEL_TRG_MASK_MSK );
    }

    bool V1742B::ReadEnableTrigger( uint8_t ch )
    {
        ch %= fChNumber;
        Group_t group = static_cast<Group_t>( ch / fChInGroup );
        ch %= fChInGroup;

        uint32_t triggerMask = ReadRegister32( GroupAddress( V1742B_CHANNEL_TRG_MASK, group ), V1742B_CHANNEL_TRG_MASK_MSK );

        return triggerMask & (1U << ch);
    }

    uint8_t V1742B::ReadEnableTrigger( Group_t group )
    {
        return ReadRegister32( GroupAddress( V1742B_CHANNEL_TRG_MASK, group ), V1742B_CHANNEL_TRG_MASK_MSK );
    }

    void V1742B::WriteThresholdTR( TR_t tr, uint16_t threshold )
    {
        Group_t group = (tr == TR_t::TR0) ? Group_t::G0 : Group_t::G2;

        WaitForSPI( group );
        WriteRegister32( GroupAddress( V1742B_TR_TRG_THR, group ), (uint32_t)threshold, V1742B_TR_TRG_THR_VAL_MSK );
    }

    uint16_t V1742B::ReadThresholdTR( TR_t tr )
    {
        Group_t group = (tr == TR_t::TR0) ? Group_t::G0 : Group_t::G2;
        return ReadRegister32( GroupAddress( V1742B_TR_TRG_THR, group ), V1742B_TR_TRG_THR_VAL_MSK );
    }

    void V1742B::WriteOffsetTR( TR_t tr, uint16_t offset )
    {
        Group_t group = (tr == TR_t::TR0) ? Group_t::G0 : Group_t::G2;

        WaitForSPI( group );
        WriteRegister32( GroupAddress( V1742B_TR_DC_OFST, group ), (uint32_t)offset, V1742B_TR_DC_OFST_VAL_MSK );
    }

    uint16_t V1742B::ReadOffsetTR( TR_t tr )
    {
        Group_t group = (tr == TR_t::TR0) ? Group_t::G0 : Group_t::G2;
        return ReadRegister32( GroupAddress( V1742B_TR_DC_OFST, group ), V1742B_TR_DC_OFST_VAL_MSK );
    }

    void V1742B::WriteSWTrigger()
    {
        WriteRegister32( V1742B_SOFT_TRG, 1 );
    }

    void V1742B::WriteGlobalTrigger( GlobalTrigger_t trigger )
    {
        uint32_t value = (static_cast<uint32_t>(trigger) << V1742B_GLB_TRG_MASK_VAL_SHFT );
        WriteRegister32( V1742B_GLB_TRG_MASK, value, V1742B_GLB_TRG_MASK_VAL_MSK );
    }

    V1742B::GlobalTrigger_t V1742B::ReadGlobalTrigger()
    {
        uint32_t value = ReadRegister32( V1742B_GLB_TRG_MASK, V1742B_GLB_TRG_MASK_VAL_MSK );
        return static_cast<GlobalTrigger_t>( value >> V1742B_GLB_TRG_MASK_VAL_SHFT );
    }

    void V1742B::WriteVetoDelay( uint16_t value )
    {
        WriteRegister32( V1742B_EXTD_VETO_DELAY, (uint32_t)value, V1742B_EXTD_VETO_DELAY_VAL_MSK );
    }

    uint16_t V1742B::ReadVetoDelay()
    {
        return ReadRegister32( V1742B_EXTD_VETO_DELAY, V1742B_EXTD_VETO_DELAY_VAL_MSK );
    }

    void V1742B::WriteTriggerPolarity( TriggerPolarity_t pol )
    {
        uint32_t value = (1U << V1742B_BOARD_CFG_TRG_POL_SHFT);
        if( pol == TriggerPolarity_t::FallingEdge )
        {
            WriteRegister32( V1742B_BOARD_CFG_SET, value );
        }
        else
        {
            WriteRegister32( V1742B_BOARD_CFG_CLR, value );
        }
    }

    V1742B::TriggerPolarity_t V1742B::ReadTriggerPolarity()
    {
        bool value = ReadRegister32( V1742B_BOARD_CFG, (1U << V1742B_BOARD_CFG_TRG_POL_SHFT) );
        return (value ? TriggerPolarity_t::FallingEdge : TriggerPolarity_t::RisingEdge);
    }

    void V1742B::WriteReadoutEnableTR( bool enable )
    {
        uint32_t value = (1U << V1742B_BOARD_CFG_TRG_DG_SHFT);
        if( enable )
        {
            WriteRegister32( V1742B_BOARD_CFG_SET, value );
        }
        else
        {
            WriteRegister32( V1742B_BOARD_CFG_CLR, value );
        }
    }

    bool V1742B::ReadReadoutEnableTR()
    {
        return ReadRegister32( V1742B_BOARD_CFG, (1U << V1742B_BOARD_CFG_TRG_DG_SHFT) );
    }

    void V1742B::WriteEnableTriggerTR( bool enable )
    {
        uint32_t value = (1U << V1742B_BOARD_CFG_TRG_EN_SHFT);
        if( enable )
        {
            WriteRegister32( V1742B_BOARD_CFG_SET, value );
        }
        else
        {
            WriteRegister32( V1742B_BOARD_CFG_CLR, value );
        }
    }

    bool V1742B::ReadEnableTriggerTR()
    {
        return ReadRegister32( V1742B_BOARD_CFG, (1U << V1742B_BOARD_CFG_TRG_EN_SHFT) );
    }
    //****** TRIGGER - ******

    //****** ACQUISITION + ******
    size_t V1742B::HelperReadCycles()
    {
        size_t nWords = ReadEventSize() * fNEventRequest * sizeof( DataWord_t );
        return ((nWords % gMaxBLT == 0) ? (nWords / gMaxBLT) : (nWords / gMaxBLT + 1));
    }

    void V1742B::WriteReadoutEnableGroup( Group_t g, bool enable )
    {
        if( ReadAcqStatus( AcqStatusBit::AcqRun ) )
        {
            PrintMessage( Message_t::WARNING, "Group enable mask MAY NOT be changed while the acquisition is running" );
            return;
        }
        uint32_t value = ReadRegister32( V1742B_GROUP_EN_MASK, V1742B_GROUP_EN_MASK_VAL_MSK );
        uint32_t change = 1U << static_cast<uint8_t>( g );
        if( enable ) { value |= change; }
        else         { value &= ~(change); }
        WriteRegister32( V1742B_GROUP_EN_MASK, value, V1742B_GROUP_EN_MASK_VAL_MSK );
    }

    void V1742B::WriteReadoutEnableGroup( bool enable )
    {
        if( ReadAcqStatus( AcqStatusBit::AcqRun ) )
        {
            PrintMessage( Message_t::WARNING, "Group enable mask MAY NOT be changed while the acquisition is running" );
            return;
        }
        WriteRegister32( V1742B_GROUP_EN_MASK, enable ? 0xF : 0, V1742B_GROUP_EN_MASK_VAL_MSK );
    }

    bool V1742B::ReadReadoutEnableGroup( Group_t g )
    {
        return ReadRegister32( V1742B_GROUP_EN_MASK, (1U << static_cast<uint8_t>( g )) );
    }

    void V1742B::WriteChannelOffset( uint8_t ch, uint16_t offset )
    {
        ch %= fChNumber;
        Group_t group = static_cast<Group_t>( ch / fChInGroup );
        ch %= fChInGroup;
        uint32_t offs = (uint32_t)offset & V1742B_CHANNEL_DC_OFST_VAL_MSK;
        uint32_t index = (ch << V1742B_CHANNEL_DC_OFST_IDX_SHFT) & V1742B_CHANNEL_DC_OFST_IDX_MSK;

        WaitForSPI( group );
        WriteRegister32( GroupAddress( V1742B_CHANNEL_DC_OFST, group ), index | offs );
    }

    uint16_t V1742B::ReadChannelOffset( uint8_t ch )
    {
        ch %= fChNumber;
        Group_t group = static_cast<Group_t>( ch / fChInGroup );
        ch = ch % fChInGroup;
        uint32_t value = (uint32_t)ch & V1742B_CHANNEL_SEL_VAL_MSK;

        WriteRegister32( GroupAddress( V1742B_CHANNEL_SEL, group ), value );

        return ReadRegister32( GroupAddress( V1742B_CHANNEL_DC_OFST,group ), V1742B_CHANNEL_DC_OFST_VAL_MSK);
    }

    void V1742B::WriteRecordLength( RecordLength_t length )
    {
        if( ReadAcqStatus( AcqStatusBit::AcqRun ) )
        {
            PrintMessage( Message_t::WARNING, "Record length MAY NOT be changed while the acquisition is running" );
            return;
        }
        WriteRegister32( V1742B_CUSTOM_SIZE, static_cast<uint32_t>(length), V1742B_CUSTOM_SIZE_VAL_MSK );
    }

    V1742B::RecordLength_t V1742B::ReadRecordLength()
    {
        return static_cast<RecordLength_t>( ReadRegister32( V1742B_CUSTOM_SIZE, V1742B_CUSTOM_SIZE_VAL_MSK ) );
    }

    void V1742B::WriteInitTestValue( uint16_t value )
    {
        WriteRegister32( V1742B_TEST_MODE_INIT_WRITE, value, V1742B_TEST_MODE_INIT_VAL_MSK );
    }

    uint16_t V1742B::ReadInitTestValue( Group_t group )
    {
        return ReadRegister32( GroupAddress( V1742B_TEST_MODE_INIT_READ, group ), V1742B_TEST_MODE_INIT_VAL_MSK );
    }

    void V1742B::WriteSamplingRate( SamplingRate_t rate )
    {
        if( ReadAcqStatus( AcqStatusBit::AcqRun ) )
        {
            PrintMessage( Message_t::WARNING, "Sampling rate MAY NOT be changed while the acquisition is running" );
            return;
        }
        WriteRegister32( V1742B_DRS4_SAMP_FREQ_WRITE, static_cast<uint32_t>(rate), V1742B_DRS4_SAMP_FREQ_VAL_MSK );
    }

    V1742B::SamplingRate_t V1742B::ReadSamplingRate( Group_t group )
    {
        return static_cast<SamplingRate_t>( ReadRegister32( GroupAddress( V1742B_DRS4_SAMP_FREQ_READ, group ), V1742B_DRS4_SAMP_FREQ_VAL_MSK ) );
    }

    uint32_t V1742B::ReadEventSize()
    {
        return ReadRegister32( V1742B_EVENT_SIZE );
    }

    void V1742B::WriteScratch( uint32_t value )
    {
        WriteRegister32( V1742B_SCRATCH, value );
    }

    uint32_t V1742B::ReadScratch()
    {
        return ReadRegister32( V1742B_SCRATCH );
    }

    void V1742B::WriteMaxEventBLT( uint16_t n )
    {
        WriteRegister32( V1742B_MAX_EVENT_BLT, n, V1742B_MAX_EVENT_BLT_VAL_MSK );
    }

    uint16_t V1742B::ReadMaxEventBLT()
    {
        return ReadRegister32( V1742B_MAX_EVENT_BLT, V1742B_MAX_EVENT_BLT_VAL_MSK );
    }

    void V1742B::WriteAcqMode( AcqMode_t mode )
    {
        uint32_t value = (1U << V1742B_BOARD_CFG_MODE_SHFT);
        if( mode == AcqMode_t::Transparent )
        {
            WriteRegister32( V1742B_BOARD_CFG_SET, value );
        }
        else
        {
            WriteRegister32( V1742B_BOARD_CFG_CLR, value );
        }
    }

    V1742B::AcqMode_t V1742B::ReadAcqMode()
    {
        bool value = ReadRegister32( V1742B_BOARD_CFG, (1U << V1742B_BOARD_CFG_MODE_SHFT) );
        return (value ? AcqMode_t::Transparent : AcqMode_t::Output);
    }

    void V1742B::WriteTRGINEnable( bool enable )
    {
        uint32_t value = (1U << V1742B_BOARD_CFG_TGIN_EN_SHFT);
        if( enable )
        {
            WriteRegister32( V1742B_BOARD_CFG_SET, value );
        }
        else
        {
            WriteRegister32( V1742B_BOARD_CFG_CLR, value );
        }
    }

    bool V1742B::ReadTRGINEnable()
    {
        return ReadRegister32( V1742B_BOARD_CFG, (1U << V1742B_BOARD_CFG_TGIN_EN_SHFT) );
    }

    void V1742B::WriteTRGINSignal( TrgInSignal_t trigger )
    {
        uint32_t value = (1U << V1742B_BOARD_CFG_TGIN_SIG_SHFT);
        if( trigger == TrgInSignal_t::Veto )
        {
            WriteRegister32( V1742B_BOARD_CFG_SET, value );
        }
        else
        {
            WriteRegister32( V1742B_BOARD_CFG_CLR, value );
        }
    }

    V1742B::TrgInSignal_t V1742B::ReadTRGINSignal()
    {
        bool value = ReadRegister32( V1742B_BOARD_CFG, (1U << V1742B_BOARD_CFG_TGIN_SIG_SHFT) );
        return value ? TrgInSignal_t::Veto : TrgInSignal_t::Gate;
    }

    void V1742B::WriteTRGOUTSignal( TrgOutSignal_t trigger )
    {
        uint32_t value = static_cast<uint32_t>(trigger) << V1742B_BOARD_CFG_TGOUT_SIG_SHFT;
        WriteRegister32( V1742B_BOARD_CFG_SET, value & V1742B_BOARD_CFG_TGOUT_SIG_MSK );
        WriteRegister32( V1742B_BOARD_CFG_CLR, value ^ V1742B_BOARD_CFG_TGOUT_SIG_MSK );
    }

    void V1742B::WriteTRGOUTGeneration( TrgOutSource_t mask )
    {
        WriteRegister32( V1742B_FRNT_PANEL_TRG_OUT, static_cast<uint32_t>( mask ),
                         static_cast<uint32_t>( TrgOutSource_t::All ) );
    }

    V1742B::TrgOutSource_t V1742B::ReadTRGOUTGeneration()
    {
        return static_cast<TrgOutSource_t>( ReadRegister32( V1742B_FRNT_PANEL_TRG_OUT,
                                            static_cast<uint32_t>( TrgOutSource_t::All ) ) );
    }

    V1742B::TrgOutSignal_t V1742B::ReadTRGOUTSignal()
    {
        uint32_t value = ReadRegister32( V1742B_BOARD_CFG, V1742B_BOARD_CFG_TGOUT_SIG_MSK );
        return static_cast<TrgOutSignal_t>( value >> V1742B_BOARD_CFG_TGOUT_SIG_SHFT );
    }

    void V1742B::WriteStartSource( StartSource_t source )
    {
        WriteRegister32( V1742B_ACQ_CTRL, static_cast<uint32_t>(source), V1742B_ACQ_CTRL_SRC_MSK );
    }

    V1742B::StartSource_t V1742B::ReadStartSource()
    {
        return static_cast<StartSource_t>( ReadRegister32( V1742B_ACQ_CTRL, V1742B_ACQ_CTRL_SRC_MSK ) );
    }

    void V1742B::Start()
    {
        uint32_t value = ReadRegister32( V1742B_ACQ_CTRL );
        WriteRegister32( V1742B_ACQ_CTRL, value | (1U << V1742B_ACQ_CTRL_START_SHFT) );
    }

    void V1742B::Stop()
    {
        uint32_t value = ReadRegister32( V1742B_ACQ_CTRL );
        WriteRegister32( V1742B_ACQ_CTRL, value & ~(1U << V1742B_ACQ_CTRL_START_SHFT) );
    }

    uint32_t V1742B::ReadAcqStatus()
    {
        return ReadRegister32( V1742B_ACQ_STATUS, static_cast<uint32_t>( AcqStatusBit::All ) );
    }

    bool V1742B::ReadAcqStatus( AcqStatusBit bit )
    {
        return ReadRegister32( V1742B_ACQ_STATUS, static_cast<uint32_t>( bit ) );
    }
    //****** ACQUISITION - ******

    //****** FRONT PANEL + ******
    void V1742B::WriteLEMOLevel( Level_t level )
    {
        uint32_t value = ReadRegister32( V1742B_FRNT_PNL_IO_CTRL );
        switch( level )
        {
            case( Level_t::NIM ) : value &= ~(1U << V1742B_FRNT_PNL_IO_LEMO_SHFT); break;
            case( Level_t::TTL ) : value |=  (1U << V1742B_FRNT_PNL_IO_LEMO_SHFT); break;
        }
        WriteRegister32( V1742B_FRNT_PNL_IO_CTRL, value );
    }

    V1742B::Level_t V1742B::ReadLEMOLevel()
    {
        uint32_t value = ReadRegister32( V1742B_FRNT_PNL_IO_CTRL, (1U << V1742B_FRNT_PNL_IO_LEMO_SHFT) );
        return static_cast<Level_t>( value );
    }

    void V1742B::WriteTRGINSync( TrgInSync_t type )
    {
        uint32_t value = ReadRegister32( V1742B_FRNT_PNL_IO_CTRL );
        switch( type )
        {
            case( TrgInSync_t::Edge )     : value &= ~(1U << V1742B_FRNT_PNL_IO_TGIN_CTRL_SHFT); break;
            case( TrgInSync_t::Duration ) : value |=  (1U << V1742B_FRNT_PNL_IO_TGIN_CTRL_SHFT); break;
        }
        WriteRegister32( V1742B_FRNT_PNL_IO_CTRL, value );
    }

    V1742B::TrgInSync_t V1742B::ReadTRGINSync()
    {
        uint32_t value = ReadRegister32( V1742B_FRNT_PNL_IO_CTRL, (1U << V1742B_FRNT_PNL_IO_TGIN_CTRL_SHFT) );
        return static_cast<TrgInSync_t>( value >> V1742B_FRNT_PNL_IO_TGIN_CTRL_SHFT );
    }

    void V1742B::WriteTRGINToMezz( bool enable )
    {
        uint32_t value = ReadRegister32( V1742B_FRNT_PNL_IO_CTRL );
        if( not enable )    { value &= ~(1U << V1742B_FRNT_PNL_IO_TGIN_MEZZ_SHFT); }
        else                { value |=  (1U << V1742B_FRNT_PNL_IO_TGIN_MEZZ_SHFT); }
        WriteRegister32( V1742B_FRNT_PNL_IO_CTRL, value );
    }

    bool V1742B::ReadTRGINToMezz()
    {
        return ReadRegister32( V1742B_FRNT_PNL_IO_CTRL, (1U << V1742B_FRNT_PNL_IO_TGIN_MEZZ_SHFT) );
    }
    //****** FRONT PANEL - ******

    void V1742B::ReadConfig( UConfig<V1742B>& config )
    {
        //Interrupts
        config.INTERRUPTS.IRQ_EVENTS = ReadIRQEvents();
        config.INTERRUPTS.IRQ_LEVEL  = ReadIRQLevel();
        config.INTERRUPTS.IRQ_VECTOR = ReadIRQVector();

        //Trigger
        config.TRIGGER.POST_TRIGGER = ReadPostTrigger( Group_t::G0 );
        for( uint8_t ch = 0; ch < fChNumber; ++ch )
        {
            config.TRIGGER.CHANNELS[ch].THRESHOLD = ReadChannelThreshold( ch );
            config.TRIGGER.CHANNELS[ch].OFFSET = ReadChannelOffset( ch );
            config.TRIGGER.CHANNELS[ch].ENABLE_TRG = ReadEnableTrigger( ch );
        }

        config.TRIGGER.TR_0.THRESHOLD = ReadThresholdTR( TR_t::TR0 );
        config.TRIGGER.TR_0.OFFSET = ReadOffsetTR( TR_t::TR0 );
        config.TRIGGER.TR_0.ENABLE_TRG = ReadEnableTriggerTR();

        config.TRIGGER.TR_1.THRESHOLD = ReadThresholdTR( TR_t::TR1 );
        config.TRIGGER.TR_1.OFFSET = ReadOffsetTR( TR_t::TR1 );
        config.TRIGGER.TR_1.ENABLE_TRG = ReadEnableTriggerTR();

        config.TRIGGER.GLB_TRG = ReadGlobalTrigger();
        config.TRIGGER.TRG_POL = ReadTriggerPolarity();

        //Acquisition
        for( size_t g = 0; g < fGroupNumber; ++g )
        {
            config.ACQUISITION.GR_ENABLE[g] = ReadReadoutEnableGroup( static_cast<Group_t>( g ) );
        }
        config.ACQUISITION.TR_ENABLE = ReadReadoutEnableTR();
        config.ACQUISITION.REC_LENGTH = ReadRecordLength();
        config.ACQUISITION.SAMP_RATE = ReadSamplingRate( Group_t::G0 );
        config.ACQUISITION.MAX_EVENTS_BLT = ReadMaxEventBLT();
        config.ACQUISITION.ACQ_MODE = ReadAcqMode();
        config.ACQUISITION.START_SOURCE = ReadStartSource();

        //Front panel
        config.FRONT_PANEL.LEMO_LVL = ReadLEMOLevel();

        config.FRONT_PANEL.TRG_IN.GATE_VETO_ENABLE = ReadTRGINEnable();
        config.FRONT_PANEL.TRG_IN.SIGNAL_TYPE = ReadTRGINSignal();
        config.FRONT_PANEL.TRG_IN.DIRECT_TO_MEZZ = ReadTRGINToMezz();
        config.FRONT_PANEL.TRG_IN.SYNC_TYPE = ReadTRGINSync();

        config.FRONT_PANEL.TRG_OUT.SIGNAL_TYPE = ReadTRGOUTSignal();
        config.FRONT_PANEL.TRG_OUT.SIGNAL_SOURCE = ReadTRGOUTGeneration();
    }

    void V1742B::WriteConfig( const UConfig<V1742B>& config )
    {
        //Interrupts
        WriteIRQEvents( config.INTERRUPTS.IRQ_EVENTS );
        WriteIRQLevel( config.INTERRUPTS.IRQ_LEVEL );
        WriteIRQVector( config.INTERRUPTS.IRQ_VECTOR );

        //Trigger
        WritePostTrigger( config.TRIGGER.POST_TRIGGER );
        for( uint8_t ch = 0; ch < fChNumber; ++ch )
        {
            WriteChannelThreshold( ch, config.TRIGGER.CHANNELS[ch].THRESHOLD );
            WriteChannelOffset( ch, config.TRIGGER.CHANNELS[ch].OFFSET );
            WriteEnableTrigger( ch, config.TRIGGER.CHANNELS[ch].ENABLE_TRG );
        }

        WriteThresholdTR( TR_t::TR0, config.TRIGGER.TR_0.THRESHOLD );
        WriteOffsetTR( TR_t::TR0, config.TRIGGER.TR_0.OFFSET );
        WriteEnableTriggerTR( config.TRIGGER.TR_0.ENABLE_TRG );
         
        WriteThresholdTR( TR_t::TR1, config.TRIGGER.TR_1.THRESHOLD );
        WriteOffsetTR( TR_t::TR1, config.TRIGGER.TR_1.OFFSET );
        WriteEnableTriggerTR( config.TRIGGER.TR_1.ENABLE_TRG );

        WriteGlobalTrigger( config.TRIGGER.GLB_TRG );
        WriteTriggerPolarity( config.TRIGGER.TRG_POL );

        //Acquisition
        for( size_t g = 0; g < fGroupNumber; ++g )
        {
            WriteReadoutEnableGroup( static_cast<Group_t>( g ), config.ACQUISITION.GR_ENABLE[g] );
        }
        WriteReadoutEnableTR( config.ACQUISITION.TR_ENABLE );
        WriteRecordLength( config.ACQUISITION.REC_LENGTH );
        WriteSamplingRate( config.ACQUISITION.SAMP_RATE );
        WriteMaxEventBLT( config.ACQUISITION.MAX_EVENTS_BLT );
        WriteAcqMode( config.ACQUISITION.ACQ_MODE );
        WriteStartSource( config.ACQUISITION.START_SOURCE );
        
        //Front Panel
        WriteLEMOLevel( config.FRONT_PANEL.LEMO_LVL );

        WriteTRGINEnable( config.FRONT_PANEL.TRG_IN.GATE_VETO_ENABLE );
        WriteTRGINSignal( config.FRONT_PANEL.TRG_IN.SIGNAL_TYPE );
        WriteTRGINToMezz( config.FRONT_PANEL.TRG_IN.DIRECT_TO_MEZZ );
        WriteTRGINSync( config.FRONT_PANEL.TRG_IN.SYNC_TYPE );

        WriteTRGOUTSignal( config.FRONT_PANEL.TRG_OUT.SIGNAL_TYPE );
        WriteTRGOUTGeneration( config.FRONT_PANEL.TRG_OUT.SIGNAL_SOURCE );
    }

    //****************************
    //****** UEVENT<V1742B> ******
    //****************************
    const std::array<UEvent<V1742B>::Helper12Bit, UEvent<V1742B>::fNHelpers> UEvent<V1742B>::fH =
                                        {
                                            Helper12Bit( false, 0,  0, 0x00000FFF, 0x000000000, 0 ),
                                            Helper12Bit( false, 12, 0, 0x00FFF000, 0x000000000, 0 ),
                                            Helper12Bit( true , 24, 8, 0xFF000000, 0x00000000F, 0 ),
                                            Helper12Bit( false,  4, 0, 0x0000FFF0, 0x000000000, 1 ),
                                            Helper12Bit( false, 16, 0, 0x0FFF0000, 0x000000000, 1 ),
                                            Helper12Bit( true,  28, 4, 0xF0000000, 0x0000000FF, 1 ),
                                            Helper12Bit( false,  8, 0, 0x000FFF00, 0x000000000, 2 ),
                                            Helper12Bit( false, 20, 0, 0xFFF00000, 0x000000000, 2 )
                                        };

    bool UEvent<V1742B>::Fill( size_t index, const VBuffer &buffer )
    {
        for( ; index < buffer.GetSize(); ++index )
        {
            if( buffer[index] != V1742B_WORD_TYPE_FILLER ) { break; }
        }
        // Parse Header
        if( index + fHeader.size() > buffer.GetSize() ) { return false; }
        if( (buffer[index] & 0xF0000000) != 0xA0000000 ) { return false; }

        fStart = index;
        for( size_t i = 0; i < fHeader.size(); ++i, ++index ) { fHeader[i] = buffer[index]; }
        index--;
        // Parse Groups
        for( size_t g = 0; g < V1742B::GetGroupNumber(); ++g )
        {
            fData[g].Clear();
            // Parse Group Header
            if( ((1U << g) & GetGroupMask()) )
            {
                ++index;
                if( index >= buffer.GetSize() ) { return false; }
                fData[g].fHeader = buffer[index];
                // Parse Channels
                ++index;
                size_t ch07Size = fData[g].GetChannelSize();
                if( index + ch07Size > buffer.GetSize() ) { return false; }
                for( size_t i = 0; i < ch07Size; i += 3 )
                {
                    for( size_t j = 0; j < fNHelpers; ++j )
                    {
                        fData[g].fData[j].push_back( Read12BitWord( buffer, index + i, j ) );
                    }
                }
                index += ch07Size;
                // Parse TRn if present
                if( fData[g].GetTRPresent() )
                {
                    size_t trSize = fData[g].GetChannelSize() / V1742B::GetChInGroup();
                    if( index + trSize > buffer.GetSize() ) { return false; }
                    for( size_t i = 0; i < trSize; i += 3 )
                    {
                        for( size_t j = 0; j < fNHelpers; ++j )
                        {
                            fData[g].fTR.push_back( Read12BitWord( buffer, index + i, j ) );
                        }
                    }
                    index += trSize;
                }
                // Parse Trigger Time Tag
                fData[g].fTTT = buffer[index];
            }
        }
        fStop = index;
        return true;
    }

    const UEvent<V1742B>::Group& UEvent<V1742B>::GetGroup( V1742B::Group_t g ) const
    {
        return fData.at( static_cast<uint8_t>( g ) );
    }

    void UEvent<V1742B>::Group::Clear()
    {
        fHeader = 0;
        fTTT = 0;
        fTR.clear();
        for( auto& ch : fData ) { ch.clear(); }
    }

    void UEvent<V1742B>::Group::Print() const
    {
        std::cout << "========== GROUP::" << V1742B::GetName() << " (start)===========\n";
        std::cout << std::setw( 20 ) << "Start index: " << GetStartIndex() << "\n";
        std::cout << std::setw( 20 ) << "Sampling rate: " << (unsigned)GetSamplingRate() << "\n";
        std::cout << std::setw( 20 ) << "TRn present: " << GetTRPresent() << "\n";
        std::cout << std::setw( 20 ) << "ChannelSize: " << GetChannelSize() << "\n";
        std::cout << std::setw( 20 ) << "TTT: " << GetTTT() << "\n";
        std::cout << "========== GROUP::" << V1742B::GetName() << " ( end )===========\n";
    }

    void UEvent<V1742B>::Print() const
    {
        std::cout << "================== EVENT::" << V1742B::GetName() << " (start)==================\n";
        std::cout << std::setw( 20 ) << "Event size: " << GetEventSize() << "\n";
        std::cout << std::setw( 20 ) << "Board ID: " << (unsigned)GetBoardID() << "\n";
        std::cout << std::setw( 20 ) << "FAIL: " << GetBoardFail() << "\n";
        std::cout << std::setw( 20 ) << "LVDS: " << (unsigned)GetLVDSPattern() << "\n";
        std::cout << std::setw( 20 ) << "Group mask: " << (unsigned)GetGroupMask() << "\n";
        std::cout << std::setw( 20 ) << "Event counter: " << GetEventCounter() << "\n";
        std::cout << std::setw( 20 ) << "Event TTT: " << GetEventTTT() << "\n";
        std::cout << "================== EVENT::" << V1742B::GetName() << "  (end)===================\n";
    }
}
