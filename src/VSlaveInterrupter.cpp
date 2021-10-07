#include "VSlaveInterrupter.h"

namespace vmeplus
{
    VSlaveInterrupter::VSlaveInterrupter( uint32_t address, uint32_t range ) :
        VSlave( address, range )
    {
    }

    VSlaveInterrupter::~VSlaveInterrupter()
    {
    }
}
