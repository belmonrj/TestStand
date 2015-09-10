void Landau()
{

  doit("20150910-1357");

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
  TLine line1(mu-0.25*sigma,0.0,mu-0.25*sigma,0.18*height);
  line1.Draw();
  TLine line2(mu-1.6*sigma,0.09*height,mu+2.4*sigma,0.09*height);
  line2.Draw();
  TLine line3(min,0.18*height,max,0.18*height);
  line3.Draw();
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
  c1->Print(Form("Figures/Distribution/%s_random.png",basename));


  // --- now make a histogram with actual data in it
  TH1D *h2 = new TH1D("h2","",100,min,max);
  ifstream fin2(Form("TEMP/%s_Unaveraged_VMin1.txt",basename));
  for(int i=0; i<number; i++)
    {
      double voltage = -9999;
      fin2>>voltage;
      voltage *= -1;
      if(voltage<9999) h2->Fill(voltage);
    }
  h2->Draw();
  // --- draw a figure with just the histogram
  c1->Print(Form("Figures/Distribution/%s_voltage.png",basename));
  //h2->Fit(fun,"R"); // looks bad...
  fun->SetParameter(0,1000); // adjust manually
  fun->Draw("same");
  // --- draw a figure with the Landau overlaid
  c1->Print(Form("Figures/Distribution/%s_foltage.png",basename));


  // --- now read in the data file for the other SiPM to draw the distributions together
  TH1D *h3 = new TH1D("h3","",100,min,max);
  ifstream fin3(Form("TEMP/%s_Unaveraged_VMin2.txt",basename));
  for(int i=0; i<number; i++)
    {
      double voltage = -9999;
      fin3>>voltage;
      voltage *= -1;
      if(voltage<9999) h3->Fill(voltage);
    }
  h3->SetLineColor(kRed);
  h2->SetLineColor(kBlue);
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
  c1->Print(Form("Figures/Distribution/%s_woltage.png",basename));

}
