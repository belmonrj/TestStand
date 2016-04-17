void TileSummary()
{

  TCanvas *c1 = new TCanvas();

  TH1F *h = new TH1F("h","",7,0,1);



  h->GetXaxis()->SetBinLabel(1,"OH-1-2");
  h->GetXaxis()->SetBinLabel(2,"OH-1-3");
  h->GetXaxis()->SetBinLabel(3,"OH-2-6");
  h->GetXaxis()->SetBinLabel(4,"OH-1-46");
  h->GetXaxis()->SetBinLabel(5,"OH-1-47");
  h->GetXaxis()->SetBinLabel(6,"OH-2-46");
  h->GetXaxis()->SetBinLabel(7,"OH-2-47");

  h->SetBinContent(1,8.6);
  h->SetBinError(1,0.1);

  h->SetBinContent(2,8.1);
  h->SetBinError(2,0.1);

  h->SetBinContent(3,9.6);
  h->SetBinError(3,0.2);

  h->SetBinContent(4,11.0);
  h->SetBinError(4,0.1);

  h->SetBinContent(5,6.5);
  h->SetBinError(5,0.3);

  h->SetBinContent(6,7.6);
  h->SetBinError(6,0.1);

  h->SetBinContent(7,9.3);
  h->SetBinError(7,0.1);

  h->Scale(2.24); // difference in gain between S12572-015P and -025P

  h->Draw();
  h->GetXaxis()->SetNdivisions(7,kFALSE);
  h->SetLineColor(kBlack);
  h->SetMarkerColor(kBlack);
  h->SetMarkerStyle(1);
  h->SetMaximum(12.0*2.24);
  h->SetMinimum(0.0);

  // c1->Print("Figs/SummaryOfTiles.png");
  // c1->Print("Figs/SummaryOfTiles.pdf");

  c1->Print("Figs/SummaryOfTiles_GainCorrected.png");
  c1->Print("Figs/SummaryOfTiles_GainCorrected.pdf");

}
