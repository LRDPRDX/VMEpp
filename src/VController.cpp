#include <cmath>

#include "CAENVMElib.h"

#include "VMaster.h"
#include "VController.h"
#include "VException.h"


namespace vmepp
{
    VController::VController() :
        VMaster(),
        VInterruptHandler(),
        fHandle( -1 )
    {
    }

    VController::~VController()
    {
        try
        {
            Close();
        }
        catch( std::exception &e )
        {
            std::cerr << "On closing controller : " << e.what() << std::endl;
            return;
        }
    }

    void VController::Close()
    {
        auto ec = CAENVME_End( fHandle );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "Close crate controller" );
        }
    }

    std::string VController::ReadBoardFWRelease()
    {
        char release[100];
        auto ec = CAENVME_BoardFWRelease( fHandle, &release[0] );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "BoardFWRelease" );
        }
        return release;
    }

    //***********************
    //****** ARBITER + ******
    //***********************
    void VController::WriteArbiterType( CVArbiterTypes type )
    {
        auto ec = CAENVME_SetArbiterType( fHandle, type );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "SetArbiterType" );
        }
    }

    CVArbiterTypes VController::ReadArbiterType( )
    {
        CVArbiterTypes type;
        auto ec = CAENVME_GetArbiterType( fHandle, &type );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "GetArbiterType" );
        }
        return type;
    }
    //***********************
    //****** ARBITER - ******
    //***********************

    //**********************
    //****** MASTER + ******
    //**********************
    void VController::ReadCycle( uint32_t address, void* data, CVAddressModifier am, CVDataWidth dw )
    {
        auto ec = CAENVME_ReadCycle( fHandle, address, data, am, dw );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "ReadCycle -> " + HelpStringCycle( address, am, dw ) );
        }
    }

    void VController::WriteCycle( uint32_t address, void* data, CVAddressModifier am, CVDataWidth dw )
    {
        auto ec = CAENVME_WriteCycle( fHandle, address, data, am, dw );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "WriteCycle -> " + HelpStringCycle( address, am, dw ) );
        }
    }

    void VController::BLTReadCycle( uint32_t address, void *buffer, int size, CVAddressModifier am, CVDataWidth dw, int *count )
    {
        auto ec = CAENVME_BLTReadCycle( fHandle, address, buffer, size, am, dw, count );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "BLTReadCycle -> " + HelpStringCycle( address, am, dw ) );
        }
    }

    void VController::MBLTReadCycle( uint32_t address, void *buffer, int size, CVAddressModifier am, int *count )
    {
        auto ec = CAENVME_MBLTReadCycle( fHandle, address, buffer, size, am, count );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "MBLTReadCycle -> " + HelpStringCycle( address, am ) );
        }
    }

    void VController::FIFOMBLTReadCycle( uint32_t address, void *buffer, int size, CVAddressModifier am, int *count )
    {
        auto ec = CAENVME_FIFOMBLTReadCycle( fHandle, address, buffer, size, am, count );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "FIFOMBLTReadCycle -> " + HelpStringCycle( address, am ) );
        }
    }

    void VController::ADOCycle( uint32_t address, CVAddressModifier am )
    {
        auto ec = CAENVME_ADOCycle( fHandle, address, am );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "ADOCycle" );
        }
    }

    void VController::ADOHCycle( uint32_t address, CVAddressModifier am )
    {
        auto ec = CAENVME_ADOHCycle( fHandle, address, am );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "ADOHCycle" );
        }
    }

    void VController::WriteFIFOMode( short value )
    {
        auto ec = CAENVME_SetFIFOMode( fHandle, value );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "SetFIFOMode" );
        }
    }

    short VController::ReadFIFOMode()
    {
        short value = 0;
        auto ec = CAENVME_GetFIFOMode( fHandle, &value );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "GetFIFOMode" );
        }
        return value;
    }
    //**********************
    //****** MASTER - ******
    //**********************

    //*********************************
    //****** INTERRUPT HANDLER + ******
    //*********************************
    void VController::IACK( CVIRQLevels level, void *vector, CVDataWidth dw )
    {
        auto ec = CAENVME_IACKCycle( fHandle, level, vector, dw );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "IACKCycle" );
        }
    }

    uint8_t VController::IRQCheck()
    {
        uint8_t mask = 0;
        auto ec = CAENVME_IRQCheck( fHandle, &mask );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "IRQCheck" );
        }
        return mask;
    }

    void VController::IRQEnable( uint32_t mask, bool enable )
    {
        auto ec = cvSuccess;
        if( enable )
        {
            ec = CAENVME_IRQEnable( fHandle, mask );
        }
        else
        {
            ec = CAENVME_IRQDisable( fHandle, mask );
        }
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "IRQEnable_or_Disable" );
        }
    }

    void VController::IRQWait( uint32_t mask, uint32_t timeout )
    {
        auto ec = CAENVME_IRQWait( fHandle, mask, timeout );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "IRQWait" );
        }
    }
    //*********************************
    //****** INTERRUPT HANDLER - ******
    //*********************************

    //*************************
    //****** REQUESTER + ******
    //*************************
    void VController::WriteRequesterType( CVRequesterTypes type )
    {
        auto ec = CAENVME_SetRequesterType( fHandle, type );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "SetRequesterType" );
        }
    }

    CVRequesterTypes VController::ReadRequesterType()
    {
        CVRequesterTypes type;
        auto ec = CAENVME_GetRequesterType( fHandle, &type );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "GetRequesterType" );
        }
        return type;
    }

    void VController::WriteReleaseType( CVReleaseTypes type )
    {
        auto ec = CAENVME_SetReleaseType( fHandle, type );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "SetReleaseType" );
        }
    }

    CVReleaseTypes VController::ReadReleaseType()
    {
        CVReleaseTypes type;
        auto ec = CAENVME_GetReleaseType( fHandle, &type );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "GetReleaseType" );
        }
        return type;
    }

    void VController::WriteBusReqLevel( CVBusReqLevels level )
    {
        auto ec = CAENVME_SetBusReqLevel( fHandle, level );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "SetBusReqLevel" );
        }
    }

    CVBusReqLevels VController::ReadBusReqLevel()
    {
        CVBusReqLevels level;
        auto ec = CAENVME_GetBusReqLevel( fHandle, &level );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "GetBusReqLevel" );
        }
        return level;
    }
    //*************************
    //****** REQUESTER + ******
    //*************************

    CVDisplay VController::ReadDisplay()
    {
        CVDisplay display;
        auto ec = CAENVME_ReadDisplay( fHandle, &display );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "ReadDisplay" );
        }
        return display;
    }
}
