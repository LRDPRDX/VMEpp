#ifndef V_PLUS_CONTROLLER_H
#define V_PLUS_CONTROLLER_H

#include <string>
#include <vector>

#include "CAENVMEtypes.h"

#include "VArbiter.h"
#include "VMaster.h"
#include "VRequester.h"
#include "VInterruptHandler.h"


namespace vmeplus
{
    class VController : virtual public VArbiter,
                        virtual public VMaster,
                        virtual public VRequester,
                        virtual public VInterruptHandler
    {
        protected :
            int32_t                 fHandle; 

        public :
            VController();
            virtual ~VController();

        public :
            std::string                 ReadBoardFWRelease();

            //  As an arbiter
            virtual void                WriteArbiterType( CVArbiterTypes type ) override;
            virtual CVArbiterTypes      ReadArbiterType() override;

            //  As a master
            void    ReadCycle( uint32_t address, void* data, CVAddressModifier am, CVDataWidth dw ) override;//throws
            void    WriteCycle( uint32_t address, void* data, CVAddressModifier am, CVDataWidth dw ) override;//throws
            void    BLTReadCycle( uint32_t address, void* data, int size, CVAddressModifier am, CVDataWidth dw, int *count ) override;//throws
            void    MBLTReadCycle( uint32_t address, void* data, int size, CVAddressModifier am, int *count ) override;//throws
            void    FIFOMBLTReadCycle( uint32_t address, void* data, int size, CVAddressModifier am, int *count ) override;//throws
            void    ADOCycle( uint32_t address, CVAddressModifier am ) override;
            void    ADOHCycle( uint32_t address, CVAddressModifier am ) override;

            void    WriteFIFOMode( short value ) override;
            short   ReadFIFOMode() override;

            //  As a requester
            virtual void                WriteRequesterType( CVRequesterTypes type ) override;
            virtual CVRequesterTypes    ReadRequesterType() override;
            virtual void                WriteReleaseType( CVReleaseTypes type ) override;
            virtual CVReleaseTypes      ReadReleaseType() override;
            virtual void                WriteBusReqLevel( CVBusReqLevels level ) override;
            virtual CVBusReqLevels      ReadBusReqLevel() override;

            //  As an interrupt handler
            void    IACK( CVIRQLevels level, void *vector, CVDataWidth dw ) override;
            uint8_t IRQCheck() override;
            void    IRQEnable( uint32_t mask, bool enable = true ) override;
            void    IRQWait( uint32_t mask, uint32_t timeout ) override;

            //  Additional functions
            CVDisplay    ReadDisplay();

        public :
            virtual void    Open( short link, short bdNum ) = 0;
            void            Close();//throws

        public :
            int32_t             GetHandle() const { return fHandle; }
    };
}
#endif
