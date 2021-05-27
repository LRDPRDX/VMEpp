void Draw()
{
    TGraph* g = new TGraph( "data.dat" );
        g->SetMarkerStyle( kCircle );
    g->Draw( "AP" );
}
