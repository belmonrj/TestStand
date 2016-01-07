//======================================================================================
// PaletteSwap - Macro to read in a .root 2D histogram, change the color palette
// and adjust the scale.
// author:  Sebastian Seeds
//======================================================================================

// Modified by Ron Belmont

void dopaletteswap(const char*);
void paletteSwap(const char*);
void paletteSwap(const char*, bool, bool, bool, bool);

void histoDraw()
{

  // dopaletteswap("20150820-1156_A4_LED_NoBlock");
  // dopaletteswap("20150820-1625_A4_LED_PanelAir");
  // dopaletteswap("20150821-1312_A4_LED_NoBlock");
  // dopaletteswap("20150821-1431_A4_LED_Panel");
  // dopaletteswap("20150821-1550_A4_LED_PanelAir");
  // dopaletteswap("20150821-1651_A4_LED_PanelAirClad");
  // dopaletteswap("20150824-1552_A4_LED_PanelAirClad");
  // dopaletteswap("20150824-1701_A4_LED_Panel");
  // dopaletteswap("20150824-1750_A4_LED_NoBlock");
  // dopaletteswap("20150825-1133_A4_LED_NoBlock");
  // dopaletteswap("20150825-1422_A4_LED_PanelAirClad");
  // dopaletteswap("20150825-1515_A4_LED_PanelAirClad");
  // dopaletteswap("20150825-1622_A4_LED_NoBlock");
  // dopaletteswap("20150825-1733_A4_LED_Panel");
  // dopaletteswap("20150826-0945_A4_LED_NoBlock");
  // dopaletteswap("20150826-1055_A4_LED_Panel");
  // dopaletteswap("20150826-1334_A4_LED_PanelAirClad");
  // dopaletteswap("20150826-1505_A4_LED_PanelAir");
  // dopaletteswap("20150827-0946_A4_LED_PanelAir");
  // dopaletteswap("20150827-1402_A4_Source_PanelAir");
  // dopaletteswap("20150828-1609_A4_Source_PanelAir");
  // dopaletteswap("20150830-1452_A4_Source_PanelAir");
  dopaletteswap("20150831-1730_A1_Source");
  dopaletteswap("20150901-0950_A1_Source");

}

void dopaletteswap(const char *basename)
{

  paletteSwap(Form("%s_VMIN_SIPM1_meanHistSub",basename));
  paletteSwap(Form("%s_VMIN_SIPM2_meanHistSub",basename));

}

void paletteSwap(const char *name)
{
  paletteSwap(name,true,true,true,true);
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
      f1->SetTitle(Form("Pulse Averaged Test Scan, Mean: %f Photo Electrons",PEMean));
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

  c1->Print(Form("Figures/%s_swapped.png",loc));
  c1->Print(Form("Figures/%s_swapped.pdf",loc));

}
