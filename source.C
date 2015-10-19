#include <algorithm> // for min_element, max_element

void source()
{

  double height = 1.0;
  double mu = 0.1;
  double sigma = 0.05;
  double min = mu - sigma*5;
  double max = mu + sigma*25;


  ifstream fin1("TEMP/20150921-1152_Unaveraged_VMin1.txt");
  double content;
  vector<double> voltage1;
  while(fin1>>content)
    {
      voltage1.push_back(content);
    }
  fin1.close();
  cout << voltage1.size() << endl;

  // --- do the same for SiPM2
  ifstream fin2("TEMP/20150921-1152_Unaveraged_VMin2.txt");
  vector<double> voltage2;
  while(fin2>>content)
    {
      voltage2.push_back(content);
    }
  fin2.close();
  cout << voltage2.size() << endl;

  // --- Background SiPM1
  ifstream fin3(Form("TEMP/20150915-1612_Unaveraged_VMin1.txt"));
  vector<double> voltage3;
  while(fin3>>content)
    {
      voltage3.push_back(content);
    }
  fin3.close();
  cout << voltage3.size() << endl;

  // --- Background SiPM2
  ifstream fin4(Form("TEMP/20150915-1612_Unaveraged_VMin2.txt"));
  vector<double> voltage4;
  while(fin4>>content)
    {
      voltage4.push_back(content);
    }
  fin4.close();
  cout << voltage4.size() << endl;



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
  TH1D *h3 = new TH1D("h3","",nbins,newmin,newmax); // bg
  TH1D *h4 = new TH1D("h4","",nbins,newmin,newmax);
  TH1D *hsum = new TH1D("hsum","",nbins,2*newmin,2*newmax);
  TH2D *hh1v2 = new TH2D("hh1v2","",2*nbins,newmin,newmax,2*nbins,newmin,newmax); // SiPM1 vs SiPM2
  TH2D *hhSvA = new TH2D("hhSvA","",2*nbins,2*newmin,2*newmax,2*nbins,-1,1); // Sum vs Asymmetry
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
      // ---
      if(i<voltage3.size())
	{
	  h3->Fill(-voltage3[i]);
	  h4->Fill(-voltage4[i]);
	}
    }


  h2->SetLineColor(kRed);
  h1->SetLineColor(kBlue);
  h2->SetLineWidth(2);
  h1->SetLineWidth(2);
  double max2 = h2->GetMaximum();
  double max1 = h1->GetMaximum();

  TLegend *leg = new TLegend(0.68,0.73,0.88,0.88);
  leg->AddEntry(h1,"SiPM1","l");
  leg->AddEntry(h2,"SiPM2","l");


  // --- now do PE conversion
  double peconvert = 0.00502;
  h1->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  h2->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  h1->GetXaxis()->SetRangeUser(0.0,80.0);
  h2->GetXaxis()->SetRangeUser(0.0,80.0);
  h1->GetXaxis()->SetTitle("Number of photoelectrons");
  h2->GetXaxis()->SetTitle("Number of photoelectrons");
  if(max2>max1)
    {
      h2->Draw();
      h1->Draw("same");
    }
  else
    {
      h1->Draw();
      h2->Draw("same");
    }
  leg->Draw();
  c1->Print("Source/source_pe.pdf");
  c1->Print("Source/source_pe.png");

  double extra = voltage1.size()/voltage3.size();
  double fgrate = 1038.0; // Hz
  double bgrate = 27.0; // Hz
  double bgscalefactor = (bgrate/fgrate)*extra;

  h3->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  h4->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  h3->Scale(bgscalefactor);
  h4->Scale(bgscalefactor);
  h3->SetLineColor(kRed);
  h4->SetLineColor(kBlue);
  h3->SetLineWidth(2);
  h4->SetLineWidth(2);
  h3->Draw("same");
  h4->Draw("same");
  c1->Print("Source/source_bpe.pdf");
  c1->Print("Source/source_bpe.png");
  TH1D *h5 = (TH1D *)h1->Clone("h5");
  TH1D *h6 = (TH1D *)h2->Clone("h6");
  h5->Add(h3,-1.0);
  h6->Add(h4,-1.0);
  // h1->SetLineWidth(1);
  // h2->SetLineWidth(1);
  // h3->SetLineWidth(1);
  // h4->SetLineWidth(1);
  // h5->SetLineWidth(2);
  // h6->SetLineWidth(2);
  h5->SetLineWidth(1);
  h6->SetLineWidth(1);
  h5->Draw("same");
  h6->Draw("same");
  c1->Print("Source/source_sbpe.pdf");
  c1->Print("Source/source_sbpe.png");
  hsum->SetLineColor(kBlack);
  hsum->SetLineWidth(2);
  hsum->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  hsum->Draw("same");
  h1->SetMaximum(1.1*hsum->GetMaximum());
  h2->SetMaximum(1.1*hsum->GetMaximum());
  TLegend *leg = new TLegend(0.5,0.73,0.88,0.88);
  leg->AddEntry(h1,"SiPM1","l");
  leg->AddEntry(h2,"SiPM2","l");
  leg->AddEntry(hsum,"(SiPM1+SiPM2)/2","l");
  leg->SetTextSize(0.05);
  leg->Draw();
  c1->Print("Source/source_asbpe.pdf");
  c1->Print("Source/source_asbpe.png");



  c1->SetLogy(0);
  c1->Clear();
  hh1v2->Draw("colz");
  hh1v2->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  hh1v2->GetYaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  hh1v2->GetXaxis()->SetTitle("#pe SiPM1");
  hh1v2->GetYaxis()->SetTitle("#pe SiPM2");
  c1->SetLogz(0);
  c1->Print("Source/source_1v2.png");
  c1->Print("Source/source_1v2.pdf");
  c1->SetLogz(1);
  c1->Print("Source/source_1v2_log.png");
  c1->Print("Source/source_1v2_log.pdf");

  hhSvA->Draw("colz");
  hhSvA->GetXaxis()->SetLimits(2*newmin/peconvert,2*newmax/peconvert);
  hhSvA->GetXaxis()->SetTitle("Sum");
  hhSvA->GetYaxis()->SetTitle("Asymmetry");
  c1->SetLogz(0);
  c1->Print("Source/source_SvA.png");
  c1->Print("Source/source_SvA.pdf");
  c1->SetLogz(1);
  c1->Print("Source/source_SvA_log.png");
  c1->Print("Source/source_SvA_log.pdf");



  hsum->GetXaxis()->SetLimits(2*newmin/peconvert,2*newmax/peconvert);
  hsum->GetXaxis()->SetRangeUser(0.0,160.0);
  hsum->GetXaxis()->SetTitle("Number of photoelectrons SiPM1+SiPM2");
  double MAX = 1.1*hsum->GetMaximum();
  hsum->Draw();
  hsum->SetMaximum(MAX);
  c1->SetLogy(0);
  c1->Print(Form("Source/source_sum.png"));
  c1->Print(Form("Source/source_sum.pdf"));
  c1->SetLogy(1);
  c1->Print(Form("Source/source_sumlog.png"));
  c1->Print(Form("Source/source_sumlog.pdf"));


  TF1 *fungaus = new TF1("fungaus","gaus",2*newmin/peconvert,2*newmax/peconvert);
  hsum->Fit(fungaus,"","",5,80);
  fungaus->Draw("same");

  hsum->SetMaximum(MAX);
  c1->SetLogy(0);
  c1->Print(Form("Source/source_gaus_sum.png"));
  c1->Print(Form("Source/source_gaus_sum.pdf"));
  c1->SetLogy(1);
  c1->Print(Form("Source/source_gaus_sumlog.png"));
  c1->Print(Form("Source/source_gaus_sumlog.pdf"));

  c1->Clear();
  TF1 *gengaus = new TF1("gengaus","[0]*TMath::Exp(-pow(fabs(x-[1]),[3])/[2])",2*newmin/peconvert,2*newmax/peconvert);
  gengaus->SetParameter(0,fungaus->GetParameter(0));
  gengaus->SetParameter(1,fungaus->GetParameter(1));
  gengaus->SetParameter(2,fungaus->GetParameter(2));
  gengaus->SetParameter(3,2.2);
  hsum->Fit(gengaus,"","",5,80);
  gengaus->Draw("same");

  hsum->SetMaximum(MAX);
  c1->SetLogy(0);
  c1->Print(Form("Source/source_gengaus_sum.png"));
  c1->Print(Form("Source/source_gengaus_sum.pdf"));
  c1->SetLogy(1);
  c1->Print(Form("Source/source_gengaus_sumlog.png"));
  c1->Print(Form("Source/source_gengaus_sumlog.pdf"));

  TLine lline(gengaus->GetParameter(1),0.0,gengaus->GetParameter(1),gengaus->GetParameter(0));
  lline.Draw();
  hsum->SetMaximum(MAX);
  c1->SetLogy(0);
  c1->Print(Form("Source/source_lgengaus_sum.png"));
  c1->Print(Form("Source/source_lgengaus_sum.pdf"));
  c1->SetLogy(1);
  c1->Print(Form("Source/source_lgengaus_sumlog.png"));
  c1->Print(Form("Source/source_lgengaus_sumlog.pdf"));


  c1->Clear();
  TF1 *skewgaus = new TF1("skewgaus","[0]*TMath::Exp(-pow(fabs(x-[1]),2)/[2])*(0.5+0.5*(TMath::Erf([3]*(x-[1])/[2])))",2*newmin/peconvert,2*newmax/peconvert);
  skewgaus->SetParameter(0,fungaus->GetParameter(0));
  skewgaus->SetParameter(1,fungaus->GetParameter(1));
  skewgaus->SetParameter(2,fungaus->GetParameter(2));
  skewgaus->SetParameter(3,0.1);
  hsum->Fit(skewgaus,"","",5,80);
  skewgaus->Draw("same");

  hsum->SetMaximum(MAX);
  c1->SetLogy(0);
  c1->Print(Form("Source/source_skewgaus_sum.png"));
  c1->Print(Form("Source/source_skewgaus_sum.pdf"));
  c1->SetLogy(1);
  c1->Print(Form("Source/source_skewgaus_sumlog.png"));
  c1->Print(Form("Source/source_skewgaus_sumlog.pdf"));


  c1->Clear();
  TF1 *skewgengaus = new TF1("skewgengaus","[0]*TMath::Exp(-pow(fabs(x-[1]),[3])/[2])*(0.5+0.5*(TMath::Erf([4]*(x-[1])/[2])))",2*newmin/peconvert,2*newmax/peconvert);
  skewgengaus->SetParameter(0,fungaus->GetParameter(0));
  skewgengaus->SetParameter(1,fungaus->GetParameter(1));
  skewgengaus->SetParameter(2,fungaus->GetParameter(2));
  skewgengaus->SetParameter(3,2.2);
  skewgengaus->SetParameter(4,0.1);
  hsum->Fit(skewgengaus,"","",5,80);
  skewgengaus->Draw("same");

  hsum->SetMaximum(MAX);
  c1->SetLogy(0);
  c1->Print(Form("Source/source_skewgengaus_sum.png"));
  c1->Print(Form("Source/source_skewgengaus_sum.pdf"));
  c1->SetLogy(1);
  c1->Print(Form("Source/source_skewgengaus_sumlog.png"));
  c1->Print(Form("Source/source_skewgengaus_sumlog.pdf"));

  TLine llline(0.95*gengaus->GetParameter(1),0.0,0.95*gengaus->GetParameter(1),1.05*gengaus->GetParameter(0));
  llline.Draw();
  hsum->SetMaximum(MAX);
  c1->SetLogy(0);
  c1->Print(Form("Source/source_lskewgengaus_sum.png"));
  c1->Print(Form("Source/source_lskewgengaus_sum.pdf"));
  c1->SetLogy(1);
  c1->Print(Form("Source/source_lskewgengaus_sumlog.png"));
  c1->Print(Form("Source/source_lskewgengaus_sumlog.pdf"));


  // --- way too much fun is about to be had...
  TF1 *fun_Sr = new TF1("fun_Sr","(x<0.546)*([0]*x-[0]*0.546) + (x>0.546)*0",0,2.5);
  TF1 *fun_Y = new TF1("fun_Y","(x<2.28)*([0]*x-[0]*2.28) + (x>2.28)*0",0,2.5);
  double slope_Sr = -14/0.546;
  double slope_Y = -8/2.28;
  fun_Sr->SetParameter(0,slope_Sr);
  fun_Y->SetParameter(0,slope_Y);

  // --- Kurie
  TH1D *h_Sr = new TH1D("h_Sr","",100,0.0,2.5);
  TH1D *h_Y = new TH1D("h_Y","",100,0.0,2.5);
  // --- Kurie squared
  TH1D *h2_Sr = new TH1D("h2_Sr","",100,0.0,2.5);
  TH1D *h2_Y = new TH1D("h2_Y","",100,0.0,2.5);
  // --- Kurie squared times momentum squared
  TH1D *h2p_Sr = new TH1D("h2p_Sr","",100,0.0,2.5);
  TH1D *h2p_Y = new TH1D("h2p_Y","",100,0.0,2.5);
  // --- Kurie squared times momentum squared times Fermi function = number
  TH1D *h2pF_Sr = new TH1D("h2pF_Sr","",100,0.0,2.5);
  TH1D *h2pF_Y = new TH1D("h2pF_Y","",100,0.0,2.5);
  for(int i=0; i<100; i++)
    {
      double x = h_Sr->GetBinCenter(i+1);
      double spec_Sr = fun_Sr->Eval(x);
      double spec_Y = fun_Y->Eval(x);
      h_Sr->SetBinContent(i+1,spec_Sr);
      h_Y->SetBinContent(i+1,spec_Y);
      double sspec_Sr = spec_Sr**2;
      double sspec_Y = spec_Y**2;
      h2_Sr->SetBinContent(i+1,sspec_Sr);
      h2_Y->SetBinContent(i+1,sspec_Y);
      double m = 0.511;
      double E = x+m;
      double p2 = E*E - m*m; // momentum squared
      double p = sqrt(p2); // momentum
      h2p_Sr->SetBinContent(i+1,p2*sspec_Sr);
      h2p_Y->SetBinContent(i+1,p2*sspec_Y);
      // ---
      double pi = 3.14159265;
      double alpha = 1.0/137.0; // fine structure constant
      double Z = 38; // atomic number
      double eta = -alpha*Z*E/p; // variable for fermi function
      double F_N = 2*pi*eta/(1-exp(-2*pi*eta)); // non relativistic Fermi function
      double gamma = E/m;
      double S = sqrt(1-alpha*alpha*Z*Z)-1; // variable for relativistic fermi function
      double relcorr = pow(E*E*(1+4*gamma*gamma)-1,S); // relativist correction
      double F = F_N*relcorr; // relativistic Fermi function
      h2pF_Sr->SetBinContent(i+1,F*p2*sspec_Sr);
      h2pF_Y->SetBinContent(i+1,F*p2*sspec_Y);
    }

  c1->SetLogy(0);
  TH1D *h2pF_sum = (TH1D*)h2pF_Sr->Clone();
  h2pF_sum->Add(h2pF_Y);
  h2pF_sum->Scale(100);
  hsum->GetFunction("skewgengaus")->SetBit(TF1::kNotDraw);
  hsum->Draw();
  //h2pF_sum->GetXaxis()->SetLimits(0,75); // default 0,2.5...
  h2pF_sum->SetLineColor(kRed);
  h2pF_sum->SetLineWidth(2);
  h2pF_sum->GetXaxis()->SetLimits(20,105); // default 0,2.5...
  h2pF_sum->Draw("same");
  c1->Print("monday.png");

  h2pF_sum->GetXaxis()->SetLimits(0,75); // default 0,2.5...
  h2pF_sum->Draw("same");
  c1->Print("monday2.png");

  h2pF_sum->GetXaxis()->SetLimits(0,120); // default 0,2.5...
  h2pF_sum->Draw("same");
  c1->Print("monday3.png");

}
