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
    json UConfigurable<V2718>::fDefaultConfig = []() {
        json j = json::object( {} );
        j["name"] = "V2718";

        j["settings"] = json::object( {} );

        // Outputs and inputs
        j["settings"] += {"inputs", {}};
        json j_inputs = json::array( {} );
        for( uint8_t i = 0; i < V2718::GetInNumber(); ++i )
        {
            j_inputs.push_back( {{"polarity", {}}, {"led_polarity", {}}} );
        }
        j["/settings/inputs"_json_pointer] = j_inputs;

        j["settings"] += {"outputs", {}};
        json j_outputs = json::array( {} );
        for( uint8_t i = 0; i < V2718::GetOutNumber(); ++i )
        {
            j_outputs.push_back( {{"polarity", {}}, {"led_polarity", {}}, {"source", {}}} );
        }
        j["/settings/outputs"_json_pointer] = j_outputs;

        // Pulsers
        j["settings"] += {"pulsers", {}};
        json j_pulsers = json::object( {} );
            j_pulsers += {"A", {}};
            j_pulsers += {"B", {}};
        json j_pulser = json::object( {} );
        j_pulser += {"frequency", {}};
        j_pulser += {"duty", {}};
        j_pulser += {"count", {}};
        j_pulser += {"start", {}};
        j_pulser += {"stop", {}};

        j_pulsers["A"] = j_pulser;
        j_pulsers["B"] = j_pulser;
        j["/settings/pulsers"_json_pointer] = j_pulsers;

        // Scaler
        j["settings"] += {"scaler", {}};
        json j_scaler = json::object( {} );
            j_scaler += {"limit", {}};
            j_scaler += {"hit", {}};
            j_scaler += {"gate", {}};
            j_scaler += {"stop", {}};
            j_scaler += {"auto_reset", {}};
        j["/settings/scaler"_json_pointer] = j_scaler;

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

    void V2718::ReadConfigImpl( nlohmann::json &j )
    {
        j = fDefaultConfig;

        // In's and Out's
        CVIOPolarity pol;
        CVLEDPolarity ledPol;
        CVIOSources src;
        for( uint8_t i = 0; i < fInNumber; ++i )
        {
            ReadInputConfig( static_cast<CVInputSelect>(i), pol, ledPol );
            j.at("settings").at("inputs").at( i ).at("polarity") = pol;
            j.at("settings").at("inputs").at( i ).at("led_polarity") = ledPol;
        }

        for( uint8_t i = 0; i < fOutNumber; ++i )
        {
            ReadOutputConfig( static_cast<CVOutputSelect>(i), pol, ledPol, src );
            j.at("settings").at("outputs").at( i ).at("polarity") = pol;
            j.at("settings").at("outputs").at( i ).at("led_polarity") = ledPol;
            j.at("settings").at("outputs").at( i ).at("source") = src;
        }

        // Pulsers
        uint32_t freq = 0;
        uint8_t  duty = 0;
        fPulserA.Read();
        fPulserA.GetSquare( freq, duty );
        j.at("settings").at("pulsers").at("A").at("frequency") = freq;
        j.at("settings").at("pulsers").at("A").at("duty") = duty;
        j.at("settings").at("pulsers").at("A").at("count") = fPulserA.GetNPulses();
        j.at("settings").at("pulsers").at("A").at("start") = fPulserA.GetStartSource();
        j.at("settings").at("pulsers").at("A").at("stop") = fPulserA.GetStopSource();

        fPulserB.Read();
        fPulserB.GetSquare( freq, duty );
        j.at("settings").at("pulsers").at("B").at("frequency") = freq;
        j.at("settings").at("pulsers").at("B").at("duty") = duty;
        j.at("settings").at("pulsers").at("B").at("count") = fPulserB.GetNPulses();
        j.at("settings").at("pulsers").at("B").at("start") = fPulserB.GetStartSource();
        j.at("settings").at("pulsers").at("B").at("stop") = fPulserB.GetStopSource();

        // Scaler
        fScaler.Read();
        j.at("settings").at("scaler").at("gate") = fScaler.GetGateSource();
        j.at("settings").at("scaler").at("stop") = fScaler.GetStopSource();
        j.at("settings").at("scaler").at("hit") = fScaler.GetHitSource();
        j.at("settings").at("scaler").at("limit") = fScaler.GetLimit();
        j.at("settings").at("scaler").at("auto_reset") = fScaler.GetAutoReset();
    }

    void V2718::WriteConfigImpl( const nlohmann::json &j )
    {
        // In's and Out's
        CVIOPolarity pol;
        CVLEDPolarity ledPol;
        CVIOSources src;
        for( uint8_t i = 0; i < fInNumber; ++i )
        {
            j.at("settings").at("inputs").at( i ).at("polarity").get_to<CVIOPolarity>(pol);
            j.at("settings").at("inputs").at( i ).at("led_polarity").get_to<CVLEDPolarity>(ledPol);
            WriteInputConfig( static_cast<CVInputSelect>(i), pol, ledPol );
        }

        for( uint8_t i = 0; i < fOutNumber; ++i )
        {
            j.at("settings").at("outputs").at( i ).at("polarity").get_to<CVIOPolarity>(pol);
            j.at("settings").at("outputs").at( i ).at("led_polarity").get_to<CVLEDPolarity>(ledPol);
            j.at("settings").at("outputs").at( i ).at("source").get_to<CVIOSources>(src);
            WriteOutputConfig( static_cast<CVOutputSelect>(i), src, pol, ledPol );
        }

        // Pulsers
        uint32_t freq = 0;
        uint8_t  duty = 0;
        uint8_t  count = 0;
        CVIOSources start = cvManualSW;
        CVIOSources stop = cvManualSW;
        j.at("settings").at("pulsers").at("A").at("frequency").get_to<uint32_t>(freq);
        j.at("settings").at("pulsers").at("A").at("duty").get_to<uint8_t>(duty);
            fPulserA.SetSquare( freq, duty );
        j.at("settings").at("pulsers").at("A").at("count").get_to<uint8_t>(count);
            fPulserA.SetNPulses( count );
        j.at("settings").at("pulsers").at("A").at("start").get_to<CVIOSources>(start);
            fPulserA.SetStartSource( start );
        j.at("settings").at("pulsers").at("A").at("stop").get_to<CVIOSources>(stop);
            fPulserA.SetStopSource( stop );
        fPulserA.Write();

        j.at("settings").at("pulsers").at("B").at("frequency").get_to<uint32_t>(freq);
        j.at("settings").at("pulsers").at("B").at("duty").get_to<uint8_t>(duty);
            fPulserB.SetSquare( freq, duty );
        j.at("settings").at("pulsers").at("B").at("count").get_to<uint8_t>(count);
            fPulserB.SetNPulses( count );
        j.at("settings").at("pulsers").at("B").at("start").get_to<CVIOSources>(start);
            fPulserB.SetStartSource( start );
        j.at("settings").at("pulsers").at("B").at("stop").get_to<CVIOSources>(stop);
            fPulserB.SetStopSource( stop );
        fPulserB.Write();

        // Scaler
        CVIOSources gate = cvManualSW;
        CVIOSources hit = cvInputSrc0;
        short limit = 0;
        short autoReset = 0;
        j.at("settings").at("scaler").at("gate").get_to<CVIOSources>( gate );
            fScaler.SetGateSource( gate );
        j.at("settings").at("scaler").at("stop").get_to<CVIOSources>( stop );
            fScaler.SetStopSource( stop );
        j.at("settings").at("scaler").at("hit").get_to<CVIOSources>( hit );
            fScaler.SetHitSource( hit );
        j.at("settings").at("scaler").at("limit").get_to<short>( limit );
            fScaler.SetLimit( limit );
        j.at("settings").at("scaler").at("auto_reset").get_to<short>( autoReset );
            fScaler.SetAutoReset( autoReset );
        fScaler.Write();
    }
    //*********************//
    //****** V2718 - ******//
    //*********************//
}
