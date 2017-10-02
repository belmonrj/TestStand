void doit(const int, const int, const int, const int, const double, const double, const char*, const char*);



void CosmicPlotter2()
{

  // doit( 976, 180, 51, 70, 0.5, 0.005,
  //       "Data/HighEta_976_First_20170424_VMIN_SIPM1.txt",
  //       "Data/HighEta_976_Second_20170504_VMIN_SIPM1.txt");
  // doit( 990, 180, 63, 90, 0.5, 0.005,
  //       "Data/HighEta_990_First_20170501_VMIN_SIPM1.txt",
  //       "Data/HighEta_990_Second_20170502_VMIN_SIPM1.txt");
  // doit( 945, 180, 63, 0, 0.5, 0.005,
  //       "Data/HighEta_945_20170413_VMIN_SIPM1.txt",
  //       "");
  // doit( 956, 180, 63, 0, 0.5, 0.005,
  //       "Data/HighEta_956_20170505_VMIN_SIPM1.txt",
  //       "");
  doit( 956, 150, 50, 120, 0.5, 0.005,
        "Data/HighEta_956_20170913-1110_VMIN_SIPM1.txt",
        "Data/HighEta_956_20170914-1652_VMIN_SIPM1.txt"
        "");
  doit( 990, 150, 50, 140, 0.5, 0.005,
        "Data/HighEta_990_20170928-1525_VMIN_SIPM1.txt",
        "Data/HighEta_990_20170929-1503_VMIN_SIPM1.txt"
        "");

}



void doit(const int tile_id, const int steps_x, const int steps_y, const int unscanned_columns,
          const double step_size, const double PE, const char* file1, const char* file2)
{
  TCanvas *c1 = new TCanvas("c1");
  TCanvas *c2 = new TCanvas("c2", "c2", 800, 450);
  TCanvas *c3 = new TCanvas("c3", "c3", 800, 450);
  TCanvas *c4 = new TCanvas("c4", "c4", 900, 450);
  TH1D *th1d_dist = new TH1D("th1d_dist", "", 50, 0, 100);
  TH2D *TileScan1 = new TH2D("TileScan1", "", steps_x, 0, steps_x * step_size, steps_y, 0, steps_y * step_size);
  TH2D *TileScan2 = new TH2D("TileScan2", "", steps_x, 0, steps_x * step_size, steps_y, 0, steps_y * step_size);
  TH2D *TileScanC = new TH2D("TileScanC", "", steps_x + unscanned_columns, 0, (steps_x + unscanned_columns) * step_size, steps_y, 0, steps_y * step_size);
  TLegend *legcent = new TLegend(0.12, 0.6, 0.3, 0.88);

  // ------------------------------------------------------------------------------
  // --- read in data

  ifstream data1;
  ifstream data2;

  double vout;
  double vout2;
  int counter1 = 0;
  int counter2 = 0;
  int column_number = 0;
  int row_number = 0;

  data1.open(file1);
  cout << "Starting First While Loop" << endl;
  double ave1 = 0;
  while (data1 >> vout)
  {
    //cout << vout << endl;
    ave1 += -vout/PE;
    column_number = counter1 / steps_y;
    row_number = counter1 % steps_y;
    th1d_dist->Fill(-vout / PE);
    TileScan1->Fill(column_number * step_size, row_number * step_size, -vout / PE);
    counter1++;
  }
  data1.close();
  ave1 /= counter1;

  cout << "Starting Third While Loop" << endl;
  data2.open(file2);
  double ave2 = 0;
  while (data2 >> vout2)
  {
    //cout << vout << endl;
    ave2 += -vout2/PE;
    column_number = counter2 / steps_y;
    row_number = counter2 % steps_y;
    TileScan2->Fill(column_number * step_size, row_number * step_size, -vout2 / PE);
    counter2++;
  }
  data2.close();
  ave2 /= counter2;


  cout << "average of first half is " << ave1 << endl;
  cout << "average of second half is " << ave2 << endl;
  double scale = ave1/ave2;
  cout << "estimated scale is " << scale << endl;

  // ------------------------------------------------------------------------------
  // --- do the merging

  int first_n_bins = (TileScan1->GetNbinsX()) * (TileScan1->GetNbinsY());
  int second_n_bins = (TileScan2->GetNbinsX()) * (TileScan2->GetNbinsY());
  int combined_n_bins = (TileScanC->GetNbinsX()) * (TileScanC->GetNbinsY());
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
      PE_Val = TileScan1->GetBinContent(column_number, row_number);
      TileScanC->SetBinContent(column_number, row_number, PE_Val);
    }
    if (i > first_n_bins)
    {
      PE_Val = TileScan2->GetBinContent( steps_y - ((combined_n_columns) - column_number), row_number);
      //TileScanC->SetBinContent(column_number, row_number, PE_Val);
      // need to apply scale for differences in gain, or something...
      TileScanC->SetBinContent(column_number, row_number, PE_Val*scale);
    }
  }



  // ------------------------------------------------------------------------------
  // --- do the plotting

  c1->cd();
  gStyle->SetOptStat(0);
  th1d_dist->GetXaxis()->SetTitle("PE (5mV/PE)");
  th1d_dist->GetYaxis()->SetTitle("Counts");
  th1d_dist->GetXaxis()->CenterTitle(true);
  th1d_dist->GetYaxis()->CenterTitle(true);
  th1d_dist->SetTitle(Form("Tile %d", tile_id));
  th1d_dist->Draw();
  c1->Print(Form("Plots/Cosmic_Energy_Histo_%d.png", tile_id));

  c2->cd();
  gStyle->SetOptStat(0);
  TileScan1->GetXaxis()->SetTitle("X Position");
  TileScan1->GetYaxis()->SetTitle("Y Position");
  TileScan1->GetXaxis()->CenterTitle(true);
  TileScan1->GetYaxis()->CenterTitle(true);
  TileScan1->SetTitle(Form("Tile %d", tile_id));
  TileScan1->Draw("colz");
  c2->Print(Form("Plots/Tile_%d_Scan.png",tile_id));

  c3->cd();
  gStyle->SetOptStat(0);
  TileScan2->GetXaxis()->SetTitle("X Position");
  TileScan2->GetYaxis()->SetTitle("Y Position");
  TileScan2->GetXaxis()->CenterTitle(true);
  TileScan2->GetYaxis()->CenterTitle(true);
  TileScan2->SetTitle(Form("Tile %d", tile_id));
  TileScan2->Draw("colz");
  c3->Print(Form("Plots/Second_Half_Tile_%d_Scan.png", tile_id));

  c4->cd();
  gStyle->SetOptStat(0);

  legcent->SetTextFont(63);
  legcent->SetTextSize(16);
  legcent->SetBorderSize(0);

  TileScanC->GetXaxis()->SetTitle("X Position");
  TileScanC->GetYaxis()->SetTitle("Y Position");
  TileScanC->GetXaxis()->CenterTitle(true);
  TileScanC->GetYaxis()->CenterTitle(true);
  TileScanC->SetTitle(Form("Tile %d", tile_id));
  legcent->Draw();
  TileScanC->Draw("colz");
  c4->Print(Form("Plots/Combined_Tile_%d_Scan.png", tile_id));

  delete c1;
  delete c2;
  delete c3;
  delete c4;

  delete th1d_dist;
  delete TileScan1;
  delete TileScan2;
  delete TileScanC;


}
