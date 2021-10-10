#include "VSlaveInterrupter.h"

namespace vmepp
{
    VSlaveInterrupter::VSlaveInterrupter( uint32_t address, uint32_t range ) :
        VSlave( address, range )
    {
    }
}
