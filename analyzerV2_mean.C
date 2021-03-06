//==========================================================================
// HCalTestStand - program to analyze SiPM Source Readout Tests
// CVS version control on Colorado local cluster
//
// authors:  Shawn Beckman - Sebastian Seeds
//==========================================================================

// Modified by Ron Belmont
// local version control with git, will add to github repository

#include <algorithm> // for std::min_element
//#include <iterator> // for std::begin(vector), end

void doana(const char*);

void analyze(const char*, const char*, bool, double);

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
  // doana("20150831-1730_A1_Source");
  // doana("20150901-0950_A1_Source");
  // doana("20150901-1748_A1_LED");
  // doana("20150902-1005_A1_LED");
  // doana("20150909-1318_A1_LED");
  // doana("20150910-1215_A1_LED"); // looks bad, problem with background subtraction (alignment)
  // doana("20150910-1736_A1_Source"); // looks bad, problem with background subtraction (alignment)
  // doana("20150911-1004_A1_Source");
  doana("20150911-1150_A1_Source");
  doana("20150911-1328_A1_Source");
  doana("20150911-1607_A1_LED");
  doana("20150911-1700_A1_LED");
  doana("20150914-1138_A1_Source");
  doana("20150914-1750_A1_LED");

}


void doana(const char *basename)
{

  char *sipm1name = Form("%s_VMIN_SIPM1",basename);
  char *sipm2name = Form("%s_VMIN_SIPM2",basename);
  char *timename = Form("%s_TIME.txt",basename);
  bool doPEConvert = true;
  //double PEvalue = 0.004386; // old value
  double PEvalue = 0.00502; // trimmed mean from 9/14/2015

  analyze(sipm1name,timename,doPEConvert,PEvalue);
  analyze(sipm2name,timename,doPEConvert,PEvalue);

}


void analyze(const char* NAME, const char* timedata, bool PEConvert, double PE)
{

  // ----------------------------------------------------------------------------------- //
  // --- MODIFY FOR SIZE OF PANEL, CHECK TOTAL FILES MANUALLY FOR COMPATIBILITY HERE --- //
  // ----------------------------------------------------------------------------------- //

  // --- CHECK THESE WHEN USING
  int scan_nxpositions = 58;
  int scan_nypositions = 9;
  int totalBins = scan_nxpositions * scan_nypositions;


  // ------------------------------
  // --- now read in the event data
  // ------------------------------


  // ----------------------------
  // --- read in the voltage data
  int numb = 0;
  vector <double> means;
  string line;
  ifstream file;
  ifstream file_cnt;
  // ----------------------------
  file.open(Form("Data/Text/%s.txt",NAME));
  for(int i = 0; i < totalBins ; i++)
    {
      double meanval_d = -9999;
      file>>meanval_d;
      if ( PEConvert ) means.push_back(-1*meanval_d);
      else means.push_back(meanval_d);
      if(i==0) cout<<meanval_d<<endl;
    }
  file.close();
  int meanSize = means.size();
  double minimum1 = *min_element(means.begin(),means.end());
  double minimum2 = means[0];
  for(int i=0; i<meanSize; i++)
    {
      if(means[i]<minimum2) minimum2 = means[i];
    }


  // -------------------------
  // --- read in the time data
  int timenumb = 0;
  vector <double> times;
  string timeline;
  ifstream timefile;
  // -------------------------
  timefile.open(Form("Data/Text/%s",timedata));
  for(int i = 0; i < totalBins ; i++)
    {
      double timeval_d = -9999;
      timefile>>timeval_d;
      times.push_back(timeval_d);
      if(i==0) cout<<timeval_d<<endl;
    }
  timefile.close();
  int timeSize = times.size();

  double TimeSum = 0;  // get total time for first column
  for ( int i=0; i<timeSize; i++ )
    {
      int row = i%scan_nypositions;
      int column = i/scan_nypositions;
      if (column == 1) TimeSum += times[i];
    }
  double TimeMean = TimeSum/scan_nypositions;


  // ---

  // --- determine the distance based on the number of steps
  // --- default is 2 steps per cm, double check stepper code
  double distanceX = (double)scan_nxpositions/2;
  double distanceY = (double)scan_nypositions/2;
  // --- make the histogram for the unsubtracted
  TH2D *meanHist = new TH2D(Form("%s_meanHist",NAME), Form("%s_meanHist",NAME),
			    scan_nxpositions,0.0,distanceX, scan_nypositions,0.0,distanceY);
  // --- make the histogram for the background subtracted
  TH2D *meanHistSub = new TH2D(Form("%s_meanHistSub",NAME), Form("%s_meanHistSub",NAME),
			       scan_nxpositions,0.0,distanceX, scan_nypositions,0.0,distanceY);

  // --- calculate the background, used for noise subtraction
  double background = 0.0;
  for(int i = 0; i < meanSize; i++)
    {
      int row = i%scan_nypositions;
      int column = i/scan_nypositions;
      double iMean = means[i];
      // --- THIS IS A HUGE PROBLEM
      if(column == 1) background += iMean;
    }
  // --- this is a huge problem
  // only valid if first column is off panel
  background = background/scan_nypositions;
  double AvgBackgroundRate = background/TimeMean;

  // ----------------------------------------------------------------------------------------
  cout << "minimum1 is " << minimum1 <<  " minimum2 is " << minimum2 << " and est background is " << AvgBackgroundRate << endl;
  // ----------------------------------------------------------------------------------------

  // --- make the 2d histograms and use the background subtraction
  for(int j = 0; j < meanSize; j++)
    {
      int row = j%scan_nypositions;
      int column = j/scan_nypositions;
      double iMean = means[j];
      double iTime = times[j];
      double iMeanSub = iMean-(AvgBackgroundRate*iTime);
      // --- convert voltage to photoelectrons
      if(PEConvert)
	{
	  iMean /= PE;
	  iMeanSub /= PE;
	}
      // --- fill the histogram without the background subtraction
      meanHist->SetBinContent(column+1,row+1,iMean);
      // --- fill the histogram with the background subtraction
      meanHistSub->SetBinContent(column + 1, row + 1, iMeanSub);
    }
  // --- write the background subtracted histogram to a ROOT file
  meanHistSub->SaveAs(Form("Data/ROOT/%s_meanHistSub.root",NAME)); // NEEDS TO BE REVISED



  // ---------------------------------------------- //
  // --- now make figures of the 2d panel plots --- //
  // ---------------------------------------------- //

  TCanvas *c1 = new TCanvas("c1","c1", 900, 588);
  TCanvas *c2 = new TCanvas("c2","c2", 900, 588);

  gStyle->SetOptStat(0);

  c1->cd();
  c1->SetRightMargin(0.15);
  meanHist->GetXaxis()->SetTitle("Position Y (cm)");
  meanHist->GetYaxis()->SetTitle("Position X (cm)");
  if (PEConvert == true) meanHist->GetZaxis()->SetTitle("Pulse Min Value (photoelectrons)");
  else meanHist->GetZaxis()->SetTitle("Pulse Min Value (V)");
  meanHist->Draw("colz");

  c2->cd();
  c2->SetRightMargin(0.15);
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
