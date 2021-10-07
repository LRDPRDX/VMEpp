#include "CAENVMElib.h"

#include "VController.h"
#include "modules/V2718.h"
#include "VException.h"

#include <cmath>

namespace vmeplus
{
    //**********************//
    //****** PULSER + ******//
    //**********************//
    bool V2718Pulser::SetSquare( uint32_t freq, uint8_t duty )
    {
        struct { double expo; double num; CVTimeUnits unit; } ss[4] = { { 1000000000., 25., cvUnit25ns },
                                                                        { 10000000.,   16., cvUnit1600ns },
                                                                        { 100000.,     41., cvUnit410us },
                                                                        { 1000.,      104., cvUnit104ms } };

        constexpr uint32_t MAX_N = 0xff;
        constexpr uint32_t MIN_N = 0x02;
        constexpr uint32_t MIN_D = 0x01;

        if( freq == 0 )
        {
            return false;
        }

        double minError;
        for( int i = 0; i < 4; ++i )
        {
            uint32_t n = std::max( MIN_N, std::min( MAX_N, (uint32_t)std::round( ss[i].expo / ss[i].num / freq ) ) );

            double error = std::fabs( ss[i].expo / ss[i].num / n - (double)freq );

            if( i == 0 )
            {
                minError = error;

                fPeriod = n;
                fWidth = std::max( MIN_D, std::min( n - 1, (uint32_t)std::round( duty * n / 100 ) ) );
                fTimeUnit = ss[i].unit;
            }
            else if( error < minError )
            {
                minError = error;

                fPeriod = n;
                fWidth = std::max( MIN_D, std::min( n - 1, (uint32_t)std::round( duty * n / 100 ) ) );
                fTimeUnit = ss[i].unit;
            }
        }

        return true;
    }

    double V2718Pulser::GetFrequencyReal() const
    {
        struct { double expo; double num; CVTimeUnits unit; } ss[4] = { { 1000000000., 25., cvUnit25ns },
                                                                        { 10000000.,   16., cvUnit1600ns },
                                                                        { 100000.,     41., cvUnit410us },
                                                                        { 1000.,      104., cvUnit104ms } };

        double freq = 0.0;
        for( int i = 0; i < 4; ++i )
        {
            if( fTimeUnit == ss[i].unit )
            {
                return ss[i].expo / ss[i].num / (double)fPeriod;
            }
        }

        return freq;
    }

    void V2718Pulser::GetSquare( uint32_t &freq, uint8_t &duty )
    {
        uint32_t expo, num;

        switch( fTimeUnit )
        {
            case( cvUnit25ns ) :    expo = 1000000000;  num = 25; break;
            case( cvUnit1600ns ) :  expo = 10000000;    num = 16; break;
            case( cvUnit410us ) :   expo = 100000;      num = 41; break;
            case( cvUnit104ms ) :   expo = 1000;        num = 104; break;
        }
        if( fPeriod )
        {
            freq = expo / num / fPeriod;
            duty = fWidth * 100 / fPeriod;
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
    std::string UConfigurable<V2718>::fName = "V2718";

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

    void V2718::ReadConfig( UConfig<V2718>& cfg )
    {
        // In's and Out's
        CVIOPolarity pol;
        CVLEDPolarity ledPol;
        CVIOSources src;
        for( uint8_t i = 0; i < fInNumber; ++i )
        {
            ReadInputConfig( static_cast<CVInputSelect>(i), pol, ledPol );
            cfg.INPUTS.at( i ).POLARITY = pol;
            cfg.INPUTS.at( i ).LED_POLARITY = ledPol;
        }

        for( uint8_t i = 0; i < fOutNumber; ++i )
        {
            ReadOutputConfig( static_cast<CVOutputSelect>(i), pol, ledPol, src );
            cfg.OUTPUTS.at( i ).POLARITY = pol;
            cfg.OUTPUTS.at( i ).LED_POLARITY = ledPol;
            cfg.OUTPUTS.at( i ).SOURCE = src;
        }

        // Pulsers
        uint32_t freq = 0;
        uint8_t  duty = 0;
        fPulserA.Read();
        fPulserA.GetSquare( freq, duty );

        cfg.PULSER_A.FREQUENCY      = freq;
        cfg.PULSER_A.DUTY           = duty;
        cfg.PULSER_A.N_PULSES       = fPulserA.GetNPulses();
        cfg.PULSER_A.START_SOURCE   = fPulserA.GetStartSource();
        cfg.PULSER_A.STOP_SOURCE    = fPulserA.GetStopSource();

        fPulserB.Read();
        fPulserB.GetSquare( freq, duty );

        cfg.PULSER_B.FREQUENCY      = freq;
        cfg.PULSER_B.DUTY           = duty;
        cfg.PULSER_B.N_PULSES       = fPulserB.GetNPulses();
        cfg.PULSER_B.START_SOURCE   = fPulserB.GetStartSource();
        cfg.PULSER_B.STOP_SOURCE    = fPulserB.GetStopSource();

        // Scaler
        fScaler.Read();
        cfg.SCALER.GATE_SOURCE  = fScaler.GetGateSource();
        cfg.SCALER.STOP_SOURCE  = fScaler.GetStopSource();
        cfg.SCALER.HIT_SOURCE   = fScaler.GetHitSource();
        cfg.SCALER.LIMIT        = fScaler.GetLimit();
        cfg.SCALER.AUTO_RESET   = fScaler.GetAutoReset();
    }

    void V2718::WriteConfig( const UConfig<V2718>& cfg )
    {
        // In's and Out's
        for( uint8_t i = 0; i < fInNumber; ++i )
        {
            WriteInputConfig( static_cast<CVInputSelect>(i),
                              cfg.INPUTS.at( i ).POLARITY,
                              cfg.INPUTS.at( i ).LED_POLARITY );
        }

        for( uint8_t i = 0; i < fOutNumber; ++i )
        {
            WriteOutputConfig( static_cast<CVOutputSelect>(i),
                               cfg.OUTPUTS.at( i ).SOURCE,
                               cfg.OUTPUTS.at( i ).POLARITY,
                               cfg.OUTPUTS.at( i ).LED_POLARITY );
        }
        // Scaler
        // NOTE: It is important to write the config
        // for the Scaler first since it overwrites
        // the purpose of the signals on the inputs
        fScaler.SetGateSource( cfg.SCALER.GATE_SOURCE );
        fScaler.SetStopSource( cfg.SCALER.STOP_SOURCE );
        fScaler.SetHitSource( cfg.SCALER.HIT_SOURCE );
        fScaler.SetLimit( cfg.SCALER.LIMIT );
        fScaler.SetAutoReset( cfg.SCALER.AUTO_RESET );
        fScaler.Write();

        // Pulsers
        fPulserA.SetSquare( cfg.PULSER_A.FREQUENCY, cfg.PULSER_A.DUTY );
        fPulserA.SetNPulses( cfg.PULSER_A.N_PULSES );
        fPulserA.SetStartSource( cfg.PULSER_A.START_SOURCE );
        fPulserA.SetStopSource( cfg.PULSER_A.STOP_SOURCE );
        fPulserA.Write();

        fPulserB.SetSquare( cfg.PULSER_B.FREQUENCY, cfg.PULSER_B.DUTY );
        fPulserB.SetNPulses( cfg.PULSER_B.N_PULSES );
        fPulserB.SetStartSource( cfg.PULSER_B.START_SOURCE );
        fPulserB.SetStopSource( cfg.PULSER_B.STOP_SOURCE );
        fPulserB.Write();
    }
    //*********************//
    //****** V2718 - ******//
    //*********************//
}
