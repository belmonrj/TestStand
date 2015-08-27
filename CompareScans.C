gStyle->SetOptTitle(1); // good times...

void CompareScans()
{

  dofix2();

  //dofix3();

}



void dofix3()
{

  // fixednumber_3
  //   (
  //    "A4_LED_Assymetry_PannelBlockOnly_Recouple_VMin_SiPM1",
  //    "20150824-1701_A4_LED_Panel_VMIN_SIPM1",
  //    "20150825-1733_A4_LED_Panel_VMIN_SIPM1",
  //    "Sebastian",
  //    "Ron 8/24",
  //    "Ron 8/25 (late)",
  //    "Comparison of scans with panel blocked",
  //    "panel blocked",
  //    "A4_LED_3Comp_Panel_SiPM1"
  //    );

  // fixednumber_3
  //   (
  //    "A4_LED_Assymetry_PannelBlockOnly_Recouple_VMin_SiPM2",
  //    "20150824-1701_A4_LED_Panel_VMIN_SIPM2",
  //    "20150825-1733_A4_LED_Panel_VMIN_SIPM2",
  //    "Sebastian",
  //    "Ron 8/24",
  //    "Ron 8/25 (late)",
  //    "Comparison of scans with panel blocked",
  //    "panel blocked",
  //    "A4_LED_3Comp_Panel_SiPM2"
  //    );


  // fixednumber_3
  //   (
  //    //"A4_LED_Assymetry_NoBlock_Recouple_VMin_SiPM1", // obvious need for 4...
  //    //"20150824-1750_A4_LED_NoBlock_VMIN_SIPM1",
  //    "20150825-1133_A4_LED_NoBlock_VMIN_SIPM1",
  //    "20150825-1622_A4_LED_NoBlock_VMIN_SIPM1",
  //    "20150826-0945_A4_LED_NoBlock_VMIN_SIPM1",
  //    //"Sebastian",
  //    //"Ron 8/24",
  //    "Ron 8/25 (early)",
  //    "Ron 8/25 (late)",
  //    "Ron 8/26",
  //    "Comparison of scans with no blocking",
  //    "no blocking",
  //    "A4_LED_3Comp_NoBlock_SiPM1"
  //    );

  // fixednumber_3
  //   (
  //    //"A4_LED_Assymetry_NoBlock_Recouple_VMin_SiPM2", // obvious need for 4...
  //    //"20150824-1750_A4_LED_NoBlock_VMIN_SIPM2",
  //    "20150825-1133_A4_LED_NoBlock_VMIN_SIPM2",
  //    "20150825-1622_A4_LED_NoBlock_VMIN_SIPM2",
  //    "20150826-0945_A4_LED_NoBlock_VMIN_SIPM2",
  //    //"Sebastian",
  //    //"Ron 8/24",
  //    "Ron 8/25 (early)",
  //    "Ron 8/25 (late)",
  //    "Ron 8/26",
  //    "Comparison of scans with no blocking",
  //    "no blocking",
  //    "A4_LED_3Comp_NoBlock_SiPM2"
  //    );

  // ---

  fixednumber_3
    (
     "20150824-1701_A4_LED_Panel_VMIN_SIPM1",
     "20150825-1733_A4_LED_Panel_VMIN_SIPM1",
     "20150826-1055_A4_LED_Panel_VMIN_SIPM1",
     "Ron 8/24",
     "Ron 8/25 (late)",
     "Ron 8/26",
     "Comparison of scans with panel blocked",
     "no blocking",
     "A4_LED_3Comp_Panel_SiPM1"
     );

  fixednumber_3
    (
     "20150824-1701_A4_LED_Panel_VMIN_SIPM2",
     "20150825-1733_A4_LED_Panel_VMIN_SIPM2",
     "20150826-1055_A4_LED_Panel_VMIN_SIPM2",
     "Ron 8/24",
     "Ron 8/25 (late)",
     "Ron 8/26",
     "Comparison of scans with panel blocked",
     "no blocking",
     "A4_LED_3Comp_Panel_SiPM2"
     );

  // ---

  fixednumber_3
    (
     "20150825-1422_A4_LED_PanelAirClad_VMIN_SIPM1",
     "20150825-1515_A4_LED_PanelAirClad_VMIN_SIPM1",
     "20150826-1334_A4_LED_PanelAirClad_VMIN_SIPM1",
     "Ron 8/25 (early)",
     "Ron 8/25 (late)",
     "Ron 8/26",
     "Comparison of scans with panel, air, and clad blocked",
     "no blocking",
     "A4_LED_3Comp_LatePanelAirClad_SiPM2"
     );

}



