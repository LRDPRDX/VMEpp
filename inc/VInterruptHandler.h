#ifndef V_PLUS_VINTERRUPT_HANDLER_H
#define V_PLUS_VINTERRUPT_HANDLER_H

#include "CAENVMEtypes.h"
#include "VModule.h"

#include <cstdint>
#include <vector>

namespace vmepp
{
    class VSlaveInterrupter;

    class VInterruptHandler : virtual public VModule
    {
        protected :
            std::vector<VSlaveInterrupter*>     fInterrupters;

        public :
            VInterruptHandler() : fInterrupters( 0 ) { };
            virtual ~VInterruptHandler() = default;

        public :
            bool    RegisterInterrupter( VSlaveInterrupter *interrupter );

        public :
            virtual void    IACK( CVIRQLevels level, void *vector, CVDataWidth dw ) = 0;
            virtual uint8_t IRQCheck() = 0;
            virtual void    IRQEnable( uint32_t mask, bool enable = true ) = 0;
            virtual void    IRQWait( uint32_t mask, uint32_t timeout ) = 0;
    };
}
#endif
