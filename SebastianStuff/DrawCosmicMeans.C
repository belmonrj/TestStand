void DrawCosmicMeans()
{
  const double PE = 0.005;

  TCanvas *c1 = new TCanvas("c1");
  TCanvas *c2 = new TCanvas("c2");
  TCanvas *c3 = new TCanvas("c3");
  c1->Divide(4,2);
  gStyle->SetOptStat(0);

  TH1D *OH12 = new TH1D("OH12","OH 1 2",50,0,100);
  TH1D *OH13 = new TH1D("OH13","OH 1 3",50,0,100);
  TH1D *OH146 = new TH1D("OH146","OH 1 46",50,0,100);
  TH1D *OH147 = new TH1D("OH147","OH 1 47",50,0,100);
  TH1D *OH26 = new TH1D("OH26","OH 2 6",50,0,100);
  TH1D *OH246 = new TH1D("OH246","OH 2 46",50,0,100);
  TH1D *OH247 = new TH1D("OH247","OH 2 47",50,0,100);
  TF1 *fgumbel = new TF1("fgumbel","([0]/sqrt(6.28))*TMath::Exp(-0.5*((x-[1])/[2] + TMath::Exp(-(x-[1])/[2])))",0,100);
  TF1 *flandau = new TF1("flandau","landau",0,100);

  TGraph *Means = new TGraph(); 
  TGraph *OldMeans = new TGraph();
  TLegend *Leg = new TLegend(0.2,0.8,0.8,0.2); 
  TLatex l;
  l.SetNDC();
  ifstream Cosmic12;
  ifstream Cosmic13;
  ifstream Cosmic146;
  ifstream Cosmic147;
  ifstream Cosmic26;
  ifstream Cosmic246;
  ifstream Cosmic247;
  
  Cosmic12.open("/Users/naglelab/CodePractice/Data_Txt_Files/20160614-1630_Unaveraged_VMin2_OH12.txt");
  Cosmic13.open("/Users/naglelab/CodePractice/Data_Txt_Files/20160616-1731_Unaveraged_VMin2_OH13.txt");
  Cosmic146.open("/Users/naglelab/CodePractice/Data_Txt_Files/20160520-1710_Unaveraged_VMin2_OH146.txt");
  Cosmic147.open("/Users/naglelab/CodePractice/Data_Txt_Files/20160516-1006_Unaveraged_VMin2_OH147.txt");
  Cosmic246.open("/Users/naglelab/CodePractice/Data_Txt_Files/20160606-1400_Unaveraged_VMin2_OH246.txt");
  Cosmic247.open("/Users/naglelab/CodePractice/Data_Txt_Files/20160602-1416_Unaveraged_VMin2_OH247.txt");
  Cosmic26.open("/Users/naglelab/CodePractice/Data_Txt_Files/20160624-1010_Unaveraged_VMin2_OH26.txt");
  
  double vmin;


  while(Cosmic12>>vmin)
    {
      if(-vmin/PE>18) OH12->Fill(-vmin/PE);
    }
  while(Cosmic13>>vmin)
    {
      if(-vmin/PE>18) OH13->Fill(-vmin/PE);
    }
  while(Cosmic146>>vmin)
    {
      if(-vmin/PE>18) OH146->Fill(-vmin/PE);
    }
  while(Cosmic147>>vmin)
    {
      if(-vmin/PE>18) OH147->Fill(-vmin/PE);
    }
  while(Cosmic246>>vmin)
    {
      if(-vmin/PE>18) OH246->Fill(-vmin/PE);
    }
  while(Cosmic247>>vmin)
    {
      if(-vmin/PE>18) OH247->Fill(-vmin/PE);
    }
  while(Cosmic26>>vmin)
    {
      if(-vmin/PE>18) OH26->Fill(-vmin/PE);
    }

  Cosmic12.close();
  Cosmic13.close();
  Cosmic146.close();
  Cosmic147.close();
  Cosmic246.close();
  Cosmic247.close();
  Cosmic26.close();

  fgumbel->SetParameter(0,1000);
  fgumbel->SetParameter(1,40);
  fgumbel->SetParameter(2,10);
  fgumbel->SetParLimits(2,0,50);
  fgumbel->SetLineColor(kBlue);

  //  Leg->AddEntry(fgumbel,"Gumbel Fit","l");
  Leg->AddEntry(flandau,"Landau Fit","l");
  Leg->AddEntry(OH12,"Cosmic Histogram","l");

  double er_y[7] = {0};
  double er_x[7] = {0};
  double x[7] = {1,2,3,4,5,6,7};
  double y[7] = {38.0,38.1,46.4,38.6,33.6,34.6,33.9};

  c1->cd(1);
  OH12->Draw();
  OH12->Fit(fgumbel,"R");
  OH12->Fit(flandau,"R");
  double mean = flandau->GetParameter(1);
  double er_mean = flandau->GetParError(1);
  er_y[0] = er_mean;
  l.DrawLatex(0.5,0.8,Form("mean = %.1f #pm %.1f",mean,er_mean));
  //  fgumbel->Draw("same");

  c1->cd(2);
  OH13->Draw();
  OH13->Fit(fgumbel,"R");
  OH13->Fit(flandau,"R");
  mean = flandau->GetParameter(1);
  er_mean = flandau->GetParError(1);
  er_y[1] = er_mean;
  l.DrawLatex(0.5,0.8,Form("mean = %.1f #pm %.1f",mean,er_mean));
  //  fgumbel->Draw("same");

  c1->cd(3);
  OH146->Draw();
  OH146->Fit(fgumbel,"R");
  OH146->Fit(flandau,"R");
  mean = flandau->GetParameter(1);
  er_mean = flandau->GetParError(1);
  er_y[2] = er_mean;
  l.DrawLatex(0.5,0.8,Form("mean = %.1f #pm %.1f",mean,er_mean));
  //  fgumbel->Draw("same");

  c1->cd(4);
  OH147->Draw();
  OH147->Fit(fgumbel,"R");
  OH147->Fit(flandau,"R");
  mean = flandau->GetParameter(1);
  er_mean = flandau->GetParError(1);
  er_y[3] = er_mean;
  l.DrawLatex(0.5,0.8,Form("mean = %.1f #pm %.1f",mean,er_mean));
  //  fgumbel->Draw("same");

  c1->cd(5);
  OH26->Draw();
  OH26->Fit(fgumbel,"R");
  OH26->Fit(flandau,"R");
  mean = flandau->GetParameter(1);
  er_mean = flandau->GetParError(1);
  er_y[4] = er_mean;
  l.DrawLatex(0.5,0.8,Form("mean = %.1f #pm %.1f",mean,er_mean));
  //  fgumbel->Draw("same");

  c1->cd(6);
  OH246->Draw();
  OH246->Fit(fgumbel,"R");
  OH246->Fit(flandau,"R");
  mean = flandau->GetParameter(1);
  er_mean = flandau->GetParError(1);
  er_y[5] = er_mean;
  l.DrawLatex(0.5,0.8,Form("mean = %.1f #pm %.1f",mean,er_mean));
  //  fgumbel->Draw("same");

  c1->cd(8);
  Leg->Draw();

  c1->cd(7);
  OH247->Draw();
  OH247->Fit(fgumbel,"R");
  OH247->Fit(flandau,"R");
  mean = flandau->GetParameter(1);
  er_mean = flandau->GetParError(1);
  er_y[6] = er_mean;
  l.DrawLatex(0.5,0.8,Form("mean = %.1f #pm %.1f",mean,er_mean));
  //  fgumbel->Draw("same");
  c1->Print("Histograms/Cosmics_All_Hists_June.pdf");

  //////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////// Plotting Means Together /////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////

  c2->cd();
  int n = 7;
  TGraphErrors* Er_Means = new TGraphErrors(n,x,y,er_x,er_y);

  Means->SetPoint(1,1,38.0);
  Means->SetPoint(2,2,38.1);
  Means->SetPoint(3,3,46.4);
  Means->SetPoint(4,4,38.6);
  Means->SetPoint(5,5,33.6);
  Means->SetPoint(6,6,34.6);
  Means->SetPoint(7,7,33.9);
  Means->SetTitle("<Cosmic> (PE) ");
  Means->GetYaxis()->SetTitle("Mean in PE (5mV/PE)");
  Means->GetXaxis()->SetTitle("Panel");
  Means->GetYaxis()->CenterTitle(true);
  Means->SetMarkerStyle(kFullCircle);
  Means->SetMinimum(30);
  Means->SetMaximum(50);

  Means->Draw("ap");
  c2->Print("Histograms/Cosmics_Means_All_June.pdf");
  Er_Means->SetMarkerStyle(kFullCircle);
  Er_Means->SetMarkerColor(kBlue);
  Er_Means->SetTitle("<Cosmic> (PE) ");
  Er_Means->GetYaxis()->SetTitle("Mean in PE (5mV/PE)");
  Er_Means->GetXaxis()->SetNdivisions(14);
  Er_Means->GetYaxis()->CenterTitle(true);
  Er_Means->Draw("AP");
  c2->Print("Histograms/Cosmics_Error_Means_All_June.pdf");

  c3->cd();
  OldMeans->SetPoint(1,1,8.6);
  OldMeans->SetPoint(2,2,8.1);
  OldMeans->SetPoint(3,3,11.0);
  OldMeans->SetPoint(4,4,6.5);
  OldMeans->SetPoint(5,5,7.8);
  OldMeans->SetPoint(6,6,8.9);
  OldMeans->SetTitle("<Cosmic> (PE) ");
  OldMeans->GetYaxis()->SetTitle("Mean in PE (?? mV/PE)");
  OldMeans->GetXaxis()->SetNdivisions(7);
  OldMeans->GetYaxis()->CenterTitle(true);
  OldMeans->SetMarkerStyle(kFullCircle);
  OldMeans->SetMinimum(5);
  OldMeans->SetMaximum(15);

  OldMeans->Draw("ap");
    
}
