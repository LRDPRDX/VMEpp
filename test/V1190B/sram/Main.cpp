#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

#include <iostream>
#include <cassert>
#include <vector>
#include <fstream>

using namespace vmepp;

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

        std::vector<int8_t> inl;
        tdc.ReadCompensation( V1190B::TrLeadLSB::ps100, 10, inl );

        std::ofstream file;
        file.open( "data.dat" );

        for( size_t i = 0; i < inl.size(); ++i )
        {
            file << i << " " << (int)inl[i] << "\n";
        }

        std::cout << "Test has been passed...OK!\n";
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
