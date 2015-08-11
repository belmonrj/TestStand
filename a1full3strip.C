void a1full3strip()
{

  ifstream fin1("A1_Full_3strip_NewSetup_VMin_SIPM1.txt");
  ifstream fin2("A1_Full_3strip_NewSetup_VMin_SIPM2.txt");

  float current1_1[58];
  float current1_2[58];
  float current1_3[58];
  float current1_A[58];

  float current2_1[58];
  float current2_2[58];
  float current2_3[58];
  float current2_A[58];

  for(int i=0; i<58; i++)
    {
      fin1>>current1_1[i];
      fin2>>current2_1[i];
    }
  for(int i=0; i<58; i++)
    {
      fin1>>current1_2[i];
      fin2>>current2_2[i];
    }
  for(int i=0; i<58; i++)
    {
      fin1>>current1_3[i];
      fin2>>current2_3[i];
    }
  fin1.close();
  fin2.close();

  for(int i=0; i<58; i++)
    {
      current1_A[i] = (current1_1[i]+current1_2[i]+current1_3[i])/3.0;
      current2_A[i] = (current2_1[i]+current2_2[i]+current2_3[i])/3.0;
    }

  float position[58];
  for(int i=0; i<58; i++)
    {
      position[i] = i*float(25.0/58);
      //cout<<i<<" "<<position[i]<<endl;
    }

  TGraph *tg1 = new TGraph(58,position,current1_A);
  tg1->SetMarkerStyle(kFullCircle);
  tg1->SetMarkerColor(kRed);
  TGraph *tg2 = new TGraph(58,position,current2_A);
  tg2->SetMarkerStyle(kFullCircle);
  tg2->SetMarkerColor(kBlue);

  tg1->Draw("ap");
  tg2->Draw("p");

  c1->Print("sipm12.png");

  float comb[58];
  for(int i=0; i<58; i++)
    {
      comb[i] = current2_A[i] - current1_A[i];
      comb[i] /= current2_A[i] + current1_A[i];
    }

  TGraph *tgC = new TGraph(58,position,comb);
  tgC->SetMarkerStyle(kFullCircle);
  tgC->SetMarkerColor(kBlack);
  tgC->Draw("ap");
  tgC->SetMinimum(-0.15);
  tgC->SetMaximum(0.15);
  tgC->GetXaxis()->SetLimits(0,25);

  c1->Print("sipmC.png");



}
