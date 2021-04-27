#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

#include <iostream>
#include <cassert>

using namespace vmeplus;

//typedef typename V1190B::ChannelPattern Pat;
typedef typename V1190B::TDC Tdc;

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

        tdc.WriteResetLoops();

        uint16_t fw = tdc.ReadMicroFWRev();
        std::cout << std::hex << "FW : " << fw << "\n";

        uint32_t tdcID0 = tdc.ReadTDC_ID( Tdc::TDC0 );
        uint32_t tdcID1 = tdc.ReadTDC_ID( Tdc::TDC1 );

        assert( (tdcID0 & 0xFFFFFFF0) == 0x8470dac0 );
        assert( (tdcID1 & 0xFFFFFFF0) == 0x8470dac0 );

        std::cout << "Test has been passed...OK!\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
