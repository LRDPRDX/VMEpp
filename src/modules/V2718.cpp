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

        const uint32_t MAX_PERIOD = 0xff;
        const uint32_t MIN_PERIOD = 0x02;

        struct { uint32_t n; int u; } sPlus, sMinus;

        uint32_t n0 = 0;

        if( freq > 0 )
        {
            for( int i = 0; i < 4; ++i )
            {
                n0 = std::floor( ss[i].expo / ss[i].num / freq );

                if( (n0 >= MIN_PERIOD) && (n0 < MAX_PERIOD) )
                {
                    sMinus = { n0,     i };
                    sPlus  = { n0 + 1, i };
                    break;
                }
                else if( n0 == 1 )
                {
                    if( i > 0 )
                    {
                        sMinus = { MAX_PERIOD, i - 1 }; // use previous range 
                        sPlus  = { n0 + 1,     i     }; // use current range, but increment period by 1
                    }
                    else
                    {
                        sPlus = sMinus = { n0 + 1, i };
                    }
                    break;
                }
                else if( n0 == MAX_PERIOD )
                {
                    if( i < 3 )
                    {
                        sMinus = { n0,         i     }; 
                        sPlus  = { MIN_PERIOD, i + 1 }; // use next range
                    }
                    else
                    {
                        sPlus = sMinus = { n0, i };
                    }
                    break;
                }
            }

            if( n0 != 0 )
            {
                double errorPlus    = std::fabs( freq - ss[sPlus.u].expo / ss[sPlus.u].num / sPlus.n );
                double errorMinus   = std::fabs( freq - ss[sMinus.u].expo / ss[sMinus.u].num / sMinus.n );
                if( errorPlus < errorMinus )
                {
                    n0 = sPlus.n;
                    fTimeUnit = ss[sPlus.u].unit;
                }
                else
                {
                    n0 = sMinus.n;
                    fTimeUnit = ss[sMinus.u].unit;
                }

                duty = ((duty > 0) ? ((duty < 100) ? duty : 99) : 1);
                uint32_t width = n0 * duty / 100;// < MAX_PERIOD
                width = (width > 0) ? width : 1;

                if( width < n0 )
                {
                    fPeriod = n0;
                    fWidth = width; 

                    return true;
                }
            }
        }

        return false;
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
        fPulserA( cvPulserA ),
        fPulserB( cvPulserB ),
        fScaler()
    {
        fPulserA.fOwner = this;
        fPulserB.fOwner = this;
        fScaler.fOwner  = this;
    }

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
    //*********************//
    //****** V2718 + ******//
    //*********************//
}
