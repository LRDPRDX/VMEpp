void c1()
{
//=========Macro generated from canvas: c1/c1
//=========  (Tue Oct 19 17:30:10 2021) by ROOT version 6.20/00
   TCanvas *c1 = new TCanvas("c1", "c1",0,65,1745,977);
   gStyle->SetOptFit(1);
   c1->Range(-35.06251,-4.640001,315.5625,52);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   Double_t Graph0_fx1[24] = {
   0,
   16,
   32,
   48,
   64,
   80,
   96,
   112,
   128,
   144,
   160,
   176,
   192,
   208,
   224,
   232,
   240,
   247,
   250,
   251,
   252,
   253,
   254,
   255};
   Double_t Graph0_fy1[24] = {
   4.8,
   4.94,
   5.4,
   5.5,
   5.6,
   6.26,
   6.41,
   7.18,
   7.95,
   8.8,
   10.08,
   11.41,
   13.38,
   16,
   20.12,
   23.2,
   27.24,
   33,
   36,
   37.3,
   38.4,
   39.99,
   41.11,
   42.56};
   TGraph *graph = new TGraph(24,Graph0_fx1,Graph0_fy1);
   graph->SetName("Graph0");
   graph->SetTitle("V895width.data");
   graph->SetFillStyle(1000);
   graph->SetMarkerStyle(4);
   
   TH1F *Graph_Graph01 = new TH1F("Graph_Graph01","V895width.data",100,0,280.5);
   Graph_Graph01->SetMinimum(1.024);
   Graph_Graph01->SetMaximum(46.336);
   Graph_Graph01->SetDirectory(0);
   Graph_Graph01->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph01->SetLineColor(ci);
   Graph_Graph01->GetXaxis()->SetTitle("register value, x [bits]");
   Graph_Graph01->GetXaxis()->CenterTitle(true);
   Graph_Graph01->GetXaxis()->SetLabelFont(42);
   Graph_Graph01->GetXaxis()->SetTitleOffset(1);
   Graph_Graph01->GetXaxis()->SetTitleFont(42);
   Graph_Graph01->GetYaxis()->SetTitle("pulse width, w [ns]");
   Graph_Graph01->GetYaxis()->CenterTitle(true);
   Graph_Graph01->GetYaxis()->SetLabelFont(42);
   Graph_Graph01->GetYaxis()->SetTitleFont(42);
   Graph_Graph01->GetZaxis()->SetLabelFont(42);
   Graph_Graph01->GetZaxis()->SetTitleOffset(1);
   Graph_Graph01->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph01);
   
   
   TF1 *fit2 = new TF1("fit","[0]/(1. - (x/[1]))",0,256, TF1::EAddToList::kNo);
   fit2->SetFillColor(19);
   fit2->SetFillStyle(0);
   fit2->SetLineColor(2);
   fit2->SetLineWidth(2);
   fit2->SetChisquare(1.823633);
   fit2->SetNDF(22);
   fit2->GetXaxis()->SetLabelFont(42);
   fit2->GetXaxis()->SetTitleOffset(1);
   fit2->GetXaxis()->SetTitleFont(42);
   fit2->GetYaxis()->SetLabelFont(42);
   fit2->GetYaxis()->SetTitleFont(42);
   fit2->SetParameter(0,4.293329);
   fit2->SetParError(0,0.03868124);
   fit2->SetParLimits(0,0,0);
   fit2->SetParameter(1,283.7155);
   fit2->SetParError(1,0.3472894);
   fit2->SetParLimits(1,0,0);
   fit2->SetParent(graph);
   graph->GetListOfFunctions()->Add(fit2);
   
   TPaveStats *ptstats = new TPaveStats(0.1501564,0.706191,0.5099062,0.8667366,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *ptstats_LaTex = ptstats->AddText("#chi^{2} / ndf = 1.824 / 22");
   ptstats_LaTex = ptstats->AddText("a        = 4.293 #pm 0.03868 ");
   ptstats_LaTex = ptstats->AddText("b        = 283.7 #pm 0.3473 ");
   ptstats->SetOptStat(0);
   ptstats->SetOptFit(111);
   ptstats->Draw();
   graph->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(graph->GetListOfFunctions());
   graph->Draw("ap");
   
   TPaveText *pt = new TPaveText(0.3646418,0.94,0.6353582,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   pt->Draw();
   TLatex *   tex = new TLatex(42.98827,26.79948,"w(x) = #frac{a}{1 - x/b} #rightarrow x = b * (1 - a/w)");
   tex->SetTextColor(2);
   tex->SetTextFont(52);
   tex->SetLineWidth(2);
   tex->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
   c1->ToggleToolBar();
}
