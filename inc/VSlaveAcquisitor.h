#ifndef V_PLUS_SLAVE_ACQUISITOR_H
#define V_PLUS_SLAVE_ACQUISITOR_H

#include <memory>
#include "VSlave.h"

namespace vmeplus
{
    class VEvent;

    class VSlaveAcquisitor : virtual public VSlave
    {
        protected :
            //uint32_t           *fBuffer;
            
            std::unique_ptr<uint32_t[]> fBuffer;
            uint32_t                    fReadBytes;
            uint32_t                    fNEventsRead;
            uint32_t                    fCurrentEvent;
            uint32_t                    fCurrentIndex;
            uint32_t                    fReadCycles;

        public :
            VSlaveAcquisitor( std::string name, uint32_t address, uint32_t range );
            virtual ~VSlaveAcquisitor();

        public :
            virtual void        AllocateBuffer() = 0;
            virtual uint32_t    ReadBuffer() = 0;
            virtual bool        GetEvent( VEvent *event );
            virtual bool        GetEventAt( uint32_t index, VEvent *event ) const = 0;
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
}
#endif
