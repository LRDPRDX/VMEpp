#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

#include <iostream>
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

        tdc.Reset();

        almostFullN = tdc.ReadIRQEvents();
        assert( almostFullN == 64 ); // default after reset

        std::cout << "Test has been passed...OK!\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
