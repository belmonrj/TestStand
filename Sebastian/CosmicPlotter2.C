#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TLine.h>
#include <TF1.h>
#include <TBox.h>
#include <TGraph.h>
#include <TH2.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

const double PE = 0.005;
const int panel_number = 990;
// const int steps_x = 92; // High Eta inner 964
// const int steps_y = 42; // High Eta inner 964
// const int steps_x = 180; // High Eta outer 945 and 990
// const int steps_y = 63; // High Eta outer 945 and 990
const int steps_x = 180; // High Eta outer 976
const int steps_y = 51; // High Eta outer 976
// const int steps_x = 84; // Inner High Eta 939, 955, and 965
// const int steps_y = 36; // Inner High Eta 939, 955, and 965
// const int steps_x = 174;
// const int steps_y = 54;
const double step_size = 0.5;
const int unscanned_columns = 110; // Inner High Eta 965

void doit(const int, const int, const int, const int, const double, const double, const char*, const char*);

void CosmicPlotter2()
{

  doit( 976, 180, 51, 110, 0.5, 0.005,
        "Data/HighEta_976_First_20170424_VMIN_SIPM1.txt",
        "Data/HighEta_976_Second_20170504_VMIN_SIPM1.txt");

}

void doit(const int panel_number, const int steps_x, const int steps_y, const int unscanned_columns,
          const double step_size, const double PE, const char* file1, const char* file2)
{
  TCanvas *c1 = new TCanvas("c1");
  TCanvas *c2 = new TCanvas("c2", "c2", 800, 450);
  TCanvas *c3 = new TCanvas("c3", "c3", 800, 450);
  TCanvas *c4 = new TCanvas("c4", "c4", 900, 450);
  TH1D *CosmicV1 = new TH1D("CosmicV1", "Cosmics: OH-2-47", 50, 0, 100);
  TH2D *Tile_Scan = new TH2D("Tile_Scan", "Tile_Scan", steps_x, 0, steps_x * step_size, steps_y, 0, steps_y * step_size);
  TH2D *Second_Half_Tile_Scan = new TH2D("Second_Half_Tile_Scan", "Second_Half_Tile_Scan", steps_x, 0, steps_x * step_size, steps_y, 0, steps_y * step_size);
  TH2D *Combined_Scan = new TH2D("SCombined_Scan", "Combined_Scan", steps_x + unscanned_columns, 0, (steps_x + unscanned_columns) * step_size, steps_y, 0, steps_y * step_size);
  TLegend *legcent = new TLegend(0.12, 0.6, 0.3, 0.88);

  ifstream CosmicTxt;
  ifstream CosmicTxt2;
  ifstream CosmicTxt3;
  // CosmicTxt.open("Data/HighEta_990_First_20170501_VMIN_SIPM1.txt"); // Tile 990
  // CosmicTxt.open("Data/HighEta_976_First_20170424_VMIN_SIPM1.txt"); // Tile 976
  // CosmicTxt.open("Data/HighEta_955_20170430_VMIN_SIPM1.txt"); // Tile 955
  // CosmicTxt.open("Data/HighEta_939_20170428_VMIN_SIPM1.txt"); // Tile 939
  // CosmicTxt.open("Data/HighEta_964_20170430_VMIN_SIPM1.txt"); // Tile 964
  // CosmicTxt.open("Data/HightEta_20170413_VMIN_SIPM1.txt"); // Tile 945
  // CosmicTxt.open("Data/HighEta_965_20170421_VMIN_SIPM1.txt"); // Tile 965
  CosmicTxt.open(file1);

///////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// Filling Histos /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

  double vout;
  double vout2;
  int data_sample_number = 0;
  int data_sample_number_2 = 0;
  int column_number = 0;
  int row_number = 0;

  while (CosmicTxt >> vout)
    //for ( int i = 0; i < 11340; ++i )
  {
    //CosmicTxt >> vout; // comment this out if using while loop
    column_number = data_sample_number / steps_y;
    row_number = data_sample_number % steps_y;
    CosmicV1->Fill(-vout / PE);
    Tile_Scan->Fill(column_number * step_size, row_number * step_size, -vout / PE);
    data_sample_number++;
  }

  CosmicTxt.close();
  // CosmicTxt2.open("Data/HighEta_20170414_VMIN_SIPM1.txt");

  // int last_column_scan_1 = column_number;
  // int length_second_scan = 0;

  // while (CosmicTxt2 >> vout)
  //   //for ( int i = 0; i < 11340; ++i )
  // {
  //   length_second_scan++; // trivial for for loop...
  // }

  // cout << "length_second_scan = " << length_second_scan << endl;

  // CosmicTxt2.close();
  //CosmicTxt3.open("Data/HighEta_20170414_VMIN_SIPM1.txt"); // Tile first scan
  // CosmicTxt3.open("Data/HighEta_964_20170430_VMIN_SIPM1.txt"); // Tile 964
  // CosmicTxt3.open("Data/HighEta_939_20170428_VMIN_SIPM1.txt"); // Tile 964
  // CosmicTxt3.open("Data/HighEta_955_20170430_VMIN_SIPM1.txt"); // Tile 955
  // CosmicTxt3.open("Data/HighEta_976_Second_20170504_VMIN_SIPM1.txt"); // Tile 976
  // CosmicTxt3.open("Data/HighEta_990_Second_20170502_VMIN_SIPM1.txt"); // Tile 990
  CosmicTxt3.open(file2); // Tile 990


  // int cutoff = length_second_scan - 5 * steps_y;
  // int cutoff_column = cutoff / steps_y;

  cout << "Starting Third While Loop" << endl;

  while (CosmicTxt3 >> vout2)
    //for ( int i = 0; i < 11340; ++i )
  {
    //CosmicTxt3 >> vout2;
    column_number = data_sample_number_2 / steps_y;
    row_number = data_sample_number_2 % steps_y;

    // if (data_sample_number_2 >= cutoff)
    // {
    //   CosmicV1->Fill(-vout / PE);
    //   Tile_Scan->Fill((last_column_scan_1 + (column_number - cutoff_column))* step_size, row_number * step_size, -vout / PE);
    // }

    Second_Half_Tile_Scan->Fill(column_number * step_size, row_number * step_size, -vout2 / PE);
    data_sample_number_2++;
  }

  CosmicTxt3.close();

///////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// Combining Histos /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

  int first_n_bins = (Tile_Scan->GetNbinsX()) * (Tile_Scan->GetNbinsY());
  int second_n_bins = (Second_Half_Tile_Scan->GetNbinsX()) * (Second_Half_Tile_Scan->GetNbinsY());
  int combined_n_bins = (Combined_Scan->GetNbinsX()) * (Combined_Scan->GetNbinsY());
  int combined_n_columns = steps_y + unscanned_columns;
  // int start_here = second_n_entries - unscanned_columns * steps_y;
  double PE_Val = 0;

  cout << "combined_n_bins = " << combined_n_bins << endl;

  for (int i = 1; i <= combined_n_bins; i++)
  {
    column_number = i / steps_y;
    row_number = i % steps_y;

    if (i <= first_n_bins)
    {
      PE_Val = Tile_Scan->GetBinContent(column_number, row_number);
      Combined_Scan->SetBinContent(column_number, row_number, PE_Val);
    }
    if (i > first_n_bins)
    {
      PE_Val = Second_Half_Tile_Scan->GetBinContent( steps_y - ((combined_n_columns) - column_number), row_number);
      Combined_Scan->SetBinContent(column_number, row_number, PE_Val);
    }
  }


///////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// Plotting /////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

  c1->cd();
  gStyle->SetOptStat(0);
  CosmicV1->GetXaxis()->SetTitle("PE (5mV/PE)");
  CosmicV1->GetYaxis()->SetTitle("Counts");
  CosmicV1->GetXaxis()->CenterTitle(true);
  CosmicV1->GetYaxis()->CenterTitle(true);
  CosmicV1->SetTitle(Form("Tile %d", panel_number));
  CosmicV1->Draw();
  c1->Print(Form("Plots/Cosmic_Energy_Histo_%d.png", panel_number));

  c2->cd();
  gStyle->SetOptStat(0);
  Tile_Scan->GetXaxis()->SetTitle("X Position");
  Tile_Scan->GetYaxis()->SetTitle("Y Position");
  Tile_Scan->GetXaxis()->CenterTitle(true);
  Tile_Scan->GetYaxis()->CenterTitle(true);
  Tile_Scan->SetTitle(Form("Tile %d", panel_number));
  Tile_Scan->Draw("colz");
  c2->Print(Form("Plots/Tile_%d_Scan.png",panel_number));

  c3->cd();
  gStyle->SetOptStat(0);
  Second_Half_Tile_Scan->GetXaxis()->SetTitle("X Position");
  Second_Half_Tile_Scan->GetYaxis()->SetTitle("Y Position");
  Second_Half_Tile_Scan->GetXaxis()->CenterTitle(true);
  Second_Half_Tile_Scan->GetYaxis()->CenterTitle(true);
  Second_Half_Tile_Scan->SetTitle(Form("Tile %d", panel_number));
  Second_Half_Tile_Scan->Draw("colz");
  c3->Print(Form("Plots/Second_Half_Tile_%d_Scan.png", panel_number));

  c4->cd();
  gStyle->SetOptStat(0);

  legcent->SetTextFont(63);
  legcent->SetTextSize(16);
  legcent->SetBorderSize(0);

  Combined_Scan->GetXaxis()->SetTitle("X Position");
  Combined_Scan->GetYaxis()->SetTitle("Y Position");
  Combined_Scan->GetXaxis()->CenterTitle(true);
  Combined_Scan->GetYaxis()->CenterTitle(true);
  Combined_Scan->SetTitle(Form("Tile %d", panel_number));
  legcent->Draw();
  Combined_Scan->Draw("colz");
  c4->Print(Form("Plots/Combined_Tile_%d_Scan.png", panel_number));
}
