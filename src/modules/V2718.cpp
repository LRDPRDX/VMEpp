#include "CAENVMElib.h"

#include "VController.h"
#include "modules/V2718.h"
#include "VException.h"

namespace vmeplus
{
    //**********************//
    //****** PULSER + ******//
    //**********************//
    void V2718Pulser::SetSquare( uint32_t freq, uint8_t duty )
    {
        struct { uint32_t expo; uint32_t num; CVTimeUnits unit; } ss[4] = { { 1000000000, 25, cvUnit25ns },
                                                                            { 10000000,   16, cvUnit1600ns },
                                                                            { 100000,     41, cvUnit410us },
                                                                            { 1000,      104, cvUnit104ms } };
        uint32_t period = 0;
        for( int i = 0; i < 4; i++ )
        {
            period = ss[i].expo / ss[i].num / freq;
            if( period && (period < 256) )
            {
                fTimeUnit = ss[i].unit;
                break;
            }
        }

        if( period )//only if  0 < period < 256; 
        {
            fPeriod = period;
            duty = ((duty > 0) ? ((duty < 100) ? duty : 99) : 1);
            uint32_t width = period * duty / 100;// < 256
            fWidth = (width ? width : 1); 
        }
        else
        {
            PrintMessage( Message_t::WARNING, "This frequency is not supported by the V2718's pulser" );
        }
    }

    void V2718Pulser::Write()
    {
        if( fOwner != nullptr )
        {
            auto ec = CAENVME_SetPulserConf( fOwner->GetHandle(),
                                             fPulser,
                                             fPeriod,
                                             fWidth,
                                             fTimeUnit,
                                             fNPulses,
                                             fStartSource,
                                             fStopSource );
            if( ec )
            {
                throw VException( static_cast<VError_t>(ec), "SetPulserConf" );
            }
        }
    }

    void V2718Pulser::Read()
    {
        if( fOwner != nullptr )
        {
            auto ec = CAENVME_GetPulserConf( fOwner->GetHandle(),
                                             fPulser,
                                            &fPeriod,
                                            &fWidth,
                                            &fTimeUnit,
                                            &fNPulses,
                                            &fStartSource,
                                            &fStopSource );
            if( ec )
            {
                throw VException( static_cast<VError_t>(ec), "GetPulserConf" );
            }
        }
    }

    void V2718Pulser::Start()
    {
        if( fOwner != nullptr )
        {
            auto ec = CAENVME_StartPulser( fOwner->GetHandle(), fPulser );
            if( ec )
            {
                throw VException( static_cast<VError_t>(ec), "StartPulser" );
            }
        }
    }

    void V2718Pulser::Stop()
    {
        if( fOwner != nullptr )
        {
            auto ec = CAENVME_StopPulser( fOwner->GetHandle(), fPulser );
            if( ec )
            {
                throw VException( static_cast<VError_t>(ec), "StopPulser" );
            }
        }
    }
    //**********************//
    //****** PULSER - ******//
    //**********************//

    //**********************//
    //****** SCALER + ******//
    //**********************//
    void V2718Scaler::Write()
    {
        if( fOwner != nullptr )
        {
            auto ec = CAENVME_SetScalerConf( fOwner->GetHandle(),
                                                     fLimit,
                                                     fAutoReset,
                                                     fHitSource,
                                                     fGateSource,
                                                     fStopSource );
            if( ec )
            {
                throw VException( static_cast<VError_t>(ec), "SetScalerConf" );
            }
        }
    }

    void V2718Scaler::Read()
    {
        if( fOwner != nullptr )
        {
            auto ec = CAENVME_GetScalerConf( fOwner->GetHandle(),
                                            &fLimit,
                                            &fAutoReset,
                                            &fHitSource,
                                            &fGateSource,
                                            &fStopSource );
            if( ec )
            {
                throw VException( static_cast<VError_t>(ec), "GetScalerConf" );
            }
        }
    }

    void V2718Scaler::Reset()
    {
        if( fOwner != nullptr )
        {
            auto ec = CAENVME_ResetScalerCount( fOwner->GetHandle() );
            if( ec )
            {
                throw VException( static_cast<VError_t>(ec), "ResetScalerCount" );
            }
        }
    }

    void V2718Scaler::EnableGate( bool enable )
    {
        if( fOwner != nullptr )
        {
            auto ec = cvSuccess;
            if( enable )
            {
                ec = CAENVME_EnableScalerGate( fOwner->GetHandle() );
            }
            else
            {
                ec = CAENVME_DisableScalerGate( fOwner->GetHandle() );
            }
            if( ec )
            {
                throw VException( static_cast<VError_t>(ec), "Enable_or_Disable_ScalerGate" );
            }
        }
    }
    //**********************//
    //****** SCALER - ******//
    //**********************//

    //*********************//
    //****** V2718 + ******//
    //*********************//
    V2718::V2718() :
        VController(),
        UConfigurable<V2718>(),
        fPulserA( cvPulserA ),
        fPulserB( cvPulserB ),
        fScaler()
    {
        fPulserA.fOwner = this;
        fPulserB.fOwner = this;
        fScaler.fOwner  = this;
    }

    template<>
    json UConfigurable<V2718>::fDefaultConfig = []() {
        json j = json::object({});
        j["name"] = "V2718";
        j["settings"] = { {"inputs", {}}, {"outputs", {}}, {"pulsers", {}}, {"scaler", {}} };
        return j;
    }();

    V2718::~V2718() { }

    void V2718::Open( short link, short bdNum )
    {
        auto ec = CAENVME_Init( cvV2718, link, bdNum, &fHandle );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "Open crate controller" );
        }
    }

    V2718Pulser& V2718::GetPulser( CVPulserSelect pulser )
    {
        switch( pulser )
        {
            case( cvPulserA ) :
                return fPulserA;
                break;
            case( cvPulserB ) :
                return fPulserB;
                break;
        }
    }

    V2718Scaler& V2718::GetScaler()
    {
        return fScaler;
    }

    void V2718::WriteOutputConfig( CVOutputSelect outputNo, CVIOSources src, CVIOPolarity polarity, CVLEDPolarity ledPolarity )
    {
        auto ec = CAENVME_SetOutputConf( fHandle,
                                         outputNo,
                                         polarity,
                                         ledPolarity,
                                         src );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "SetOutputConf" );
        }
    }

    void V2718::ReadOutputConfig( CVOutputSelect outputNo, CVIOPolarity &polarity, CVLEDPolarity &ledPolarity, CVIOSources &src )
    {
        auto ec = CAENVME_GetOutputConf( fHandle,
                                         outputNo,
                                         &polarity,
                                         &ledPolarity,
                                         &src );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "GetOutputConf" );
        }
    }

    void V2718::WriteInputConfig( CVInputSelect inputNo, CVIOPolarity polarity, CVLEDPolarity ledPolarity )
    {
        auto ec = CAENVME_SetInputConf( fHandle,
                                        inputNo,
                                        polarity,
                                        ledPolarity );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "SetInputConf" );
        }
    }

    void V2718::ReadInputConfig( CVInputSelect inputNo, CVIOPolarity &polarity, CVLEDPolarity &ledPolarity )
    {
        auto ec = CAENVME_GetInputConf( fHandle,
                                        inputNo,
                                        &polarity,
                                        &ledPolarity );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "GetInputConf" );
        }
    }

    void V2718::ReadConfig( nlohmann::json &config )
    {
        using json = nlohmann::json;
        config.clear();
    }

    void V2718::WriteConfig( const nlohmann::json &config )
    {
    }
    //*********************//
    //****** V2718 - ******//
    //*********************//
}
