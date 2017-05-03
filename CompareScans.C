// --- this macro compares 1d projections of different scans of the same smart tile



void dofix2();
void dofix3();
void fixednumber_2(const char*,const char*,const char*,const char*,const char*,const char*,const char*);
void fixednumber_3(const char*,const char*,const char*,const char*,const char*,const char*,const char*,const char*,const char*);



void CompareScans()
{
  gStyle->SetOptTitle(1);
  dofix2();
  dofix3();
}



void dofix3()
{

  // ---

  fixednumber_3
    (
     "20150911-1150_A1_Source_VMIN_SIPM1",
     "20150911-1328_A1_Source_VMIN_SIPM1",
     "20150914-1138_A1_Source_VMIN_SIPM1",
     "9/11 early",
     "9/11 late",
     "9/14",
     "Comparison of scans",
     "Source",
     "A1_Source_3Comp_91114-SiPM1"
     );

  fixednumber_3
    (
     "20150911-1150_A1_Source_VMIN_SIPM2",
     "20150911-1328_A1_Source_VMIN_SIPM2",
     "20150914-1138_A1_Source_VMIN_SIPM2",
     "9/11 early",
     "9/11 late",
     "9/14",
     "Comparison of scans",
     "Source",
     "A1_Source_3Comp_91114-SiPM2"
     );

  // ---

  fixednumber_3
    (
     "20150911-1607_A1_LED_VMIN_SIPM1",
     "20150911-1700_A1_LED_VMIN_SIPM1",
     "20150914-1750_A1_LED_VMIN_SIPM1",
     "9/11 early",
     "9/11 late",
     "9/14",
     "Comparison of scans",
     "LED",
     "A1_LED_3Comp_91114-SiPM1"
     );

  fixednumber_3
    (
     "20150911-1607_A1_LED_VMIN_SIPM2",
     "20150911-1700_A1_LED_VMIN_SIPM2",
     "20150914-1750_A1_LED_VMIN_SIPM2",
     "9/11 early",
     "9/11 late",
     "9/14",
     "Comparison of scans",
     "LED",
     "A1_LED_3Comp_91114-SiPM2"
     );


}



