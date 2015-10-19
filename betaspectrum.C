void betaspectrum()
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
