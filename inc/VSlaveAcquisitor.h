#ifndef V_PLUS_SLAVE_ACQUISITOR_H
#define V_PLUS_SLAVE_ACQUISITOR_H

#include <memory>
#include "VSlave.h"

namespace vmeplus
{
    template<typename MODULE_NAME>
    class UEvent;

    template<typename MODULE_NAME>
    class VSlaveAcquisitor : virtual public VSlave
    {
        protected :
            std::unique_ptr<uint32_t[]> fBuffer;
            uint32_t                    fReadBytes;
            uint32_t                    fNEventsRead;
            uint32_t                    fCurrentEvent;
            uint32_t                    fCurrentIndex;
            uint32_t                    fReadCycles;

        public :
            VSlaveAcquisitor( std::string name, uint32_t address, uint32_t range ) :
                VSlave( name, address, range ),

                fReadBytes( 0 ),
                fNEventsRead( 0 ),
                fCurrentEvent( 0 ),
                fCurrentIndex( 0 ),
                fReadCycles( 1 )
            {
            }
            ~VSlaveAcquisitor() override = default;

        public :
            virtual void        AllocateBuffer() = 0;
            virtual uint32_t    ReadBuffer() = 0;
            virtual bool        GetEvent( UEvent<MODULE_NAME> &event );
            virtual bool        GetEventAt( uint32_t index, UEvent<MODULE_NAME> &event ) const = 0;
            void                SetReadCycles( uint32_t n ) { fReadCycles = (n ? (n <= 32 ? n : 32) : 1); }
            uint32_t            GetReadCycles() { return fReadCycles; }

            uint32_t            GetNEventsRead() const { return fNEventsRead; }
            uint32_t            GetReadBytes() const { return fReadBytes; }
            uint32_t            GetCurrentIndex() const { return fCurrentIndex; }
            uint32_t            GetCurrentEvent() const { return fCurrentEvent; }

            void                ResetIndex()
                                {
                                  fCurrentEvent = 0;
                                  fCurrentIndex = 0;
                                  fNEventsRead = 0;
                                }
    };

    template<typename MODULE_NAME>
    bool VSlaveAcquisitor<MODULE_NAME>::GetEvent( UEvent<MODULE_NAME> &event )
    {
        if( GetEventAt( fCurrentIndex, event ) )
        {
            fCurrentIndex = event.GetStop() + 1;
            fNEventsRead++;
            return true;
        }
        else
        {
            return false;
        }
    }
}

#endif
