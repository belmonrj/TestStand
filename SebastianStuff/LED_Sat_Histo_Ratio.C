void LED_Sat_Histo_Ratio()
{


  TCanvas *C1 = new TCanvas();

  double biasHG[19] = {800,850,900,950,1000,1050,1100,1120,1130,1140,1150,1160,1170,1200,1300,1400,1500,1600,1700};
  double biasLG[19] = {800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1550,1600,1650,1700};
  double voutHG[19] = {5.5,19.2,71.0,183.0,366.2,614,924,1085,1162,1222,1300,1386,1558,1558,1558,1558,1558,1558};
  double voutLG[19] = {0,0,3.5,10.3,25,42,63,88,113,138,164,193,215,246,271,297,325,357,379};
  double ratio[7];
  double PlotBias[7] = {800,850,900,950,1000,1050,1100};
  double PEHG = 1;
  double PELG = 1;

  for (int i=0;i<19;i++)
    {
      voutLG[i] = voutLG[i]/PELG;
      voutHG[i] = voutHG[i]/PEHG;
    }

  for (int i = 0;i<7;i++)
    {
      ratio[i] = voutLG[i]/voutHG[i];
      cout << ratio[i]<<endl; 
      cout<<voutHG[i]<<endl;
    }

}
