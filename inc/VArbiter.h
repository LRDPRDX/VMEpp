#ifndef V_PLUS_ARBITER_H 
#define V_PLUS_ARBITER_H

#include <CAENVMEtypes.h>

#include "VModule.h"

namespace vmepp
{
    class VArbiter : virtual public VModule
    {
        public :
            VArbiter() {};
            virtual ~VArbiter() = default;

        public :
            virtual void            WriteArbiterType( CVArbiterTypes type ) noexcept( false ) = 0;
            virtual CVArbiterTypes  ReadArbiterType() noexcept( false ) = 0;
    };
}
#endif
