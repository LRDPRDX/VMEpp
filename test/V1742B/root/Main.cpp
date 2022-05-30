#include <iostream>

#include <TH1F.h>
#include <TGraph.h>
#include <TCanvas.h>

#include "VException.h"
#include "modules/V2718.h"
#include "modules/V1190B.h"
#include "modules/V895.h"
#include "modules/V1742B.h"

using namespace vmepp;


void Draw()
{
    TCanvas* canvas = new TCanvas( "Test", "Test", 800, 800 );
        canvas->Divide( 2, 2 );
    TGraph *wfCorr = new TGraph();
        wfCorr->SetName( "Corrections Applied");
        wfCorr->SetTitle( "Corrections Applied");
    TGraph *wfNoCorr = new TGraph();
        wfNoCorr->SetName( "No Correction");
        wfNoCorr->SetTitle( "No Correction");

    TH1F *hCorr = new TH1F( "Corrections Applied (hist)", "Corrections Applied (hist)", 4096, 0, 4096 );
    TH1F *hNoCorr = new TH1F( "No Correction (hist)", "No Correction (hist)", 4096, 0, 4096 );

    V2718 controller;
    V1742B digitizer( 0x50000000 );
    digitizer.SetPathToCorrectionTable( "./correction.x742_corr" );

    try
    {
        controller.Open( 0, 0 );
        controller.RegisterSlave( &digitizer );
        controller.Initialize();

        digitizer.WriteReadoutEnableGroup( V1742B::Group_t::G0, true );
        digitizer.WriteReadoutEnableTR( true );
        digitizer.WriteRecordLength( V1742B::RecordLength_t::s1024 );
        digitizer.WriteSamplingRate( V1742B::SamplingRate_t::M5000 );

        digitizer.WriteTriggerPolarity( V1742B::TriggerPolarity_t::FallingEdge );
        digitizer.WriteGlobalTrigger( V1742B::GlobalTrigger_t::SWOnly );
        digitizer.WriteChannelOffset(0, 0x8000);
        //digitizer.WriteEnableTrigger(0, true); 
        digitizer.WriteAcqMode( V1742B::AcqMode_t::Output );
        //digitizer.WriteChannelThreshold(0, thr);
        
        //*****************************
        //****** ACQUISITION +++ ******
        //*****************************
        VBuffer buffer;
        UParser parser;
        UEvent<V1742B> event;
                
        digitizer.Start();
        usleep(1e3); 
        digitizer.WriteSWTrigger();
        usleep(1e3); 
        digitizer.WriteSWTrigger();
        usleep(1e3); 
        digitizer.WriteSWTrigger();
        usleep(1e3); 
        digitizer.WriteSWTrigger();
        usleep(1e3); 
        digitizer.Stop();

        digitizer.ReadBuffer( buffer );

        if(parser.GetEvent(event, buffer)){
            auto& grp0 = event.GetGroup( V1742B::Group_t::G0 );
            auto wf = grp0.GetChannel(0);
            wfNoCorr->Set( wf.size() );
            for( size_t i = 0; i < wf.size(); ++i )
            {
                wfNoCorr->SetPoint( i, i, wf[i] );
                hNoCorr->Fill( wf[i] );
            }

            digitizer.ApplyCorrection(event);
            wf = grp0.GetChannel(0);
            wfCorr->Set( wf.size() );
            for( size_t i = 0; i < wf.size(); ++i )
            {
                wfCorr->SetPoint( i, i, wf[i] );
                hCorr->Fill( wf[i] );
            }
        }
        //*****************************
        //****** ACQUISITION --- ******
        //*****************************
        canvas->cd( 1 ); 
        wfNoCorr->Draw( "APL" );

        canvas->cd( 2 ); 
        wfCorr->Draw( "APL" );

        canvas->cd( 3 ); 
        hNoCorr->Draw();

        canvas->cd( 4 ); 
        hCorr->Draw();
    }
    catch( const VException& e )
    {
        std::cout << e.what() << "\n";
        std::cout << e.GetInfo() << "\n";
        std::cout << e.GetHint() << "\n";
    }
}
