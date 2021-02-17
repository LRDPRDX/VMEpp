#ifndef V_PLUS_V2718_H
#define V_PLUS_V2718_H

#include <CAENVMEtypes.h>
#include "VController.h"

namespace vmeplus
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
            void                SetTimeUnit( CVTimeUnits unit )   { fTimeUnit = unit; }  
            CVTimeUnits         GetTimeUnit()                     { return fTimeUnit; }  
            void                SetNPulses( unsigned char n )     { fNPulses = n; }
            unsigned char       GetNPulses() const                { return fNPulses; }
            void                SetStartSource( CVIOSources src ) { fStartSource = src; }
            CVIOSources         GetStartSource()                  { return fStartSource; }
            void                SetStopSource( CVIOSources src )  { fStopSource = src; }
            CVIOSources         GetStopSource()                   { return fStopSource; }

            void                SetSquare( uint32_t freq, uint8_t duty = 50 );
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
            ~V2718Scaler() {};
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
    
    class V2718 : public VController
    {
        protected :
            V2718Pulser     fPulserA;
            V2718Pulser     fPulserB;
            V2718Scaler     fScaler;

        public :
            V2718 ();
            virtual ~V2718 ();
            virtual void Open( short link, short bdNum ) override;

        public :
            V2718Pulser& GetPulser( CVPulserSelect pulser );
            V2718Scaler& GetScaler();

            void WriteOutputConfig( CVOutputSelect outputNo, CVIOSources src, CVIOPolarity polarity = cvDirect, CVLEDPolarity ledPolarity = cvActiveHigh );
            void ReadOutputConfig( CVOutputSelect outputNo, CVIOPolarity &polarity, CVLEDPolarity &ledPolarity, CVIOSources &src );
    
            void WriteInputConfig( CVInputSelect inputNo, CVIOPolarity polarity = cvDirect, CVLEDPolarity ledPolarity = cvActiveHigh );
            void ReadInputConfig( CVInputSelect inputNo, CVIOPolarity &polarity, CVLEDPolarity &ledPolarity );
    };
}
#endif
