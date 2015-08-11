void plotfit()
{

  ifstream finR("rusty.dat");
  ifstream finS("sebastian.dat");

  float rustyX[9], rustyY[9];
  for(int i=0; i<9; i++)
    {
      finR>>rustyX[i]>>rustyY[i];
    }

  float sebastianX[11], sebastianY[11];
  for(int i=0; i<11; i++)
    {
      finS>>sebastianX[i]>>sebastianY[i];
    }

  finR.close();
  finS.close();

  TGraph *tgrusty = new TGraph(9,rustyX,rustyY);
  tgrusty->SetMarkerStyle(kFullCircle);
  tgrusty->SetMarkerColor(kBlack);

  TGraph *tgsebastian = new TGraph(11,sebastianX,sebastianY);
  tgsebastian->SetMarkerStyle(kFullSquare);
  tgsebastian->SetMarkerColor(kRed);

  TH2F *h2 = new TH2F("h2","",1,0,25,1,-0.5,0.5);
  h2->Draw();
  h2->GetXaxis()->SetTitle("Distance from SiPM");
  h2->GetYaxis()->SetTitle("Ratio of light yields");
  tgrusty->Draw("p");
  tgsebastian->Draw("p");

  TLegend *leg = new TLegend(0.18,0.68,0.38,0.88);
  leg->AddEntry(tgrusty,"ACU","p");
  leg->AddEntry(tgsebastian,"UCB","p");
  leg->Draw();


  // wrong coordinate system gives good looking fit
  //TF1 *fun = new TF1("fun","(([0]*TMath::Exp(x/[1])+(1-[0])*TMath::Exp(x/[2]))-([0]*TMath::Exp((25-x)/[1])+(1-[0])*TMath::Exp((25-x)/[2])))/(([0]*TMath::Exp(x/[1])+(1-[0])*TMath::Exp(x/[2]))+([0]*TMath::Exp((25-x)/[1])+(1-[0])*TMath::Exp((25-x)/[2])))",0,25);
  // correct (?) coordinate system gives much uglier fits
  //TF1 *fun = new TF1("fun","(([0]*TMath::Exp(-x/[1])+(1-[0])*TMath::Exp(-x/[2]))-([0]*TMath::Exp((x-25)/[1])+(1-[0])*TMath::Exp((x-25)/[2])))/(([0]*TMath::Exp(-x/[1])+(1-[0])*TMath::Exp(-x/[2]))+([0]*TMath::Exp((x-25)/[1])+(1-[0])*TMath::Exp((x-25)/[2])))",0,25);
  // correct (?) coordinate system gives much uglier fits
  TF1 *fun = new TF1("fun","(([0]*TMath::Exp((x-25)/[1])+(1-[0])*TMath::Exp((x-25)/[2]))-([0]*TMath::Exp(-x/[1])+(1-[0])*TMath::Exp(-x/[2])))/(([0]*TMath::Exp(-x/[1])+(1-[0])*TMath::Exp(-x/[2]))+([0]*TMath::Exp((x-25)/[1])+(1-[0])*TMath::Exp((x-25)/[2])))",0,25);
  fun->SetParameter(0,0.5); // light fraction
  // fun->SetParameter(1,300); // decay constant in fiber core
  //fun->SetParameter(2,3); // decay constant in fiber cladding
  fun->FixParameter(1,350); // decay constant in fiber core
  fun->FixParameter(2,4); // decay constant in fiber cladding


  //bool dofit = false;
  bool dofit = true;
  if(dofit)
    {

      tgsebastian->Fit("fun","","",0,25);

      double frac = fun->GetParameter(0);
      double core = fun->GetParameter(1);
      double clad = fun->GetParameter(2);

      cout<<frac<<" "<<core<<" "<<clad<<endl;

      TLatex *tex1 = new TLatex(7,-0.25,Form("f_{core} = %f",frac));
      tex1->SetTextColor(kRed);
      tex1->Draw();
      TLatex *tex2 = new TLatex(7,-0.325,Form("#lambda_{core} = %.1f (FIXED)",core));
      tex2->SetTextColor(kBlue);
      tex2->Draw();
      TLatex *tex3 = new TLatex(7,-0.4,Form("#lambda_{clad} = %.1f (FIXED)",clad));
      tex3->SetTextColor(kBlue);
      tex3->Draw();

      TF1 *fun2 = (TF1 *)fun->Clone();
      fun2->SetLineColor(kBlack);
      tgrusty->Fit(fun2,"","",0,25);

      double frac = fun2->GetParameter(0);
      double core = fun2->GetParameter(1);
      double clad = fun2->GetParameter(2);

      TLatex *tex1 = new TLatex(15,-0.25,Form("f_{core} = %f",frac));
      tex1->SetTextColor(kBlack);
      tex1->Draw();
      // TLatex *tex2 = new TLatex(7,-0.325,Form("#lambda_{core} = %f",core));
      // tex2->SetTextColor(kRed);
      // tex2->Draw();
      // TLatex *tex3 = new TLatex(7,-0.4,Form("#lambda_{clad} = %f",clad));
      // tex3->SetTextColor(kRed);
      // tex3->Draw();

      c1->Print("fig.png");

    }

  fun->Draw();

  c1->Print("fun.png");



  // fiber Y11, attenuation length >3.5m

}
