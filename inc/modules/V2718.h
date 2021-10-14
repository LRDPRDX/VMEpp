#ifndef V_PLUS_V2718_H
#define V_PLUS_V2718_H

#include <array>

#include <CAENVMEtypes.h>
#include "VController.h"
#include "UConfigurable.h"

#include "cereal/cereal.hpp"
#include "cereal/types/array.hpp"


namespace vmepp
{
    class V2718;

    class V2718Pulser
    {
        protected :
            V2718          *fOwner;

        protected :
            CVPulserSelect  fPulser;
            unsigned char   fPeriod;
            unsigned char   fWidth;
            CVTimeUnits     fTimeUnit;
            unsigned char   fNPulses;
            CVIOSources     fStartSource;
            CVIOSources     fStopSource;

            V2718Pulser( CVPulserSelect pulser ) :
                fOwner( nullptr ),
                fPulser( pulser ),
                fPeriod( 1 ),
                fWidth( 1 ),
                fTimeUnit( cvUnit104ms ),
                fNPulses( 1 ),
                fStartSource( cvManualSW ),
                fStopSource( cvManualSW )
            {}
            ~V2718Pulser() { }
            V2718Pulser( const V2718Pulser &other ) = delete;
            V2718Pulser& operator=( const V2718Pulser &other ) = delete;

        public :
            CVPulserSelect      GetPulser() const                 { return fPulser; }
            void                SetPeriod( unsigned char period ) { fPeriod = period; }
            unsigned char       GetPeriod() const                 { return fPeriod; }
            void                SetWidth( unsigned char width )   { fWidth = width; }
            unsigned char       GetWidth() const                  { return fWidth; }
            double              GetFrequencyReal() const;

            void                SetTimeUnit( CVTimeUnits unit )   { fTimeUnit = unit; }
            CVTimeUnits         GetTimeUnit()                     { return fTimeUnit; }
            void                SetNPulses( unsigned char n )     { fNPulses = n; }
            unsigned char       GetNPulses() const                { return fNPulses; }
            void                SetStartSource( CVIOSources src ) { fStartSource = src; }
            CVIOSources         GetStartSource()                  { return fStartSource; }
            void                SetStopSource( CVIOSources src )  { fStopSource = src; }
            CVIOSources         GetStopSource()                   { return fStopSource; }

            bool                SetSquare( uint32_t freq, uint8_t duty = 50 );
            void                GetSquare( uint32_t &freq, uint8_t &duty );

        public :
            void Write();
            void Read();
            void Start();
            void Stop();

            friend class V2718;
    };


    class V2718Scaler
    {
        protected :
            V2718          *fOwner;

        protected :
            short           fLimit;
            short           fAutoReset;
            CVIOSources     fHitSource;
            CVIOSources     fGateSource;
            CVIOSources     fStopSource;

            V2718Scaler( ) :
                fOwner( nullptr ),
                fLimit( 1 ),
                fAutoReset( 1 ),
                fHitSource( cvInputSrc0 ),
                fGateSource( cvManualSW ),
                fStopSource( cvManualSW )
            {}
            ~V2718Scaler() = default;
            V2718Scaler( const V2718Scaler &other ) = delete;
            V2718Scaler& operator=( const V2718Scaler &other ) = delete;

        public :

            void            SetLimit( short limit )             { fLimit = limit; }
            short           GetLimit()                          { return fLimit; }
            void            SetAutoReset( short autoReset )     { fAutoReset = (autoReset ? 1 : 0); }
            short           GetAutoReset()                      { return (fAutoReset ? 1 : 0); }
            void            SetHitSource( CVIOSources src )     { fHitSource = src; }
            CVIOSources     GetHitSource()                      { return fHitSource; }
            void            SetGateSource( CVIOSources src )    { fGateSource = src; }
            CVIOSources     GetGateSource()                     { return fGateSource; }
            void            SetStopSource( CVIOSources src )    { fStopSource = src; }
            CVIOSources     GetStopSource()                     { return fStopSource; }

        public :
            void Write();
            void Read();
            void Reset();
            void EnableGate( bool enable = true );

            friend class V2718;
    };

    class V2718 : public VController, public UConfigurable<V2718>
    {
        protected :
            static uint8_t const fInNumber = 2;
            static uint8_t const fOutNumber = 5;

        public :
            static uint8_t constexpr GetInNumber() { return fInNumber; }
            static uint8_t constexpr GetOutNumber() { return fOutNumber; }

        protected :
            V2718Pulser     fPulserA;
            V2718Pulser     fPulserB;
            V2718Scaler     fScaler;

        public :
            V2718 ();
            ~V2718() override = default;
            virtual void Open( short link, short bdNum ) override;

        public :
            enum class Out_t        { OUT0, OUT1, OUT2, OUT3, OUT4 };
            enum class In_t         { IN0, IN1 };
            enum class Polarity_t   { DIRECT, INVERTED };
            enum class LED_t        { ACTIVE_HIGH, ACTIVE_LOW }; 
            enum class Src_t        { SW, IN0, IN1, COINCIDENCE, VME, PULS_SCAL };

