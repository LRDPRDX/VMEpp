#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

#include <iostream>
#include <cassert>

using namespace vmeplus;

typedef typename V1190B::IError_t Err_t;
typedef typename V1190B::MaxHitsPerEvent NHits;
typedef typename V1190B::FIFOSize NWords;

int main()
{
    std::cout << "Test begins...\n";
    V2718 controller;
    V1190B tdc(0x20080000);
    
    try
    {
        controller.Open( 0, 0 ); 
        controller.RegisterSlave( &tdc );
        controller.Initialize();

        // HEADER & TRAILER
        tdc.WriteEnableHeaderTrailer( true ); 
        bool headTrail = tdc.ReadEnableHeaderTrailer();
        assert( headTrail == true );
        tdc.WriteEnableHeaderTrailer( false );
        headTrail = tdc.ReadEnableHeaderTrailer();
        assert( headTrail == false );

        // MAX HITS PER EVENT
        tdc.WriteMaxHitsPerEvent( NHits::N128 );
        NHits nHits = tdc.ReadMaxHitsPerEvent();
        assert( nHits == NHits::N128 );

        tdc.WriteMaxHitsPerEvent( NHits::N32 );
        nHits = tdc.ReadMaxHitsPerEvent();
        assert( nHits == NHits::N32 );

        tdc.WriteMaxHitsPerEvent( NHits::NO_LIMIT );
        nHits = tdc.ReadMaxHitsPerEvent();
        assert( nHits == NHits::NO_LIMIT );

        tdc.WriteMaxHitsPerEvent( NHits::INVALID );
        nHits = tdc.ReadMaxHitsPerEvent();
        assert( nHits == NHits::INVALID );

        //tdc.WriteMaxHitsPerEvent( 34 ); // ERROR !

        // ERROR MARK & BYPASS
        // no read so just write
        tdc.WriteEnableErrMark( true );
        tdc.WriteEnableErrMark( false );
        tdc.WriteEnableBypass( true );
        tdc.WriteEnableBypass( false );

        // ERROR PATTERN
        uint16_t ePatternW = Err_t::VERNIER | Err_t::L1_BUF_PRTY | Err_t::ROUT_STATE | Err_t::CTRL_PRTY;
        tdc.WriteErrorPattern( ePatternW );
        uint16_t ePatternR = tdc.ReadErrorPattern();
        assert( ePatternW == ePatternR );

        ePatternW = Err_t::ALL;
        tdc.WriteErrorPattern( ePatternW );
        ePatternR = tdc.ReadErrorPattern();
        assert( ePatternW == ePatternR );

        // BUFFER SIZE
        tdc.WriteEffSizeFIFO( NWords::W4 );
        uint16_t nWords = tdc.ReadEffSizeFIFO();
        assert( nWords == 4 );

        tdc.WriteEffSizeFIFO( NWords::W256 );
        nWords = tdc.ReadEffSizeFIFO();
        assert( nWords == 256 );

        //tdc.WriteEffSizeFIFO( 13 ); // ERROR !

        std::cout << "Test has been passed...OK!\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
