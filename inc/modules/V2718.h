#ifndef V_PLUS_V2718_H
#define V_PLUS_V2718_H

#include <array>
#include <memory>

#include <CAENVMEtypes.h>
#include <memory>
#include "VController.h"
#include "UConfigurable.h"

#include "cereal/cereal.hpp"
#include "cereal/types/array.hpp"


namespace vmepp
{
    class V2718 : public VController, public UConfigurable<V2718>
    {
        protected :
            static uint8_t const fInNumber = 2;
            static uint8_t const fOutNumber = 5;

        public :
            static uint8_t constexpr GetInNumber() { return fInNumber; }
            static uint8_t constexpr GetOutNumber() { return fOutNumber; }
        public :
            class Pulser;
            class Scaler;

        protected :
            std::unique_ptr<Pulser>    fPulserA;
            std::unique_ptr<Pulser>    fPulserB;
            std::unique_ptr<Scaler>    fScaler;

        public :
            V2718 ();
            ~V2718() override = default;
            virtual void Open( short link, short bdNum ) override;

        public :
            enum class Out_t            { OUT0, OUT1, OUT2, OUT3, OUT4 };
            enum class In_t             { IN0, IN1 };
            enum class Polarity_t       { DIRECT, INVERTED };
            enum class LEDPolarity_t    { ACTIVE_HIGH, ACTIVE_LOW };
            enum class Src_t            { SW, IN0, IN1, COINCIDENCE, VME, PULS_SCAL };
            enum class Pulser_t         { A, B };

            struct OutputConfig
            {
                Src_t           SOURCE;
                Polarity_t      POLARITY;
                LEDPolarity_t   LED_POLARITY;

                OutputConfig( Src_t src = Src_t::SW,
                              Polarity_t pol = Polarity_t::DIRECT,
                              LEDPolarity_t ledPol = LEDPolarity_t::ACTIVE_HIGH ) :
                    SOURCE( src ), POLARITY( pol ), LED_POLARITY( ledPol )
                {
                }
            };

            struct InputConfig
            {
                Polarity_t      POLARITY;
                LEDPolarity_t   LED_POLARITY;

                InputConfig( Polarity_t pol = Polarity_t::DIRECT,
                             LEDPolarity_t ledPol = LEDPolarity_t::ACTIVE_HIGH ) :
                    POLARITY( pol ), LED_POLARITY( ledPol )
                {
                }
            };

            Pulser* GetPulser( Pulser_t pulser );
            Scaler* GetScaler();

            void WriteOutputConfig( Out_t n, const OutputConfig& cfg );
            void ReadOutputConfig( Out_t n, OutputConfig& cfg );

            void WriteInputConfig( In_t n, const InputConfig& cfg );
            void ReadInputConfig( In_t n, InputConfig& cfg );

        public :
            void ReadConfig( UConfig<V2718>& config ) override;
            void WriteConfig( const UConfig<V2718>& config ) override;
    };

    template<>
    const std::string UConfigurable<V2718>::fName;

    template<>
    struct UConfig<V2718>
    {
        struct Input
        {
            V2718::Polarity_t       POLARITY;
            V2718::LEDPolarity_t    LED_POLARITY;

            Input() :
                POLARITY( V2718::Polarity_t::DIRECT ),
                LED_POLARITY( V2718::LEDPolarity_t::ACTIVE_HIGH )
            {
            }

            template <class TArchive>
            void serialize( TArchive& ar )
            {
                ar( cereal::make_nvp( "polarity", POLARITY ),
                    cereal::make_nvp( "led_polarity", LED_POLARITY ) );
            }
        };

        struct Output
        {
            V2718::Polarity_t       POLARITY;
            V2718::LEDPolarity_t    LED_POLARITY;
            V2718::Src_t            SOURCE;

            Output() :
                POLARITY( V2718::Polarity_t::DIRECT ),
                LED_POLARITY( V2718::LEDPolarity_t::ACTIVE_HIGH ),
                SOURCE( V2718::Src_t::SW )
            {
            }

            template <class TArchive>
            void serialize( TArchive& ar )
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
            V2718::Src_t    START_SOURCE;
            V2718::Src_t    STOP_SOURCE;

            Pulser() :
                FREQUENCY( 10 ),
                DUTY( 50 ),
                N_PULSES( 0 ),
                START_SOURCE( V2718::Src_t::SW ),
                STOP_SOURCE( V2718::Src_t::SW )
            {
            }

            template <class TArchive>
            void serialize( TArchive& ar )
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
            V2718::Src_t    HIT_SOURCE;
            V2718::Src_t    GATE_SOURCE;
            V2718::Src_t    STOP_SOURCE;

