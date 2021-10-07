#ifndef V_PLUS_ARBITER_H 
#define V_PLUS_ARBITER_H

#include <CAENVMEtypes.h>

namespace vmeplus
{
    class VArbiter
    {
        public :
            VArbiter() {};
            VArbiter( const VArbiter &other ) = delete;
            VArbiter& operator=( const VArbiter &other ) = delete;
            virtual ~VArbiter() {};

        public :
            virtual void            WriteArbiterType( CVArbiterTypes type ) = 0;
            virtual CVArbiterTypes  ReadArbiterType() = 0;
    };
}
#endif
