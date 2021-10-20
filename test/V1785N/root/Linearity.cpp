#include <TGraph.h>
#include <TAxis.h>
#include <TStyle.h>
#include <TF1.h>

void Linearity()
{
    gStyle->SetOptFit( 1 );

    Double_t x[7] = { 0.120, 0.20, 0.4, 0.620, 1.0, 2.0, 3.0 };
    Double_t y[7] = { 205, 288, 487, 702, 1101, 2100, 3115 };
    TGraph *g = new TGraph( 7, x, y );
        g->SetName( "Linearity" );
        g->SetTitle( "Linearity" );
        g->SetMarkerStyle( kCircle );
        g->GetXaxis()->SetTitle( "V_{in}, [V]" );
        g->GetXaxis()->CenterTitle( kTRUE );
        g->GetYaxis()->SetTitle( "f(V_{in}), [ADC counts]" );
        g->GetYaxis()->CenterTitle( kTRUE );

    TF1 *f = new TF1( "f", "[0] + [1]*x" );
    f->SetParNames( "#mu_{ped}", "k" );

    g->Fit( f );
    g->Draw( "AP" );

}
