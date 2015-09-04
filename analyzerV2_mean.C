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

  // doana("20150820-1156_A4_LED_NoBlock");
  // doana("20150820-1625_A4_LED_PanelAir");
  // doana("20150821-1312_A4_LED_NoBlock");
  // doana("20150821-1431_A4_LED_Panel");
  // doana("20150821-1550_A4_LED_PanelAir");
  // doana("20150821-1651_A4_LED_PanelAirClad");
  // doana("20150824-1552_A4_LED_PanelAirClad");
  // doana("20150824-1701_A4_LED_Panel");
  // doana("20150824-1750_A4_LED_NoBlock");
  // doana("20150825-1133_A4_LED_NoBlock");
  // doana("20150825-1422_A4_LED_PanelAirClad");
  // doana("20150825-1515_A4_LED_PanelAirClad");
  // doana("20150825-1622_A4_LED_NoBlock");
  // doana("20150825-1733_A4_LED_Panel");
  // doana("20150826-0945_A4_LED_NoBlock");
  // doana("20150826-1055_A4_LED_Panel");
  // doana("20150826-1334_A4_LED_PanelAirClad");
  // doana("20150826-1505_A4_LED_PanelAir");
  // doana("20150827-0946_A4_LED_PanelAir");
  // doana("20150827-1402_A4_Source_PanelAir");
  // doana("20150828-1609_A4_Source_PanelAir");
  // doana("20150830-1452_A4_Source_PanelAir");
  doana("20150831-1730_A1_Source");
  doana("20150901-0950_A1_Source");
  doana("20150901-1748_A1_LED");
  doana("20150902-1005_A1_LED");

}


void doana(const char *basename)
{

  char *sipm1name = Form("%s_VMIN_SIPM1",basename);
  char *sipm2name = Form("%s_VMIN_SIPM2",basename);
  char *timename = Form("%s_TIME.txt",basename);
  bool doPEConvert = true;
  double PEvalue = 0.004386;

  analyze(sipm1name,timename,doPEConvert,PEvalue);
  analyze(sipm2name,timename,doPEConvert,PEvalue);

}


void analyze(const char* NAME, const char* timedata, bool PEConvert, double PE)
{

  ////////////////////////////////////////////////////////////////////////////////
  //MODIFY FOR SIZE OF PANEL. CHECK TOTAL FILES MANUALLY FOR COMPATIBILITY HERE.//
  ////////////////////////////////////////////////////////////////////////////////

  int statistics = 1500; //Total pulse events per location

  // CHECK THESE WHEN USING
  int scan_nxpositions = 58;
  int scan_nypositions = 9;

  // ADJUST AS NEEDED
  //double PE = 0.004386; //Measured value of PE peak spacing. -- // this is SiPM 1

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
  //cout<<Form("Data/Text/%s",timedata)<<endl;;

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

  c1->Print(Form("Figures/Burn/%s_meanHist.png",NAME));
  c2->Print(Form("Figures/Burn/%s_meanHistSub.png",NAME));
  c1->Print(Form("Figures/Burn/%s_meanHist.pdf",NAME));
  c2->Print(Form("Figures/Burn/%s_meanHistSub.pdf",NAME));

  delete c1;
  delete c2;

}
