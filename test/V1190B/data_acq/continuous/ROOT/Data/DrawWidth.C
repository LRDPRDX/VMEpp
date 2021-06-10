Double_t Fit( Double_t* x, Double_t* par )
{
    Double_t xx = x[0];

    Double_t A      = par[0];
    Double_t tau    = par[1];

    return A / (1. - TMath::Exp( (xx - 257)* tau ));
}

void DrawWidth()
{
    gStyle->SetOptFit( true );

    TGraph* g = new TGraph( "V895width.dat" );
        g->SetMarkerStyle( kCircle );
    g->Draw( "AP" );

    TF1* fit = new TF1( "fit", "[0]/(1. - (x/[1]))", 0, 256 );
        fit->SetParNames( "a", "b" );
        fit->SetParameter( 0, 5 );
        fit->SetParameter( 1, 256);

    g->Fit( fit, "R" );


}
