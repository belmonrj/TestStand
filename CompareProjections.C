void CompareProjections()
{

  fixednumber
    (
     "20150821-1312_A4_LED_NoBlock_VMIN_SIPM1",
     "20150821-1431_A4_LED_Panel_VMIN_SIPM1",
     "20150821-1550_A4_LED_PanelAir_VMIN_SIPM1",
     "20150821-1651_A4_LED_PanelAirClad_VMIN_SIPM1",
     "20150821_A4_LED_Comp_SiPM1"
     );

  fixednumber
    (
     "20150824-1750_A4_LED_NoBlock_VMIN_SIPM1",
     "20150824-1701_A4_LED_Panel_VMIN_SIPM1",
     "20150824-1701_A4_LED_Panel_VMIN_SIPM1",
     "20150824-1552_A4_LED_PanelAirClad_VMIN_SIPM1",
     "20150824_A4_LED_Comp_SiPM"
     );

  fixednumber
    (
     "20150826-0945_A4_LED_NoBlock_VMIN_SIPM1",
     "20150826-1055_A4_LED_Panel_VMIN_SIPM1",
     "20150826-1505_A4_LED_PanelAir_VMIN_SIPM1",
     "20150826-1334_A4_LED_PanelAirClad_VMIN_SIPM1",
     "20150826_A4_LED_Comp_SiPM1"
     );

}



void fixednumber(char *name1, char *name2, char *name3, char *name4, char *outname)
{

  // --- get the files
  TFile *file1 = TFile::Open(Form("Data/ROOT/%s_meanHistSub_projections.root",name1));
  TFile *file2 = TFile::Open(Form("Data/ROOT/%s_meanHistSub_projections.root",name2));
  TFile *file3 = TFile::Open(Form("Data/ROOT/%s_meanHistSub_projections.root",name3));
  TFile *file4 = TFile::Open(Form("Data/ROOT/%s_meanHistSub_projections.root",name4));

  // --- check that the files exist
  if(file1==NULL) {cout<<"Cannot find file "<<Form("%s_meanHistSub_projections.root",name1)<<endl;return;}
  if(file2==NULL) {cout<<"Cannot find file "<<Form("%s_meanHistSub_projections.root",name2)<<endl;return;}
  if(file3==NULL) {cout<<"Cannot find file "<<Form("%s_meanHistSub_projections.root",name3)<<endl;return;}
  if(file4==NULL) {cout<<"Cannot find file "<<Form("%s_meanHistSub_projections.root",name4)<<endl;return;}

  // --- not sure if it's possible to generalize, since the 2d histo is in a different file
  for(int i=0; i<9; i++)
    {
      // --- get the histograms
      TH1D *h1 = (TH1D *)file1->Get(Form("%s_meanHistSub_hproj_%d",name1,i));
      TH1D *h2 = (TH1D *)file2->Get(Form("%s_meanHistSub_hproj_%d",name2,i));
      TH1D *h3 = (TH1D *)file3->Get(Form("%s_meanHistSub_hproj_%d",name3,i));
      TH1D *h4 = (TH1D *)file4->Get(Form("%s_meanHistSub_hproj_%d",name4,i));

      // --- set draw properties, very simple for now
      h1->SetLineColor(kBlack);
      h2->SetLineColor(kRed);
      h3->SetLineColor(kBlue);
      h4->SetLineColor(kGreen+2);
      h1->SetLineWidth(2);
      h2->SetLineWidth(2);
      h3->SetLineWidth(2);
      h4->SetLineWidth(2);

      // --- get the maxima
      double max1 = h1->GetMaximum();
      double max2 = h2->GetMaximum();
      double max3 = h3->GetMaximum();
      double max4 = h4->GetMaximum();

      // --- crude and hackish way to sort the maxima
      double max = 0;
      if(max1>max) max = max1;
      if(max2>max) max = max2;
      if(max3>max) max = max3;
      if(max4>max) max = max4;

      // --- draw the largest histogram first to prevent scale issues
      if(max==max1)
	{
	  h1->SetMinimum(0.1);
	  h1->GetXaxis()->SetTitle("Distance (cm)");
	  h1->GetYaxis()->SetTitle("Number of photoelectrons");
	  h1->Draw();
	  h2->Draw("same");
	  h3->Draw("same");
	  h4->Draw("same");
	}
      else if(max==max2)
	{
	  h2->SetMinimum(0.1);
	  h2->GetXaxis()->SetTitle("Distance (cm)");
	  h2->GetYaxis()->SetTitle("Number of photoelectrons");
	  h2->Draw();
	  h1->Draw("same");
	  h3->Draw("same");
	  h4->Draw("same");
	}
      else if(max==max3)
	{
	  h3->SetMinimum(0.1);
	  h3->GetXaxis()->SetTitle("Distance (cm)");
	  h3->GetYaxis()->SetTitle("Number of photoelectrons");
	  h3->Draw();
	  h1->Draw("same");
	  h2->Draw("same");
	  h4->Draw("same");
	}
      else if(max==max4)
	{
	  h4->SetMinimum(0.1);
	  h4->GetXaxis()->SetTitle("Distance (cm)");
	  h4->GetYaxis()->SetTitle("Number of photoelectrons");
	  h4->Draw();
	  h1->Draw("same");
	  h2->Draw("same");
	  h3->Draw("same");
	}
      else {cout<<"no maximum found???"<<endl;}

      // --- unfortunately the legend has to be controlled completely manually
      // --- there's no way to automate it since it will always vary on a case by case basis
      TLegend *leg = new TLegend(0.48,0.73,0.88,0.88);
      leg->AddEntry(h1,"No light blocking","l");
      leg->AddEntry(h2,"Add panel light blocking","l");
      leg->AddEntry(h3,"Add air light blocking","l");
      leg->AddEntry(h4,"Add cladding light blocking","l");
      leg->SetFillStyle(0);
      leg->SetTextSize(0.04);
      leg->Draw();

      // --- similarly there's no good way to generalize the figure name
      c1->Print(Form("Figures/PROJCOMP_%s_proj_%d.png",outname,i));
      c1->Print(Form("Figures/PROJCOMP_%s_proj_%d.pdf",outname,i));

    }

}
