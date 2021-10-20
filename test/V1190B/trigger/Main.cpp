#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

#include <iostream>

using namespace vmepp;

int main()
{
    V2718 controller;
    V1190B tdc(0x20080000);
    
    try
    {
        controller.Open( 0, 0 ); 
        controller.RegisterSlave( &tdc );
        controller.Initialize();

        tdc.WriteWindowWidth(0x15);
        //from 1 to 2088 (0xFFF), Default setting: 0x14 -> 500 ns

        tdc.WriteWindowOffset(0xFFD9);
        //from –2048 (hex 0xF800) to +40 (hex 0x0028), Default setting: 0xFFD8 -> -1 μs

        tdc.WriteExtraSearchMargin(0x09);
        //the margin value (clock cycles) can be any 12 bit value (bits [12;15] are meaningless),
        // Default setting: 0x08 -> 200 ns

        tdc.WriteRejectMargin(0x05);
        //The margin value can be any 12 bit value (bits [12;15] are meaningless),
        // 0 sets the margin at the beginning of the match window, Default setting: 0x04 -> 100 ns
        tdc.WriteEnableSubTrigger(false);
        ///LSB = 0 -> trigger time subtraction disabled;
        ///LSB = 1 -> trigger time subtraction enabled
        
        uint16_t winWidth, winOffs, swMargin, rejMargin, subTrg;
        V1190B::TriggerData trigger(winWidth, winOffs, swMargin, rejMargin, subTrg);

        tdc.ReadTriggerConfiguration(trigger);

        std::cout << "WIN WIDTH " << trigger.winWidth << std::endl;
        std::cout << "WIN OFFS " << trigger.winOffs << std::endl;
        std::cout << "EXTRA SEARCH MARGIN " << trigger.swMargin << std::endl;
        std::cout << "REJECT MARGIN " << trigger.rejMargin << std::endl;
        std::cout << "SUB TRG " << trigger.subTrigger << std::endl;
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
