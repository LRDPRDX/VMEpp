#include "VMaster.h"
#include "VSlave.h"
#include "VException.h"

#include <sstream>
#include <algorithm>

namespace vmepp
{
    VMaster::~VMaster()
    {
        auto temp = fSlaves;
        for( auto s : temp ) { s->Release(); }
    }

    void VMaster::RegisterSlave( VSlave *slave )
    {
        if( (slave == nullptr) or (slave->fMaster != nullptr) )
        {
            throw( VException( VError_t::vBadSlave, "Slave pointer is invalid, or this slave is already has a master (might be the one you're trying to register to)" ) );
        }
        uint32_t newB = slave->GetBaseAddress();
        uint32_t newR = slave->GetRange();
        for( auto it = fSlaves.cbegin(); it != fSlaves.cend(); ++it )
        {
            uint32_t curB = (*it)->GetBaseAddress();
            uint32_t curR = (*it)->GetRange();
            //Check for overlapping of addresses
            if( std::max( curB, newB ) <= std::min( curB + curR, newB + newR ) )
            {
                throw( VException( VError_t::vBadSlave, "Trying to register a slave with a bad address (address overlapping) :" + std::to_string( newB ) ) );
            }
        }
        slave->fMaster = this;
        fSlaves.push_back( slave );
    }


    void VMaster::UnregisterSlave( VSlave *slave ) noexcept( true )
    {
        slave->fMaster = nullptr;
        fSlaves.remove( slave );
    }


    std::string VMaster::HelpStringCycle( uint32_t address, CVAddressModifier am, CVDataWidth dw ) const
    {
        std::stringstream ss;
        ss << std::hex << "Address: " << std::hex << address << " AM: " << std::hex << am << " DW: " << dw;
        return ss.str();
    }

    std::string VMaster::HelpStringCycle( uint32_t address, CVAddressModifier am ) const
    {
        std::stringstream ss;
        ss << std::hex << "Address: " << std::hex << address << " AM: " << std::hex << am;
        return ss.str();
    }

    void VMaster::Initialize()
    {
        PrintMessage( Message_t::INFO, "Initializing registered boards..." );
        for( auto it = fSlaves.cbegin(); it != fSlaves.cend(); ++it )
        {
            if( (*it) != nullptr )
            {
                (*it)->Initialize();
            }
        }
    }
}
