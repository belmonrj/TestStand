void plotfit()
{

  // create a new TCanvas
  TCanvas *c1 = new TCanvas();

  // read in the data from ACU
  float rustyX[9], rustyY[9];
  ifstream finR("rusty.dat");
  for(int i=0; i<9; i++)
    {
      finR>>rustyX[i]>>rustyY[i];
    }
  finR.close();

  // read in the data from UCB
  float sebastianX[11], sebastianY[11];
  ifstream finS("sebastian.dat");
  for(int i=0; i<11; i++)
    {
      finS>>sebastianX[i]>>sebastianY[i];
    }
  finS.close();

  // make the TGraph to draw the ACU data
  TGraph *tgrusty = new TGraph(9,rustyX,rustyY);
  tgrusty->SetMarkerStyle(kFullCircle);
  tgrusty->SetMarkerColor(kBlack);

  // make the TGraph to draw the UCB data
  TGraph *tgsebastian = new TGraph(11,sebastianX,sebastianY);
  tgsebastian->SetMarkerStyle(kFullSquare);
  tgsebastian->SetMarkerColor(kRed);

  // make a blank histogram to set the drawing range and axes
  TH2F *h2 = new TH2F("h2","",1,0,25,1,-0.5,0.5);
  h2->Draw();
  h2->GetXaxis()->SetTitle("Distance from SiPM");
  h2->GetYaxis()->SetTitle("Asymmetry of light yields");
  // draw the TGraphs
  tgrusty->Draw("p");
  tgsebastian->Draw("p");

  // make a legend
  TLegend *leg = new TLegend(0.18,0.68,0.38,0.88);
  leg->AddEntry(tgrusty,"ACU","p");
  leg->AddEntry(tgsebastian,"UCB","p");
  leg->Draw();

  // define a function to describe the data
  // this function defines and asymmetry B-A/A+B
  // it assumes the light yield has two components, core and cladding
  TF1 *fun = new TF1("fun","(([0]*TMath::Exp((x-25)/[1])+(1-[0])*TMath::Exp((x-25)/[2]))-([0]*TMath::Exp(-x/[1])+(1-[0])*TMath::Exp(-x/[2])))/(([0]*TMath::Exp(-x/[1])+(1-[0])*TMath::Exp(-x/[2]))+([0]*TMath::Exp((x-25)/[1])+(1-[0])*TMath::Exp((x-25)/[2])))",0,25);
  fun->SetParameter(0,0.5); // light fraction in fiber core
  fun->FixParameter(1,350); // decay constant in fiber core
  fun->FixParameter(2,5); // decay constant in fiber cladding
  //fun->FixParameter(2,4); // decay constant in fiber cladding

  // fit the UCB data
  tgsebastian->Fit("fun","","",0,25);

  // get the fit parameters
  double frac = fun->GetParameter(0);
  double core = fun->GetParameter(1);
  double clad = fun->GetParameter(2);
  double Efrac = fun->GetParError(0);

  // use the fit parameters to put text boxes with fit information on the plt
  TLatex *tex1 = new TLatex(7,-0.25,Form("f_{core} = %.3f #pm %.3f",frac,Efrac));
  tex1->SetTextColor(kRed);
  tex1->Draw();
  TLatex *tex2 = new TLatex(7,-0.325,Form("#lambda_{core} = %.1f (FIXED)",core));
  tex2->SetTextColor(kBlue);
  tex2->Draw();
  TLatex *tex3 = new TLatex(7,-0.4,Form("#lambda_{clad} = %.1f (FIXED)",clad));
  tex3->SetTextColor(kBlue);
  tex3->Draw();

  // clone the function to make a new one for a separate fit to the ACU data
  TF1 *fun2 = (TF1 *)fun->Clone();
  fun2->SetLineColor(kBlack);
  // fit the ACU data
  tgrusty->Fit(fun2,"","",0,25);
  // get the parameter
  frac = fun2->GetParameter(0);
  Efrac = fun2->GetParError(0);
  // draw the parameter info on the plot
  //TLatex *tex1 = new TLatex(15,-0.25,Form("f_{core} = %.3f #pm %.3f",frac,Efrac));
  TLatex *tex1 = new TLatex(16,-0.25,Form("f_{core} = %.3f #pm %.3f",frac,Efrac));
  tex1->SetTextColor(kBlack);
  tex1->Draw();
  // print the plot to a file and clear the canvas
  TLine line(0,0,25,0);
  line.SetLineWidth(2);
  line.SetLineStyle(2);
  line.Draw();
  c1->Print("acu-cub-comp_fig.png");
  c1->Print("acu-cub-comp_fig.pdf");
  c1->Clear();

  // define two linear functions to fit the data
  TF1 *funlin1 = new TF1("funlin1","pol1",0,25);
  TF1 *funlin2 = new TF1("funlin2","pol1",0,25);
  funlin2->SetLineColor(kBlack);
  h2->Draw();
  tgrusty->Draw("p");
  tgsebastian->Draw("p");
  // do the linear fits
  tgsebastian->Fit(funlin1,"","",0,25);
  tgrusty->Fit(funlin2,"","",0,25);
  // print to file and clear the canvas
  line.Draw();
  c1->Print("acu-cub-comp_figlinfit.png");
  c1->Print("acu-cub-comp_figlinfit.pdf");
  c1->Clear();



  // ---

  // Adjust parameters to play around and see what the output looks like
  TF1 *fun3 = (TF1 *)fun->Clone();
  fun3->SetParameter(0,0.5);
  fun3->FixParameter(1,350);
  fun3->FixParameter(2,4);
  fun3->Draw();
  line.Draw();
  c1->Print("fun.png");



  // fiber Y11, attenuation length >3.5m

}
