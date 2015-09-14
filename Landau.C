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
  fun->SetParameter(1,0.12); // adjust manually
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

}
