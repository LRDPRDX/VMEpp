#ifndef V_PLUS_VINTERRUPT_HANDLER_H
#define V_PLUS_VINTERRUPT_HANDLER_H

#include "CAENVMEtypes.h"
#include <cstdint>
#include <vector>

namespace vmeplus
{
    class VSlaveInterrupter;

    class VInterruptHandler
    {
        protected :
            std::vector<VSlaveInterrupter*>     fInterrupters;

        public :
            VInterruptHandler();
            VInterruptHandler( const VInterruptHandler &other ) = delete;
            VInterruptHandler& operator=( const VInterruptHandler &other ) = delete;
            virtual ~VInterruptHandler();

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