void fixednumber_3(char *name1, char *name2, char *name3, char *short1, char * short2, char *short3, char *longdescription, char *shortdescription, char *outname)
{

  // --- get the files
  TFile *file1 = TFile::Open(Form("Data/ROOT/%s_meanHistSub_projections.root",name1));
  TFile *file2 = TFile::Open(Form("Data/ROOT/%s_meanHistSub_projections.root",name2));
  TFile *file3 = TFile::Open(Form("Data/ROOT/%s_meanHistSub_projections.root",name3));

  // --- check that the files exist
  if(file1==NULL) {cout<<"Cannot find file "<<Form("%s_meanHistSub_projections.root",name1)<<endl;return;}
  if(file2==NULL) {cout<<"Cannot find file "<<Form("%s_meanHistSub_projections.root",name2)<<endl;return;}
  if(file3==NULL) {cout<<"Cannot find file "<<Form("%s_meanHistSub_projections.root",name3)<<endl;return;}

  // --- not sure if it's possible to generalize, since the 2d histo is in a different file
  for(int i=0; i<9; i++)
    {
      // --- get the histograms
      TH1D *h1 = (TH1D *)file1->Get(Form("%s_meanHistSub_hproj_%d",name1,i));
      TH1D *h2 = (TH1D *)file2->Get(Form("%s_meanHistSub_hproj_%d",name2,i));
      TH1D *h3 = (TH1D *)file3->Get(Form("%s_meanHistSub_hproj_%d",name3,i));

      // --- set draw properties, very simple for now
      h1->SetLineColor(kBlack);
      h2->SetLineColor(kRed);
      h3->SetLineColor(kBlue);
      h1->SetLineWidth(2);
      h2->SetLineWidth(2);
      h3->SetLineWidth(2);

      // --- get the maxima
      double max1 = h1->GetMaximum();
      double max2 = h2->GetMaximum();
      double max3 = h3->GetMaximum();

      // --- crude and hackish way to sort the maxima
      double max = 0;
      if(max1>max) max = max1;
      if(max2>max) max = max2;
      if(max3>max) max = max3;

      // --- draw the largest histogram first to prevent scale issues
      if(max==max1)
	{
	  h1->SetMinimum(0.1);
	  h1->SetTitle(longdescription);
	  h1->GetXaxis()->SetTitle("Distance (cm)");
	  h1->GetYaxis()->SetTitle("Number of photoelectrons");
	  h1->Draw();
	  h2->Draw("same");
	  h3->Draw("same");
	}
      else if(max==max2)
	{
	  h2->SetMinimum(0.1);
	  h2->GetXaxis()->SetTitle("Distance (cm)");
	  h2->GetYaxis()->SetTitle("Number of photoelectrons");
	  h2->Draw();
	  h1->Draw("same");
	  h3->Draw("same");
	}
      else if(max==max3)
	{
	  h3->SetMinimum(0.1);
	  h3->GetXaxis()->SetTitle("Distance (cm)");
	  h3->GetYaxis()->SetTitle("Number of photoelectrons");
	  h3->Draw();
	  h1->Draw("same");
	  h2->Draw("same");
	}
      else {cout<<"no maximum found???"<<endl;}

      double xmax = 0;
      if(max == max1) xmax = h1->GetBinCenter(h1->GetMaximumBin());
      if(max == max2) xmax = h2->GetBinCenter(h2->GetMaximumBin());
      if(max == max3) xmax = h3->GetBinCenter(h3->GetMaximumBin());

      // --- unfortunately the legend has to be controlled completely manually
      // --- there's no way to automate it since it will always vary on a case by case basis
      TLegend *leg;
      if(xmax<15) leg = new TLegend(0.58,0.73,0.88,0.88);
      else leg = new TLegend(0.18,0.73,0.48,0.88);
      leg->AddEntry(h1,short1,"l");
      leg->AddEntry(h2,short2,"l");
      leg->AddEntry(h3,short3,"l");
      leg->SetTextSize(0.04);
      leg->Draw();

      // --- similarly there's no good way to generalize the figure name
      // c1->Print(Form("Figures/%s_scancomp_proj_%d.png",outname,i));
      // c1->Print(Form("Figures/%s_scancomp_proj_%d.pdf",outname,i));
      c1->Print(Form("Figures/SCANCOMP_%s_proj_%d.png",outname,i));
      c1->Print(Form("Figures/SCANCOMP_%s_proj_%d.pdf",outname,i));

    }

}



