//==========================================================================
// HCalTestStand - program to analyze SiPM Source Readout Tests
// CVS version control on Colorado local cluster 
// 
// authors:  Shawn Beckman - Sebastian Seeds
//==========================================================================

// STL                                                                                                         
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
// C                                                                                                           
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
// ROOT
#include <TAxis.h>
#include <TPad.h>
#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TNtuple.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TMath.h>
#include <TSpectrum.h>
#include <TList.h>
#include <TPaveStats.h>
#include <TGaxis.h>
#include <TRandom.h>
#include <TSystem.h>
#include <TProfile.h>
#include <TObject.h>
#include <TString.h>

////////////////////////////////////////////////////////////////////
// organizing files, and defining things before running over data //
////////////////////////////////////////////////////////////////////

int analyzerV2_mean(const char* datafile, const char* timedata, bool PEConvert = true) {

////////////////////////////////////////////////////////////////////////////////
//MODIFY FOR SIZE OF PANEL. CHECK TOTAL FILES MANUALLY FOR COMPATIBILITY HERE.// 
////////////////////////////////////////////////////////////////////////////////

  int statistics = 1500; //Total pulse events per location 
  
  int scan_nxpositions = 58; 
  int scan_nypositions = 9; 

  double PE = .004386; //Measured value of PE peak spacing.

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

  file.open(Form("%s",datafile));
  
  for(int i = 0; i < totalBins ; i++)
    {
      string meanval_s;
      getline(file,meanval_s);
      double meanval_d = stod(meanval_s);
      means.push_back(-1*meanval_d);
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
      
  timefile.open(Form("%s",timedata));
  
  for(int i = 0; i < totalBins ; i++)  //Gets time data
    {
      string timeval_s;
      getline(timefile,timeval_s);
      double timeval_d = stod(timeval_s);
      times.push_back(timeval_d);
    }
  
  timefile.close();
      
  int timeSize = times.size();

  double TimeSum = 0;
  
  for(int i=0; i<timeSize; i++){  //Gets total time for first column
    
    int row = i%scan_nypositions;
    int column = i/scan_nypositions;
    
    
    if (column == 1){
      TimeSum += times[i];
      
    }
  }
  
  double TimeMean = TimeSum/scan_nypositions;
  
  ////////////////////////////////////////////////////////////
  char Name[] = "meanHistSub"; //Name the saved histogram HERE
  ////////////////////////////////////////////////////////////

  double distanceX = scan_nxpositions/2; //For 2 steps per cm. Check Stepper code for agreement
  double distanceY = scan_nypositions/2;
    
  TH2D *meanHist = new TH2D("meanHist", "meanHist", scan_nxpositions,0.0,distanceX, scan_nypositions,0.0,distanceY); // Modify numbers for cm scale on panel.
  
  TH2D *meanHistSub = new TH2D(Form("%s",Name), Form("%s",Name), scan_nxpositions,0.0,distanceX, scan_nypositions,0.0,distanceY); // Modify numbers for cm scale on panel.
  
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
    
  meanHistSub->SaveAs(Form("%s.root",Name));

  TCanvas *c1 = new TCanvas("c1","c1", 900, 588);
  TCanvas *c2 = new TCanvas("c2","c2", 900, 588);

  gStyle->SetOptStat(0);

  c1->cd();
  c1->SetRightMargin(0.15);
  meanHist->SetTitle(Form("Mean %d events",statistics));
  meanHist->GetXaxis()->SetTitle("Position Y (cm)");
  meanHist->GetYaxis()->SetTitle("Position X (cm)");
  if (PEConvert == true)
    {
      meanHist->GetZaxis()->SetTitle("Pulse Min Value (photoelectrons)");
    }else
    {
      meanHist->GetZaxis()->SetTitle("Pulse Min Value (V)");
    }
  meanHist->Draw("colz");
  
  c2->cd();
  c2->SetRightMargin(0.15);
  meanHistSub->SetTitle(Form("Mean %d events - Background Subtracted",statistics));
  meanHistSub->GetXaxis()->SetTitle("Position Y (cm)");
  meanHistSub->GetYaxis()->SetTitle("Position X (cm)");
  if (PEConvert == true)
    {
      meanHistSub->GetZaxis()->SetTitle("Pulse Min Value (photoelectrons)");
    }else
    {
      meanHistSub->GetZaxis()->SetTitle("Pulse Min Value (V)");
    }
  meanHistSub->Draw("colz");
  
  return 0;
}

