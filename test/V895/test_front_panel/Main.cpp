#include "VException.h"
#include "modules/V2718.h"
#include "modules/V895.h"

#include <iostream>
#include <unistd.h>

using namespace vmepp;

int main()
{
    V2718 controller;
    //Instantiate a descriminator
    V895 dm( 0x40080000 );
    
    try
    {
        controller.Open( 0, 0 ); 
        controller.RegisterSlave( &dm );
        controller.Initialize();

        //Print some info
        dm.Print();

        dm.WriteOutWidth( 43 );
        //Set 10mV threshold on each channel
        //(for test purposes)
        for( uint16_t ch = 0; ch < 16; ch++ )
        {
            dm.WriteThreshold( ch, 10 );
        }
        //But enable only the channels 15
        dm.Enable( 0x8000 );
    }
    catch( VException &sw )
    {
        std::cerr << sw.what() << "\n"; 
        std::cerr << sw.GetInfo() << "\n"; 
    }
    return 0;
}