void dofix2()
{

  // fixednumber_2
  //   (
  //    "20150824-1701_A4_LED_Panel_VMIN_SIPM1",
  //    "20150825-1733_A4_LED_Panel_VMIN_SIPM1",
  //    "Ron 8/24",
  //    "Ron 8/25 (late)",
  //    "Comparison of scans with panel blocked",
  //    "panel blocked",
  //    "A4_LED_Comp_Panel_SiPM1"
  //    );

  // fixednumber_2
  //   (
  //    "20150824-1701_A4_LED_Panel_VMIN_SIPM2",
  //    "20150825-1733_A4_LED_Panel_VMIN_SIPM2",
  //    "Ron 8/24",
  //    "Ron 8/25 (late)",
  //    "Comparison of scans with panel blocked",
  //    "panel blocked",
  //    "A4_LED_Comp_Panel_SiPM2"
  //    );

  // // ---

  // fixednumber_2
  //   (
  //    "20150824-1552_A4_LED_PanelAirClad_VMIN_SIPM1",
  //    "20150825-1422_A4_LED_PanelAirClad_VMIN_SIPM1",
  //    "Ron 8/24",
  //    "Ron 8/25 (early)",
  //    "Comparison of scans with panel blocked",
  //    "panel blocked",
  //    "A4_LED_Comp_EarlyPanelAirClad_SiPM1"
  //    );

  // fixednumber_2
  //   (
  //    "20150825-1515_A4_LED_PanelAirClad_VMIN_SIPM1",
  //    "20150826-1334_A4_LED_PanelAirClad_VMIN_SIPM1",
  //    "Ron 8/25 (late)",
  //    "Ron 8/26",
  //    "Comparison of scans with panel blocked",
  //    "panel blocked",
  //    "A4_LED_Comp_LatePanelAirClad_SiPM1"
  //    );

  // fixednumber_2
  //   (
  //    "20150824-1552_A4_LED_PanelAirClad_VMIN_SIPM1",
  //    "20150826-1334_A4_LED_PanelAirClad_VMIN_SIPM1",
  //    "Ron 8/24",
  //    "Ron 8/26",
  //    "Comparison of scans with panel blocked",
  //    "panel blocked",
  //    "A4_LED_Comp_MixedWidePanelAirClad_SiPM1"
  //    );

  // fixednumber_2
  //   (
  //    "20150825-1422_A4_LED_PanelAirClad_VMIN_SIPM1",
  //    "20150826-1334_A4_LED_PanelAirClad_VMIN_SIPM1",
  //    "Ron 8/25 (early)",
  //    "Ron 8/26",
  //    "Comparison of scans with panel blocked",
  //    "panel blocked",
  //    "A4_LED_Comp_MixedNarrowPanelAirClad_SiPM1"
  //    );

  // ---

  fixednumber_2
    (
     "20150826-1505_A4_LED_PanelAir_VMIN_SIPM1",
     "20150827-0946_A4_LED_PanelAir_VMIN_SIPM1",
     "Ron 8/26",
     "Ron 8/27",
     "Comparison of scans with panel and air blocked",
     "panel and air blocked",
     "A4_LED_Comp_PanelAir_SiPM1"
     );

  fixednumber_2
    (
     "20150826-1505_A4_LED_PanelAir_VMIN_SIPM2",
     "20150827-0946_A4_LED_PanelAir_VMIN_SIPM2",
     "Ron 8/26",
     "Ron 8/27",
     "Comparison of scans with panel and air blocked",
     "panel and air blocked",
     "A4_LED_Comp_PanelAir_SiPM2"
     );



}