void fixednumber_3(const char* name1,
                   const char* name2,
                   const char* name3,
                   const char* short1,
                   const char* short2,
                   const char* short3,
                   const char* longdescription,
                   const char* shortdescription,
                   const char* outname)
{

  // --- get the files
  TFile *file1 = TFile::Open(Form("Data/ROOT/%s_meanHistSub_projections.root",name1));
  TFile *file2 = TFile::Open(Form("Data/ROOT/%s_meanHistSub_projections.root",name2));
  TFile *file3 = TFile::Open(Form("Data/ROOT/%s_meanHistSub_projections.root",name3));

  // --- check that the files exist
  if(file1==NULL) {cout<<"Cannot find file "<<Form("Data/ROOT/%s_meanHistSub_projections.root",name1)<<endl;return;}
  if(file2==NULL) {cout<<"Cannot find file "<<Form("Data/ROOT/%s_meanHistSub_projections.root",name2)<<endl;return;}
  if(file3==NULL) {cout<<"Cannot find file "<<Form("Data/ROOT/%s_meanHistSub_projections.root",name3)<<endl;return;}

  // --- not sure if it's possible to generalize, since the 2d histo is in a different file
  for(int i=0; i<9; i++)
    {
      // --- get the histograms
      TH1D *h1 = (TH1D *)file1->Get(Form("%s_meanHistSub_hprojX_%d",name1,i));
      TH1D *h2 = (TH1D *)file2->Get(Form("%s_meanHistSub_hprojX_%d",name2,i));
      TH1D *h3 = (TH1D *)file3->Get(Form("%s_meanHistSub_hprojX_%d",name3,i)); // a few files are missing this for some reason
      if(h1==NULL) {cout<<"Cannot find h1 "<<Form("%s_meanHistSub_hprojX_%d",name1,i)<<" from file "<<Form("Data/ROOT/%s_meanHistSub_projections.root",name1)<<endl;return;}
      if(h2==NULL) {cout<<"Cannot find h2 "<<Form("%s_meanHistSub_hprojX_%d",name1,i)<<" from file "<<Form("Data/ROOT/%s_meanHistSub_projections.root",name2)<<endl;return;}
      if(h3==NULL) {cout<<"Cannot find h3 "<<Form("%s_meanHistSub_hprojX_%d",name1,i)<<" from file "<<Form("Data/ROOT/%s_meanHistSub_projections.root",name3)<<endl;return;}

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



      TCanvas* c1 = new TCanvas();

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
      leg->SetFillStyle(0);
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

  fixednumber_2
    (
     "20150911-1150_A1_Source_VMIN_SIPM1",
     "20150911-1328_A1_Source_VMIN_SIPM1",
     "9/11 early",
     "9/11 late",
     "Comparison of Source scans (coated panel)",
     "coated panel",
     "20150911-A1_Source_Comp_SiPM1"
     );

  fixednumber_2
    (
     "20150911-1150_A1_Source_VMIN_SIPM2",
     "20150911-1328_A1_Source_VMIN_SIPM2",
     "9/11 early",
     "9/11 late",
     "Comparison of Source scans (coated panel)",
     "coated panel",
     "20150911-A1_Source_Comp_SiPM2"
     );

  fixednumber_2
    (
     "20150911-1607_A1_LED_VMIN_SIPM1",
     "20150911-1700_A1_LED_VMIN_SIPM1",
     "9/11 early",
     "9/11 late",
     "Comparison of LED scans (coated panel)",
     "coated panel",
     "20150911-A1_LED_Comp_SiPM1"
     );

  fixednumber_2
    (
     "20150911-1607_A1_LED_VMIN_SIPM2",
     "20150911-1700_A1_LED_VMIN_SIPM2",
     "9/11 early",
     "9/11 late",
     "Comparison of LED scans (coated panel)",
     "coated panel",
     "20150911-A1_LED_Comp_SiPM2"
     );

}



void fixednumber_2(const char* name1,
                   const char* name2,
                   const char* short1,
                   const char* short2,
                   const char* longdescription,
                   const char* shortdescription,
                   const char* outname)
{

  // --- get the files
  TFile *file1 = TFile::Open(Form("Data/ROOT/%s_meanHistSub_projections.root",name1));
  TFile *file2 = TFile::Open(Form("Data/ROOT/%s_meanHistSub_projections.root",name2));

  // --- check that the files exist
  if(file1==NULL) {cout<<"Cannot find file "<<Form("Data/ROOT/%s_meanHistSub_projections.root",name1)<<endl;return;}
  if(file2==NULL) {cout<<"Cannot find file "<<Form("Data/ROOT/%s_meanHistSub_projections.root",name2)<<endl;return;}

  // --- not sure if it's possible to generalize, since the 2d histo is in a different file
  for(int i=0; i<9; i++)
    {
      // --- get the histograms
      TH1D *h1 = (TH1D *)file1->Get(Form("%s_meanHistSub_hprojX_%d",name1,i));
      TH1D *h2 = (TH1D *)file2->Get(Form("%s_meanHistSub_hprojX_%d",name2,i));
      if(h1==NULL) {cout<<"Cannot find h1 "<<Form("%s_meanHistSub_hprojX_%d",name1,i)<<" from file "<<Form("Data/ROOT/%s_meanHistSub_projections.root",name1)<<endl;return;}
      if(h2==NULL) {cout<<"Cannot find h2 "<<Form("%s_meanHistSub_hprojX_%d",name2,i)<<" from file "<<Form("Data/ROOT/%s_meanHistSub_projections.root",name2)<<endl;return;}

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



      TCanvas* c1 = new TCanvas();

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

      double xmax = 0;
      if(max == max1) xmax = h1->GetBinCenter(h1->GetMaximumBin());
      if(max == max2) xmax = h2->GetBinCenter(h2->GetMaximumBin());

      // --- unfortunately the legend has to be controlled completely manually
      // --- there's no way to automate it since it will always vary on a case by case basis
      TLegend *leg;
      // --- try these for LED scans
      // if(xmax<15) leg = new TLegend(0.58,0.73,0.88,0.88);
      // else leg = new TLegend(0.18,0.73,0.48,0.88);
      // --- try this for Source scans
      leg = new TLegend(0.52,0.23,0.72,0.38);
      leg->AddEntry(h1,short1,"l");
      leg->AddEntry(h2,short2,"l");
      leg->SetTextSize(0.04);
      leg->SetFillStyle(0);
      leg->Draw();

      // --- similarly there's no good way to generalize the figure name
      // c1->Print(Form("Figures/%s_scancomp_proj_%d.png",outname,i));
      // c1->Print(Form("Figures/%s_scancomp_proj_%d.pdf",outname,i));
      c1->Print(Form("Figures/SCANCOMP_%s_proj_%d.png",outname,i));
      c1->Print(Form("Figures/SCANCOMP_%s_proj_%d.pdf",outname,i));

    }

}



