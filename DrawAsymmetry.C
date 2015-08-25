void DrawAsymmetry()
{

  // start("A1_LED_ColTests_Fiber1_VMIN_SIPM1_meanHistSub","A1_LED_ColTests_Fiber1_VMIN_SIPM2_meanHistSub");
  // start("A1_LED_ColTests_Fiber2_VMIN_SIPM1_meanHistSub","A1_LED_ColTests_Fiber2_VMIN_SIPM2_meanHistSub");
  // start("A1_LED_ColTests_Fiber3_VMIN_SIPM1_meanHistSub","A1_LED_ColTests_Fiber3_VMIN_SIPM2_meanHistSub");
  // start("A1_LED_ColTests_Fiber4_VMIN_SIPM1_meanHistSub","A1_LED_ColTests_Fiber4_VMIN_SIPM2_meanHistSub");

  // start("A3_LED_Assymetry_VMin_SiPM1_meanHistSub","A3_LED_Assymetry_VMin_SiPM2_meanHistSub");

  // start("A3_LED_Assymetry_PannelBlock_VMin_SiPM1_meanHistSub","A3_LED_Assymetry_PannelBlock_VMin_SiPM2_meanHistSub");

  start("A3_LED_Assymetry_CladBlock_Recouple_VMin_SiPM1_meanHistSub"      ,"A3_LED_Assymetry_CladBlock_Recouple_VMin_SiPM2_meanHistSub"      );
  start("A3_LED_Assymetry_NoBlock_Recouple_VMin_SiPM1_meanHistSub"        ,"A3_LED_Assymetry_NoBlock_Recouple_VMin_SiPM2_meanHistSub"        );
  start("A3_LED_Assymetry_PannelAirBlock_Recouple_VMin_SiPM1_meanHistSub" ,"A3_LED_Assymetry_PannelAirBlock_Recouple_VMin_SiPM2_meanHistSub" );
  start("A3_LED_Assymetry_PannelBlockOnly_Recouple_VMin_SiPM1_meanHistSub","A3_LED_Assymetry_PannelBlockOnly_Recouple_VMin_SiPM2_meanHistSub");

}



void start(const char* name1, const char* name2)
{

  TFile *file1 = TFile::Open(Form("Data/ROOT/%s_projections.root",name1));
  TFile *file2 = TFile::Open(Form("Data/ROOT/%s_projections.root",name2));

  if(file1==NULL) {cout<<"cannot find file1"<<endl;return;}
  if(file2==NULL) {cout<<"cannot find file2"<<endl;return;}

  calc(file1,file2,name1,name2,0);
  calc(file1,file2,name1,name2,1);
  calc(file1,file2,name1,name2,2);
  calc(file1,file2,name1,name2,3);
  calc(file1,file2,name1,name2,4);
  calc(file1,file2,name1,name2,5);
  calc(file1,file2,name1,name2,6);
  calc(file1,file2,name1,name2,7);
  calc(file1,file2,name1,name2,8);

}



void calc(TFile *file1, TFile *file2, const char* name1, const char* name2, int projnumb)
{

  // generalize
  TH1D *hp5_1 = (TH1D *)file1->Get(Form("%s_hproj_%d",name1,projnumb));
  TH1D *hp5_2 = (TH1D *)file2->Get(Form("%s_hproj_%d",name2,projnumb));

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

  // double max1 = hp5_1->GetBinContent(hp5_1->GetMaxBin());
  // double max2 = hp5_2->GetBinContent(hp5_2->GetMaxBin());

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

  // ---------

  // need to adjust on case by case basis, unfortunately
  char name3[200];
  strcpy(name3,name1);
  name3[30] = '\0'; // hackish way of truncating string

  // generalize
  c1->Print(Form("Figures/%s_together_p%d.png",name3,projnumb));
  c1->Print(Form("Figures/%s_together_p%d.pdf",name3,projnumb));

  TH1D *hp5_asymm = (TH1D *)hp5_1->Clone();
  for(int i=0; i<nbins; i++)
    {
      double A = hp5_1->GetBinContent(i+1);
      double B = hp5_2->GetBinContent(i+1);
      double content = (B-A)/(B+A);
      hp5_asymm->SetBinContent(i+1,content);
      hp5_asymm->SetBinError(i+1,0);
      //if(i<5||i>(nbins-6)) hp5_asymm->SetBinContent(i+1,-9); // skip the first 5 and last 5
      //if(i<5||i>(nbins-6)) hp5_asymm->SetBinContent(i+1,-9); // skip the first 5 and last 5
    }

  hp5_asymm->Rebin(2);
  hp5_asymm->Scale(0.5); // rebinning a th1 requires scaling
  hp5_asymm->SetMarkerColor(kBlack);
  hp5_asymm->SetMarkerStyle(kFullCircle);
  hp5_asymm->Draw("ex0p");
  hp5_asymm->GetYaxis()->SetTitle("Asymmetry of light yields");
  hp5_asymm->SetMinimum(-1);
  hp5_asymm->SetMaximum(1);

  // generalize
  c1->Print(Form("Figures/%s_asymmetry_p%d.png",name3,projnumb));
  c1->Print(Form("Figures/%s_asymmetry_p%d.pdf",name3,projnumb));

}
