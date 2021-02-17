#include "VSlaveInterrupter.h"

namespace vmeplus
{
    VSlaveInterrupter::VSlaveInterrupter( std::string name, uint32_t address, uint32_t range ) :
        VSlave( name, address, range )
    {
    }

    VSlaveInterrupter::~VSlaveInterrupter()
    {
    }
}
