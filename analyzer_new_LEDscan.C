//==========================================================================
// HCalTestStand - program to analyze SiPM Source Readout Tests
// CVS version control on Colorado local cluster
//
// authors:  Shawn Beckman - Sebastian Seeds
//==========================================================================

// Modified by Ron Belmont
// local version control with git, also on github repository (username belmonrj)

#include <algorithm> // for std::min_element
//#include <iterator> // for std::begin(vector), end

void doana(const char*, const int, const int);
void analyze(const char*, const char*, bool, double, const int, const int);

void analyzer_new_LEDscan()
{

  doana("20151105-1515",58,32);
  doana("20151106-1503",58,32);
  doana("20151216-1647",44,30);

}


void doana(const char *basename, const int nxbins, const int nybins)
{

  char *sipm1name = Form("%s_VMIN_SIPM1",basename);
  char *sipm2name = Form("%s_VMIN_SIPM2",basename);
  char *timename = Form("%s_TIME.txt",basename);
  bool doPEConvert = true;
  //double PEvalue = 0.004386; // old value
  double PEvalue = 0.00502; // trimmed mean from 9/14/2015

  analyze(sipm1name,timename,doPEConvert,PEvalue,nxbins,nybins);
  analyze(sipm2name,timename,doPEConvert,PEvalue,nxbins,nybins);

}


void analyze(const char* NAME, const char* timedata, bool PEConvert, double PE, const int nxbins, const int nybins)
{

  // ----------------------------------------------------------------------------------- //
  // --- MODIFY FOR SIZE OF PANEL, CHECK TOTAL FILES MANUALLY FOR COMPATIBILITY HERE --- //
  // ----------------------------------------------------------------------------------- //

  // --- CHECK THESE WHEN USING
  // --- IDEA: PASS THESE AS FUNCTION ARGUMENTS
  // int nxbins = 58; // small angled tile
  // int nybins = 32; // small angled tile
  // int nxbins = 44; // small square bottom tile
  // int nybins = 30; // small square bottom  tile
  int totalBins = nxbins * nybins;


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
  file.open(Form("TEMP/%s.txt",NAME));
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
  timefile.open(Form("TEMP/%s",timedata));
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
      int row = i%nybins;
      int column = i/nybins;
      if (column == 1) TimeSum += times[i];
    }
  double TimeMean = TimeSum/nybins;


  // ---

  // --- determine the distance based on the number of steps
  // --- default is 2 steps per cm, double check stepper code
  double distanceX = (double)nxbins/2;
  double distanceY = (double)nybins/2;
  // --- make the histogram for the unsubtracted
  TH2D *meanHist = new TH2D(Form("%s_meanHist",NAME), Form("%s_meanHist",NAME),
			    nxbins,0.0,distanceX, nybins,0.0,distanceY);
  // --- make the histogram for the background subtracted
  TH2D *meanHistSub = new TH2D(Form("%s_meanHistSub",NAME), Form("%s_meanHistSub",NAME),
			       nxbins,0.0,distanceX, nybins,0.0,distanceY);

  // --- calculate the background, used for noise subtraction
  double background = 0.0;
  for(int i = 0; i < meanSize; i++)
    {
      int row = i%nybins;
      int column = i/nybins;
      double iMean = means[i];
      // --- THIS IS A HUGE PROBLEM
      if(column == 1) background += iMean;
    }
  // --- this is a huge problem
  // only valid if first column is off panel
  background = background/nybins;
  double AvgBackgroundRate = background/TimeMean;

  // ----------------------------------------------------------------------------------------
  cout << "minimum1 is " << minimum1 <<  " minimum2 is " << minimum2 << " and est background is " << AvgBackgroundRate << endl;
  // ----------------------------------------------------------------------------------------

  // --- make the 2d histograms and use the background subtraction
  for(int j = 0; j < meanSize; j++)
    {
      int row = j%nybins;
      int column = j/nybins;
      double iMean = means[j];
      double iTime = times[j];
      //double iMeanSub = iMean-(AvgBackgroundRate*iTime);
      double iMeanSub = iMean-minimum1; // new method...
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
  meanHistSub->SaveAs(Form("Data/NewLED/%s_meanHistSub.root",NAME)); // NEEDS TO BE REVISED



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
