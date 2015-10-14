void Landau()
{

  dorandom(2000);
  return;
  // dorandom(10000);

  // doit("20150910-1357");
  // doit("20150915-1539");
  // doit("20150915-1557");
  // doit("20150915-1612");

  compare("20150915-1539","20150915-1612","20150915_FGBG");

  doit("20150921-1152");

  compare("20150921-1152","20150915-1612","20150921_FGBG",10);

}

void compare(const char *fgname, const char *bgname, const char *outname, const double extra = 1.0)
{

  // --- first draw a simple histogram to show the basic shape and properties of the Landau distribution
  double height = 1.0;
  double mu = 0.1;
  double sigma = 0.05;
  // double min = mu - sigma*5;
  // double max = mu + sigma*25;
  double min = mu - sigma*3;
  double max = mu + sigma*7;
  TF1 *fun = new TF1("fun","[0]*TMath::Landau(x,[1],[2])",min,max);
  fun->SetParameter(0,height);
  fun->SetParameter(1,mu);
  fun->SetParameter(2,sigma);


  int nbins = 50;
  // --- Signal SiPM1
  TH1D *h1 = new TH1D("h1","",nbins,min,max);
  ifstream fin1(Form("TEMP/%s_Unaveraged_VMin1.txt",fgname));
  int counter = 0;
  double voltage;
  while(fin1>>voltage)
    {
      h1->Fill(-voltage);
      counter++;
    }
  fin1.close();
  cout << counter << " events read in " << endl;
  // --- Signal SiPM2
  TH1D *h2 = new TH1D("h2","",nbins,min,max);
  ifstream fin2(Form("TEMP/%s_Unaveraged_VMin2.txt",fgname));
  counter = 0;
  double voltage;
  while(fin2>>voltage)
    {
      h2->Fill(-voltage);
      counter++;
    }
  fin2.close();
  cout << counter << " events read in " << endl;


  // --- Background SiPM1
  TH1D *h3 = new TH1D("h3","",nbins,min,max);
  ifstream fin3(Form("TEMP/%s_Unaveraged_VMin1.txt",bgname));
  counter = 0;
  while(fin3>>voltage)
    {
      h3->Fill(-voltage);
      counter++;
    }
  fin3.close();
  cout << counter << " events read in " << endl;
  // --- Background SiPM2
  TH1D *h4 = new TH1D("h4","",nbins,min,max);
  ifstream fin4(Form("TEMP/%s_Unaveraged_VMin2.txt",bgname));
  counter = 0;
  while(fin4>>voltage)
    {
      h4->Fill(-voltage);
      counter++;
    }
  fin4.close();
  cout << counter << " events read in " << endl;





  h1->SetLineColor(kRed);
  h2->SetLineColor(kBlue);
  h1->SetLineWidth(2);
  h2->SetLineWidth(2);

  h3->SetLineColor(kRed);
  h4->SetLineColor(kBlue);
  h3->SetLineWidth(2);
  h4->SetLineWidth(2);

  double fgrate = 1038.0; // Hz
  double bgrate = 27.0; // Hz
  double bgscalefactor = (bgrate/fgrate)*extra;

  h3->Scale(bgscalefactor);
  h4->Scale(bgscalefactor);

  // --- now do PE conversion
  double peconvert = 0.00502; // volts per photoelectrion
  h1->GetXaxis()->SetLimits(min/peconvert,max/peconvert);
  h2->GetXaxis()->SetLimits(min/peconvert,max/peconvert);
  h3->GetXaxis()->SetLimits(min/peconvert,max/peconvert);
  h4->GetXaxis()->SetLimits(min/peconvert,max/peconvert);
  h1->GetYaxis()->SetTitle("Counts");
  h2->GetYaxis()->SetTitle("Counts");
  h1->GetXaxis()->SetTitle("Number of photoelectrons");
  h2->GetXaxis()->SetTitle("Number of photoelectrons");

  // --- now draw
  double max1 = h1->GetMaximum();
  double max2 = h2->GetMaximum();
  double maxX = 0;
  if(max1>max2)
    {
      h1->Draw();
      h2->Draw("same");
      h3->Draw("same");
      h4->Draw("same");
      maxX = max1;
    }
  else
    {
      h2->Draw();
      h1->Draw("same");
      h3->Draw("same");
      h4->Draw("same");
      maxX = max2;
    }
  TLegend *leg = new TLegend(0.68,0.73,0.88,0.88);
  leg->AddEntry(h1,"SiPM1","l");
  leg->AddEntry(h2,"SiPM2","l");
  leg->Draw();
  TLatex *tex = new TLatex((0.25*max+min)/peconvert,0.15*maxX,"Scaled background");
  tex->Draw();
  TArrow *ar1 = new TArrow((0.25*max+min)/peconvert,0.15*maxX,(0.2*max+min)/peconvert,0.1*maxX,0.02,"|>");
  ar1->SetLineWidth(2);
  ar1->Draw();
  c1->Print(Form("Figures/Distribution/%s_pe.pdf",outname));
  c1->Print(Form("Figures/Distribution/%s_pe.png",outname));
  h1->SetMinimum(0.05);
  h2->SetMinimum(0.05);
  c1->SetLogy();
  c1->Print(Form("Figures/Distribution/%s_log_pe.pdf",outname));
  c1->Print(Form("Figures/Distribution/%s_log_pe.png",outname));

  c1->SetLogy(0);
  h1->SetMinimum(0.0);
  h2->SetMinimum(0.0);
  TH1D *h5 = (TH1D *)h1->Clone("h5");
  TH1D *h6 = (TH1D *)h2->Clone("h6");
  h5->Add(h3,-1.0);
  h6->Add(h4,-1.0);
  h5->SetLineStyle(2);
  h6->SetLineStyle(2);
  h5->SetLineWidth(3);
  h6->SetLineWidth(3);
  h5->Draw("same");
  h6->Draw("same");
  c1->Print(Form("Figures/Distribution/%s_pe_sub.pdf",outname));
  c1->Print(Form("Figures/Distribution/%s_pe_sub.png",outname));
  h1->SetMinimum(0.05);
  h2->SetMinimum(0.05);
  c1->SetLogy();
  c1->Print(Form("Figures/Distribution/%s_log_pe_sub.pdf",outname));
  c1->Print(Form("Figures/Distribution/%s_log_pe_sub.png",outname));
  c1->SetLogy(0);

  delete fun;
  delete h1;
  delete h2;
  delete h3;
  delete h4;
  delete tex;
  delete ar1;

}

