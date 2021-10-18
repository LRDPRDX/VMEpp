/*
   To test this example you may use the following configuration:
   
   1) Connect any output (say ch 12) of the Descriminator V895 to the
      TRIGGER input of the CONTROL connector of the TDC V1190B

   2) Connect the output (ch 12) of the Descriminator V895 to the input of TDC V1190B

   3) Send the Test signal to the V895 N times (see "WriteIRQEvents" parameter )
*/
#include <iostream>

#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"
#include "modules/V895.h"

using namespace vmepp;

int main()
{
    V2718 controller;
    V1190B tdc( 0x20080000 );
    V895 disc( 0x40080000 );

    try
    {
        controller.Open( 0, 0 );
        controller.RegisterSlave( &tdc );
        controller.RegisterSlave( &disc );
        controller.Initialize();

        disc.EnableOnly( 14 );
        disc.WriteOutWidth( 30 );

        V2718::OutputConfig outCfg;
        outCfg.SOURCE = V2718::Src_t::PULS_SCAL;
        controller.WriteOutputConfig( V2718::Out_t::OUT0, outCfg );

        V2718::Pulser* pA = controller.GetPulser( V2718::Pulser_t::A );
        pA->SetSquare( 5000, 1 );
        pA->SetNPulses( 0 );
        pA->SetStartSource( V2718::Src_t::SW );
        pA->SetStopSource( V2718::Src_t::SW );
        pA->Write();
        pA->Start();

        tdc.WriteDetection( V1190B::EdgeDetect_t::PAIR );
        V1190B::PairRes pRes;
            pRes.EDGE = V1190B::ResLeadEdgeTime::ps100;
            pRes.WIDTH = V1190B::ResPulseWidth::ps400;
        tdc.WritePairRes( pRes );
        tdc.WriteAcqMode( V1190B::TriggerMode_t::MATCHING );
        tdc.WriteWindowWidth( 20 ); // 500 ns
        tdc.WriteWindowOffset( -10 ); // -250 ns
        tdc.WriteEnableSubTrigger( true );

        tdc.WriteIRQEvents( 4000 );
        tdc.WriteIRQVector( 3 );
        tdc.WriteIRQLevel( 1 );

        tdc.WriteControl( V1190B::Control_t::EVENT_FIFO_EN, false );

        tdc.SetReadCycles( 2 );
        tdc.AllocateBuffer();

        uint32_t nEvents = 0;
        while( nEvents < 20000 )
        {
            controller.IRQEnable( cvIRQ1 );
            controller.IRQWait( cvIRQ1, 1000 );
            tdc.ReadBuffer();
            std::cout << tdc.GetReadBytes() << "\n";

            UEvent<V1190B> event;
            while ( tdc.GetEvent( event ) )
            {
                //std::cout << event.GetStart() << "\n";
                //for( auto hit = event.cbegin(); hit != event.cend(); ++hit )
                //{
                //}
            }
            nEvents += tdc.GetNEventsRead();
            //tdc.WriteSoftwareClear();
        }

        pA->Stop();
    }
    catch( const VException &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.GetInfo() << std::endl;
    }
}
