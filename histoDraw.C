//======================================================================================
// PaletteSwap - Macro to read in a .root 2D histogram, change the color palette
// and adjust the scale.
// author:  Sebastian Seeds
//======================================================================================

// Modified by Ron Belmont



void histoDraw()
{

  paletteSwap("20150824-1552_A3_LED_CladBlock_Recouple2_VMIN_SIPM1_meanHistSub");
  paletteSwap("20150824-1701_A3_LED_PanelBlock_Recouple2_VMIN_SIPM1_meanHistSub");
  paletteSwap("20150824-1750_A3_LED_NoBlock_Recouple2_VMIN_SIPM1_meanHistSub");

  paletteSwap("20150824-1552_A3_LED_CladBlock_Recouple2_VMIN_SIPM2_meanHistSub");
  paletteSwap("20150824-1701_A3_LED_PanelBlock_Recouple2_VMIN_SIPM2_meanHistSub");
  paletteSwap("20150824-1750_A3_LED_NoBlock_Recouple2_VMIN_SIPM2_meanHistSub");

  paletteSwap("20150825-1133_A3_LED_NoBlock_VMIN_SIPM1_meanHistSub");
  paletteSwap("20150825-1133_A3_LED_NoBlock_VMIN_SIPM2_meanHistSub");

}



