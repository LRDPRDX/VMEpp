#ifndef V_PLUS_EVENT_H
#define V_PLUS_EVENT_H

#include <cstdint>

namespace vmeplus
{
    class VEvent
    {
        protected :
            uint32_t fStart;
            uint32_t fStop;

        public :
            VEvent();
            virtual     ~VEvent();

        public :
            virtual uint32_t GetSize() const = 0;

            uint32_t GetStart() const { return fStart; }
            uint32_t GetStop() const { return fStop; }
            void     Reset() { fStart= 0; fStop = 0; }

            
    };
}
#endif
