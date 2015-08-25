//==========================================================================
// HCalTestStand - program to analyze SiPM Source Readout Tests
// CVS version control on Colorado local cluster
//
// authors:  Shawn Beckman - Sebastian Seeds
//==========================================================================

// Modified by Ron Belmont
// local version control with git, will add to github repository



void analyzerV2_mean()
{

  // analyze("A1_LED_ColTests_Fiber1_VMIN_SIPM1","A1_LED_ColTests_Fiber1_TIME.txt",true);
  // analyze("A1_LED_ColTests_Fiber2_VMIN_SIPM1","A1_LED_ColTests_Fiber2_TIME.txt",true);
  // analyze("A1_LED_ColTests_Fiber3_VMIN_SIPM1","A1_LED_ColTests_Fiber3_TIME.txt",true);
  // analyze("A1_LED_ColTests_Fiber4_VMIN_SIPM1","A1_LED_ColTests_Fiber4_TIME.txt",true);

  // analyze("A1_LED_ColTests_Fiber1_VMIN_SIPM2","A1_LED_ColTests_Fiber1_TIME.txt",true);
  // analyze("A1_LED_ColTests_Fiber2_VMIN_SIPM2","A1_LED_ColTests_Fiber2_TIME.txt",true);
  // analyze("A1_LED_ColTests_Fiber3_VMIN_SIPM2","A1_LED_ColTests_Fiber3_TIME.txt",true);
  // analyze("A1_LED_ColTests_Fiber4_VMIN_SIPM2","A1_LED_ColTests_Fiber4_TIME.txt",true);

  // ---

  // analyze("A3_LED_Assymetry_VMin_SiPM1","A3_LED_Assymetry_Time.txt",true);
  // analyze("A3_LED_Assymetry_VMin_SiPM2","A3_LED_Assymetry_Time.txt",true);

  // analyze("A3_LED_Assymetry_PannelBlock_VMin_SiPM1","A3_LED_Assymetry_PannelBlock_Time.txt",true);
  // analyze("A3_LED_Assymetry_PannelBlock_VMin_SiPM2","A3_LED_Assymetry_PannelBlock_Time.txt",true);

  // ---

  // analyze("A3_LED_Assymetry_CladBlock_Recouple_VMin_SiPM1",       "A3_LED_Assymetry_CladBlock_Recouple_Time.txt",       true);
  // analyze("A3_LED_Assymetry_NoBlock_Recouple_VMin_SiPM1",         "A3_LED_Assymetry_NoBlock_Recouple_Time.txt",         true);
  // analyze("A3_LED_Assymetry_PannelAirBlock_Recouple_VMin_SiPM1",  "A3_LED_Assymetry_PannelAirBlock_Recouple_Time.txt",  true);
  // analyze("A3_LED_Assymetry_PannelBlockOnly_Recouple_VMin_SiPM1", "A3_LED_Assymetry_PannelBlockOnly_Recouple_Time.txt", true);

  // analyze("A3_LED_Assymetry_CladBlock_Recouple_VMin_SiPM2",       "A3_LED_Assymetry_CladBlock_Recouple_Time.txt",       true);
  // analyze("A3_LED_Assymetry_NoBlock_Recouple_VMin_SiPM2",         "A3_LED_Assymetry_NoBlock_Recouple_Time.txt",         true);
  // analyze("A3_LED_Assymetry_PannelAirBlock_Recouple_VMin_SiPM2",  "A3_LED_Assymetry_PannelAirBlock_Recouple_Time.txt",  true);
  // analyze("A3_LED_Assymetry_PannelBlockOnly_Recouple_VMin_SiPM2", "A3_LED_Assymetry_PannelBlockOnly_Recouple_Time.txt", true);

  // ---

  analyze("20150824-1552_A3_LED_CladBlock_Recouple2_VMIN_SIPM1","20150824-1552_A3_LED_CladBlock_Recouple2_TIME.txt",true);
  analyze("20150824-1552_A3_LED_CladBlock_Recouple2_VMIN_SIPM2","20150824-1552_A3_LED_CladBlock_Recouple2_TIME.txt",true);
  analyze("20150824-1701_A3_LED_PanelBlock_Recouple2_VMIN_SIPM1","20150824-1701_A3_LED_PanelBlock_Recouple2_TIME.txt",true);
  analyze("20150824-1701_A3_LED_PanelBlock_Recouple2_VMIN_SIPM2","20150824-1701_A3_LED_PanelBlock_Recouple2_TIME.txt",true);
  analyze("20150824-1750_A3_LED_NoBlock_Recouple2_VMIN_SIPM1","20150824-1750_A3_LED_NoBlock_Recouple2_TIME.txt",true);
  analyze("20150824-1750_A3_LED_NoBlock_Recouple2_VMIN_SIPM2","20150824-1750_A3_LED_NoBlock_Recouple2_TIME.txt",true);

  analyze("20150825-1133_A3_LED_NoBlock_VMIN_SIPM1","20150825-1133_A3_LED_NoBlock_TIME.txt",true);
  analyze("20150825-1133_A3_LED_NoBlock_VMIN_SIPM2","20150825-1133_A3_LED_NoBlock_TIME.txt",true);

  // ---

}



