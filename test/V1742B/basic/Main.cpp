#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1742B.h"

#include <iostream>
#include <unistd.h>
#include <cassert>

#include "cereal/archives/xml.hpp"

using namespace vmepp;

int main()
{
    V2718 controller;
    V1742B digitizer( 0x50000000 );

    try
    {
        controller.Open( 0, 0 );
        controller.RegisterSlave( &digitizer );
        controller.Initialize();

        // Board configuration register
        digitizer.WriteTestModeEnable( true );
        assert( digitizer.ReadTestModeEnable() == true );

        digitizer.WriteTestModeEnable( false );
        assert( digitizer.ReadTestModeEnable() == false );

        digitizer.WriteTriggerPolarity( V1742B::TriggerPolarity_t::RisingEdge );
        assert( digitizer.ReadTriggerPolarity() == V1742B::TriggerPolarity_t::RisingEdge );

        digitizer.WriteTriggerPolarity( V1742B::TriggerPolarity_t::FallingEdge );
        assert( digitizer.ReadTriggerPolarity() == V1742B::TriggerPolarity_t::FallingEdge );

        digitizer.WriteReadoutEnableTR();
        assert( digitizer.ReadReadoutEnableTR() == true );

        digitizer.WriteReadoutEnableTR( false );
        assert( digitizer.ReadReadoutEnableTR() == false );

        digitizer.WriteReadoutEnableTR();
        assert( digitizer.ReadReadoutEnableTR() == true );

        digitizer.WriteEnableTriggerTR( false );
        assert( digitizer.ReadEnableTriggerTR() == false );

        digitizer.WriteAcqMode( V1742B::AcqMode_t::Output );
        assert( digitizer.ReadAcqMode() == V1742B::AcqMode_t::Output );

        digitizer.WriteAcqMode( V1742B::AcqMode_t::Transparent );
        assert( digitizer.ReadAcqMode() == V1742B::AcqMode_t::Transparent );

        digitizer.WriteTRGINEnable();
        assert( digitizer.ReadTRGINEnable() == true );

        digitizer.WriteTRGINEnable( false );
        assert( digitizer.ReadTRGINEnable() == false );

        digitizer.WriteTRGINSignal( V1742B::TrgInSignal_t::Gate );
        assert( digitizer.ReadTRGINSignal() == V1742B::TrgInSignal_t::Gate );

        digitizer.WriteTRGINSignal( V1742B::TrgInSignal_t::Veto );
        assert( digitizer.ReadTRGINSignal() == V1742B::TrgInSignal_t::Veto );

        digitizer.WriteTRGOUTSignal( V1742B::TrgOutSignal_t::NoSignal );
        assert( digitizer.ReadTRGOUTSignal() == V1742B::TrgOutSignal_t::NoSignal );

        digitizer.WriteTRGOUTSignal( V1742B::TrgOutSignal_t::AllTRn );
        assert( digitizer.ReadTRGOUTSignal() == V1742B::TrgOutSignal_t::AllTRn );

        digitizer.WriteTRGOUTSignal( V1742B::TrgOutSignal_t::AcceptedTRn );
        assert( digitizer.ReadTRGOUTSignal() == V1742B::TrgOutSignal_t::AcceptedTRn );

        digitizer.WriteTRGOUTSignal( V1742B::TrgOutSignal_t::BusyGroups );
        assert( digitizer.ReadTRGOUTSignal() == V1742B::TrgOutSignal_t::BusyGroups );


        // Acquisition Control Register
        digitizer.WriteStartSource( V1742B::StartSource_t::SW );
        assert( digitizer.ReadStartSource() == V1742B::StartSource_t::SW );

        // Acquisition Status Register
        std::cout << "Acquisition Status Register :\n";
        std::cout << "RUN           : " << digitizer.ReadAcqStatus( V1742B::AcqStatusBit::AcqRun ) << "\n";
        std::cout << "Event Ready   : " << digitizer.ReadAcqStatus( V1742B::AcqStatusBit::EventReady ) << "\n";
        std::cout << "Event Full    : " << digitizer.ReadAcqStatus( V1742B::AcqStatusBit::EventFull ) << "\n";
        std::cout << "PLL Clk Ext   : " << digitizer.ReadAcqStatus( V1742B::AcqStatusBit::PLLClockExt ) << "\n";
        std::cout << "PLL Good      : " << digitizer.ReadAcqStatus( V1742B::AcqStatusBit::PLLGood ) << "\n";
        std::cout << "Board Ready   : " << digitizer.ReadAcqStatus( V1742B::AcqStatusBit::BoardReady ) << "\n";
        std::cout << "S-IN status   : " << digitizer.ReadAcqStatus( V1742B::AcqStatusBit::StatusSIN ) << "\n";
        std::cout << "TRG-IN status : " << digitizer.ReadAcqStatus( V1742B::AcqStatusBit::StatusTRGIN ) << "\n";

        // Front Panel
        digitizer.WriteLEMOLevel( V1742B::Level_t::NIM );
        assert( digitizer.ReadLEMOLevel() == V1742B::Level_t::NIM );

        digitizer.WriteLEMOLevel( V1742B::Level_t::TTL );
        assert( digitizer.ReadLEMOLevel() == V1742B::Level_t::TTL );
    }
    catch( const VException& e )
    {
        std::cout << e.what() << "\n";
        std::cout << e.GetInfo() << "\n";
        std::cout << e.GetHint() << "\n";
    }
    return 0;
}
