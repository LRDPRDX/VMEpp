#include "CAENVMElib.h"

#include "VController.h"
#include "modules/V2718.h"
#include "VException.h"

#include <cmath>

namespace vmepp
{
    //**********************//
    //****** PULSER + ******//
    //**********************//
    bool V2718::Pulser::SetSquare( uint32_t freq, uint8_t duty )
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

    double V2718::Pulser::GetFrequencyReal() const
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

    void V2718::Pulser::GetSquare( uint32_t &freq, uint8_t &duty )
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

    void V2718::Pulser::Write()
    {
        if( fOwner != nullptr )
        {
            auto ec = CAENVME_SetPulserConf( fOwner->GetHandle(),
                                             static_cast<CVPulserSelect>( fPulser ),
                                             fPeriod,
                                             fWidth,
                                             fTimeUnit,
                                             fNPulses,
                                             static_cast<CVIOSources>( fStartSource ),
                                             static_cast<CVIOSources>( fStopSource ) );
            if( ec )
            {
                throw VException( static_cast<VError_t>(ec), "SetPulserConf" );
            }
        }
    }

    void V2718::Pulser::Read()
    {
        CVIOSources start, stop;
        if( fOwner != nullptr )
        {
            auto ec = CAENVME_GetPulserConf( fOwner->GetHandle(),
                                             static_cast<CVPulserSelect>( fPulser ),
                                            &fPeriod,
                                            &fWidth,
                                            &fTimeUnit,
                                            &fNPulses,
                                            &start,
                                            &stop );
            fStartSource = static_cast<V2718::Src_t>( start );
            fStopSource = static_cast<V2718::Src_t>( stop );
            if( ec )
            {
                throw VException( static_cast<VError_t>(ec), "GetPulserConf" );
            }
        }
    }

    void V2718::Pulser::Start()
    {
        if( fOwner != nullptr )
        {
            auto ec = CAENVME_StartPulser( fOwner->GetHandle(), static_cast<CVPulserSelect>( fPulser ) );
            if( ec )
            {
                throw VException( static_cast<VError_t>(ec), "StartPulser" );
            }
        }
    }

    void V2718::Pulser::Stop()
    {
        if( fOwner != nullptr )
        {
            auto ec = CAENVME_StopPulser( fOwner->GetHandle(), static_cast<CVPulserSelect>( fPulser ) );
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
    void V2718::Scaler::Write()
    {
        if( fOwner != nullptr )
        {
            auto ec = CAENVME_SetScalerConf( fOwner->GetHandle(),
                                                     fLimit,
                                                     fAutoReset,
                                                     static_cast<CVIOSources>( fHitSource ),
                                                     static_cast<CVIOSources>( fGateSource ),
                                                     static_cast<CVIOSources>( fStopSource ) );
            if( ec )
            {
                throw VException( static_cast<VError_t>(ec), "SetScalerConf" );
            }
        }
    }

    void V2718::Scaler::Read()
    {
        CVIOSources hit, gate, stop;
        if( fOwner != nullptr )
        {
            auto ec = CAENVME_GetScalerConf( fOwner->GetHandle(),
                                            &fLimit,
                                            &fAutoReset,
                                            &hit,
                                            &gate,
                                            &stop );
            fHitSource = static_cast<V2718::Src_t>( hit );
            fGateSource = static_cast<V2718::Src_t>( gate );
            fStopSource = static_cast<V2718::Src_t>( stop );
            if( ec )
            {
                throw VException( static_cast<VError_t>(ec), "GetScalerConf" );
            }
        }
    }

    void V2718::Scaler::Reset()
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

    void V2718::Scaler::EnableGate( bool enable )
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
        fPulserA( new Pulser( Pulser_t::A ) ),
        fPulserB( new Pulser( Pulser_t::B ) ),
        fScaler( new Scaler )
    {
        fPulserA->fOwner = this;
        fPulserB->fOwner = this;
        fScaler->fOwner  = this;
    }

    template<>
    const std::string UConfigurable<V2718>::fName = "V2718";

    void V2718::Open( short link, short bdNum )
    {
        auto ec = CAENVME_Init( cvV2718, link, bdNum, &fHandle );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "Open crate controller" );
        }
    }

    V2718::Pulser* V2718::GetPulser( Pulser_t pulser )
    {
        switch( pulser )
        {
            case( Pulser_t::A ) :
                return fPulserA.get();
                break;
            case( Pulser_t::B ) :
                return fPulserB.get();
                break;
        }
    }

    V2718::Scaler* V2718::GetScaler()
    {
        return fScaler.get();
    }

