#include <algorithm> // for min_element, max_element

double peconvert = 0.00502; // volts per photoelectrion

void simplecosmics()
{

  doit("20150930-1720");
  doit("20151009-1743");

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
  h1->Draw();
  // c1->Print(Form("Cosmics/%s_uglydata.png",basename));
  c1->SetLogy();
  // c1->Print(Form("Cosmics/%s_uglydatalog.png",basename));
  c1->SetLogy(0);
  // h1->SetMaximum(120);
  // c1->Print(Form("Cosmics/%s_uglydatanotlog.png",basename));

  // --- rescale the histograms from volts to photoelectrons
  h1->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  h1->GetXaxis()->SetTitle("Number of photoelectrons");
  h1->GetYaxis()->SetTitle("Counts");
  h1->Draw();
  // c1->Print(Form("Cosmics/%s_uglydata.png",basename));
  c1->SetLogy();
  // c1->Print(Form("Cosmics/%s_uglydatalog.png",basename));
  c1->SetLogy(0);
  h1->SetMaximum(210);
  // c1->Print(Form("Cosmics/%s_uglydatanotlog.png",basename));

  // --- define Landau function and draw
  // --- don't fit yet because the data have tons of ugly low voltage1 background
  double height = 1049;
  double mu = 23;
  double sigma = 3;
  TF1 *fun = new TF1("fun","[0]*TMath::Landau(x,[1],[2])",newmin/peconvert,newmax/peconvert);
  fun->SetParameter(0,height);
  fun->SetParameter(1,mu);
  fun->SetParameter(2,sigma);
  fun->Draw("same");
  fun->SetLineColor(kGreen+2);
  fun->SetLineWidth(1);
  c1->Print(Form("Cosmics/%s_uglydatanotlogfit.png",basename));
  c1->Print(Form("Cosmics/%s_uglydatanotlogfit.pdf",basename));
  c1->SetLogy(1);
  c1->Print(Form("Cosmics/%s_uglydatalogfit.png",basename));
  c1->Print(Form("Cosmics/%s_uglydatalogfit.pdf",basename));

  // --- now draw SiPM2 on top
  h1->SetLineColor(kRed);
  h2->SetLineColor(kBlue);
  h2->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  h2->Draw("same");
  c1->SetLogy(0);
  c1->Print(Form("Cosmics/%s_uglydatabothnotlogfit.png",basename));
  c1->Print(Form("Cosmics/%s_uglydatabothnotlogfit.pdf",basename));
  c1->SetLogy(1);
  c1->Print(Form("Cosmics/%s_uglydatabothlogfit.png",basename));
  c1->Print(Form("Cosmics/%s_uglydatabothlogfit.pdf",basename));

  h1->SetMaximum(1.15*h1->GetBinContent(h1->GetMaximumBin()));

  c1->SetLogy(0);
  c1->Print(Form("Cosmics/%s_fuglydatabothnotlogfit.png",basename));
  c1->Print(Form("Cosmics/%s_fuglydatabothnotlogfit.pdf",basename));
  c1->SetLogy(1);
  c1->Print(Form("Cosmics/%s_fuglydatabothlogfit.png",basename));
  c1->Print(Form("Cosmics/%s_fuglydatabothlogfit.pdf",basename));

  double bgscale = 650; // guess...
  TF1 *bun = new TF1("bun","[0]*TMath::Exp([1]*x) + [2]*TMath::Exp([3]*x)",newmin/peconvert,newmax/peconvert);
  bun->SetParameter(0,bgscale*7.17020);
  bun->SetParameter(1,-5.10223e-1);
  bun->SetParameter(2,bgscale*1.22363e-1);
  bun->SetParameter(3,-8.84094e-2);
  bun->SetLineColor(kGreen+2);
  bun->SetLineWidth(1);
  bun->Draw("same");

  c1->SetLogy(0);
  c1->Print(Form("Cosmics/%s_buglydatabothnotlogfit.png",basename));
  c1->Print(Form("Cosmics/%s_buglydatabothnotlogfit.pdf",basename));
  c1->SetLogy(1);
  c1->Print(Form("Cosmics/%s_buglydatabothlogfit.png",basename));
  c1->Print(Form("Cosmics/%s_buglydatabothlogfit.pdf",basename));



  TF1 *ultrafun = new TF1("ultrafun","[0]*TMath::Exp([1]*x) + [2]*TMath::Exp([3]*x) + [4]*TMath::Landau(x,[5],[6])",newmin/peconvert,newmax/peconvert);
  ultrafun->SetParameter(0,bgscale*7.17020);
  ultrafun->SetParameter(1,-5.10223e-1);
  ultrafun->SetParameter(2,bgscale*1.22363e-1);
  ultrafun->SetParameter(3,-8.84094e-2);
  ultrafun->SetParameter(4,height);
  ultrafun->SetParameter(5,mu);
  ultrafun->SetParameter(6,sigma);
  ultrafun->SetLineColor(kBlack);
  ultrafun->Draw("same");

  TLegend *leg = new TLegend(0.48,0.68,0.88,0.88);
  leg->AddEntry(h1,"SiPM1","l");
  leg->AddEntry(h2,"SiPM2","l");
  leg->AddEntry(fun,"Signal (Landau)","l");
  leg->AddEntry(bun,"Background (double expo)","l");
  leg->AddEntry(ultrafun,"Combined functional fit","l");
  leg->Draw();

  c1->SetLogy(0);
  c1->Print(Form("Cosmics/%s_bhuglydatabothnotlogfit.png",basename));
  c1->Print(Form("Cosmics/%s_bhuglydatabothnotlogfit.pdf",basename));
  c1->SetLogy(1);
  c1->Print(Form("Cosmics/%s_bhuglydatabothlogfit.png",basename));
  c1->Print(Form("Cosmics/%s_bhuglydatabothlogfit.pdf",basename));

  h1->Fit(ultrafun,"R");
  bun->SetParameter(0,ultrafun->GetParameter(0));
  bun->SetParameter(1,ultrafun->GetParameter(1));
  bun->SetParameter(2,ultrafun->GetParameter(2));
  bun->SetParameter(3,ultrafun->GetParameter(3));
  fun->SetParameter(0,ultrafun->GetParameter(4));
  fun->SetParameter(1,ultrafun->GetParameter(5));
  fun->SetParameter(2,ultrafun->GetParameter(6));
  c1->SetLogy(0);
  c1->Print(Form("Cosmics/%s_bhfuglydatabothnotlogfit.png",basename));
  c1->Print(Form("Cosmics/%s_bhfuglydatabothnotlogfit.pdf",basename));
  c1->SetLogy(1);
  c1->Print(Form("Cosmics/%s_bhfuglydatabothlogfit.png",basename));
  c1->Print(Form("Cosmics/%s_bhfuglydatabothlogfit.pdf",basename));

  h1->SetMaximum(210);
  c1->SetLogy(0);
  c1->Print(Form("Cosmics/%s_pfuglydatabothnotlogfit.png",basename));
  c1->Print(Form("Cosmics/%s_pfuglydatabothnotlogfit.pdf",basename));
  c1->SetLogy(1);
  c1->Print(Form("Cosmics/%s_pfuglydatabothlogfit.png",basename));
  c1->Print(Form("Cosmics/%s_pfuglydatabothlogfit.pdf",basename));



  // ---

  h1->SetMaximum(1.15*h1->GetBinContent(h1->GetMaximumBin()));
  h1->Draw();
  h2->Draw("same");

  TF1 *pain = new TF1("pain","[0]*7.17020*TMath::Exp([1]*x) + [0]*1.22363*TMath::Exp([2]*x) + [3]*TMath::Landau(x,[4],[5])",newmin/peconvert,newmax/peconvert);
  pain->SetParameter(0,bgscale);
  pain->FixParameter(1,-5.10223e-1);
  pain->FixParameter(2,-8.84094e-2);
  pain->SetParameter(3,height);
  pain->SetParameter(4,mu);
  pain->SetParameter(5,sigma);
  pain->SetLineColor(kBlack);
  pain->Draw("same");

  h1->Fit(pain,"R");
  c1->SetLogy(0);
  c1->Print(Form("Cosmics/%s_iampain.png",basename));
  c1->Print(Form("Cosmics/%s_iampain.pdf",basename));
  c1->SetLogy(1);
  c1->Print(Form("Cosmics/%s_iampain_log.png",basename));
  c1->Print(Form("Cosmics/%s_iampain_log.pdf",basename));




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



  hsum->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  hsum->GetXaxis()->SetTitle("Number of photoelectrons (SiPM1+SiPM2)/2");
  hsum->Draw();
  c1->Print(Form("Cosmics/%s_temp.png",basename));
  c1->Print(Form("Cosmics/%s_temp.pdf",basename));
  hsum->SetMaximum(175);
  c1->Print(Form("Cosmics/%s_templow.png",basename));
  c1->Print(Form("Cosmics/%s_templow.pdf",basename));
  c1->SetLogy();
  hsum->SetMaximum(1.1*h1->GetBinContent(hsum->GetMaximumBin()));
  c1->Print(Form("Cosmics/%s_templog.png",basename));
  c1->Print(Form("Cosmics/%s_templog.pdf",basename));


  bgscale = 100;
  TF1 *simplefun = new TF1("simplefun","[0]*TMath::Exp([1]*x) + [2]*TMath::Landau(x,[3],[4])",newmin/peconvert,newmax/peconvert);
  simplefun->SetParameter(0,bgscale*7.17020);
  simplefun->SetParameter(1,-5.10223e-1);
  simplefun->SetParameter(2,height);
  simplefun->SetParameter(3,mu);
  simplefun->SetParameter(4,sigma);
  simplefun->SetLineColor(kBlack);
  c1->Clear();
  //simplefun->Draw("same");
  //hsum->Fit(simplefun,"","",0,120);
  //hsum->Fit(ultrafun,"","",0,120);
  //hsum->Fit("landau","","",15,120);
  hsum->Fit(fun,"","",20,60);
  fun->SetLineColor(kRed);
  fun->SetLineWidth(2);
  fun->Draw("same");

  cout << fun->GetChisquare() << "/" << fun->GetNDF() << endl;

  c1->SetLogy(0);
  c1->Print(Form("Cosmics/%s_tempfit.png",basename));
  c1->Print(Form("Cosmics/%s_tempfit.pdf",basename));
  hsum->SetMaximum(175);
  c1->Print(Form("Cosmics/%s_templowfit.png",basename));
  c1->Print(Form("Cosmics/%s_templowfit.pdf",basename));
  c1->SetLogy(1);
  hsum->SetMaximum(1.1*h1->GetBinContent(hsum->GetMaximumBin()));
  c1->Print(Form("Cosmics/%s_templogfit.png",basename));
  c1->Print(Form("Cosmics/%s_templogfit.pdf",basename));

  c1->Clear();
  hsum->Draw();
  double hax = fun->GetParameter(0);
  double mux = fun->GetParameter(1);
  double six = fun->GetParameter(2);
  fun->SetParameter(0,0.95*hax);
  fun->SetParameter(1,mux);
  fun->SetParameter(2,1.1*six);
  //fun->Draw("same");
  //hsum->Fit(simplefun,"","",0,60);

  TF1 *fun2 = new TF1("fun2","([0]/sqrt(6.28))*TMath::Exp(-0.5*((x-[1])/[2] + TMath::Exp(-(x-[1])/[2])))",0,120);
  fun2->SetParameter(0,hax);
  fun2->SetParameter(1,mux);
  fun2->SetParameter(2,six);
  //fun2->SetLineColor(kBlack);

  hsum->Fit(fun2,"","",15,60);
  fun2->Draw("same");

  cout << fun2->GetChisquare() << "/" << fun2->GetNDF() << endl;

  double par1 = fun2->GetParameter(1);
  TLine line(par1,0,par1,0.24*fun2->GetParameter(0));
  line.Draw();

  c1->SetLogy(0);
  c1->Print(Form("Cosmics/%s_tempffit.png",basename));
  c1->Print(Form("Cosmics/%s_tempffit.pdf",basename));
  hsum->SetMaximum(175);
  c1->Print(Form("Cosmics/%s_templowffit.png",basename));
  c1->Print(Form("Cosmics/%s_templowffit.pdf",basename));
  c1->SetLogy(1);
  hsum->SetMaximum(1.1*h1->GetBinContent(hsum->GetMaximumBin()));
  c1->Print(Form("Cosmics/%s_templogffit.png",basename));
  c1->Print(Form("Cosmics/%s_templogffit.pdf",basename));



}
