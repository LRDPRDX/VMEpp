#include <TGraph.h>


void Linearity()
{
    gStyle->SetOptFit( 1 );

    TGraph *g = new TGraph( "data.txt" );
        g->SetName( "Linearity" );
        g->SetTitle( "Linearity" );
        g->SetMarkerStyle( kCircle );
        g->GetXaxis()->SetTitle( "V_{in}, [mV]" );
        g->GetXaxis()->CenterTitle( kTRUE );
        g->GetYaxis()->SetTitle( "f(V_{in}), [ADC counts]" );
        g->GetYaxis()->CenterTitle( kTRUE );

    TF1 *f = new TF1( "f", "[0] + [1]*x" );
    f->SetParNames( "#mu_{ped}", "k" );

    g->Fit( f ); 
    g->Draw( "AP" );

}
