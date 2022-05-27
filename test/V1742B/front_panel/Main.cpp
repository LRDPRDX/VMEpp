#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1742B.h"

#include <iostream>
#include <cassert>

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

        digitizer.WriteLEMOLevel( V1742B::Level_t::NIM );
        assert( digitizer.ReadLEMOLevel() == V1742B::Level_t::NIM );
        digitizer.WriteLEMOLevel( V1742B::Level_t::TTL );
        assert( digitizer.ReadLEMOLevel() == V1742B::Level_t::TTL );

        //********************
        //****** TRG-IN ******
        //********************
        digitizer.WriteTRGINEnable( true );
        assert( digitizer.ReadTRGINEnable() );
        digitizer.WriteTRGINEnable( false );
        assert( not digitizer.ReadTRGINEnable() );

        digitizer.WriteTRGINSignal( V1742B::TrgInSignal_t::Gate );
        assert( digitizer.ReadTRGINSignal() == V1742B::TrgInSignal_t::Gate );
        digitizer.WriteTRGINSignal( V1742B::TrgInSignal_t::Veto );
        assert( digitizer.ReadTRGINSignal() == V1742B::TrgInSignal_t::Veto );

        digitizer.WriteTRGINSync( V1742B::TrgInSync_t::Edge );
        assert( digitizer.ReadTRGINSync() == V1742B::TrgInSync_t::Edge );
        digitizer.WriteTRGINSync( V1742B::TrgInSync_t::Duration );
        assert( digitizer.ReadTRGINSync() == V1742B::TrgInSync_t::Duration );

        digitizer.WriteTRGINToMezz( true );
        assert( digitizer.ReadTRGINToMezz() );
        digitizer.WriteTRGINToMezz( false );
        assert( not digitizer.ReadTRGINToMezz() );

        //*********************
        //****** TRG-OUT ******
        //*********************
        digitizer.WriteTRGOUTSignal( V1742B::TrgOutSignal_t::NoSignal );
        assert( digitizer.ReadTRGOUTSignal() == V1742B::TrgOutSignal_t::NoSignal );
        digitizer.WriteTRGOUTSignal( V1742B::TrgOutSignal_t::AllTRn );
        assert( digitizer.ReadTRGOUTSignal() == V1742B::TrgOutSignal_t::AllTRn );
        digitizer.WriteTRGOUTSignal( V1742B::TrgOutSignal_t::AcceptedTRn );
        assert( digitizer.ReadTRGOUTSignal() == V1742B::TrgOutSignal_t::AcceptedTRn );
        digitizer.WriteTRGOUTSignal( V1742B::TrgOutSignal_t::BusyGroups );
        assert( digitizer.ReadTRGOUTSignal() == V1742B::TrgOutSignal_t::BusyGroups );

        auto src = V1742B::TrgOutSource_t::None;
        digitizer.WriteTRGOUTGeneration( src );
        assert( digitizer.ReadTRGOUTGeneration() == src );
        src = V1742B::TrgOutSource_t::G0 | V1742B::TrgOutSource_t::G1 | V1742B::TrgOutSource_t::AND;
        digitizer.WriteTRGOUTGeneration( src );
        auto srcRead = digitizer.ReadTRGOUTGeneration();
        assert( (srcRead & V1742B::TrgOutSource_t::G0) == V1742B::TrgOutSource_t::G0 );
        assert( (srcRead & V1742B::TrgOutSource_t::G1) == V1742B::TrgOutSource_t::G1 );
        assert( (srcRead & V1742B::TrgOutSource_t::AND) == V1742B::TrgOutSource_t::AND );
        assert( (srcRead & V1742B::TrgOutSource_t::LVDS) == V1742B::TrgOutSource_t::None );

        PrintMessage( Message_t::INFO, "Test has been passed SUCCESSFULLY!" );
    }
    catch( const VException& e )
    {
        std::cout << e.what() << "\n";
        std::cout << e.GetInfo() << "\n";
        std::cout << e.GetHint() << "\n";
    }
    return 0;
}