    void V2718::WriteOutputConfig( Out_t n, const OutputConfig& cfg )
    {
        auto ec = CAENVME_SetOutputConf( fHandle,
                                         static_cast<CVOutputSelect>( n ),
                                         static_cast<CVIOPolarity>( cfg.POLARITY ),
                                         static_cast<CVLEDPolarity>( cfg.LED_POLARITY ),
                                         static_cast<CVIOSources>( cfg.SOURCE ) );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "SetOutputConf" );
        }
    }

    void V2718::ReadOutputConfig( Out_t n, OutputConfig& cfg )
    {
        CVIOPolarity pol; CVLEDPolarity ledPol; CVIOSources src;
        auto ec = CAENVME_GetOutputConf( fHandle,
                                         static_cast<CVOutputSelect>( n ),
                                         &pol,
                                         &ledPol,
                                         &src );
        cfg.POLARITY = static_cast<V2718::Polarity_t>( pol );
        cfg.LED_POLARITY = static_cast<V2718::LEDPolarity_t>( ledPol );
        cfg.SOURCE = static_cast<V2718::Src_t>( src );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "GetOutputConf" );
        }
    }

    void V2718::WriteInputConfig( In_t n, const InputConfig& cfg )
    {
        auto ec = CAENVME_SetInputConf( fHandle,
                                        static_cast<CVInputSelect>( n ),
                                        static_cast<CVIOPolarity>( cfg.POLARITY ),
                                        static_cast<CVLEDPolarity>( cfg.LED_POLARITY ) );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "SetInputConf" );
        }
    }

    void V2718::ReadInputConfig( In_t n, InputConfig& cfg )
    {
        CVIOPolarity pol; CVLEDPolarity ledPol;
        auto ec = CAENVME_GetInputConf( fHandle,
                                        static_cast<CVInputSelect>( n ),
                                        &pol,
                                        &ledPol );
        cfg.POLARITY = static_cast<V2718::Polarity_t>( pol );
        cfg.LED_POLARITY = static_cast<V2718::LEDPolarity_t>( ledPol );
        if( ec )
        {
            throw VException( static_cast<VError_t>(ec), "GetInputConf" );
        }
    }

    void V2718::ReadConfig( UConfig<V2718>& cfg )
    {
        // In's and Out's
        V2718::InputConfig iTemp;
        for( uint8_t i = 0; i < fInNumber; ++i )
        {
            ReadInputConfig( static_cast<In_t>(i), iTemp );
            cfg.INPUTS.at( i ).POLARITY = iTemp.POLARITY;
            cfg.INPUTS.at( i ).LED_POLARITY = iTemp.LED_POLARITY;
        }

        V2718::OutputConfig oTemp;
        for( uint8_t i = 0; i < fOutNumber; ++i )
        {
            ReadOutputConfig( static_cast<Out_t>(i), oTemp );
            cfg.OUTPUTS.at( i ).POLARITY = oTemp.POLARITY;
            cfg.OUTPUTS.at( i ).LED_POLARITY = oTemp.LED_POLARITY;
            cfg.OUTPUTS.at( i ).SOURCE = oTemp.SOURCE;
        }

        // Pulsers
        uint32_t freq = 0;
        uint8_t  duty = 0;
        fPulserA->Read();
        fPulserA->GetSquare( freq, duty );

        cfg.PULSER_A.FREQUENCY      = freq;
        cfg.PULSER_A.DUTY           = duty;
        cfg.PULSER_A.N_PULSES       = fPulserA->GetNPulses();
        cfg.PULSER_A.START_SOURCE   = fPulserA->GetStartSource();
        cfg.PULSER_A.STOP_SOURCE    = fPulserA->GetStopSource();

        fPulserB->Read();
        fPulserB->GetSquare( freq, duty );

        cfg.PULSER_B.FREQUENCY      = freq;
        cfg.PULSER_B.DUTY           = duty;
        cfg.PULSER_B.N_PULSES       = fPulserB->GetNPulses();
        cfg.PULSER_B.START_SOURCE   = fPulserB->GetStartSource();
        cfg.PULSER_B.STOP_SOURCE    = fPulserB->GetStopSource();

        // Scaler
        fScaler->Read();
        cfg.SCALER.GATE_SOURCE  = fScaler->GetGateSource();
        cfg.SCALER.STOP_SOURCE  = fScaler->GetStopSource();
        cfg.SCALER.HIT_SOURCE   = fScaler->GetHitSource();
        cfg.SCALER.LIMIT        = fScaler->GetLimit();
        cfg.SCALER.AUTO_RESET   = fScaler->GetAutoReset();
    }

    void V2718::WriteConfig( const UConfig<V2718>& cfg )
    {
        // In's and Out's
        for( uint8_t i = 0; i < fInNumber; ++i )
        {
            V2718::InputConfig iTemp( cfg.INPUTS.at( i ).POLARITY,
                                      cfg.INPUTS.at( i ).LED_POLARITY );
            WriteInputConfig( static_cast<In_t>(i), iTemp );
        }

        for( uint8_t i = 0; i < fOutNumber; ++i )
        {
            V2718::OutputConfig oTemp( cfg.OUTPUTS.at( i ).SOURCE,
                                       cfg.OUTPUTS.at( i ).POLARITY,
                                       cfg.OUTPUTS.at( i ).LED_POLARITY );
            WriteOutputConfig( static_cast<Out_t>(i), oTemp );
        }
        // Scaler
        // NOTE: It is important to write the config
        // for the Scaler first since it overwrites
        // the purpose of the signals on the inputs
        fScaler->SetGateSource( cfg.SCALER.GATE_SOURCE );
        fScaler->SetStopSource( cfg.SCALER.STOP_SOURCE );
        fScaler->SetHitSource( cfg.SCALER.HIT_SOURCE );
        fScaler->SetLimit( cfg.SCALER.LIMIT );
        fScaler->SetAutoReset( cfg.SCALER.AUTO_RESET );
        fScaler->Write();

        // Pulsers
        fPulserA->SetSquare( cfg.PULSER_A.FREQUENCY, cfg.PULSER_A.DUTY );
        fPulserA->SetNPulses( cfg.PULSER_A.N_PULSES );
        fPulserA->SetStartSource( cfg.PULSER_A.START_SOURCE );
        fPulserA->SetStopSource( cfg.PULSER_A.STOP_SOURCE );
        fPulserA->Write();

        fPulserB->SetSquare( cfg.PULSER_B.FREQUENCY, cfg.PULSER_B.DUTY );
        fPulserB->SetNPulses( cfg.PULSER_B.N_PULSES );
        fPulserB->SetStartSource( cfg.PULSER_B.START_SOURCE );
        fPulserB->SetStopSource( cfg.PULSER_B.STOP_SOURCE );
        fPulserB->Write();
    }
    //*********************//
    //****** V2718 - ******//
    //*********************//
}
