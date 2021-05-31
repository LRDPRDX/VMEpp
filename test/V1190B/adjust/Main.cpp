#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

#include <iostream>
#include <cassert>

using namespace vmeplus;

typedef typename V1190B::AdjustOffset AdjustOffset; 

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

        // Global offset
        AdjustOffset gOffset;
        tdc.WriteGlobalOffset( AdjustOffset( 0x0321, 0x0011 ) );
        tdc.ReadGlobalOffset( gOffset );
        assert( gOffset.coarseCounter == 0x0321 );
        assert( gOffset.fineCounter == 0x0011 );

        tdc.WriteGlobalOffset( AdjustOffset( 0x1567, 0x0035 ) ); // the overflowed values set on purpose
        tdc.ReadGlobalOffset( gOffset );
        assert( gOffset.coarseCounter == 0x0567 ); // Only 11 LSB actually are meaningful
        assert( gOffset.fineCounter == 0x0015 );   // Only 5 LSB actually are meaningful

        // Channel offset
        for( uint8_t i = 0; i < tdc.GetChNumber(); ++i )
        {
            tdc.WriteAdjustChannel( i * 4, i );
        }

        for( uint8_t i = 0; i < tdc.GetChNumber(); ++i )
        {
            uint16_t offs = tdc.ReadAdjustChannel( i );
            assert( offs == i * 4 );
        }

        std::cout << "Test has been passed...OK!\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