            Scaler() :
                LIMIT( 0 ),
                AUTO_RESET( 1 ),
                HIT_SOURCE( V2718::Src_t::IN0 ),
                GATE_SOURCE( V2718::Src_t::SW ),
                STOP_SOURCE( V2718::Src_t::IN1 )
            {
            }

            template <class TArchive>
            void serialize( TArchive& ar )
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

        template <class TArchive>
        void serialize( TArchive& ar )
        {
            ar( cereal::make_nvp( "inputs", INPUTS ),
                cereal::make_nvp( "outputs", OUTPUTS ),
                cereal::make_nvp( "pulser_A", PULSER_A ),
                cereal::make_nvp( "pulser_B", PULSER_B ),
                cereal::make_nvp( "scaler", SCALER ) );
        }
    };

    class V2718::Pulser
    {
        protected :
            V2718          *fOwner;

        protected :
            V2718::Pulser_t fPulser;
            unsigned char   fPeriod;
            unsigned char   fWidth;
            CVTimeUnits     fTimeUnit;
            unsigned char   fNPulses;
            V2718::Src_t    fStartSource;
            V2718::Src_t    fStopSource;

            Pulser( V2718::Pulser_t pulser ) :
                fOwner( nullptr ),
                fPulser( pulser ),
                fPeriod( 1 ),
                fWidth( 1 ),
                fTimeUnit( cvUnit104ms ),
                fNPulses( 1 ),
                fStartSource( V2718::Src_t::SW ),
                fStopSource( V2718::Src_t::SW )
            {}
            ~Pulser() = default;
            Pulser( const Pulser &other ) = delete;
            Pulser& operator=( const Pulser &other ) = delete;

        public :
            V2718::Pulser_t     GetPulser() const                 { return fPulser; }
            void                SetPeriod( unsigned char period ) { fPeriod = period; }
            unsigned char       GetPeriod() const                 { return fPeriod; }
            void                SetWidth( unsigned char width )   { fWidth = width; }
            unsigned char       GetWidth() const                  { return fWidth; }
            double              GetFrequencyReal() const;

            void                SetTimeUnit( CVTimeUnits unit )   { fTimeUnit = unit; }
            CVTimeUnits         GetTimeUnit()                     { return fTimeUnit; }
            void                SetNPulses( unsigned char n )     { fNPulses = n; }
            unsigned char       GetNPulses() const                { return fNPulses; }
            void                SetStartSource( V2718::Src_t src ) { fStartSource = src; }
            V2718::Src_t        GetStartSource()                  { return fStartSource; }
            void                SetStopSource( V2718::Src_t src )  { fStopSource = src; }
            V2718::Src_t        GetStopSource()                   { return fStopSource; }

            bool                SetSquare( uint32_t freq, uint8_t duty = 50 );
            void                GetSquare( uint32_t &freq, uint8_t &duty );

        public :
            void Write();
            void Read();
            void Start();
            void Stop();

            friend class V2718;
            friend class std::default_delete<Pulser>;
    };


    class V2718::Scaler
    {
        protected :
            V2718          *fOwner;

        protected :
            short           fLimit;
            short           fAutoReset;
            V2718::Src_t    fHitSource;
            V2718::Src_t    fGateSource;
            V2718::Src_t    fStopSource;

            Scaler( ) :
                fOwner( nullptr ),
                fLimit( 1 ),
                fAutoReset( 1 ),
                fHitSource( V2718::Src_t::IN0 ),
                fGateSource( V2718::Src_t::SW ),
                fStopSource( V2718::Src_t::SW )
            {}
            ~Scaler() = default;
            Scaler( const Scaler &other ) = delete;
            Scaler& operator=( const Scaler &other ) = delete;

        public :

            void            SetLimit( short limit )             { fLimit = limit; }
            short           GetLimit()                          { return fLimit; }
            void            SetAutoReset( short autoReset )     { fAutoReset = (autoReset ? 1 : 0); }
            short           GetAutoReset()                      { return (fAutoReset ? 1 : 0); }
            void            SetHitSource( V2718::Src_t src )    { fHitSource = src; }
            V2718::Src_t    GetHitSource()                      { return fHitSource; }
            void            SetGateSource( V2718::Src_t src )   { fGateSource = src; }
            V2718::Src_t    GetGateSource()                     { return fGateSource; }
            void            SetStopSource( V2718::Src_t src )   { fStopSource = src; }
            V2718::Src_t    GetStopSource()                     { return fStopSource; }

        public :
            void Write();
            void Read();
            void Reset();
            void EnableGate( bool enable = true );

            friend class V2718;
            friend class std::default_delete<Scaler>;
    };

}
#endif
