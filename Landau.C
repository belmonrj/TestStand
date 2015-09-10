void Landau()
{

  doit();

}

void doit()
{

  double height = 100.0;
  double mu = 0.0;
  double sigma = 1.0;
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
  c1->Print("blah.png");

}
