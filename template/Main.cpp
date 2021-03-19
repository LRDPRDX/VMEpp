#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"

#include <iostream>

using namespace vmeplus;

int main()
{
    V2718 controller;
    //address??????????????
    V1190B tdc("V1190B", 0x00000000, 0xFFFFFFFF);
    
    try
    {
        controller.Open( 0, 0 ); 
        controller.RegisterSlave( &tdc );
        controller.Initialize();

        tdc.WriteMicro((V1190B::Opcode){V1190B::Command::SET_WIN_WIDTH, 0x00});
        //from 1 to 2088 (0xFFF), Default setting: 0x14 -> 500 ns
        tdc.WriteMicro(0x15);

        tdc.WriteMicro((V1190B::Opcode){V1190B::Command::SET_WIN_OFFS, 0x00});
        //from –2048 (hex 0xF800) to +40 (hex 0x0028), Default setting: 0xFFD8 -> -1 μs
        tdc.WriteMicro(0xFFD9);

        tdc.WriteMicro((V1190B::Opcode){V1190B::Command::SET_SW_MARGIN, 0x00});
        //the margin value (clock cycles) can be any 12 bit value (bits [12;15] are meaningless),
        // Default setting: 0x08 -> 200 ns
        tdc.WriteMicro(0x08);

        tdc.WriteMicro((V1190B::Opcode){V1190B::Command::SET_REJ_MARGIN, 0x00});
        //The margin value can be any 12 bit value (bits [12;15] are meaningless),
        // 0 sets the margin at the beginning of the match window, Default setting: 0x04 -> 100 ns
        tdc.WriteMicro(0x05);

        tdc.WriteMicro((V1190B::Opcode){V1190B::Command::EN_SUB_TRG, 0x00});

        tdc.WriteMicro((V1190B::Opcode){V1190B::Command::READ_TRG_CONF, 0x00});
        //LSB = 0 -> trigger time subtraction disabled;
        //LSB = 1 -> trigger time subtraction enabled

        std::cout << "WIN WIDTH " << tdc.ReadMicro() << std::endl;
        std::cout << "WIN OFFS " << tdc.ReadMicro() << std::endl;
        std::cout << "EXTRA SEARCH MARGIN " << tdc.ReadMicro() << std::endl;
        std::cout << "REJECT MARGIN " << tdc.ReadMicro() << std::endl;
        std::cout << "SUB TRG " << tdc.ReadMicro() << std::endl;
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << "\n"; 
        std::cerr << e.GetInfo() << "\n"; 
    }

    return 0;
}
