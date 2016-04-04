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

void doana(const char*, const char*, const int, const int, const bool); // without offsets...
void doana(const char*, const char*, const int, const int, const int, const int, const bool); // with offsets...
void analyze(const char*, const char*, const bool, const double, const int, const int); // without offsets...
void analyze(const char*, const char*, const bool, const double, const int, const int, const int, const int); // with offsets...

void PlotLEDscan()
{

  // doana("20151105-1515",58,32,true);
  // doana("20151106-1503",58,32,true);
  // doana("20151216-1647",44,30,false); // works well, but the data are not useful
  // doana("20151113-1313",48,30,false); // small angled tile
  // doana("20160106-1320",48,28,false); // small square-bottom tile, 405 nm
  // doana("20160106-1636",48,28,false); // small square-bottom tile, 375 nm
  // doana("20151026-1704",174,50,false); // large tile, 405 nm
  // doana("20160107-0222",174,50,false); // large tile, 375 nm, bad alignment
  // doana("20160107-1522",174,50,1,-1,false); // large tile, 361 nm
  // doana("20160111-1335",174,50,0,-1,false); // large tile, 375 nm, improved alignment...
  // doana("20160113-1238",174,50,0,0,false); // large tile, 405 nm, changed alignment...
  doana("20160329-1516","OH-2-47",174,54,0,-2,false); // large tile, 405 nm, changed alignment...
  doana("20160330-1235","OH-2-46",174,54,0,-1,false); // large tile, 405 nm, changed alignment...
  doana("20160403-1706","OH-2-6", 174,54,0,-2,false); // large tile, 405 nm, changed alignment...

}


void doana(const char *basename, const char *handle, const int nxbins, const int nybins, const int xoff, const int yoff, const bool dosipm2)
{

  char *sipm1name = Form("%s_VMIN_SIPM1",basename);
  char *sipm2name = Form("%s_VMIN_SIPM2",basename);
  bool doPEConvert = true;
  //double PEvalue = 0.004386; // old value
  double PEvalue = 0.00502; // trimmed mean from 9/14/2015

  analyze(sipm1name,handle,doPEConvert,PEvalue,nxbins,nybins,xoff,yoff);
  if(dosipm2) analyze(sipm2name,handle,doPEConvert,PEvalue,nxbins,nybins,xoff,yoff);

}


void analyze(const char* NAME, const char* handle, const bool PEConvert, const double PE, const int nxbins, const int nybins, const int xoff, const int yoff)
{

  // ----------------------------------------------------------------------------------- //
  // --- MODIFY FOR SIZE OF PANEL, CHECK TOTAL FILES MANUALLY FOR COMPATIBILITY HERE --- //
  // ----------------------------------------------------------------------------------- //

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
  file.open(Form("Data/%s.txt",NAME));
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

  // ----------------------------------------------------------------------------------------
  //cout << "minimum1 is " << minimum1 <<  " minimum2 is " << minimum2 << " and est background is " << AvgBackgroundRate << endl;
  cout << "minimum1 is " << minimum1 <<  " minimum2 is " << minimum2 << endl;
  // ----------------------------------------------------------------------------------------

  // --- make the 2d histograms and use the background subtraction
  for(int j = 0; j < meanSize; j++)
    {
      int row = j%nybins;
      int column = j/nybins;
      double iMean = means[j];
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
      meanHistSub->SetBinContent(column + 1 + xoff, row + 1 + yoff, iMeanSub);
    }
  // --- write the background subtracted histogram to a ROOT file
  //meanHistSub->SaveAs(Form("Data/NewLED/%s_meanHistSub.root",NAME)); // NEEDS TO BE REVISED



  // ---------------------------------------------- //
  // --- now make figures of the 2d panel plots --- //
  // ---------------------------------------------- //

  int canvasx = 900;
  int canvasy = 588;
  if ( nxbins == 174 )
    {
      canvasx = 1800;
      canvasy = 522;
    }

  TCanvas *c1 = new TCanvas("c1","c1", canvasx, canvasy);

  gStyle->SetOptStat(0);

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

  c1->SetRightMargin(0.15);
  gStyle->SetOptTitle(1);
  meanHistSub->SetTitle(Form("Tile ID: %s",handle));
  meanHistSub->GetXaxis()->SetTitle("Position Y (cm)");
  meanHistSub->GetYaxis()->SetTitle("Position X (cm)");
  if (PEConvert == true) meanHistSub->GetZaxis()->SetTitle("Pulse Min Value (photoelectrons)");
  else meanHistSub->GetZaxis()->SetTitle("Pulse Min Value (V)");
  //meanHistSub->GetZaxis()->SetMinimum(-0.001); // to correct color scale for bins with exactly zero value
  meanHistSub->SetMinimum(-0.001); // to correct color scale for bins with exactly zero value
  meanHistSub->Draw("colz");
  // c1->Print(Form("Figs/LEDscan_%s_meanHistSub.png",NAME));
  // c1->Print(Form("Figs/LEDscan_%s_meanHistSub.pdf",NAME));
  c1->Print(Form("Figs/LEDscan_%s.png",handle));
  c1->Print(Form("Figs/LEDscan_%s.pdf",handle));

  delete c1;

}