void analyze(const char* NAME, const char* timedata, bool PEConvert = true)
{

  ////////////////////////////////////////////////////////////////////////////////
  //MODIFY FOR SIZE OF PANEL. CHECK TOTAL FILES MANUALLY FOR COMPATIBILITY HERE.//
  ////////////////////////////////////////////////////////////////////////////////

  int statistics = 1500; //Total pulse events per location

  // CHECK THESE WHEN USING
  int scan_nxpositions = 58;
  int scan_nypositions = 9;

  // ADJUST AS NEEDED
  double PE = 0.004386; //Measured value of PE peak spacing. -- // this is SiPM 1

  int totalBins = scan_nxpositions * scan_nypositions;

  double background = 0.0; //Defining for noise subtraction

  ////////////////////////////////////////////
  /////////////getting event data/////////////
  ////////////////////////////////////////////

  int numb = 0;
  vector <double> means;
  string line;
  ifstream file;
  ifstream file_cnt;

  file.open(Form("Data/Text/%s.txt",NAME));

  for(int i = 0; i < totalBins ; i++)
    {
      // string meanval_s;
      // getline(file,meanval_s);
      // double meanval_d = std::stod(meanval_s);
      // means.push_back(-1*meanval_d);
      double meanval_d = -9999;
      file>>meanval_d;
      if ( PEConvert ) means.push_back(-1*meanval_d);
      else means.push_back(meanval_d);
      if(i==0) cout<<meanval_d<<endl;
    }

  file.close();

  int meanSize = means.size();

  /////////////////////////////////////////////////////////////////////////////
  //////////////////// Getting Average background Rate ////////////////////////
  /////////////////////////////////////////////////////////////////////////////

  int timenumb = 0;
  vector <double> times;
  string timeline;
  ifstream timefile;

  timefile.open(Form("Data/Text/%s",timedata));

  for(int i = 0; i < totalBins ; i++)  //Gets time data
    {
      // string timeval_s;
      // getline(timefile,timeval_s);
      // double timeval_d = std::stod(timeval_s);
      // times.push_back(timeval_d);
      double timeval_d = -9999;
      timefile>>timeval_d;
      times.push_back(timeval_d);
      if(i==0) cout<<timeval_d<<endl;
    }

  timefile.close();

  int timeSize = times.size();

  double TimeSum = 0;

  for ( int i=0; i<timeSize; i++ )
    {  //Gets total time for first column

      int row = i%scan_nypositions;
      int column = i/scan_nypositions;

      if (column == 1) TimeSum += times[i];
    }

  double TimeMean = TimeSum/scan_nypositions;

  ////////////////////////////////////////////////////////////
  //char NAME[] = "meanHistSub"; //Name the saved histogram HERE
  ////////////////////////////////////////////////////////////

  double distanceX = (double)scan_nxpositions/2; //For 2 steps per cm. Check Stepper code for agreement
  double distanceY = (double)scan_nypositions/2;

  TH2D *meanHist = new TH2D(Form("%s_meanHist",NAME), Form("%s_meanHist",NAME),
			    scan_nxpositions,0.0,distanceX, scan_nypositions,0.0,distanceY); // Modify numbers for cm scale on panel.

  TH2D *meanHistSub = new TH2D(Form("%s_meanHistSub",NAME), Form("%s_meanHistSub",NAME),
			       scan_nxpositions,0.0,distanceX, scan_nypositions,0.0,distanceY); // Modify numbers for cm scale on panel.

  for(int i = 0; i < meanSize; i++)
    {

      int row = i%scan_nypositions;
      int column = i/scan_nypositions;
      double iMean = means[i];

      if(column == 1)
	{
	  background += iMean;  // Gets total event data for first column
	}

      meanHist->SetBinContent(column+1,row+1,iMean);  //Fills unsubtracted histogram
      if (PEConvert == true)
	{
	  meanHist->SetBinContent(column+1,row+1,iMean/PE);
	}
    }

  background = background/scan_nypositions;
  double AvgBackgroundRate = background/TimeMean; //Only valid if first column is off panel

  for(int j = 0; j < meanSize; j++)
    {
      int row = j%scan_nypositions;
      int column = j/scan_nypositions;

      double iMean = means[j];
      double iTime = times[j];
      double iMeanSubPE = iMean-(AvgBackgroundRate*iTime);

      if (PEConvert == true)
	{
	  iMeanSubPE = (iMean-(AvgBackgroundRate*iTime))/PE;
	}

      meanHistSub->SetBinContent(column + 1, row + 1, iMeanSubPE);
    }

  meanHistSub->SaveAs(Form("Data/ROOT/%s_meanHistSub.root",NAME)); // NEEDS TO BE REVISED

  TCanvas *c1 = new TCanvas("c1","c1", 900, 588);
  TCanvas *c2 = new TCanvas("c2","c2", 900, 588);

  gStyle->SetOptStat(0);

  c1->cd();
  c1->SetRightMargin(0.15);
  meanHist->SetTitle(Form("Mean %d events",statistics));
  meanHist->GetXaxis()->SetTitle("Position Y (cm)");
  meanHist->GetYaxis()->SetTitle("Position X (cm)");
  if (PEConvert == true) meanHist->GetZaxis()->SetTitle("Pulse Min Value (photoelectrons)");
  else meanHist->GetZaxis()->SetTitle("Pulse Min Value (V)");
  meanHist->Draw("colz");

  c2->cd();
  c2->SetRightMargin(0.15);
  meanHistSub->SetTitle(Form("Mean %d events - Background Subtracted",statistics));
  meanHistSub->GetXaxis()->SetTitle("Position Y (cm)");
  meanHistSub->GetYaxis()->SetTitle("Position X (cm)");
  if (PEConvert == true) meanHistSub->GetZaxis()->SetTitle("Pulse Min Value (photoelectrons)");
  else meanHistSub->GetZaxis()->SetTitle("Pulse Min Value (V)");
  meanHistSub->Draw("colz");

  c1->Print(Form("Figures/%s_meanHist.png",NAME));
  c2->Print(Form("Figures/%s_meanHistSub.png",NAME));
  c1->Print(Form("Figures/%s_meanHist.pdf",NAME));
  c2->Print(Form("Figures/%s_meanHistSub.pdf",NAME));

  delete c1;
  delete c2;

}
