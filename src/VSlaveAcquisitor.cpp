#include "VSlaveAcquisitor.h"
#include "VEvent.h"

namespace vmeplus
{
    VSlaveAcquisitor::VSlaveAcquisitor( std::string name, uint32_t address, uint32_t range ) :
        VSlave( name, address, range ),

        fReadBytes( 0 ),
        fNEventsRead( 0 ),
        fCurrentEvent( 0 ),
        fCurrentIndex( 0 ),
        fReadCycles( 1 )
    {
    }

    VSlaveAcquisitor::~VSlaveAcquisitor()
    {
    }
    

    bool VSlaveAcquisitor::GetEvent( VEvent *event )
    {   
        if( GetEventAt( fCurrentIndex, event ) )
        {
            fCurrentIndex = event->GetStop() + 1;
            fNEventsRead++;
            return true;
        }
        else
        {   
            return false; 
        }   
    }   
}
