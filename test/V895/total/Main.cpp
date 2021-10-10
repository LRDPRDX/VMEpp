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

        dm.WriteOutWidth( 0xFF );
        //Set 10mV threshold on each channel
        //(for test purposes)
        //and output width to 40ns (see V895's manual)
        for( uint8_t ch = 0; ch < dm.GetChNumber(); ch++ )
        {
            dm.WriteThreshold( ch, 10 );
        }
        //But enable only channel 12
        dm.EnableOnly( 12 );
        //Set majority level to 3 channels
        //I.e. when number of active channels >= 3
        //the module send a NIM high to the MAJ output
        dm.WriteMajLevel( 3 );
        //Generate every (enabled) outputs 100 times
        //(for test purposes)
        int counter = 100;
        std::cout << "Wait...\n";
        while( counter )
        {
            dm.SendTest();
            counter--;
            usleep( 100000 ); //~10Hz
        }
        //Release resources (not necessary)
        //controller.Close();        
    }
    catch( const VException& e )
    {
        std::cerr << e.what() << "\n"; 
        std::cerr << e.GetInfo() << "\n"; 
    }
    return 0;
}
