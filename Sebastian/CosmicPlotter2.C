void doit(const int, const int, const int, const int, const double, const double, const char*, const char*);



void CosmicPlotter2()
{

  doit( 976, 180, 51, 70, 0.5, 0.005,
        "Data/HighEta_976_First_20170424_VMIN_SIPM1.txt",
        "Data/HighEta_976_Second_20170504_VMIN_SIPM1.txt");
  doit( 990, 180, 63, 90, 0.5, 0.005,
        "Data/HighEta_990_First_20170501_VMIN_SIPM1.txt",
        "Data/HighEta_990_Second_20170502_VMIN_SIPM1.txt");
  doit( 945, 180, 63, 0, 0.5, 0.005,
        "Data/HighEta_945_20170413_VMIN_SIPM1.txt",
        "");
  doit( 956, 180, 63, 0, 0.5, 0.005,
        "Data/HighEta_956_20170505_VMIN_SIPM1.txt",
        "");

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

  // ------------------------------------------------------------------------------
  // --- read in data

  ifstream CosmicTxt;
  ifstream CosmicTxt3;

  double vout;
  double vout2;
  int data_sample_number = 0;
  int data_sample_number_2 = 0;
  int column_number = 0;
  int row_number = 0;

  CosmicTxt.open(file1);
  cout << "Starting First While Loop" << endl;
  double ave1 = 0;
  while (CosmicTxt >> vout)
  {
    ave1 += -vout/PE;
    column_number = data_sample_number / steps_y;
    row_number = data_sample_number % steps_y;
    CosmicV1->Fill(-vout / PE);
    Tile_Scan->Fill(column_number * step_size, row_number * step_size, -vout / PE);
    data_sample_number++;
  }
  CosmicTxt.close();
  ave1 /= data_sample_number;

  cout << "Starting Third While Loop" << endl;
  CosmicTxt3.open(file2);
  double ave2 = 0;
  while (CosmicTxt3 >> vout2)
  {
    ave2 += -vout2/PE;
    column_number = data_sample_number_2 / steps_y;
    row_number = data_sample_number_2 % steps_y;
    Second_Half_Tile_Scan->Fill(column_number * step_size, row_number * step_size, -vout2 / PE);
    data_sample_number_2++;
  }
  CosmicTxt3.close();
  ave2 /= data_sample_number_2;


  cout << "average of first half is " << ave1 << endl;
  cout << "average of second half is " << ave2 << endl;
  double scale = ave1/ave2;
  cout << "estimated scale is " << scale << endl;

  // ------------------------------------------------------------------------------
  // --- do the merging

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
      //Combined_Scan->SetBinContent(column_number, row_number, PE_Val);
      // need to apply scale for differences in gain, or something...
      Combined_Scan->SetBinContent(column_number, row_number, PE_Val*scale);
    }
  }



  // ------------------------------------------------------------------------------
  // --- do the plotting

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

  delete c1;
  delete c2;
  delete c3;
  delete c4;

  delete CosmicV1;
  delete Tile_Scan;
  delete Second_Half_Tile_Scan;
  delete Combined_Scan;


}
