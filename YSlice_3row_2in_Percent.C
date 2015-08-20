//==========================================================================
// xSlice - Macro to read in a 2D histogram, take a slice, and
// project it for comparison.
// author:  Sebastian Seeds
//==========================================================================

//STD
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

//Root
#include "TAxis.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TPaletteAxis.h"
#include "TPad.h"
#include "TLine.h"
#include "TArc.h"
#include "TEllipse.h"
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

using namespace std;

//Macro which makes two defined cuts on the y-axis of a TH2D. These cuts produce two TH1D histograms with a window about the center value defined by percent deviation from the mean.

void ySlice(char loc[20], int yLoc_A = 7, int yLoc_B = 15, double percent = 100)
{
  //Reading in histogram and defining dimensions

  TFile* inFile = new TFile(Form("%s.root",loc), "read");

  inFile->ls();

  TH2D * h1 = (TH2D*)inFile->Get(Form("%s",loc));
  if(h1==NULL)
    {
      cout<<"no histogram available, exiting"<<endl;
      return;
    }

  int xPos = h1->GetNbinsX();
  int yPos = h1->GetNbinsY();

  //Defining TH1Ds

  TH1D * p1 = new TH1D("p1",Form("X Projection, Averaged over 3 Rows at Y = %d, With %.1f %% Window About the Mean at the Middle",yLoc_A, percent), xPos, 0.0, xPos/2);
  TH1D * p2 = new TH1D("p2",Form("X Projection, Averaged over 3 Rows at Y = %d, With %.1f %% Window About the Mean at the Middle",yLoc_B, percent), xPos, 0.0, xPos/2);

  double projBin1 = 0.0;
  double projBin2 = 0.0;

  //Filling TH1D from first location on Y
  for (int x = 1; x <= xPos; x++)
    {
      projBin1 = 0.0;
      for (int y = yLoc_A; y <= (yLoc_A+3); y++)
	{
	  int h1Bin = h1->GetBin(x,y);
	  projBin1 += h1->GetBinContent(h1Bin);
	}
      p1->SetBinContent(x,projBin1/3);
      cout << "From p1: " << projBin1 << endl;
    }

  //Filling TH1D from second location on Y
  for (int x = 1; x <= xPos; x++)
    {
      projBin2 = 0.0;
      for (int y = yLoc_B; y <= (yLoc_B+3); y++)
	{
	  int h1Bin = h1->GetBin(x,y);
	  projBin2 += h1->GetBinContent(h1Bin);
	}
      p2->SetBinContent(x,projBin2/3);
      cout << "From p2: " << projBin2 << endl;
    }

  int NumberOfBins = p1->GetNbinsX();
  int middleLoc = NumberOfBins/2;
  double summ1 = 0;
  double avg1 = 0;
  double summ2 = 0;
  double avg2 = 0;

  //Getting mean and setting window about the center value
  for (int x = middleLoc - 2; x <= middleLoc + 2; x++)
    {
      double BinCont1 = p1->GetBinContent(x);
      summ1 = summ1 + BinCont1;
    }
  avg1 = summ1/5;

  for (int x = middleLoc - 2; x <= middleLoc + 2; x++)
    {
      double BinCont2 = p2->GetBinContent(x);
      summ2 = summ2 + BinCont2;
    }
  avg2 = summ2/5;

  p1->SetMaximum((1 + percent/100)*avg1);
  p1->SetMinimum((1 - percent/100)*avg1);
  p2->SetMaximum((1 + percent/100)*avg2);
  p2->SetMinimum((1 - percent/100)*avg2);

  cout<<"Average PE 1:"<<avg1<<endl<<"Average PE 2:"<<avg2<<endl;

  TCanvas *c1 = new TCanvas("c1","c1",800, 588);
  TCanvas *c2 = new TCanvas("c2","c2",800, 588);

  gStyle->SetOptStat(0);

  c1->cd();

  p1->GetXaxis()->SetTitle("Position Across Panel (cm)");
  p1->GetYaxis()->SetTitle("Mean Number of Photoelectrons");
  p1->Draw("");

  c2->cd();

  p2->GetXaxis()->SetTitle("Position Across Panel (cm)");
  p2->GetYaxis()->SetTitle("Mean Number of Photoelectrons");
  p2->Draw("");

}
