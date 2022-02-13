#ifndef V_PLUS_EVENT_H
#define V_PLUS_EVENT_H

#include <cstddef>

#include "VModule.h"
#include "VSlaveAcquisitor.h"


namespace vmepp
{
    class VEvent
    {
        protected :
            size_t fStart;
            size_t fStop;

        public :
            VEvent() :
                fStart( 0 ),
                fStop( 0 )
            { };
            virtual ~VEvent() = default;

        public :
            virtual bool Fill( size_t index, const VBuffer &buffer ) = 0;

        public :
            size_t  GetStart() const { return fStart; }
            size_t  GetStop() const { return fStop; }
            void    Reset() { fStart = 0; fStop = 0; }
    };
}
#endif
