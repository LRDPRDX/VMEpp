#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

#include <iostream>
#include <iomanip>
#include <cassert>

using namespace vmeplus;

typedef typename V1190B::TriggerMode_t Trg_t;
typedef typename V1190B::Config_t      Cfg_t;

int main()
{
    V2718 controller;
    V1190B tdc(0x20080000);
    
    try
    {
        std::cout << "Test begins...\n";
        controller.Open( 0, 0 ); 
        controller.RegisterSlave( &tdc );
        controller.Initialize();

        tdc.Reset();

        uint16_t almostFullN = tdc.ReadIRQEvents();
        std::cout << "AFL : " << almostFullN << "\n";

        tdc.WriteIRQEvents( 300 );
        almostFullN = tdc.ReadIRQEvents();
        assert( almostFullN == 300 );

        tdc.WriteIRQEvents( 0 );
        almostFullN = tdc.ReadIRQEvents();
        assert( almostFullN == 1 );

        uint32_t eCounter = tdc.ReadEventCounter();
        uint16_t nEvents = tdc.ReadEventsStored();

        tdc.Reset();

        almostFullN = tdc.ReadIRQEvents();
        assert( almostFullN == 64 ); // default after reset

        tdc.WriteAcqMode( V1190B::TriggerMode_t::MATCHING );

        tdc.SendSWTrigger();
        std::cout << std::setw( 20 ) << "EVENT COUNTER : " << eCounter << "\n";
        std::cout << std::setw( 20 ) << "EVENTS STORED : " << nEvents << "\n";

        tdc.WriteDummy32( 0x12345678 );
        uint32_t dummy32 = tdc.ReadDummy32();

        tdc.WriteDummy16( 0x1234 );
        uint16_t dummy16 = tdc.ReadDummy16();

        assert( dummy32 == 0x12345678 );
        assert( dummy16 == 0x1234 );

        V1190B::ProgOUT pout = V1190B::ProgOUT::DATA_READY;

        tdc.WritePOut( V1190B::ProgOUT::DATA_READY );
        pout = tdc.ReadPOut();
        assert( pout == V1190B::ProgOUT::DATA_READY );

        tdc.WritePOut( V1190B::ProgOUT::ALM_FULL );
        pout = tdc.ReadPOut();
        assert( pout == V1190B::ProgOUT::ALM_FULL );

        tdc.WritePOut( V1190B::ProgOUT::FULL );
        pout = tdc.ReadPOut();
        assert( pout == V1190B::ProgOUT::FULL );

        tdc.WritePOut( V1190B::ProgOUT::ERROR );
        pout = tdc.ReadPOut();
        assert( pout == V1190B::ProgOUT::ERROR );

        std::cout << "Test has been passed...OK!\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
