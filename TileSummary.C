void TileSummary()
{

  TCanvas *c1 = new TCanvas();

  TH1F *h = new TH1F("h","",7,0,1);



  h->GetXaxis()->SetBinLabel(1,"OH-1-2");  // 2.73746
  h->GetXaxis()->SetBinLabel(2,"OH-1-3");  // 2.7485
  h->GetXaxis()->SetBinLabel(3,"OH-2-6");  // 2.79458
  h->GetXaxis()->SetBinLabel(4,"OH-1-46"); // 3.0858
  h->GetXaxis()->SetBinLabel(5,"OH-1-47"); // 2.52869
  h->GetXaxis()->SetBinLabel(6,"OH-2-46"); // 2.86719
  h->GetXaxis()->SetBinLabel(7,"OH-2-47"); // 2.89562

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

  // c1->Print("Figures/SummaryOfTiles.png");
  // c1->Print("Figures/SummaryOfTiles.pdf");

  c1->Print("Figures/SummaryOfTiles_GainCorrected.png");
  c1->Print("Figures/SummaryOfTiles_GainCorrected.pdf");

  TH1F* h2 = (TH1F*)h->Clone();

  h2->SetBinContent(1,2.73746);
  h2->SetBinContent(2,2.7485 );
  h2->SetBinContent(3,2.79458);
  h2->SetBinContent(4,3.0858 );
  h2->SetBinContent(5,2.52869);
  h2->SetBinContent(6,2.86719);
  h2->SetBinContent(7,2.89562);

  h2->SetBinError(1,0.01);
  h2->SetBinError(2,0.01);
  h2->SetBinError(3,0.01);
  h2->SetBinError(4,0.01);
  h2->SetBinError(5,0.01);
  h2->SetBinError(6,0.01);
  h2->SetBinError(7,0.01);

  h2->Scale(7.1);
  h2->SetMarkerColor(kBlack);
  h2->SetMarkerStyle(kOpenCircle);
  h2->Draw("ex0p same");

  h->Fit("pol0","R");
  h2->Fit("pol0","R");

  TLegend* leg = new TLegend(0.18,0.18,0.38,0.38);
  leg->AddEntry(h,"Cosmics with mixed SiPMs","el");
  leg->AddEntry(h2,"Average of LED scan with single SiPM (#times 7.1)","p");
  leg->SetTextSize(0.04);
  leg->Draw();


  c1->Print("Figures/SummaryOfTiles_WithLEDScans.png");
  c1->Print("Figures/SummaryOfTiles_WithLEDScans.pdf");

}