void fixednumber_2(char *name1, char *name2, char *short1, char * short2, char *longdescription, char *shortdescription, char *outname)
{

  // --- get the files
  TFile *file1 = TFile::Open(Form("Data/ROOT/%s_meanHistSub_projections.root",name1));
  TFile *file2 = TFile::Open(Form("Data/ROOT/%s_meanHistSub_projections.root",name2));

  // --- check that the files exist
  if(file1==NULL) {cout<<"Cannot find file "<<Form("%s_meanHistSub_projections.root",name1)<<endl;return;}
  if(file2==NULL) {cout<<"Cannot find file "<<Form("%s_meanHistSub_projections.root",name2)<<endl;return;}

  // --- not sure if it's possible to generalize, since the 2d histo is in a different file
  for(int i=0; i<9; i++)
    {
      // --- get the histograms
      TH1D *h1 = (TH1D *)file1->Get(Form("%s_meanHistSub_hproj_%d",name1,i));
      TH1D *h2 = (TH1D *)file2->Get(Form("%s_meanHistSub_hproj_%d",name2,i));

      // --- set draw properties, very simple for now
      h1->SetLineColor(kBlack);
      h2->SetLineColor(kRed);
      h1->SetLineWidth(2);
      h2->SetLineWidth(2);

      // --- get the maxima
      double max1 = h1->GetMaximum();
      double max2 = h2->GetMaximum();

      // --- crude and hackish way to sort the maxima
      double max = 0;
      if(max1>max) max = max1;
      if(max2>max) max = max2;

      // --- draw the largest histogram first to prevent scale issues
      if(max==max1)
	{
	  h1->SetMinimum(0.1);
	  h1->SetTitle(longdescription);
	  h1->GetXaxis()->SetTitle("Distance (cm)");
	  h1->GetYaxis()->SetTitle("Number of photoelectrons");
	  h1->Draw();
	  h2->Draw("same");
	}
      else if(max==max2)
	{
	  h2->SetMinimum(0.1);
	  h2->GetXaxis()->SetTitle("Distance (cm)");
	  h2->GetYaxis()->SetTitle("Number of photoelectrons");
	  h2->Draw();
	  h1->Draw("same");
	}
      else {cout<<"no maximum found???"<<endl;}

      // --- unfortunately the legend has to be controlled completely manually
      // --- there's no way to automate it since it will always vary on a case by case basis
      TLegend *leg = new TLegend(0.48,0.73,0.88,0.88);
      leg->AddEntry(h1,short1,"l");
      leg->AddEntry(h2,short2,"l");
      leg->SetTextSize(0.04);
      leg->Draw();

      // --- similarly there's no good way to generalize the figure name
      // c1->Print(Form("Figures/%s_scancomp_proj_%d.png",outname,i));
      // c1->Print(Form("Figures/%s_scancomp_proj_%d.pdf",outname,i));
      c1->Print(Form("Figures/SCANCOMP_%s_proj_%d.png",outname,i));
      c1->Print(Form("Figures/SCANCOMP_%s_proj_%d.pdf",outname,i));

    }

}



