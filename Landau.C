void Landau()
{

  doit();

}

void doit()
{

  double height = 1.0;
  double mu = 0.0;
  double sigma = 0.005;
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
  c1->Print("smooth.png");

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
  c1->Print("random.png");

  TH1D *h2 = new TH1D("h2","",100,min,max);
  ifstream fin("TEMP/20150909-1724_Unaveraged_VMin1.txt");
  for(int i=0; i<number; i++)
    {
      double voltage = -9999;
      fin>>voltage;
      voltage *= -1;
      if(voltage<9999) h2->Fill(voltage);
    }
  h2->Draw();
  c1->Print("voltage.png");

}
