#include <algorithm> // for min_element, max_element

double peconvert = 0.00502; // volts per photoelectrion

void bigtilecosmics()
{

  // doit("20151022-1538");
  // doit("20151023-1307");
  // doit("20151027-1804");
  // doit("20151029-1340");
  // doit("20151030-1532");
  doit("20151103-1512");

}


void doit(const char *basename)
{

  // --- read in the data and create a vector with all the values
  // --- note that this code requires an duplicates to have already been cleaned out
  // --- this is automatically fixed with the new version of the DAQ/stepper code
  // --- but the user would do well do double check all output files anyway
  //ifstream fin1("TEMP/20150930-1720_Unaveraged_VMin1.txt");
  ifstream fin1(Form("TEMP/%s_Unaveraged_VMin1.txt",basename));
  double content;
  vector<double> voltage1;
  while(fin1>>content)
    {
      voltage1.push_back(content);
    }
  fin1.close();
  cout << voltage1.size() << endl;

  // --- do the same for SiPM2
  //ifstream fin2("TEMP/20150930-1720_Unaveraged_VMin2.txt");
  ifstream fin2(Form("TEMP/%s_Unaveraged_VMin2.txt",basename));
  vector<double> voltage2;
  while(fin2>>content)
    {
      voltage2.push_back(content);
    }
  fin2.close();
  cout << voltage2.size() << endl;

  // --- get the number of entries and the min and max
  int number = voltage1.size();
  double max = *max_element(voltage1.begin(),voltage1.end());
  double min = *min_element(voltage1.begin(),voltage1.end());
  cout << max << endl;
  cout << min << endl;
  // --- use the min and max to calculate a range for the histogram
  double newmax = min*-0.95;
  double newmin = max*-1.05 - newmax*0.1;
  // --- create the new histogram
  const int nbins = 100;
  TH1D *h1 = new TH1D("h1","",nbins,newmin,newmax);
  TH1D *h2 = new TH1D("h2","",nbins,newmin,newmax);
  TH1D *hsum = new TH1D("hsum","",nbins,2*newmin,2*newmax);
  TH2D *hh1v2 = new TH2D("hh1v2","",nbins*2,newmin,newmax,nbins*2,newmin,newmax); // SiPM1 vs SiPM2
  TH2D *hhSvA = new TH2D("hhSvA","",nbins*2,2*newmin,2*newmax,nbins*2,-1,1); // Sum vs Asymmetry
  TH2D *hh1v2_cut1 = new TH2D("hh1v2_cut1","",nbins*2,newmin,newmax,nbins*2,newmin,newmax); // SiPM1 vs SiPM2
  TH2D *hhSvA_cut1 = new TH2D("hhSvA_cut1","",nbins*2,2*newmin,2*newmax,nbins*2,-1,1); // Sum vs Asymmetry
  TH2D *hh1v2_cut2 = new TH2D("hh1v2_cut2","",nbins*2,newmin,newmax,nbins*2,newmin,newmax); // SiPM1 vs SiPM2
  TH2D *hhSvA_cut2 = new TH2D("hhSvA_cut2","",nbins*2,2*newmin,2*newmax,nbins*2,-1,1); // Sum vs Asymmetry
  vector<double> sum;
  vector<double> asym;
  // --- loop over the vector to fill the histogram
  for(int i=0; i<number; i++)
    {
      // --- SiPM1
      h1->Fill(-voltage1[i]);
      // --- SiPM2
      h2->Fill(-voltage2[i]);
      // --- SiPM1 vs SiPM2
      hh1v2->Fill(-voltage1[i],-voltage2[i]);
      // --- sum vs asymmetry
      double tempsum = -voltage1[i] + -voltage2[i];
      double tempasym = (voltage1[i] - voltage2[i]) / (-voltage1[i] + -voltage2[i]);
      hsum->Fill(tempsum);
      hhSvA->Fill(tempsum,tempasym);
      sum.push_back(tempsum);
      asym.push_back(tempasym);
      // --- now do some cuts...
      if(fabs(tempasym)<0.4)
	{
	  hh1v2_cut1->Fill(-voltage1[i],-voltage2[i]);
	  hhSvA_cut1->Fill(tempsum,tempasym);
	}
      if((voltage1[i]<(voltage2[i]+20*peconvert))&&(voltage2[i]<(voltage1[i]+20*peconvert)))
	{
	  hh1v2_cut2->Fill(-voltage1[i],-voltage2[i]);
	  hhSvA_cut2->Fill(tempsum,tempasym);
	}
    }

  // --- make a canvas and draw the histogram
  TCanvas *c1 = new TCanvas("c1","",800,600);

  // --- rescale the histograms from volts to photoelectrons
  h1->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  h1->GetXaxis()->SetTitle("Number of photoelectrons");
  h1->GetYaxis()->SetTitle("Counts");

  // --- define Landau function and draw
  // --- don't fit yet because the data have tons of ugly low voltage1 background
  double height = 1049;
  double mu = 23;
  double sigma = 3;
  //TF1 *fun = new TF1("fun","[0]*TMath::Landau(x,[1],[2])",newmin/peconvert,newmax/peconvert);
  TF1 *fun = new TF1("fun","[0]*TMath::Landau(x,[1],[2])",2*newmin/peconvert,2*newmax/peconvert);
  fun->SetParameter(0,height);
  fun->SetParameter(1,mu);
  fun->SetParameter(2,sigma);


  double bgscale = 650; // guess...





  c1->SetLogy(0);
  c1->Clear();
  hh1v2->Draw("colz");
  hh1v2->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  hh1v2->GetYaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  hh1v2->GetXaxis()->SetTitle("#pe SiPM1");
  hh1v2->GetYaxis()->SetTitle("#pe SiPM2");
  c1->SetLogz(0);
  c1->Print(Form("Cosmics/%s_cosmics_1v2.png",basename));
  c1->Print(Form("Cosmics/%s_cosmics_1v2.pdf",basename));
  c1->SetLogz(1);
  c1->Print(Form("Cosmics/%s_cosmics_1v2_log.png",basename));
  c1->Print(Form("Cosmics/%s_cosmics_1v2_log.pdf",basename));

  hhSvA->Draw("colz");
  hhSvA->GetXaxis()->SetLimits(2*newmin/peconvert,2*newmax/peconvert);
  hhSvA->GetXaxis()->SetTitle("Sum");
  hhSvA->GetYaxis()->SetTitle("Asymmetry");
  c1->SetLogz(0);
  c1->Print(Form("Cosmics/%s_cosmics_SvA.png",basename));
  c1->Print(Form("Cosmics/%s_cosmics_SvA.pdf",basename));
  c1->SetLogz(1);
  c1->Print(Form("Cosmics/%s_cosmics_SvA_log.png",basename));
  c1->Print(Form("Cosmics/%s_cosmics_SvA_log.pdf",basename));


  // --- now for some cuts...

  c1->SetLogz(0);
  hh1v2_cut1->Draw("colz");
  hh1v2_cut1->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  hh1v2_cut1->GetYaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  hh1v2_cut1->GetXaxis()->SetTitle("#pe SiPM1");
  hh1v2_cut1->GetYaxis()->SetTitle("#pe SiPM2");
  TLatex *tex1 = new TLatex(0.2,0.8,"|Asymmetry|<0.4");
  tex1->SetTextSize(0.05);
  tex1->SetNDC(kTRUE);
  tex1->Draw();
  c1->SetLogz(0);
  c1->Print(Form("Cosmics/%s_cosmics_1v2_cut1.png",basename));
  c1->Print(Form("Cosmics/%s_cosmics_1v2_cut1.pdf",basename));
  c1->SetLogz(1);
  c1->Print(Form("Cosmics/%s_cosmics_1v2_cut1_log.png",basename));
  c1->Print(Form("Cosmics/%s_cosmics_1v2_cut1_log.pdf",basename));

  hhSvA_cut1->Draw("colz");
  hhSvA_cut1->GetXaxis()->SetLimits(2*newmin/peconvert,2*newmax/peconvert);
  hhSvA_cut1->GetXaxis()->SetTitle("Sum");
  hhSvA_cut1->GetYaxis()->SetTitle("Asymmetry");
  tex1->Draw();
  c1->SetLogz(0);
  c1->Print(Form("Cosmics/%s_cosmics_SvA_cut1.png",basename));
  c1->Print(Form("Cosmics/%s_cosmics_SvA_cut1.pdf",basename));
  c1->SetLogz(1);
  c1->Print(Form("Cosmics/%s_cosmics_SvA_cut1_log.png",basename));
  c1->Print(Form("Cosmics/%s_cosmics_SvA_cut1_log.pdf",basename));




  c1->SetLogz(0);
  hh1v2_cut2->Draw("colz");
  hh1v2_cut2->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  hh1v2_cut2->GetYaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  hh1v2_cut2->GetXaxis()->SetTitle("#pe SiPM1");
  hh1v2_cut2->GetYaxis()->SetTitle("#pe SiPM2");
  TLatex *tex2 = new TLatex(0.2,0.8,"SiPMA < SiPMB + 20");
  tex2->SetTextSize(0.05);
  tex2->SetNDC(kTRUE);
  tex2->Draw();
  c1->SetLogz(0);
  c1->Print(Form("Cosmics/%s_cosmics_1v2_cut2.png",basename));
  c1->Print(Form("Cosmics/%s_cosmics_1v2_cut2.pdf",basename));
  c1->SetLogz(1);
  c1->Print(Form("Cosmics/%s_cosmics_1v2_cut2_log.png",basename));
  c1->Print(Form("Cosmics/%s_cosmics_1v2_cut2_log.pdf",basename));

  hhSvA_cut2->Draw("colz");
  hhSvA_cut2->GetXaxis()->SetLimits(2*newmin/peconvert,2*newmax/peconvert);
  hhSvA_cut2->GetXaxis()->SetTitle("Sum");
  hhSvA_cut2->GetYaxis()->SetTitle("Asymmetry");
  tex2->Draw();
  c1->SetLogz(0);
  c1->Print(Form("Cosmics/%s_cosmics_SvA_cut2.png",basename));
  c1->Print(Form("Cosmics/%s_cosmics_SvA_cut2.pdf",basename));
  c1->SetLogz(1);
  c1->Print(Form("Cosmics/%s_cosmics_SvA_cut2_log.png",basename));
  c1->Print(Form("Cosmics/%s_cosmics_SvA_cut2_log.pdf",basename));



  hsum->SetLineColor(kBlack);
  hsum->SetLineWidth(2);
  hsum->GetXaxis()->SetLimits(2*newmin/peconvert,2*newmax/peconvert);
  hsum->GetXaxis()->SetRangeUser(0.0,120.0);
  hsum->GetXaxis()->SetTitle("Number of photoelectrons SiPM1+SiPM2");
  hsum->Draw();
  c1->Print(Form("Cosmics/%s_temp.png",basename));
  c1->Print(Form("Cosmics/%s_temp.pdf",basename));
  hsum->SetMaximum(30);
  c1->Print(Form("Cosmics/%s_templow.png",basename));
  c1->Print(Form("Cosmics/%s_templow.pdf",basename));
  c1->SetLogy();
  hsum->SetMaximum(1.1*h1->GetBinContent(hsum->GetMaximumBin()));
  c1->Print(Form("Cosmics/%s_templog.png",basename));
  c1->Print(Form("Cosmics/%s_templog.pdf",basename));


  // ---------------------------------------------------------
  bgscale = 100;
  h1->Fit(fun,"","",20,60);
  //h1->Fit(fun,"","",40,80);
  fun->SetLineColor(kRed);
  fun->SetLineWidth(2);
  fun->Draw("same");

  cout << fun->GetChisquare() << "/" << fun->GetNDF() << endl;

  c1->SetLogy(0);
  c1->Print(Form("Cosmics/%s_tempfit.png",basename));
  c1->Print(Form("Cosmics/%s_tempfit.pdf",basename));
  h1->SetMaximum(175);
  c1->Print(Form("Cosmics/%s_templowfit.png",basename));
  c1->Print(Form("Cosmics/%s_templowfit.pdf",basename));
  h1->SetMaximum(100);
  c1->Print(Form("Cosmics/%s_tempLOWfit.png",basename));
  c1->Print(Form("Cosmics/%s_tempLOWfit.pdf",basename));
  c1->SetLogy(1);
  h1->SetMaximum(1.1*h1->GetBinContent(h1->GetMaximumBin()));
  c1->Print(Form("Cosmics/%s_templogfit.png",basename));
  c1->Print(Form("Cosmics/%s_templogfit.pdf",basename));

  c1->Clear();
  h1->Draw();
  double hax = fun->GetParameter(0);
  double mux = fun->GetParameter(1);
  double six = fun->GetParameter(2);
  fun->SetParameter(0,0.95*hax);
  fun->SetParameter(1,mux);
  fun->SetParameter(2,1.1*six);
  //fun->Draw("same");
  //h1->Fit(simplefun,"","",0,60);

  TF1 *fun2 = new TF1("fun2","([0]/sqrt(6.28))*TMath::Exp(-0.5*((x-[1])/[2] + TMath::Exp(-(x-[1])/[2])))",0,120);
  // fun2->SetParameter(0,hax);
  // fun2->SetParameter(1,mux);
  // fun2->SetParameter(2,six);
  fun2->SetParameter(0,93);
  fun2->SetParameter(1,35);
  fun2->SetParameter(2,5);
  //fun2->SetLineColor(kBlack);

  //h1->Fit(fun2,"","",15,60);
  //h1->Fit(fun2,"","",30,120);
  h1->Fit(fun2,"","",20,80);
  fun2->Draw("same");

  cout << fun2->GetChisquare() << "/" << fun2->GetNDF() << endl;

  double par1 = fun2->GetParameter(1);
  TLine line(par1,0,par1,0.24*fun2->GetParameter(0));
  line.Draw();

  c1->SetLogy(0);
  c1->Print(Form("Cosmics/%s_tempffit.png",basename));
  c1->Print(Form("Cosmics/%s_tempffit.pdf",basename));
  h1->SetMaximum(25);
  c1->Print(Form("Cosmics/%s_templowffit.png",basename));
  c1->Print(Form("Cosmics/%s_templowffit.pdf",basename));
  h1->SetMaximum(0.3*fun2->GetParameter(0));
  c1->Print(Form("Cosmics/%s_tempLOWffit.png",basename));
  c1->Print(Form("Cosmics/%s_tempLOWffit.pdf",basename));
  c1->SetLogy(1);
  h1->SetMaximum(1.1*h1->GetBinContent(h1->GetMaximumBin()));
  c1->Print(Form("Cosmics/%s_templogffit.png",basename));
  c1->Print(Form("Cosmics/%s_templogffit.pdf",basename));



}
