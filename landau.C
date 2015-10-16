void landau()
{

  // dorandom(1000000,0.1,0.05);
  // dorandom(1000000,0.2,0.05);
  // dorandom(1000000,0.3,0.05);
  // dorandom(1000000,0.4,0.05);
  // dorandom(1000000,0.5,0.05);
  // dorandom(1000000,0.6,0.05);

  // dorandom(1000000,0.6,0.05);
  // dorandom(1000000,0.6,0.10);
  // dorandom(1000000,0.6,0.15);
  // dorandom(1000000,0.6,0.20);
  // dorandom(1000000,0.6,0.25);
  // dorandom(1000000,0.6,0.30);


  dorandom(1000000,20,1);
  dorandom(1000000,20,2);
  dorandom(1000000,20,3);
  dorandom(1000000,20,4);
  dorandom(1000000,20,5);
  dorandom(1000000,20,6);
  dorandom(1000000,20,7);
  dorandom(1000000,20,8);
  dorandom(1000000,20,9);
  dorandom(1000000,20,10);

}

void dorandom(int number = 2000,  double mu = 0.1,   double sigma = 0.05)
{

  gRandom->SetSeed(0);

  // --- parameters for fit
  double height = 1.0; // ...
  double min = mu - sigma*5;
  //double max = mu + sigma*25; // mean calc tuned for 25...
  double max = mu + sigma*50;

  // --- next make a histogram and fill with random values weighted by the Landau distribution
  TH1D *h1 = new TH1D("h1","",100,min,max);
  TF1 *fun = new TF1("fun","[0]*TMath::Landau(x,[1],[2])",min,max);
  fun->SetParameter(0,height);
  fun->SetParameter(1,mu);
  fun->SetParameter(2,sigma);
  fun->SetParameter(0,number*0.3); // give good initial guess

  double mode = mu - 0.22278*sigma;

  for(int i=0; i<number; i++)
    {
      double value = gRandom->Landau(mu,sigma);
      h1->Fill(value);
    }
  double mean = h1->GetMean();
  double predictedmean = 2.86754*sigma + mode;
  h1->Draw();
  //h1->Fit(fun,"R");
  h1->GetXaxis()->SetTitle("Independent variable");
  h1->GetYaxis()->SetTitle("Counts");
  TLatex *tex1 = new TLatex(0.45,0.8,Form("#mu_{in} = %f, #mu_{out} = %f",mu,fun->GetParameter(1)));
  tex1->SetNDC();
  tex1->Draw();
  TLatex *tex2 = new TLatex(0.45,0.75,Form("#sigma_{in} = %f, #sigma_{out} = %f",sigma,fun->GetParameter(2)));
  tex2->SetNDC();
  tex2->Draw();
  TLatex *tex3 = new TLatex(0.45,0.7,Form("%d random throws",number));
  tex3->SetNDC();
  tex3->Draw();
  TLatex *tex4 = new TLatex(0.45,0.65,Form("mode = %f",mode));
  tex4->SetNDC();
  tex4->Draw();
  TLatex *tex5 = new TLatex(0.45,0.6,Form("mean_{histo} #approx %f",mean));
  tex5->SetNDC();
  tex5->Draw();
  // TLatex *tex6 = new TLatex(0.45,0.55,Form("mode/mean #approx %f",mode/h1->GetMean()));
  // TLatex *tex6 = new TLatex(0.45,0.55,Form("(mean-mode)/sigma #approx %f",(mean-mode)/sigma));
  TLatex *tex6 = new TLatex(0.45,0.55,Form("mean_{predicted} = %f",predictedmean));
  tex6->SetNDC();
  tex6->Draw();
  TLatex *tex7 = new TLatex(0.45,0.50,Form("%% diff on mean = %f%%",100*fabs(mean-predictedmean)/mean));
  tex7->SetNDC();
  tex7->Draw();
  height = fun->GetParameter(0);
  //TLine line1(mu-0.25*sigma,0.0,mu-0.25*sigma,0.18*height);
  TLine line1(mode,0.0,mode,0.18*height);
  line1.Draw();
  TLine line2(mu-1.6*sigma,0.09*height,mu+2.4*sigma,0.09*height);
  line2.Draw();
  TLine line3(min,0.18*height,max,0.18*height);
  line3.Draw();
  c1->Print(Form("Figures/Distribution/landau_%d_mu%f_sigms%f_random.pdf",number,mu,sigma));
  c1->Print(Form("Figures/Distribution/landau_%d_mu%f_sigms%f_random.png",number,mu,sigma));

  cout << (mean-mode)/sigma << endl;

  delete h1;
  delete fun;



  TF1 *fun = new TF1("fun","[0]*TMath::Landau(x,[1],[2])",min,0.5*max);
  fun->SetParameter(0,height);
  fun->SetParameter(1,mu);
  fun->SetParameter(2,sigma);

  double height2 = 0.748*height;
  double mu2 = 0.9*mu;
  double sigma2 = 1.05*sigma;

  TF1 *fun2 = new TF1("fun2","([0]/sqrt(6.28))*TMath::Exp(-0.5*((x-[1])/[2] + TMath::Exp(-(x-[1])/[2])))",min,0.5*max);
  fun2->SetParameter(0,height2);
  fun2->SetParameter(1,mu2);
  fun2->SetParameter(2,sigma2);

  fun->Draw();
  fun2->SetLineColor(kBlue);
  fun2->Draw("same");

  c1->Print("Figures/Distribution/landau_parameterized.png");
  c1->Print("Figures/Distribution/landau_parameterized.pdf");

  TLine line1(mu2,0,mu2,height*0.18);
  line1.Draw();

  //TLine line2(height*0.09,mu2,height*0.09,mu2+sigma2);
  TLine line2(mu2,height*0.09,mu2+sigma2,height*0.09);
  line2.Draw();

  c1->Print("Figures/Distribution/landau_parameterized2.png");
  c1->Print("Figures/Distribution/landau_parameterized2.pdf");

}

