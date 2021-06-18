#ifndef V_PLUS_SLAVE_INTERRUPTER_H 
#define V_PLUS_SLAVE_INTERRUPTER_H

#include "VSlave.h"

namespace vmeplus
{
    class VInterruptHandler;
    class VSlave;

    class VSlaveInterrupter : virtual public VSlave
    {
        public :
            VSlaveInterrupter( std::string name, uint32_t address, uint32_t range );
            virtual ~VSlaveInterrupter();

        public :
            virtual void        WriteIRQLevel( uint16_t level ) = 0;
            virtual uint16_t    ReadIRQLevel() = 0;
            virtual void        WriteIRQVector( uint16_t vector ) = 0;
            virtual uint16_t    ReadIRQVector() = 0;
            virtual void        ISR( uint16_t vector ) = 0;

            virtual void        Reset() = 0;
    };
}
#endif
