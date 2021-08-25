void Draw()
{
    TGraph* g = new TGraph( "./data.dat" );

    g->Draw( "APL" );
}
