void betaspectrum()
{

  histograms();
  //functionsonly();

}

void histograms()
{

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

  TH1D *h_sum = (TH1D*)h_Sr->Clone();
  h_sum->Add(h_Y);
  h_sum->Draw();
  h_sum->GetYaxis()->SetTitle("Fermi-Kurie variable (#sqrt{N/p^{2}F(E,Z)})");
  h_sum->GetXaxis()->SetTitle("#beta kinetic energy (MeV)");
  h_sum->SetLineColor(kBlack);
  h_Sr->SetLineColor(kGreen+2);
  h_Y->SetLineColor(kBlue);
  TLegend *leg = new TLegend(0.4,0.68,0.88,0.88);;
  leg->AddEntry(h_sum,"Combined (observed)","l");
  leg->AddEntry(h_Sr,"Strontium (isolated)","l");
  leg->AddEntry(h_Y,"Yttrium (isolated)","l");
  leg->SetTextSize(0.05);
  leg->Draw();
  h_Sr->Draw("same");
  h_Y->Draw("same");
  h_sum->Draw("same");
  c1->Print("Source/strontium_kurie.png");
  c1->Print("Source/strontium_kurie.pdf");

  TH1D *h2_sum = (TH1D*)h2_Sr->Clone();
  h2_sum->Add(h2_Y);
  h2_sum->Draw();
  h2_sum->GetXaxis()->SetTitle("#beta kinetic energy (MeV)");
  h2_Sr->Draw("same");
  h2_Y->Draw("same");
  c1->Print("Source/strontium_kurie2.png");

  TH1D *h2p_sum = (TH1D*)h2p_Sr->Clone();
  h2p_sum->Add(h2p_Y);
  h2p_sum->Draw();
  h2p_sum->GetXaxis()->SetTitle("#beta kinetic energy (MeV)");
  h2p_Sr->Draw("same");
  h2p_Y->Draw("same");
  c1->Print("Source/strontium_kurie2p2.png");

  TH1D *h2pF_sum = (TH1D*)h2pF_Sr->Clone();
  h2pF_sum->Add(h2pF_Y);
  h2pF_sum->Draw();
  h2pF_sum->GetYaxis()->SetTitle("Counts (arb. norm.)");
  h2pF_sum->GetXaxis()->SetTitle("#beta kinetic energy (MeV)");
  h2pF_sum->SetLineColor(kBlack);
  h2pF_Sr->SetLineColor(kGreen+2);
  h2pF_Y->SetLineColor(kBlue);
  h2pF_Sr->Draw("same");
  h2pF_Y->Draw("same");
  h2pF_sum->Draw("same");
  delete leg;
  TLegend *leg = new TLegend(0.62,0.68,0.88,0.88);;
  leg->AddEntry(h_sum,"Combined","l");
  leg->AddEntry(h_Sr,"Strontium","l");
  leg->AddEntry(h_Y,"Yttrium","l");
  leg->SetTextSize(0.05);
  leg->Draw();
  c1->Print("Source/strontium_kurie2p2F.png");
  c1->Print("Source/strontium_kurie2p2F.pdf");
  delete leg;
}

