void Draw()
{
    TCanvas* c = new TCanvas( "c", "c", 800, 600 );
    TGraph* g = new TGraph( "./data.dat" );

    g->Draw( "AP" );
        g->GetXaxis()->SetTitle( "f_{set}, [Hz]" );
        g->GetXaxis()->CenterTitle( kTRUE );
        g->GetYaxis()->SetTitle( "#frac{f_{set} - f_{real}}{f_{set}}" );
        g->GetYaxis()->CenterTitle( kTRUE );

    c->SetLogx();
    c->SetGridx();
    c->SetGridy();
}
