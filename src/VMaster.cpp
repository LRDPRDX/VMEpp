#include "VMaster.h"
#include "VSlave.h"
#include "VException.h"

#include <sstream>

namespace vmeplus
{
    void VMaster::RegisterSlave( VSlave *slave )
    {
        if( slave == nullptr )
        {
            throw( VException( VError_t::vBadSlave, "Slave pointer is invalid" ) );
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


    void VMaster::UnregisterSlave( VSlave *slave )
    {
        for( auto it = fSlaves.begin(); it != fSlaves.end(); )
        {
            if( *it == slave )
            {
                (*it)->fMaster = nullptr;
                it = fSlaves.erase( it ); 
            }
            else
            {
                ++it;
            }
        }
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