            V2718Pulser& GetPulser( CVPulserSelect pulser );
            V2718Scaler& GetScaler();

            void WriteOutputConfig( Out_t n, Src_t src,
                                    Polarity_t pol = Polarity_t::DIRECT,
                                    LED_t led = LED_t::ACTIVE_HIGH );
            void ReadOutputConfig( CVOutputSelect outputNo, CVIOPolarity &polarity, CVLEDPolarity &ledPolarity, CVIOSources &src );

            void WriteInputConfig( CVInputSelect inputNo, CVIOPolarity polarity = cvDirect, CVLEDPolarity ledPolarity = cvActiveHigh );
            void ReadInputConfig( CVInputSelect inputNo, CVIOPolarity &polarity, CVLEDPolarity &ledPolarity );

        public :
            void    ReadConfig( UConfig<V2718>& config ) override;
            void    WriteConfig( const UConfig<V2718>& config ) override;
    };

    template<>
    const std::string UConfigurable<V2718>::fName;

    template<>
    struct UConfig<V2718>
    {
        struct Input
        {
            CVIOPolarity    POLARITY;
            CVLEDPolarity   LED_POLARITY;

            Input() :
                POLARITY( CVIOPolarity::cvDirect ),
                LED_POLARITY( CVLEDPolarity::cvActiveHigh )
            {
            }

            template <class Archive>
            void serialize( Archive& ar )
            {
                ar( cereal::make_nvp( "polarity", POLARITY ),
                    cereal::make_nvp( "led_polarity", LED_POLARITY ) );
            }
        };

        struct Output
        {
            CVIOPolarity    POLARITY;
            CVLEDPolarity   LED_POLARITY;
            CVIOSources     SOURCE;

            Output() :
                POLARITY( CVIOPolarity::cvDirect ),
                LED_POLARITY( CVLEDPolarity::cvActiveHigh ),
                SOURCE( CVIOSources::cvManualSW )
            {
            }

            template <class Archive>
            void serialize( Archive& ar )
            {
                ar( cereal::make_nvp( "polarity", POLARITY ),
                    cereal::make_nvp( "led_polarity", LED_POLARITY ),
                    cereal::make_nvp( "source", SOURCE ) );
            }
        };

        struct Pulser
        {
            uint32_t        FREQUENCY;
            uint8_t         DUTY;
            unsigned char   N_PULSES;
            CVIOSources     START_SOURCE;
            CVIOSources     STOP_SOURCE;

            Pulser() :
                FREQUENCY( 10 ),
                DUTY( 50 ),
                N_PULSES( 0 ),
                START_SOURCE( CVIOSources::cvManualSW ),
                STOP_SOURCE( CVIOSources::cvManualSW )
            {
            }

            template <class Archive>
            void serialize( Archive& ar )
            {
                ar( cereal::make_nvp( "frequency", FREQUENCY ),
                    cereal::make_nvp( "duty", DUTY ),
                    cereal::make_nvp( "N", N_PULSES ),
                    cereal::make_nvp( "start", START_SOURCE ),
                    cereal::make_nvp( "stop", STOP_SOURCE ) );
            }
        };

        struct Scaler
        {
            short           LIMIT;
            short           AUTO_RESET;
            CVIOSources     HIT_SOURCE;
            CVIOSources     GATE_SOURCE;
            CVIOSources     STOP_SOURCE;

            Scaler() :
                LIMIT( 0 ),
                AUTO_RESET( 1 ),
                HIT_SOURCE( CVIOSources::cvInputSrc0 ),
                GATE_SOURCE( CVIOSources::cvManualSW ),
                STOP_SOURCE( CVIOSources::cvInputSrc1 )
            {
            }

            template <class Archive>
            void serialize( Archive& ar )
            {
                ar( cereal::make_nvp( "limit", LIMIT ),
                    cereal::make_nvp( "duty", AUTO_RESET ),
                    cereal::make_nvp( "hit", HIT_SOURCE ),
                    cereal::make_nvp( "gate", GATE_SOURCE ),
                    cereal::make_nvp( "stop", STOP_SOURCE ) );
            }
        };

        std::array<Input, V2718::GetInNumber()>     INPUTS;
        std::array<Output, V2718::GetOutNumber()>   OUTPUTS;
        Pulser                                      PULSER_A;
        Pulser                                      PULSER_B;
        Scaler                                      SCALER;

        template <class Archive>
        void serialize( Archive& ar )
        {
            ar( cereal::make_nvp( "inputs", INPUTS ),
                cereal::make_nvp( "outputs", OUTPUTS ),
                cereal::make_nvp( "pulser_A", PULSER_A ),
                cereal::make_nvp( "pulser_B", PULSER_B ),
                cereal::make_nvp( "scaler", SCALER ) );
        }
    };
}
#endif