void dorandom(int number = 2000)
{

  gRandom->SetSeed(0);

  // --- parameters for fit
  double height = 1.0;
  double mu = 0.1;
  double sigma = 0.05;
  double min = mu - sigma*5;
  double max = mu + sigma*25;

  // --- next make a histogram and fill with random values weighted by the Landau distribution
  TH1D *h1 = new TH1D("h1","",100,min,max);
  TF1 *fun = new TF1("fun","[0]*TMath::Landau(x,[1],[2])",min,max);
  fun->SetParameter(0,height);
  fun->SetParameter(1,mu);
  fun->SetParameter(2,sigma);
  fun->SetParameter(0,number*0.3); // give good initial guess

  fun->Draw();
  fun->GetXaxis()->SetTitle("Independent variable");
  fun->GetYaxis()->SetTitle("Probability amplitude");
  height = fun->GetParameter(0);
  TLine line1(mu-0.25*sigma,0.0,mu-0.25*sigma,0.18*height); // mode line
  line1.Draw();
  TLine line2(mu-1.6*sigma,0.09*height,mu+2.4*sigma,0.09*height); // FWHM line
  line2.Draw();
  TLine line3(min,0.18*height,max,0.18*height); // height/normalization line
  line3.Draw();
  c1->Print(Form("Figures/Distribution/Landau_smooth.pdf"));
  c1->Print(Form("Figures/Distribution/Landau_smooth.png"));

  for(int i=0; i<number; i++)
    {
      double value = gRandom->Landau(mu,sigma);
      h1->Fill(value);
    }
  h1->Draw();
  h1->Fit(fun,"R");
  h1->GetXaxis()->SetTitle("Independent variable");
  h1->GetYaxis()->SetTitle("Counts");
  TLatex *tex1 = new TLatex(mu+7*sigma,0.16.5*fun->GetParameter(0),Form("#mu_{in} = %f, #mu_{out} = %f",mu,fun->GetParameter(1)));
  tex1->Draw();
  TLatex *tex2 = new TLatex(mu+7*sigma,0.14.5*fun->GetParameter(0),Form("#sigma_{in} = %f, #sigma_{out} = %f",sigma,fun->GetParameter(2)));
  tex2->Draw();
  TLatex *tex3 = new TLatex(mu+7*sigma,0.12.5*fun->GetParameter(0),Form("%d random throws",number));
  tex3->Draw();
  TLatex *tex4 = new TLatex(mu+7*sigma,0.10.5*fun->GetParameter(0),Form("mean_{histo} = %f",h1->GetMean()));
  tex4->Draw();
  // TLatex *tex5 = new TLatex(mu+7*sigma,0.08.5*fun->GetParameter(0),Form("#mu_{out}/mean = %f",fun->GetParameter(1)/h1->GetMean()));
  // tex5->Draw();
  TLatex *tex5 = new TLatex(mu+7*sigma,0.08.5*fun->GetParameter(0),
			    Form("mode_{histo} = %f, mode_{fun} = %f",h1->GetBinCenter(h1->GetMaximumBin()),mu-0.25*sigma));
  tex5->Draw();
  height = fun->GetParameter(0);
  TLine line1(mu-0.25*sigma,0.0,mu-0.25*sigma,0.18*height);
  line1.Draw();
  TLine line2(mu-1.6*sigma,0.09*height,mu+2.4*sigma,0.09*height);
  line2.Draw();
  TLine line3(min,0.18*height,max,0.18*height);
  line3.Draw();
  c1->Print(Form("Figures/Distribution/Landau_%d_random.pdf",number));
  c1->Print(Form("Figures/Distribution/Landau_%d_random.png",number));



  delete h1;
  delete fun;

}

