void DrawAsymmetry()
{

  // start("20150826-1505_A4_LED_PanelAir");
  // start("20150827-0946_A4_LED_PanelAir");

  start("20150831-1730_A1_Source");
  start("20150901-0950_A1_Source");

}



void start(const char *basename)
{

  TFile *file1 = TFile::Open(Form("Data/ROOT/%s_VMIN_SIPM1_meanHistSub_projections.root",basename));
  TFile *file2 = TFile::Open(Form("Data/ROOT/%s_VMIN_SIPM2_meanHistSub_projections.root",basename));

  if(file1==NULL) {cout<<"cannot find file1"<<endl;return;}
  if(file2==NULL) {cout<<"cannot find file2"<<endl;return;}

  calc(file1,file2,basename,0);
  calc(file1,file2,basename,1);
  calc(file1,file2,basename,2);
  calc(file1,file2,basename,3);
  calc(file1,file2,basename,4);
  calc(file1,file2,basename,5);
  calc(file1,file2,basename,6);
  calc(file1,file2,basename,7);
  calc(file1,file2,basename,8);

}



void calc(TFile *file1, TFile *file2, const char* basename, int projnumb)
{

  TH1D *hp5_1 = (TH1D *)file1->Get(Form("%s_VMIN_SIPM1_meanHistSub_hproj_%d",basename,projnumb));
  TH1D *hp5_2 = (TH1D *)file2->Get(Form("%s_VMIN_SIPM2_meanHistSub_hproj_%d",basename,projnumb));

  if(hp5_1==NULL) {cout<<"cannot find hp5_1"<<endl; return;}
  if(hp5_2==NULL) {cout<<"cannot find hp5_2"<<endl; return;}

  int nbins = hp5_1->GetNbinsX();

  // ----------------------------------------------------------------------------
  // --- one of the preamps was broken, so we only had measurements from 1 SiPM
  // --- I made this fake data part to help in the code development while waiting
  // --- for new data with both SiPMs to be available
  // ----------------------------------------------------------------------------
  bool makefakedata = false;
  if(makefakedata)
    {
      for(int i=0; i<nbins; i++)
	{
	  hp5_2->SetBinContent(i+1,hp5_1->GetBinContent(nbins-i));
	}
    }

  double max1 = hp5_1->GetMaximum();
  double max2 = hp5_2->GetMaximum();

  hp5_1->GetXaxis()->SetTitle("distance (cm)");
  hp5_2->GetXaxis()->SetTitle("distance (cm)");
  hp5_1->GetYaxis()->SetTitle("Number of photoelectrons");
  hp5_2->GetYaxis()->SetTitle("Number of photoelectrons");

  if(max1>=max2)
    {
      // case 1
      hp5_1->Draw();
      hp5_2->Draw("same");
    }
  else
    {
      // case 2
      hp5_2->Draw();
      hp5_1->Draw("same");
    }

  hp5_1->SetLineColor(kBlue);
  hp5_2->SetLineColor(kRed);
  hp5_1->SetLineWidth(2);
  hp5_2->SetLineWidth(2);

  // legend coordinates may occasionally need to be relocated
  TLegend *leg = new TLegend(0.4,0.2,0.6,0.4);
  leg->AddEntry(hp5_1,"SiPM1","l");
  leg->AddEntry(hp5_2,"SiPM2","l");
  leg->Draw();

  // generalize
  c1->Print(Form("Figures/ATOGETHER_%s_p%d.png",basename,projnumb));
  c1->Print(Form("Figures/ATOGETHER_%s_p%d.pdf",basename,projnumb));

  ofstream fout(Form("Data/Text/Asymmetry/%s_asymmetry_p%d.txt",basename,projnumb));
  TH1D *hp5_asymm = (TH1D *)hp5_1->Clone();
  for(int i=0; i<nbins; i++)
    {
      double A = hp5_1->GetBinContent(i+1);
      double B = hp5_2->GetBinContent(i+1);
      double content = (B-A)/(B+A);
      hp5_asymm->SetBinContent(i+1,content);
      hp5_asymm->SetBinError(i+1,0);
      fout<<hp5_asymm->GetBinCenter(i+1)<<" "
	  <<hp5_asymm->GetBinContent(i+1)<<endl;
      // --- picking which points to skip needs to be evaluated on a case-by-case basis
      // if(i<2||i>(nbins-3)) hp5_asymm->SetBinContent(i+1,-9); // skip the first 2 and last 2
      // if(i<2||i>(nbins-3)) hp5_asymm->SetBinContent(i+1,-9); // skip the first 2 and last 2
      if(i<3||i>(nbins-4)) hp5_asymm->SetBinContent(i+1,-9); // skip the first 2 and last 2
      if(i<3||i>(nbins-4)) hp5_asymm->SetBinContent(i+1,-9); // skip the first 2 and last 2
      // if(i<5||i>(nbins-6)) hp5_asymm->SetBinContent(i+1,-9); // skip the first 5 and last 5
      // if(i<5||i>(nbins-6)) hp5_asymm->SetBinContent(i+1,-9); // skip the first 5 and last 5
    }
  fout.close();

  hp5_asymm->Rebin(2);
  hp5_asymm->Scale(0.5); // rebinning a th1 requires scaling
  hp5_asymm->SetMarkerColor(kBlack);
  hp5_asymm->SetMarkerStyle(kFullCircle);
  hp5_asymm->Draw("ex0p");
  hp5_asymm->GetYaxis()->SetTitle("Asymmetry of light yields");
  hp5_asymm->SetMinimum(-1);
  hp5_asymm->SetMaximum(1);
  TLine line(0.0,0.0,29.0,0.0);
  line.SetLineStyle(2);
  line.SetLineWidth(2);
  line.Draw();

  // generalize
  c1->Print(Form("Figures/ASYMMETRY_%s_p%d.png",basename,projnumb));
  c1->Print(Form("Figures/ASYMMETRY_%s_p%d.pdf",basename,projnumb));

}
