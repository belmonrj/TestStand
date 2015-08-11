void a1full3strip()
{

  // read in the data files for SiPM 1 and 2
  // data file is single column format
  // first 58 rows correspond to first stripe along the panel, second
  ifstream fin1("A1_Full_3strip_NewSetup_VMin_SIPM1.txt");
  ifstream fin2("A1_Full_3strip_NewSetup_VMin_SIPM2.txt");

  // arrays for current values from SiPM 1
  float current1_1[58]; // row 1
  float current1_2[58]; // row 2
  float current1_3[58]; // row 3
  float current1_A[58]; // average of all three rows

  // arrays for current values from SiPM 2
  float current2_1[58]; // row 1
  float current2_2[58]; // row 2
  float current2_3[58]; // row 3
  float current2_A[58]; // average of all three rows

  // read in the first row
  for(int i=0; i<58; i++)
    {
      fin1>>current1_1[i];
      fin2>>current2_1[i];
    }
  // read in the second row
  for(int i=0; i<58; i++)
    {
      fin1>>current1_2[i];
      fin2>>current2_2[i];
    }
  // read in the third row
  for(int i=0; i<58; i++)
    {
      fin1>>current1_3[i];
      fin2>>current2_3[i];
    }
  // close the file now that we're finished
  fin1.close();
  fin2.close();

  // calculate the averages over the three rows
  for(int i=0; i<58; i++)
    {
      current1_A[i] = (current1_1[i]+current1_2[i]+current1_3[i])/3.0;
      current2_A[i] = (current2_1[i]+current2_2[i]+current2_3[i])/3.0;
    }

  // calculate the actual panel position based on the bin number
  float position[58];
  for(int i=0; i<58; i++)
    {
      position[i] = i*float(25.0/58);
    }

  // make TGraphs for plotting the points (averages)
  TGraph *tg1 = new TGraph(58,position,current1_A);
  tg1->SetMarkerStyle(kFullCircle);
  tg1->SetMarkerColor(kRed);
  TGraph *tg2 = new TGraph(58,position,current2_A);
  tg2->SetMarkerStyle(kFullCircle);
  tg2->SetMarkerColor(kBlue);
  // draw
  tg1->Draw("ap");
  tg2->Draw("p");
  // print to file
  c1->Print("sipm12.png");

  // calculate the asymmetry between the two
  float comb[58];
  for(int i=0; i<58; i++)
    {
      comb[i] = current2_A[i] - current1_A[i];
      comb[i] /= current2_A[i] + current1_A[i];
    }

  // make TGraph for the asymmetry
  TGraph *tgC = new TGraph(58,position,comb);
  tgC->SetMarkerStyle(kFullCircle);
  tgC->SetMarkerColor(kBlack);
  tgC->Draw("ap");
  tgC->SetMinimum(-0.15);
  tgC->SetMaximum(0.15);
  tgC->GetXaxis()->SetLimits(0,25);
  // print to file
  c1->Print("sipmC.png");



}