void doit(const char *basename)
{

  // --- first draw a simple histogram to show the basic shape and properties of the Landau distribution
  double height = 1.0;
  double mu = 0.1;
  double sigma = 0.05;
  double min = mu - sigma*5;
  double max = mu + sigma*25;
  TF1 *fun = new TF1("fun","[0]*TMath::Landau(x,[1],[2])",min,max);
  fun->SetParameter(0,height);
  fun->SetParameter(1,mu);
  fun->SetParameter(2,sigma);
  fun->Draw();
  fun->GetXaxis()->SetTitle("Independent variable");
  fun->GetYaxis()->SetTitle("Probability amplitude");
  TLine line1(mu-0.25*sigma,0.0,mu-0.25*sigma,0.18*height);
  line1.Draw();
  TLine line2(mu-1.6*sigma,0.09*height,mu+2.4*sigma,0.09*height);
  line2.Draw();
  TLine line3(min,0.18*height,max,0.18*height);
  line3.Draw();
  c1->Print(Form("Figures/Distribution/%s_smooth.pdf",basename));
  c1->Print(Form("Figures/Distribution/%s_smooth.png",basename));


  // --- next make a histogram and fill with random values weighted by the Landau distribution
  TH1D *h1 = new TH1D("h1","",100,min,max);
  int number = 2000; // number of "events"
  fun->SetParameter(0,number*0.3); // give good initial guess
  for(int i=0; i<number; i++)
    {
      double value = gRandom->Landau(mu,sigma);
      h1->Fill(value);
    }
  h1->Draw();
  h1->Fit(fun,"R");
  h1->GetXaxis()->SetTitle("Signal (V)");
  h1->GetYaxis()->SetTitle("Counts");
  TLatex *tex1 = new TLatex(mu+7*sigma,0.16.5*fun->GetParameter(0),Form("#mu_{in} = %f, #mu_{out} = %f",mu,fun->GetParameter(1)));
  tex1->Draw();
  TLatex *tex2 = new TLatex(mu+7*sigma,0.14.5*fun->GetParameter(0),Form("#sigma_{in} = %f, #sigma_{out} = %f",sigma,fun->GetParameter(2)));
  tex2->Draw();
  TLatex *tex3 = new TLatex(mu+7*sigma,0.12.5*fun->GetParameter(0),Form("%d random throws",number));
  tex3->Draw();
  c1->Print(Form("Figures/Distribution/%s_random.pdf",basename));
  c1->Print(Form("Figures/Distribution/%s_random.png",basename));


  // --- now make a histogram with actual data in it
  TH1D *h2 = new TH1D("h2","",100,min,max);
  ifstream fin2(Form("TEMP/%s_Unaveraged_VMin1.txt",basename));
  int counter = 0;
  double voltage;
  while(fin2>>voltage)
    {
      h2->Fill(-voltage);
      counter++;
    }
  fin2.close();
  cout << counter << " events read in " << endl;
  h2->Draw();
  h2->GetXaxis()->SetTitle("Signal (V)");
  h2->GetYaxis()->SetTitle("Counts");
  // --- draw a figure with just the histogram
  c1->Print(Form("Figures/Distribution/%s_voltage.pdf",basename));
  c1->Print(Form("Figures/Distribution/%s_voltage.png",basename));
  //h2->Fit(fun,"R"); // looks bad...
  fun->SetParameter(0,1000); // adjust manually
  fun->SetParameter(1,0.10); // adjust manually
  fun->SetParameter(2,0.05); // adjust manually
  fun->Draw("same");
  // --- draw a figure with the Landau overlaid
  c1->Print(Form("Figures/Distribution/%s_foltage.pdf",basename));
  c1->Print(Form("Figures/Distribution/%s_foltage.png",basename));


  // --- now read in the data file for the other SiPM to draw the distributions together
  TH1D *h3 = new TH1D("h3","",100,min,max);
  ifstream fin3(Form("TEMP/%s_Unaveraged_VMin2.txt",basename));
  counter = 0;
  while(fin3>>voltage)
    {
      h3->Fill(-voltage);
      counter++;
    }
  fin3.close();
  cout << counter << " events read in " << endl;
  h3->GetXaxis()->SetTitle("Signal (V)");
  h3->GetYaxis()->SetTitle("Counts");
  h3->SetLineColor(kRed);
  h2->SetLineColor(kBlue);
  h3->SetLineWidth(2);
  h2->SetLineWidth(2);
  double max3 = h3->GetMaximum();
  double max2 = h2->GetMaximum();
  if(max3>max2)
    {
      h3->Draw();
      h2->Draw("same");
    }
  else
    {
      h2->Draw();
      h3->Draw("same");
    }
  TLegend *leg = new TLegend(0.68,0.73,0.88,0.88);
  leg->AddEntry(h2,"SiPM1","l");
  leg->AddEntry(h3,"SiPM2","l");
  leg->Draw();
  c1->Print(Form("Figures/Distribution/%s_woltage.pdf",basename));
  c1->Print(Form("Figures/Distribution/%s_woltage.png",basename));
  fun->SetLineColor(kBlack);
  fun->Draw("same");
  // --- draw a figure with the Landau overlaid
  c1->Print(Form("Figures/Distribution/%s_boltage.pdf",basename));
  c1->Print(Form("Figures/Distribution/%s_boltage.png",basename));


  // --- now do PE conversion
  double peconvert = 0.00502;
  h2->GetXaxis()->SetLimits(min/peconvert,max/peconvert);
  h3->GetXaxis()->SetLimits(min/peconvert,max/peconvert);
  h2->GetXaxis()->SetTitle("Number of photoelectrons");
  h3->GetXaxis()->SetTitle("Number of photoelectrons");
  if(max3>max2)
    {
      h3->Draw();
      h2->Draw("same");
    }
  else
    {
      h2->Draw();
      h3->Draw("same");
    }
  leg->Draw();
  c1->Print(Form("Figures/Distribution/%s_pe.pdf",basename));
  c1->Print(Form("Figures/Distribution/%s_pe.png",basename));
  // --- needs some work
  // fun->Draw("same");
  // // --- draw a figure with the Landau overlaid
  // c1->Print(Form("Figures/Distribution/%s_fpe.pdf",basename));
  // c1->Print(Form("Figures/Distribution/%s_fpe.png",basename));



  delete h1;
  delete h2;
  delete h3;
  delete fun;

}
