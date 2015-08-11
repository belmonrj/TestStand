void DrawDiscrim()
{

  TCanvas *c1 = new TCanvas();

  ifstream fin("ScanDiscrimVoltage_20150724.dat");

  float volt[7], counts_source[7], counts_none[7], ratio[7];

  for(int i=0; i<7; i++)
    {
      fin>>volt[i]>>counts_source[i]>>counts_none[i];
      ratio[i] = counts_source[i]/counts_none[i];
    }

  TGraph *tg_source = new TGraph(7,volt,counts_source);
  TGraph *tg_none = new TGraph(7,volt,counts_none);
  TGraph *tg_ratio = new TGraph(7,volt,ratio);

  tg_source->SetMarkerStyle(kFullSquare);
  tg_none->SetMarkerStyle(kFullCircle);
  tg_source->SetMarkerColor(kRed);
  tg_none->SetMarkerColor(kBlack);

  tg_source->Draw("ap");
  tg_source->SetMaximum(3e4);
  tg_source->SetMinimum(7e0);
  tg_source->GetXaxis()->SetLimits(0.0,1.0);
  tg_source->GetXaxis()->SetTitle("Discriminator Voltage, SiPM 1 and 2 [V]");
  tg_source->GetYaxis()->SetTitle("Counts");
  tg_none->Draw("p");
  TLegend *leg = new TLegend(0.58,0.68,0.88,0.88);
  leg->AddEntry(tg_source,"Source Present","p");
  leg->AddEntry(tg_none,"No Source","p");
  leg->SetTextSize(0.045);
  leg->Draw();

  c1->SetLogy();

  c1->Print("CountsVsVoltage.png");

  c1->SetLogy(0);

  tg_ratio->SetMarkerColor(kBlue);
  tg_ratio->SetMarkerStyle(kFullSquare);
  tg_ratio->Draw("ap");
  tg_ratio->SetMaximum(16.0);
  tg_ratio->SetMinimum(0.0);
  tg_ratio->GetXaxis()->SetLimits(0.0,1.0);
  tg_ratio->GetXaxis()->SetTitle("Discriminator Voltage, SiPM 1 and 2 [V]");
  tg_ratio->GetYaxis()->SetTitle("Ratio of Counts, Source/None");

  TArrow *ar1 = new TArrow(0.3,15.0,0.05,15.0);
  ar1->SetLineWidth(2);
  ar1->Draw();
  TLatex *tex1 = new TLatex(0.32,14.75,"Noise dominated");
  tex1->SetTextSize(0.045);
  tex1->Draw();
  TArrow *ar2 = new TArrow(0.7,14.0,0.95,14.0);
  ar2->SetLineWidth(2);
  ar2->Draw();
  TLatex *tex1 = new TLatex(0.32,13.75,"Cosmics dominated");
  tex1->SetTextSize(0.045);
  tex1->Draw();

  c1->Print("RatioVsVoltage.png");


}