void functionsonly()
{

  TF1 *fun_Sr = new TF1("fun_Sr","(x<0.546)*([0]*x-[0]*0.546) + (x>0.546)*0",0,2.5);
  TF1 *fun_Y = new TF1("fun_Y","(x<2.28)*([0]*x-[0]*2.28) + (x>2.28)*0",0,2.5);
  TF1 *fun_comb = new TF1("fun_comb","(x<0.546)*([0]*x-[0]*0.546 + [1]*x-[1]*2.28) + (x>0.546 && x<2.28)*([1]*x-[1]*2.28) + (x>2.28)*0",0,2.5);

  // --- these slopes match the plot in Meyerhof, Phys Rev 74 (1946) 621
  double slope_Sr = -14/0.546;
  double slope_Y = -8/2.28;

  fun_Sr->SetParameter(0,slope_Sr);
  fun_Sr->SetLineColor(kGreen+2);
  fun_Y->SetParameter(0,slope_Y);
  fun_Y->SetLineColor(kBlue);

  fun_comb->SetParameter(0,slope_Sr);
  fun_comb->SetParameter(1,slope_Y);
  fun_comb->SetLineColor(kBlack);

  TH2D *h = new TH2D("h","",1,0.0,2.5,1,0.0,25.0);
  h->GetXaxis()->SetTitle("#beta kinetic energy (MeV)");
  //h->GetYaxis()->SetTitle("#sqrt{#frac{N}{p^{2}F(E,Z)}}");
  h->GetYaxis()->SetTitle("#sqrt{N/p^{2}F(E,Z)}");
  h->Draw();

  TLegend *leg = new TLegend(0.4,0.68,0.88,0.88);
  leg->AddEntry(fun_Sr,"Strontium (isolated)","l");
  leg->AddEntry(fun_Y,"Yttrium","l");
  leg->AddEntry(fun_comb,"Combined (observed)","l");
  leg->SetTextSize(0.05);
  leg->Draw();

  fun_Sr->Draw("same");
  fun_Y->Draw("same");
  fun_comb->Draw("same");

  c1->Print("spectrumbeta.png");
  c1->Print("spectrumbeta.pdf");



  // --- matches a plot I saw on the website for the Czech Technical University
  // h->GetYaxis()->SetLimits(0.0,60.0);
  // slope_Sr = -40/0.546;
  // slope_Y = -10/2.28;
  // fun_Sr->SetParameter(0,slope_Sr);
  // fun_Y->SetParameter(0,slope_Y);
  // fun_comb->SetParameter(0,slope_Sr);
  // fun_comb->SetParameter(1,slope_Y);
  // fun_Sr->Draw("same");
  // fun_Y->Draw("same");
  // fun_comb->Draw("same");
  // c1->Print("spectrum2beta.png");

  TF1 *ffun_comb = new TF1("ffun_comb","(x<0.546)*([0]*x-[0]*0.546 + [1]*x-[1]*2.28)**2 + (x>0.546 && x<2.28)*([1]*x-[1]*2.28)**2 + (x>2.28)*0",0,2.5);

  TH2D *h = new TH2D("h","",1,0.0,2.5,1,0.0,625.0);
  h->GetXaxis()->SetTitle("#beta kinetic energy (MeV)");
  //h->GetYaxis()->SetTitle("#frac{N}{p^{2}F(E,Z)}");
  h->GetYaxis()->SetTitle("N/p^{2}F(E,Z)");
  h->Draw();
  ffun_comb->SetParameter(0,slope_Sr);
  ffun_comb->SetParameter(1,slope_Y);
  ffun_comb->SetLineColor(kBlack);
  ffun_comb->Draw("same");

  c1->Print("squectrumbeta.png");
  c1->Print("squectrumbeta.pdf");


  TF1 *fffun_comb = new TF1("fffun_comb","(x<0.546)*((x+1)*(x+1)-(0.511*0.511))*([0]*x-[0]*0.546 + [1]*x-[1]*2.28)**2 + (x>0.546 && x<2.28)*((x+1)*(x+1)-(0.511*0.511))*([1]*x-[1]*2.28)**2 + (x>2.28)*0",0,2.5);

  TH2D *h = new TH2D("h","",1,0.0,2.5,1,0.0,625.0);
  h->GetXaxis()->SetTitle("#beta kinetic energy (MeV)");
  //h->GetYaxis()->SetTitle("#frac{N}{p^{2}F(E,Z)}");
  h->GetYaxis()->SetTitle("N/F(E,Z)");
  h->Draw();
  fffun_comb->SetParameter(0,slope_Sr);
  fffun_comb->SetParameter(1,slope_Y);
  fffun_comb->SetLineColor(kBlack);
  fffun_comb->Draw("same");

  c1->Print("fermitrumbeta.png");
  c1->Print("fermitrumbeta.pdf");


  TF1 *final_comb = new TF1("final_comb","(x<0.546)*(2*pi*(0.0072993*38*(x+1)/sqrt((x+1)*(x+1)-(0.511*0.511)))/(1-exp(-2*pi*(0.0072993*38*(x+1)/sqrt((x+1)*(x+1)-(0.511*0.511))))))*((x+1)*(x+1)-(0.511*0.511))*([0]*x-[0]*0.546 + [1]*x-[1]*2.28)**2 + (x>0.546 && x<2.28)*(2*pi*(0.0072993*38*(x+1)/sqrt((x+1)*(x+1)-(0.511*0.511)))/(1-exp(-2*pi*(0.0072993*38*(x+1)/sqrt((x+1)*(x+1)-(0.511*0.511))))))*((x+1)*(x+1)-(0.511*0.511))*([1]*x-[1]*2.28)**2 + (x>2.28)*0",0,2.5);

  TH2D *h = new TH2D("h","",1,0.0,2.5,1,0.0,900.0);
  h->GetXaxis()->SetTitle("#beta kinetic energy (MeV)");
  h->GetYaxis()->SetTitle("N");
  h->Draw();
  final_comb->SetParameter(0,slope_Sr);
  final_comb->SetParameter(1,slope_Y);
  final_comb->SetLineColor(kBlack);
  final_comb->Draw("same");

  c1->Print("finaltrumbeta.png");
  c1->Print("finaltrumbeta.pdf");


}
