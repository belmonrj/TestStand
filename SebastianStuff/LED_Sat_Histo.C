void LED_Sat_Histo()
{

  TCanvas *C1 = new TCanvas();
  C1->Divide(1,2);

  //  TF1 *Func1 = new TF1("Func1","pol2",0,1700);
  //  TF1 *Func2 = new TF1("Func2","pol4",0,1700);
  //  TNtuple *nt = new TNtuple("nt","nt",bias:voutHG:voutLG);

  ifstream fin1;
  ifstream fin2;
  fin1.open("HG_LED_Sat_20160216.txt");
  fin2.open("LG_LED_Sat_20160216.txt");

  double volts1;
  double volts2;
  vector<double>hg_volts;
  vector<double>lg_volts;

  while(fin1>>volts1)
    {
      hg_volts.push_back(volts1);
    }

  while(fin2>>volts2)
    {
      lg_volts.push_back(volts2);
    }

  int entries_hg = hg_volts.size();
  int entries_lg = lg_volts.size();

  double led_bias_hg[entries_hg];
  double led_bias_lg[entries_lg];
  double ratio[entries_hg];
  double PEHG = 1;
  double PELG = 1;


  for (int i=0;i<entries_hg;i++)
   {
     led_bias_hg[i] = 800 + (10*i);
   }

  for (int i=0;i<entries_lg;i++)
   {
     led_bias_lg[i] = 800 + (10*i);
   }

  if (entries_hg != entries_lg)
    {
      cout << " HG and LG of different length " << endl;
    }
  for(int i=0; i<entries_hg;i++)
    {
      ratio[i] = (hg_volts[i])/(lg_volts[i]);
    }

  TGraph *TG1 = new TGraph();
  TGraph *TG2 = new TGraph();
  TGraph *TG3 = new TGraph();

  for(int i = 0;i<entries_hg;i++)
    {
      TG1->SetPoint(i,led_bias_lg[i],lg_volts[i]);
      TG2->SetPoint(i,led_bias_hg[i],hg_volts[i]);
      TG3->SetPoint(i,led_bias_lg[i],ratio[i]);
    } 


  C1->cd(1);
  TG1->SetMarkerStyle(kFullCircle);
  TG1->SetMarkerColor(kRed);
  TG1->SetTitle("LED Bias Voltage vs Scope Output");
  TG1->GetXaxis()->SetTitle("LED Bias Voltage (mV)");
  TG1->GetYaxis()->SetTitle("Scope Output (mV)");
  TG1->GetXaxis()->CenterTitle(true);
  TG1->GetYaxis()->CenterTitle(true);
  TG1->GetYaxis()->SetTitleOffset(1.2);
  TG1->SetMinimum(-10);
  TG1->SetMaximum(1200);

  TG2->SetMarkerStyle(kFullTriangleUp);
  TG2->SetMarkerColor(kBlue);

  TG1->Draw("ap");
  TG2->Draw("psame");

  leg = new TLegend(0.1,0.7,0.3,.9);
  leg->AddEntry(TG1,"Low Gain","p");
  leg->AddEntry(TG2,"High Gain","p");
  //  leg->AddEntry(Func1,"Quadratic Fit","l");
  //  leg->AddEntry(Func2,"Quartic Fit","l");
  
  leg->Draw();

  //  TG1->Fit(Func2,"R");
  //  TG1->Fit(Func1,"R");
  //  Func2->SetLineColor(4);
  //  Func2->Draw("same");
 
 
  C1->cd(2);
  TG3->SetTitle("LED Bias Voltage vs Ratio of HG/LG");
  TG3->GetXaxis()->SetTitle("LED Bias Voltage (mV)");
  TG3->GetYaxis()->SetTitle("Ratio of HG/LG");
  TG3->GetXaxis()->CenterTitle(true);
  TG3->GetYaxis()->CenterTitle(true);
  TG3->GetYaxis()->SetTitleOffset(1.2);

  TG3->SetMarkerStyle(kFullCircle);
  TG3->Draw("ap");


  //  leg2 = new TLegend(0.1,0.7,0.3,.9);
  //  leg2->AddEntry(TG3,"Ratio HG/LG","p");
  //  leg2->Draw();

 

}
  
