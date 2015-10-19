void betaspectrum()
{



  TF1 *fun_Sr = new TF1("fun_Sr","(x<0.546)*([0]*x-[0]*0.546) + (x>0.546)*0",0,2.5);
  TF1 *fun_Y = new TF1("fun_Y","(x<2.28)*([0]*x-[0]*2.28) + (x>2.28)*0",0,2.5);

  //TF1 *fun_comb = new TF1("fun_comb","[0]*x-[0]*0.546 + [1]*x-[1]*2.28",0,2.5);
  //TF1 *fun_comb = new TF1("fun_comb","(x<0.546)*([0]*x-[0]*0.546 + [1]*x-[1]*2.28) + (x>0.546 && x<2.28)*([1]*x-[1]*2.28) + (x>2.28)*0",0,2.5);
  TF1 *fun_comb = new TF1("fun_comb","(x<0.546)*([0]*x-[0]*0.546 + [1]*x-[1]*2.28) + (x>0.546 && x<2.28)*([1]*x-[1]*2.28) + (x>2.28)*0",0,2.5);
  //TF1 *fun_comb = fun_Sr + fun_Y;

  double slope_Sr = -70.0;
  double slope_Y = -5.0;

  fun_Sr->SetParameter(0,slope_Sr);
  fun_Sr->SetLineColor(kGreen+2);
  fun_Y->SetParameter(0,slope_Y);
  fun_Y->SetLineColor(kBlue);

  fun_comb->SetParameter(0,slope_Sr);
  fun_comb->SetParameter(1,slope_Y);
  fun_comb->SetLineColor(kBlack);

  TH2D *h = new TH2D("h","",1,0.0,2.5,1,0.0,60.0);
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

  c1->Print("betaspectrum.png");

}
