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


void doana(const char*, const char*, const int, const int);
void analyze(const char*, const char*, const int, const int, bool, double);

void analyzer_smalltile()
{

  // doana("20151113-1313","Low eta tile",48,30);
  // doana("20160106-1320","Low eta tile",48,28);
  // doana("20160106-1636","Low eta tile",48,28);
  doana("20170430-1100","Tile 955",84,36);
  doana("20170428-1400","Tile 939",84,36);
  doana("20170421-1522","Tile 965",84,36);
  doana("20170430-2227","Tile 964",92,42);

}


void doana(const char *basename, const char* title, const int nx, const int ny)
{

  char *sipm1name = Form("%s_VMIN_SIPM1",basename);
  bool doPEConvert = true;
  double PEvalue = 0.00502; // unknown for small tile, assuming previous for now

  analyze(sipm1name,title,nx,ny,doPEConvert,PEvalue);

}


void analyze(const char* NAME, const char* histotitle, const int scan_nxpositions, const int scan_nypositions, bool PEConvert, double PE)
{

  TString ts_name(NAME); // for some reason NAME starts to get corrupted...

  // ----------------------------------------------------------------------------------- //
  // --- MODIFY FOR SIZE OF PANEL, CHECK TOTAL FILES MANUALLY FOR COMPATIBILITY HERE --- //
  // ----------------------------------------------------------------------------------- //

  // --- CHECK THESE WHEN USING
  // int scan_nxpositions = 48;
  // int scan_nypositions = 28;
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
  file.open(Form("Data/SmallPanel/%s.txt",ts_name.Data()));
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
  double maximum1 = *max_element(means.begin(),means.end());
  double minimum2 = means[0];
  for(int i=0; i<meanSize; i++)
    {
      if(means[i]<minimum2) minimum2 = means[i];
    }


  // -------------------------


  // ---

  // --- determine the distance based on the number of steps
  // --- default is 2 steps per cm, double check stepper code
  double distanceX = (double)scan_nxpositions/2;
  double distanceY = (double)scan_nypositions/2;
  // --- make the histogram for the unsubtracted
  TH2D *meanHist = new TH2D(Form("meanHist"), histotitle,
			    scan_nxpositions,0.0,distanceX, scan_nypositions,0.0,distanceY);
  // --- make the histogram for the background subtracted
  TH2D *meanHistSub = new TH2D(Form("meanHistSub"), histotitle,
			       scan_nxpositions,0.0,distanceX, scan_nypositions,0.0,distanceY);

  // --- make a 1d histogram to look at the distirbution of values
  double lo = (minimum1/PE)-2.0;
  double hi = (maximum1/PE)+2.0;
  TH1D *th1d_mean = new TH1D("th1d_mean",histotitle,100,lo,hi);
  lo = -2.0;
  hi = ((maximum1-minimum1)/PE)+2.0;
  TH1D *th1d_meanSub = new TH1D("th1d_meanSub",histotitle,100,lo,hi);
  TH1D *th1d_meanSubTrunc = new TH1D("th1d_meanSubTrunc",histotitle,100,lo,hi);



  // ----------------------------------------------------------------------------------------
  cout << "minimum1 is " << minimum1 <<  " minimum2 is " << minimum2 << endl;
  // ----------------------------------------------------------------------------------------

  // --- make the 2d histograms and use the background subtraction
  for(int j = 0; j < meanSize; j++)
    {
      int row = j%scan_nypositions;
      int column = j/scan_nypositions;
      double iMean = means[j];
      double iMeanSub = iMean-minimum1;
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
      // --- fill the 1d
      th1d_mean->Fill(iMean);
      // --- fill the 1d sub
      th1d_meanSub->Fill(iMeanSub);
      if ( iMeanSub > 0.1*(maximum1/PE) ) th1d_meanSubTrunc->Fill(iMeanSub);
    }
  // --- write the background subtracted histogram to a ROOT file
  meanHistSub->SaveAs(Form("Data/SmallPanel/%s_meanHistSub.root",ts_name.Data())); // NEEDS TO BE REVISED



  // ---------------------------------------------- //
  // --- now make figures of the 2d panel plots --- //
  // ---------------------------------------------- //

  //TCanvas *c1 = new TCanvas("c1","c1", 900, 600); // low eta tiles
  TCanvas *c1 = new TCanvas("c1","c1", 900, 400); // high eta tiles
  c1->SetTicks();

  gStyle->SetOptStat(0);


  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

  c1->SetTopMargin(0.10);
  c1->SetLeftMargin(0.09);
  c1->SetRightMargin(0.14);
  c1->SetBottomMargin(0.12);
  meanHist->GetXaxis()->SetTitle("Position Y (cm)");
  meanHist->GetYaxis()->SetTitle("Position X (cm)");
  if (PEConvert == true) meanHist->GetZaxis()->SetTitle("Pulse Min Value (photoelectrons)");
  else meanHist->GetZaxis()->SetTitle("Pulse Min Value (V)");
  meanHist->Draw("colz");
  c1->Print(Form("Figures/Burn/%s_meanHist.png",ts_name.Data()));
  c1->Print(Form("Figures/Burn/%s_meanHist.pdf",ts_name.Data()));

  double plottextsize = 0.06;
  meanHistSub->SetTitle("");
  meanHistSub->GetXaxis()->SetTitle("Position Y (cm)");
  meanHistSub->GetYaxis()->SetTitle("Position X (cm)");
  meanHistSub->GetXaxis()->SetTitleOffset(0.9);
  meanHistSub->GetYaxis()->SetTitleOffset(0.7);
  meanHistSub->GetXaxis()->SetTitleSize(plottextsize);
  meanHistSub->GetYaxis()->SetTitleSize(plottextsize);
  meanHistSub->GetXaxis()->SetLabelSize(plottextsize);
  meanHistSub->GetYaxis()->SetLabelSize(plottextsize);
  if (PEConvert == true) meanHistSub->GetZaxis()->SetTitle("Pulse Min Value (photoelectrons)");
  else meanHistSub->GetZaxis()->SetTitle("Pulse Min Value (V)");
  meanHistSub->GetZaxis()->SetTitleOffset(0.7);
  meanHistSub->GetZaxis()->SetTitleSize(plottextsize);
  meanHistSub->GetZaxis()->SetLabelSize(plottextsize);
  meanHistSub->Draw("colz");
  TLatex *tex = new TLatex(0.45,0.9,histotitle);
  tex->SetNDC();
  tex->SetTextSize(plottextsize);
  tex->DrawLatex(0.45,0.92,histotitle);
  c1->Print(Form("Figures/Burn/%s_meanHistSub.png",ts_name.Data()));
  c1->Print(Form("Figures/Burn/%s_meanHistSub.pdf",ts_name.Data()));


  TCanvas* c2 = new TCanvas("c2","",800,650);
  c2->SetTicks();
  c2->SetTopMargin(0.10);
  c2->SetLeftMargin(0.13);
  c2->SetRightMargin(0.02);
  c2->SetBottomMargin(0.13);
  th1d_meanSub->SetTitle("");
  th1d_meanSub->GetXaxis()->SetTitle("Signal (number of photoelectrons)");
  th1d_meanSub->GetYaxis()->SetTitle("Number of scan points");
  th1d_meanSub->GetXaxis()->SetTitleOffset(0.9);
  th1d_meanSub->GetYaxis()->SetTitleOffset(0.9);
  th1d_meanSub->GetXaxis()->SetTitleSize(plottextsize);
  th1d_meanSub->GetYaxis()->SetTitleSize(plottextsize);
  th1d_meanSub->GetXaxis()->SetLabelSize(plottextsize);
  th1d_meanSub->GetYaxis()->SetLabelSize(plottextsize);
  th1d_meanSub->Draw();
  tex->DrawLatex(0.45,0.92,histotitle);
  c2->Print(Form("Figures/Burn/%s_1dMeanSub.png",ts_name.Data()));
  c2->Print(Form("Figures/Burn/%s_1dMeanSub.pdf",ts_name.Data()));
  c2->SetLogy();
  c2->Print(Form("Figures/Burn/%s_log1dMeanSub.png",ts_name.Data()));
  c2->Print(Form("Figures/Burn/%s_log1dMeanSub.pdf",ts_name.Data()));

  c2->SetLogy(0);
  th1d_meanSubTrunc->SetTitle("");
  th1d_meanSubTrunc->GetXaxis()->SetTitle("Signal (number of photoelectrons)");
  th1d_meanSubTrunc->GetYaxis()->SetTitle("Number of scan points");
  th1d_meanSubTrunc->GetXaxis()->SetTitleOffset(0.9);
  th1d_meanSubTrunc->GetYaxis()->SetTitleOffset(0.9);
  th1d_meanSubTrunc->GetXaxis()->SetTitleSize(plottextsize);
  th1d_meanSubTrunc->GetYaxis()->SetTitleSize(plottextsize);
  th1d_meanSubTrunc->GetXaxis()->SetLabelSize(plottextsize);
  th1d_meanSubTrunc->GetYaxis()->SetLabelSize(plottextsize);
  th1d_meanSubTrunc->Draw();
  double mean1 = th1d_meanSubTrunc->GetMean();
  double sigm1 = th1d_meanSubTrunc->GetRMS();
  tex->DrawLatex(0.15,0.92,Form("%s, #mu = %.1f, #sigma/#mu = %.2f",histotitle,mean1,sigm1/mean1));
  c2->Print(Form("Figures/Burn/%s_1dMeanSubTrunc.png",ts_name.Data()));
  c2->Print(Form("Figures/Burn/%s_1dMeanSubTrunc.pdf",ts_name.Data()));
  TF1* fun = new TF1("fun","gaus",mean1-sigm1,mean1+1.5*sigm1);
  th1d_meanSubTrunc->Fit(fun,"R");
  double mean2 = fun->GetParameter(1);
  double sigm2 = fun->GetParameter(2);
  //tex->DrawLatex(0.15,0.85,Form("#mu = %.1f, #sigma/#mu = %.2f",mean2,sigm2/mean2));
  tex->DrawLatex(0.75,0.78,Form("#mu = %.1f",mean2));
  tex->DrawLatex(0.75,0.70,Form("#sigma/#mu = %.2f",sigm2/mean2));
  c2->Print(Form("Figures/Burn/%s_fit1dMeanSubTrunc.png",ts_name.Data()));
  c2->Print(Form("Figures/Burn/%s_fit1dMeanSubTrunc.pdf",ts_name.Data()));



  c1->cd();
  meanHistSub->Draw("colz");
  tex->DrawLatex(0.15,0.92,Form("%s, Mean = %.1f photoelectrons, Relative Variance = %.1f%%",histotitle,mean2,100.0*sigm2/mean2));
  //tex->DrawLatex(0.15,0.92,Form("%s, Mean = %.1f pe, Rel. Var. = %.1f%%",histotitle,mean2,100.0*sigm2/mean2));
  c1->Print(Form("Figures/Burn/%s_RVmeanHistSub.png",ts_name.Data()));
  c1->Print(Form("Figures/Burn/%s_RVmeanHistSub.pdf",ts_name.Data()));



  delete meanHist;
  delete meanHistSub;
  delete th1d_mean;
  delete th1d_meanSub;
  delete th1d_meanSubTrunc;
  delete c1;
  delete c2;

}