void paletteSwap(const char* loc, bool rot = true, bool average = true, bool varNorm = true, bool flip = true)
{

  TFile* inFile = TFile::Open(Form("Data/ROOT/%s.root",loc));

  inFile->ls();


  //TH2D * h1 = (TH2D*)inFile->Get("timeHist");
  //TH2D * h2 = (TH2D*)inFile->Get("timeHist");
  // TH2D * h1 = (TH2D*)inFile->Get("meanHistSub");
  // TH2D * h2 = (TH2D*)inFile->Get("meanHistSub");
  TH2D * h1 = (TH2D*)inFile->Get(loc);
  TH2D * h2 = (TH2D*)inFile->Get(loc);
  //TH2D * h1 = (TH2D*)inFile->Get("twoDhist");
  //TH2D * h2 = (TH2D*)inFile->Get("twoDhist");

  int xBPositions = h1->GetNbinsX();
  int yBPositions = h1->GetNbinsY();

  TH1D * p1 = new TH1D("p1","p1", yBPositions, 0.0, 15.0);
  TGraph2D * b1 = new TGraph2D();

  int np = xBPositions*yBPositions;
  int I = 0;

  cout << "X Positions are " << xBPositions << "." << endl;
  cout << "Y Positions are " << yBPositions << "." << endl;

  TH2D * m1 = new TH2D("m1","m1", yBPositions, 0, 17.547, xBPositions, 0, 10.528);

  const Int_t NRGB = 3;
  const Int_t NCont = 50;

  double min = 1000.0;
  double Nmin = 0.0;
  double Nmax = 0.0;
  double max = 0.0;
  int TBins = xBPositions*yBPositions;
  double PETot = 0.0;
  double PEMean = 0.0;

  //Loop over histogram and obtain maximum and minimum Z values for scale//

  for (int x = 1; x <= xBPositions; x++)
    {
      for (int y = 1; y <= yBPositions; y++)
	{
	  int bin = h1->GetBin(x,y);
	  double binVal = h1->GetBinContent(bin);

	  PETot += binVal;

	  if (binVal > max) max = binVal;
	  if (binVal < min) min = binVal;
	}
    }

  PEMean = PETot/TBins;

  cout << PEMean << endl;

  cout << "!!!!!!!!!!!!" << endl;


  //Rotate and Flip histogram to obtain top-down view of panel

  if (varNorm == false)
    {
      if (rot == false)
	{
	  for (int x = 1; x <= xBPositions; x++)
	    {
	      for (int y = 1; y <= yBPositions; y++)
		{
		  int bin = h1->GetBin(x,y);
		  double binVal = h1->GetBinContent(bin);
		  if (average == true)
		    {
		      h1->SetBinContent(bin,binVal/PEMean);
		    }else
		    {
		      h1->SetBinContent(bin,binVal);
		    }
		}
	    }
	}else
	{
	  for (int x = 1; x <= yBPositions; x++)
	    {
	      for (int y = 1; y <= xBPositions; y++)
		{
		  int bin = h1->GetBin(y,x);
		  double binVal = h1->GetBinContent(bin);
		  if (average == true)
		    {
		      int binrot = m1->GetBin(x,y);
		      m1->SetBinContent(binrot,binVal/PEMean);
		    }else
		    {
		      int binrot = m1->GetBin(x,y);
		      m1->SetBinContent(binrot,binVal);
		    }
		}
	    }
	}
    }else
    {
      for (int x = 1; x <= yBPositions; x++)
	{
	  for (int y = 1; y <= xBPositions; y++)
	    {
	      int bin = h1->GetBin(y,x);
	      double binVal = h1->GetBinContent(bin);
	      {
		int binrot = m1->GetBin(x,y);
		m1->SetBinContent(binrot,binVal);
	      }
	    }
	}

      int xAveBins = xBPositions;
      double xAveTot = 0.0;
      double xNormConst = 0.0;
      min = 0.0;
      max = 0.0;

      for (int x = 1; x <= yBPositions; x++)
	{
	  xAveTot = 0.0;
	  xNormConst = 0.0;
	  if (x > 0 && x < yBPositions +1)
	    {
	      for (int y = 1; y <= xBPositions; y++)
		{
		  if (y > 0 && y <= xBPositions && x > 0 && x <= yBPositions +1)
		    {
		      int binrot = m1->GetBin(x,y);
		      double binVal = m1->GetBinContent(binrot);
		      xAveTot += binVal;
		    }
		}
	      if (varNorm == true)
		{
		  xNormConst = xAveTot/xAveBins;
		  int binxProjB = p1->GetBin(x);
		  p1->SetBinContent(binxProjB,xNormConst);
		  for (int y = 1; y <= xBPositions; y++)
		    {
		      int binrot = m1->GetBin(x,y);
		      double binVal = m1->GetBinContent(binrot);
		      if (binVal/xNormConst > max) max = binVal/xNormConst;
		      if (binVal/xNormConst < min) min = binVal/xNormConst;
		      m1->SetBinContent(binrot,(binVal/xNormConst));
		      b1->SetPoint(I,x,y,binVal);
		      I++;
		    }
		}
	    }
	}
    }

  cout << "HEY" << endl;

  //Simple Flip//

  //TH2D * f1 = new TH2D("f1","f1", xBPositions, 0, 29, yBPositions, 0, 1.5); // need to add selectivity
  TH2D * f1 = new TH2D("f1","f1", xBPositions, 0, 29, yBPositions, 0, 4.5);

  for (int iii = 1; iii<=xBPositions; iii++)
    {
      for (int jjj = 1; jjj<=yBPositions; jjj++)
	{
	  int binflip = h1->GetBin(iii,jjj);
	  double binflipval = h1->GetBinContent(binflip);
	  int newbin = f1->GetBin(iii,yBPositions-jjj+1);
	  f1->SetBinContent(newbin,binflipval);

	  /*
	  // Throwing out outliers (NEED TO TRACK THESE DOWN)
	  if (binflipval > 0.5)
	  {
	  f1->SetBinContent(newbin,binflipval);
	  }else
	  {
	  f1->SetBinContent(newbin,1);
	  }
	  */
	}
    }

  Nmin = min/PEMean;
  Nmax = max/PEMean;

  double NmaxAdj = Nmax-1;
  double NminAdj = (Nmin-1)*(-1);
  double Grail = 0.0;

  if (NmaxAdj > NminAdj)
    {
      Grail = NmaxAdj;
    } else
    {
      Grail = NminAdj;
    }

  cout << "Minimum PE value is " << min << "." << endl;
  cout << "Maximum PE value is " << max << "." << endl;

  gStyle->SetOptStat(0);
  gStyle->SetCanvasBorderSize(2);
  gStyle->SetPadBottomMargin(0.10);
  gStyle->SetPadTopMargin(0.10);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.15);
  gStyle->SetFrameBorderMode(1);

  TCanvas *c1 = new TCanvas("c1","c1",1200, 588);
  //TCanvas *c2 = new TCanvas("c2","c2",1200, 588);
  //TCanvas *c3 = new TCanvas("c3","c3",1200, 588);

  cout << min << "   " << max << endl;

  c1->cd();

  if (varNorm==true)
    {
      //Palette for normalizing response across panel
      Int_t MyPalette[100];
      Double_t r[]    = {0.0, 0.0, 1.0, .75, 1.0};
      Double_t g[]    = {0.0, 0.0, 1.0, 0.0, 0.0};
      Double_t b[]    = {1.0, .75, 1.0, 0.0, 0.0};
      Double_t stop[] = {0.0, 0.0, .50, 1.0, 1.0};
      Int_t FI = TColor::CreateGradientColorTable(5, stop, r, g, b, 100);

      for (int i=0;i<100;i++) MyPalette[i] = FI+i;
      gStyle->SetPalette(100, MyPalette);
      h1->SetAxisRange(1-Grail,1+Grail,"Z");
    }

  else
    {
      //Palette for raw data
      Int_t MyPalette[100];
      Double_t r[]    = {0.0, 0.0, 1.0, .75, 1.0};
      Double_t g[]    = {0.0, 0.0, 1.0, 0.0, 0.0};
      Double_t b[]    = {1.0, .75, 1.0, 0.0, 0.0};
      Double_t stop[] = {0.0, 0.0, .57, 1.0, 1.0};
      Int_t FI = TColor::CreateGradientColorTable(5, stop, r, g, b, 100);

      for (int i=0;i<100;i++) MyPalette[i] = FI+i;
      gStyle->SetPalette(100, MyPalette);
      h1->SetAxisRange(1-Grail,1+Grail,"Z");
    }

  //f1->SaveAs("test.root");

  PEMean = PEMean * pow(10,8);

  cout << PEMean << endl;

  cout << "!!!!!!!!!!!!!" << endl;

  if (flip == true)
    {
      f1->SetTitle(Form("Pulse Averaged Test Scan, Mean:  Photo Electrons",PEMean));
      f1->SetTitleSize(0.10);
      f1->GetXaxis()->SetTitle("X-Axis in cm");
      f1->GetXaxis()->SetTitleSize(0.04);
      f1->GetYaxis()->SetTitle("Y-Axis in cm");
      f1->GetYaxis()->SetTitleSize(0.04);
      f1->GetZaxis()->SetTitle("Mean Number of Photoelectrons");
      f1->GetZaxis()->SetTitleSize(0.04);
      f1->GetXaxis()->CenterTitle();
      f1->GetYaxis()->CenterTitle();
      f1->GetZaxis()->CenterTitle();
      //f1->SetAxisRange(1-Grail,1+Grail,"Z");
      f1->Draw("colz");
      f1->SetContour(1000);
    }
  //Drawing in the panel and fiber
  /*
    c1->Update();
    TLine* Top = new TLine(1.75,9.33,16.9,9.33);
    Top -> SetLineColor(kBlack);
    Top -> SetLineWidth(3);
    Top -> Draw("only");
    TLine* Right = new TLine(16.9,9.35,16.9,.96);
    Right -> SetLineColor(kBlack);
    Right -> SetLineWidth(3);
    Right -> Draw("only");
    TLine* Bot = new TLine(1.75,.96,16.9,.96);
    Bot -> SetLineColor(kBlack);
    Bot -> SetLineWidth(3);
    Bot -> Draw("only");
    TLine* Left = new TLine(1.75,.96,1.75,9.35);
    Left -> SetLineColor(kBlack);
    Left -> SetLineWidth(3);
    Left -> Draw("only");
    TLine* Fiber1 = new TLine(1.73,7.75,16.9,7.75);
    Fiber1 -> SetLineColor(kMagenta);
    Fiber1 -> SetLineWidth(5);
    Fiber1 -> Draw("only");
    TLine* Fiber2 = new TLine(1.73,5.4,16.9,5.4);
    Fiber2 -> SetLineColor(kOrange);
    Fiber2 -> SetLineWidth(5);
    Fiber2 -> Draw("only");
    TLine* Fiber3 = new TLine(1.73,4.1,16.9,4.1);
    Fiber3 -> SetLineColor(kOrange);
    Fiber3 -> SetLineWidth(5);
    Fiber3 -> Draw("only");
    TLine* Fiber4 = new TLine(1.73,2.7,16.9,2.7);
    Fiber4 -> SetLineColor(kOrange);
    Fiber4 -> SetLineWidth(5);
    Fiber4 -> Draw("only");
  */
  /*    c2->cd();

	if (rot == false)
	{
	h1->SetTitle(Form("Pulse Averaged Test Scan, Mean: %f Volts",PEMean));
	h1->SetTitleSize(0.10);
	h1->GetXaxis()->SetTitle("X-Axis in cm");
	h1->GetXaxis()->SetTitleSize(0.04);
	h1->GetYaxis()->SetTitle("Y-Axis in cm");
	h1->GetYaxis()->SetTitleSize(0.04);
	h1->GetZaxis()->SetTitle("Mean Voltage (Volts)");
	h1->GetZaxis()->SetTitleSize(0.04);
	h1->GetXaxis()->CenterTitle();
	h1->GetYaxis()->CenterTitle();
	h1->GetZaxis()->CenterTitle();
	//h1->SetAxisRange(1-Grail,1+Grail,"Z");
	h1->Draw("colz");
	h1->SetContour(1000);
	}else
	{
	m1->SetTitle("Pulse Averaged Test Scan");
	m1->SetTitleSize(0.10);
	m1->GetXaxis()->SetTitle("X-Axis in cm");
	m1->GetXaxis()->SetTitleSize(0.04);
	m1->GetYaxis()->SetTitle("Y-Axis in cm");
	m1->GetYaxis()->SetTitleSize(0.04);
	m1->GetZaxis()->SetTitle("Normalized Mean Voltage");
	m1->GetZaxis()->SetTitleSize(0.04);
	m1->GetXaxis()->CenterTitle();
	m1->GetYaxis()->CenterTitle();
	m1->GetZaxis()->CenterTitle();
	//m1->SetAxisRange(1-Grail,1+Grail,"Z");
	m1->Draw("colz");
	m1->SetContour(1000);

	cout << "bin sdsdfw3:" << m1->GetBin(20,18) << endl;
	cout << "bin sdsdfw3:" << m1->GetBinContent(955) << endl;
	cout << "bin sdsdfw3:" << m1->GetBinContent(1010) << endl;
	cout << "bin rtt55t5y3:" << m1->GetBinContent(1065) << endl;
	cout << "bin sdsdfw3:" << m1->GetBinContent(259) << endl;
	cout << "bin sdsdfw3:" << m1->GetBinContent(314) << endl;
	cout << "bin rtt55t5y3:" << m1->GetBinContent(369) << endl;

	}
	if (varNorm == true)
	{
	c1->Update();
	c2->cd();
	p1->SetTitle("Panel Values Projected Onto X-Axis, Averaged Over Central Y Bins");
	p1->SetTitleSize(0.10);
	p1->GetXaxis()->SetTitle("X-Axis in cm");
	p1->GetXaxis()->SetTitleSize(0.04);
	p1->GetYaxis()->SetTitle(Form("Panel Values in Volts (Total for Center / %i)",xBPositions -10));
	p1->GetYaxis()->SetTitleSize(0.04);
	p1->GetXaxis()->CenterTitle();
	p1->GetYaxis()->CenterTitle();
	p1->Draw("colz");

	}

	TCanvas *c3 = new TCanvas("c3","c3",588, 1200);
	UInt_t Number = 3;
	Double_t Red[Number]    = { 1.00, 0.00, 0.00};
	Double_t Green[Number]  = { 0.00, 1.00, 0.00};
	Double_t Blue[Number]   = { 1.00, 0.00, 1.00};
	Double_t Length[Number] = { 0.00, 0.50, 1.00 };
	Int_t nb=50;
	TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);

	if (varNorm == true)
	{
	c2->Update();
	c3->cd();
	b1->Draw("surf2z");
	b1->GetHistogram()->SetContour(75);
	}
  */

  c1->Print(Form("Figures/%s_swapped.png",loc));
  c1->Print(Form("Figures/%s_swapped.pdf",loc));

}
