#ifndef V_PLUS_MASTER_H
#define V_PLUS_MASTER_H

#include <string>
#include <list>

#include "CAENVMEtypes.h"
#include "VModule.h"
#include "VSlave.h"

namespace vmepp
{
    class VMaster : virtual public VModule
    {
        private :
            std::list<VSlave*>          fSlaves;
            void                        UnregisterSlave( VSlave *slave ) noexcept( true );

        public :
            VMaster() { }
            virtual ~VMaster();

        public :
            void                        RegisterSlave( VSlave *slave );
            void                        Initialize();
            size_t                      GetNSlaves() const { return fSlaves.size(); }

        public :
            virtual void                ReadCycle( uint32_t address, void* data, CVAddressModifier am, CVDataWidth dw ) = 0;
            virtual void                WriteCycle( uint32_t address, void* data, CVAddressModifier am, CVDataWidth dw ) = 0;
            virtual void                BLTReadCycle( uint32_t address, void* data, int size, CVAddressModifier am, CVDataWidth dw, int *count ) = 0;
            virtual void                MBLTReadCycle( uint32_t address, void* data, int size, CVAddressModifier am, int *count ) = 0;
            virtual void                FIFOBLTReadCycle( uint32_t address, void* data, int size, CVAddressModifier am, CVDataWidth dw,  int *count )  = 0;
            virtual void                ADOCycle( uint32_t address, CVAddressModifier am ) = 0;
            virtual void                ADOHCycle( uint32_t address, CVAddressModifier am ) = 0;
            virtual void                WriteFIFOMode( short value )  = 0;
            virtual short               ReadFIFOMode() = 0;

        public :
            std::string                 HelpStringCycle( uint32_t address, CVAddressModifier am, CVDataWidth dw ) const;
            std::string                 HelpStringCycle( uint32_t address, CVAddressModifier am ) const;
            friend void VSlave::Release();
    };
}
#endif
