void DrawAsymmetry()
{

  // start("20150826-1505_A4_LED_PanelAir");
  // start("20150827-0946_A4_LED_PanelAir");

  start("20150831-1730_A1_Source");
  start("20150901-0950_A1_Source");
  // start("20150901-1748_A1_LED");
  // start("20150902-1005_A1_LED");

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
  leg->SetFillStyle(0);
  leg->Draw();

  // generalize
  c1->Print(Form("Figures/ATOGETHER_%s_p%d.png",basename,projnumb));
  c1->Print(Form("Figures/ATOGETHER_%s_p%d.pdf",basename,projnumb));

  ofstream fout(Form("Data/Text/Asymmetry/%s_asymmetry_p%d.txt",basename,projnumb));
  TH1D *hp5_asymm = (TH1D *)hp5_1->Clone();
  float x[58];
  float y[58];
  float y1[58];
  float y2[58];
  TH1D *h1 = new TH1D("h1","",50,0,25);
  TH1D *h2 = new TH1D("h2","",50,0,25);
  for(int i=0; i<nbins; i++)
    {
      double A = hp5_1->GetBinContent(i+1);
      double B = hp5_2->GetBinContent(i+1);
      double content = (B-A)/(B+A);
      hp5_asymm->SetBinContent(i+1,content);
      hp5_asymm->SetBinError(i+1,0);
      // ---
      x[i] = hp5_1->GetBinCenter(i+1);
      y[i] = content;
      y1[i] = A;
      y2[i] = B;
      x[i] -= 2.0; // offset for off-panel part of scan, needs manual adjustment
      if(i>3&&i<54)
	{
	  // offset for off-panel, needs manual adjustment
	  h1->SetBinContent(i-3,A);
	  h2->SetBinContent(i-3,B);
	  h1->SetBinError(i-3,0);
	  h2->SetBinError(i-3,0);
	}
      // ---
      fout<<hp5_asymm->GetBinCenter(i+1)<<" "
	  <<hp5_asymm->GetBinContent(i+1)<<endl;
      // --- picking which points to skip needs to be evaluated on a case-by-case basis
      // if(i<2||i>(nbins-3)) hp5_asymm->SetBinContent(i+1,-9); // skip the first 2 and last 2
      // if(i<2||i>(nbins-3)) hp5_asymm->SetBinContent(i+1,-9); // skip the first 2 and last 2
      // if(i<3||i>(nbins-4)) hp5_asymm->SetBinContent(i+1,-9); // skip the first 2 and last 2
      // if(i<3||i>(nbins-4)) hp5_asymm->SetBinContent(i+1,-9); // skip the first 2 and last 2
      // if(i<5||i>(nbins-6)) hp5_asymm->SetBinContent(i+1,-9); // skip the first 5 and last 5
      // if(i<5||i>(nbins-6)) hp5_asymm->SetBinContent(i+1,-9); // skip the first 5 and last 5
    }
  fout.close();

  // hp5_asymm->Rebin(2);
  // hp5_asymm->Scale(0.5); // rebinning a th1 requires scaling
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

  // ---

  h1->SetLineColor(kBlue);
  h2->SetLineColor(kRed);
  h1->SetLineWidth(2);
  h2->SetLineWidth(2);
  h1->SetMinimum(0);
  h2->SetMinimum(0);

  h1->GetXaxis()->SetTitle("distance (cm)");
  h2->GetXaxis()->SetTitle("distance (cm)");
  h1->GetYaxis()->SetTitle("Number of photoelectrons");
  h2->GetYaxis()->SetTitle("Number of photoelectrons");

  TGraph *tgy1 = new TGraph(58,x,y1);
  TGraph *tgy2 = new TGraph(58,x,y2);

  tgy1->SetLineColor(kBlue);
  tgy2->SetLineColor(kRed);
  tgy1->SetLineWidth(2);
  tgy2->SetLineWidth(2);
  tgy1->SetMinimum(0);
  tgy2->SetMinimum(0);

  tgy1->GetXaxis()->SetTitle("distance (cm)");
  tgy2->GetXaxis()->SetTitle("distance (cm)");
  tgy1->GetYaxis()->SetTitle("Number of photoelectrons");
  tgy2->GetYaxis()->SetTitle("Number of photoelectrons");

  // TF1 *funx1 = new TF1("funx1","pol0(0)+expo(1)",0,25);
  // funx1->SetLineColor(kBlue);
  // tgy1->Fit(funx1,"R");
  // TF1 *funx2 = new TF1("funx2","pol0(0)+expo(1)",0,25);
  // funx2->SetLineColor(kRed);
  // tgy2->Fit(funx2,"R");

  c1->Clear();

  float maxx = 0;
  if(max1>=max2)
    {
      // case 1
      maxx = max1;
      h1->Draw();
      h2->Draw("same");
    }
  else
    {
      // case 2
      maxx = max2;
      h2->Draw();
      h1->Draw("same");
    }

  TF1 *funx1 = new TF1("funx1","[0]+[1]*TMath::Exp(-x/[2])",0,25);
  funx1->SetLineColor(kBlue);
  funx1->SetParameter(0,5.0);
  funx1->SetParameter(1,5.0);
  funx1->FixParameter(2,5.0);
  tgy1->Fit(funx1,"","",1,24);
  TF1 *funx2 = new TF1("funx2","[0]+[1]*TMath::Exp((x-25)/[2])",0,25);
  funx2->SetLineColor(kRed);
  funx2->SetParameter(0,5.0);
  funx2->SetParameter(1,5.0);
  funx2->FixParameter(2,5.0);
  tgy2->Fit(funx2,"","",1,24);

  // tgy1->Draw("same");
  // tgy2->Draw("same");
  funx1->Draw("same");
  funx2->Draw("same");

  float numcore1 = funx1->GetParameter(0);
  float numclad1 = funx1->GetParameter(1);
  float numcore2 = funx2->GetParameter(0);
  float numclad2 = funx2->GetParameter(1);
  float Enumcore1 = funx1->GetParError(0);
  float Enumclad1 = funx1->GetParError(1);
  float Enumcore2 = funx2->GetParError(0);
  float Enumclad2 = funx2->GetParError(1);

  float fracore1 = (numcore1)/(numcore1+numclad1);
  float fracore2 = (numcore2)/(numcore2+numclad2);
  // ---
  float partB;
  partB = sqrt(Enumcore1**2+Enumclad2**2);
  float Efracore1 = fracore1*sqrt((Enumcore1/numcore1)**2+(partB/(numcore1+numclad1))**2);
  partB = sqrt(Enumcore2**2+Enumclad2**2);
  float Efracore2 = fracore2*sqrt((Enumcore2/numcore2)**2+(partB/(numcore2+numclad2))**2);

  TLatex *texAC1 = new TLatex(2,0.27*maxx,Form("f_{core} = %.3f #pm %.3f",fracore1,Efracore1));
  texAC1->SetTextColor(kBlue);
  texAC1->Draw();
  TLatex *texAC2 = new TLatex(15,0.27*maxx,Form("f_{core} = %.3f #pm %.3f",fracore2,Efracore2));
  texAC2->SetTextColor(kRed);
  texAC2->Draw();
  // ---
  TLatex *texAA1 = new TLatex(2,0.18*maxx,Form("N_{core} = %.3f #pm %.3f",numcore1,Enumcore1));
  texAA1->SetTextColor(kBlue);
  texAA1->Draw();
  TLatex *texAA2 = new TLatex(15,0.18*maxx,Form("N_{core} = %.3f #pm %.3f",numcore2,Enumcore2));
  texAA2->SetTextColor(kRed);
  texAA2->Draw();
  TLatex *texAB1 = new TLatex(2,0.1*maxx,Form("N_{clad} = %.3f #pm %.3f",numclad1,Enumclad1));
  texAB1->SetTextColor(kBlue);
  texAB1->Draw();
  TLatex *texAB2 = new TLatex(15,0.1*maxx,Form("N_{clad} = %.3f #pm %.3f",numclad2,Enumclad2));
  texAB2->SetTextColor(kRed);
  texAB2->Draw();


  // ---

  c1->Print(Form("Figures/FITATOGETHER_%s_p%d.png",basename,projnumb));
  c1->Print(Form("Figures/FITATOGETHER_%s_p%d.pdf",basename,projnumb));

  // ---

  // define a function to describe the data
  // this function defines and asymmetry B-A/A+B
  // it assumes the light yield has two components, core and cladding
  TF1 *fun = new TF1("fun","(([0]*TMath::Exp((x-25)/[1])+(1-[0])*TMath::Exp((x-25)/[2]))-([0]*TMath::Exp(-x/[1])+(1-[0])*TMath::Exp(-x/[2])))/(([0]*TMath::Exp(-x/[1])+(1-[0])*TMath::Exp(-x/[2]))+([0]*TMath::Exp((x-25)/[1])+(1-[0])*TMath::Exp((x-25)/[2])))",0,29);
  fun->SetParameter(0,0.5); // light fraction in fiber core
  fun->FixParameter(1,350); // decay constant in fiber core
  fun->FixParameter(2,5); // decay constant in fiber cladding
  TF1 *fun2 = new TF1("fun2","[3]+(([0]*TMath::Exp((x-25)/[1])+(1-[0])*TMath::Exp((x-25)/[2]))-([0]*TMath::Exp(-x/[1])+(1-[0])*TMath::Exp(-x/[2])))/(([0]*TMath::Exp(-x/[1])+(1-[0])*TMath::Exp(-x/[2]))+([0]*TMath::Exp((x-25)/[1])+(1-[0])*TMath::Exp((x-25)/[2])))",0,29);
  fun2->SetParameter(0,0.5); // light fraction in fiber core
  fun2->FixParameter(1,350); // decay constant in fiber core
  fun2->FixParameter(2,5); // decay constant in fiber cladding
  fun2->SetParameter(3,0.01);

  // fit the data
  //hp5_asymm->Fit("fun","","",4,25);
  //fun->Draw("same");
  //cout<<"entries "<<hp5_asymm->GetEntries()<<endl;

  //nbins = hp5_asymm->GetNbinsX();
  // float *x = new float[nbins];
  // float *y = new float[nbins];
  cout<<"x "<<x[15]<<" y "<<y[15]<<endl;
  //TGraph *tg = new TGraph(nbins,x,y);
  TGraph *tg = new TGraph(58,x,y);
  tg->SetMarkerStyle(kFullCircle);
  tg->Draw("ap");
  tg->GetYaxis()->SetTitle("Asymmetry of light yields");
  tg->GetXaxis()->SetTitle("Panel position (cm)");
  tg->GetXaxis()->SetLimits(0,25);
  float scale = 0.4;
  tg->SetMinimum(-1*scale);
  tg->SetMaximum(1*scale);
  tg->Fit("fun2","","",0,25);
  TLine line2(0.0,0.0,25.0,0.0);
  line2.SetLineStyle(2);
  line2.SetLineWidth(2);
  line2.Draw();

  // get the fit parameters
  double frac = fun2->GetParameter(0);
  double core = fun2->GetParameter(1);
  double clad = fun2->GetParameter(2);
  double Efrac = fun2->GetParError(0);
  double off = fun2->GetParameter(3);
  double Eoff = fun2->GetParError(3);

  // use the fit parameters to put text boxes with fit information on the plt
  TLatex *tex1 = new TLatex(15,-0.45*scale,Form("f_{core} = %.3f #pm %.3f",frac,Efrac));
  tex1->SetTextColor(kRed);
  tex1->Draw();
  TLatex *tex2 = new TLatex(15,-0.60*scale,Form("#lambda_{core} = %.1f (FIXED)",core));
  tex2->SetTextColor(kBlue);
  tex2->Draw();
  TLatex *tex3 = new TLatex(15,-0.75*scale,Form("#lambda_{clad} = %.1f (FIXED)",clad));
  tex3->SetTextColor(kBlue);
  tex3->Draw();
  TLatex *texX = new TLatex(5,-0.75*scale,Form("offset = %.3f #pm %.3f",off,Eoff));
  texX->SetTextColor(kGreen+2);
  texX->Draw();

  // generalize
  c1->Print(Form("Figures/FITASYMMETRY_%s_p%d.png",basename,projnumb));
  c1->Print(Form("Figures/FITASYMMETRY_%s_p%d.pdf",basename,projnumb));

  delete h1;
  delete h2;

}
