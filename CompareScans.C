gStyle->SetOptTitle(1); // good times...

void CompareScans()
{

  // "A3_LED_Assymetry_NoBlock_Recouple_VMin_SiPM1",
  // "A3_LED_Assymetry_PannelBlockOnly_Recouple_VMin_SiPM1",
  // "A3_LED_Assymetry_PannelAirBlock_Recouple_VMin_SiPM1",
  // "A3_LED_Assymetry_CladBlock_Recouple_VMin_SiPM1",

  // "20150824-1750_A3_LED_NoBlock_Recouple2_VMIN_SIPM1",
  // "20150824-1701_A3_LED_PanelBlock_Recouple2_VMIN_SIPM1",
  // "20150824-1701_A3_LED_PanelBlock_Recouple2_VMIN_SIPM1",

  fixednumber_2
    (
     "A3_LED_Assymetry_CladBlock_Recouple_VMin_SiPM1",
     "20150824-1552_A3_LED_CladBlock_Recouple2_VMIN_SIPM1",
     "Sebastian",
     "Ron",
     "Comparison of scans with cladding blocked",
     "cladding blocked",
     "A3_LED_Comp_CladBlock_SiPM1"
     );

  fixednumber_2
    (
     "A3_LED_Assymetry_PannelBlockOnly_Recouple_VMin_SiPM1",
     "20150824-1701_A3_LED_PanelBlock_Recouple2_VMIN_SIPM1",
     "Sebastian",
     "Ron",
     "Comparison of scans with panel and air blocked",
     "panel blocked",
     "A3_LED_Comp_PanelBlock_SiPM1"
     );

  fixednumber_2
    (
     "A3_LED_Assymetry_NoBlock_Recouple_VMin_SiPM1",
     "20150825-1133_A3_LED_NoBlock_VMIN_SIPM1",
     "Sebastian",
     "Ron",
     "Comparison of scans with no blocking",
     "no blocking",
     "A3_LED_Comp_NoBlock_SiPM1"
     );

  fixednumber_2
    (
     "20150824-1750_A3_LED_NoBlock_Recouple2_VMIN_SIPM1",
     "20150825-1133_A3_LED_NoBlock_VMIN_SIPM1",
     "Ron 8/24",
     "Ron 8/25",
     "Comparison of scans with no blocking",
     "no blocking",
     "A3_LED_Comp_NoBlock-x2_SiPM1"
     );

  // SiPM2 needs better legend...
  fixednumber_2
    (
     "20150824-1750_A3_LED_NoBlock_Recouple2_VMIN_SIPM2",
     "20150825-1133_A3_LED_NoBlock_VMIN_SIPM2",
     "Ron 8/24",
     "Ron 8/25",
     "Comparison of scans with no blocking",
     "no blocking",
     "A3_LED_Comp_NoBlock-x2_SiPM2"
     );

  fixednumber_2
    (
     "20150824-1552_A3_LED_CladBlock_Recouple2_VMIN_SIPM1",
     "20150825-1422_A3_LED_PanelAirClad_VMIN_SIPM1",
     "Ron 8/24",
     "Ron 8/25",
     "Comparison of scans with panel, air, and clad blocking",
     "clad blocking",
     "A3_LED_Comp_PanelAirClad-x2_SiPM1"
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
