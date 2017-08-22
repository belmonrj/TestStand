void LED_LowGain_Extended()
{
  TCanvas *c1 = new TCanvas();
  c1->SetGrid();
  c1->Divide(1,2);



  ifstream fin1;
  ifstream fin2;
  ifstream fin3;
  ifstream fin4;  
  ifstream fin5; 
  ifstream fin6; 
  fin1.open("LG_LED_Sat_Extended_20160224.txt");
  fin2.open("Led_Bias_LG_Extended_20160224.txt");
  fin3.open("Slope_LG_Extended_20160224.txt");
  fin4.open("Occupancy.txt");
  fin5.open("DataTheifLuminosity.csv");
  fin6.open("DataTheifCurrent.csv");
  
  double volts;
  double bias;
  double slope;
  double occ;
  double lum;
  double cur;
  double cur2;
  double fvolt;
  vector<double>lg_volts;
  vector<double>lg_bias;
  vector<double>lg_slope;
  vector<double>occupancy;
  vector<double>luminosity;
  vector<double>current;
  vector<double>current2;
  vector<double>ForVolt;


  while(fin1>>volts)
    {
      lg_volts.push_back(volts);
    }
  while(fin2>>bias)
    {
      lg_bias.push_back(bias);
    }
  while(fin3>>slope)
    {
      lg_slope.push_back(slope);
    }
  while(fin4>>occ)
    {
      occupancy.push_back(occ);
    }
  while(fin5>>cur>>lum)
    {
      luminosity.push_back(lum);
      current.push_back(cur);
    }
  while(fin6>>cur2>>fvolt)
    {
      current2.push_back(cur2);
      ForVolt.push_back(fvolt);
    }


  int entries = lg_volts.size();
  int OccupancyEntries = occupancy.size();
  int LumEntries = luminosity.size();
  int ForVoltEntries = ForVolt.size();
  double OccSpacing = (lg_bias[0]-lg_bias[entries])/OccupancyEntries;

  TGraph *TG1 = new TGraph();
  TGraph *TG2 = new TGraph();
  TGraph *TG3 = new TGraph();
  TGraph *TG4 = new TGraph();
  TGraph *TG5 = new TGraph();
  TGraph *TG6 = new TGraph();

  for (int i=0; i<entries; i++)
    {
      TG1->SetPoint(i,lg_bias[i],lg_volts[i]);
      TG2->SetPoint(i,lg_bias[i],lg_slope[i]);
    }
  double extrafactor = 1.1;
 for (int i=0; i<OccupancyEntries; i++)
    {
      TG3->SetPoint(i,lg_bias[0] + OccSpacing*extrafactor*i,1550*occupancy[i]);
      //      TG3->Scale();
    }
 for (int i=0; i<OccupancyEntries; i++)
    {
      TG4->SetPoint(i,lg_bias[0] + OccSpacing*(2.2)*i,1550*occupancy[i]);
      //      TG3->Scale();
    }
 for (int i=0; i<LumEntries; i++)
    {
      TG5->SetPoint(i,current[i],luminosity[i]);
      //      TG3->Scale();
    }
 for (int i=0; i<ForVoltEntries; i++)
    {
      TG6->SetPoint(i,ForVolt[i],Current2[i]);
      //      TG3->Scale();
    }

  c1->cd(1);
  TG1->SetMarkerStyle(kFullCircle);
  TG1->SetMarkerColor(kBlue);
  TG1->SetTitle("Extended Low Gain: LED Bias vs Scope Output");
  TG1->GetYaxis()->SetTitle("Scope Output (mV)");
  TG1->GetXaxis()->SetTitle("LED Bias (mV)");
  TG1->GetXaxis()->SetLabelSize(0.04);
  TG1->GetYaxis()->SetLabelSize(0.04);
  TG1->GetXaxis()->CenterTitle(true);
  TG1->GetYaxis()->CenterTitle(true);
  TG1->Draw();
  TG3->SetMarkerStyle(kOpenCircle);
  TG3->Draw("p");
  //  TLine *line = new TLine(0,1560,10000,1560);
  //  line->Draw();


  c1->cd(2);

  TG1->SetMarkerStyle(kFullCircle);
  TG1->SetMarkerColor(kBlue);
  TG4->SetMarkerStyle(kOpenCircle);
  TG1->Draw();
  TG4->Draw("p");
  //  TG2->SetMarkerStyle(kFullCircle);
  //  TG2->SetTitle("Scope Output Rate of Increase Per Step in VBias ");
  //  TG2->GetYaxis()->SetTitle("Slope between each data point");
  //  TG2->GetXaxis()->SetTitle("LED Bias (mV)");
  //  TG2->GetXaxis()->CenterTitle(true);
  //  TG2->GetYaxis()->CenterTitle(true);
  //  TG2->Draw();


}
