const int npixels = 1000;
const int nbins = 100;
const int multiplier = (npixels/nbins)*10;
const int offset = multiplier/10;


void doit(const int, const int, double&, double&, double&);


void pixels_1d()
{

  TCanvas *c1 = new TCanvas();

  double x[nbins];
  double occupancy[nbins] = {0};
  double derivative[nbins] = {-1};
  double efficiency[nbins] = {0};

  for ( int i = 0; i < nbins; ++i )
    {
      int nphotons = i*multiplier + offset;
      double occ = 0;
      double sat = 0;
      double eff = 0;
      doit(npixels,nphotons,occ,sat,eff);
      x[i] = (double)nphotons;
      x[i] /= (double)npixels;
      occupancy[i] = occ;
      efficiency[i] = eff;
      if ( i > 0 ) derivative[i] = (occupancy[i] - occupancy[i-1])/(x[i] - x[i-1]);
    }

  TGraph *tg1 = new TGraph(nbins,x,occupancy);
  tg1->SetMarkerStyle(kOpenCircle);
  tg1->Draw("ap");
  tg1->SetMaximum(1.1);
  tg1->SetMinimum(-0.1);
  tg1->GetXaxis()->SetLimits(-0.2,10.2);
  tg1->GetXaxis()->SetTitle("Number of photons thrown per pixel");
  tg1->GetYaxis()->SetTitle("Fraction of pixels fired");

  c1->Print("occupancy.png");

  TGraph *tg2 = new TGraph(nbins,x,derivative);
  tg2->SetMarkerStyle(kOpenCircle);
  tg2->Draw("ap");
  tg2->SetMaximum(1.1);
  tg2->SetMinimum(-0.1);
  tg2->GetXaxis()->SetLimits(-0.2,10.2);
  tg2->GetXaxis()->SetLimits(-0.2,10.2);
  tg2->GetXaxis()->SetTitle("Number of photons thrown per pixel");
  tg2->GetYaxis()->SetTitle("Derivative of fraction of pixels fired");

  c1->Print("derivative.png");

  TGraph *tg3 = new TGraph(nbins,x,efficiency);
  tg3->SetMarkerStyle(kOpenCircle);
  tg3->Draw("ap");
  tg3->SetMaximum(1.1);
  tg3->SetMinimum(-0.1);
  tg3->GetXaxis()->SetLimits(-0.2,10.2);
  tg3->GetXaxis()->SetLimits(-0.2,10.2);
  tg3->GetXaxis()->SetTitle("Number of photons thrown per pixel");
  tg3->GetYaxis()->SetTitle("Efficiency of photon collection");

  c1->Print("efficiency.png");

}


void doit(const int npixels, const int nphotons, double& occupancy, double& saturation, double& efficiency)
{

  TH1D *h = new TH1D("h","",npixels,0.0,1.0);

  for ( int i = 0; i < nphotons; ++i )
    {
      double random = gRandom->Uniform(0,1.0);
      h->Fill(random);
    }

  int count_filled = 0;
  int count_saturated = 0;

  for ( int i = 0; i < npixels; ++i )
    {
      int number = (int)h->GetBinContent(i+1);
      if ( number > 0 ) ++count_filled;
      if ( number > 1 ) ++count_saturated;
    }

  occupancy = (double)count_filled/(double)npixels;

  saturation = (double)count_saturated/(double)npixels;

  efficiency = (double)count_filled/(double)nphotons;

  delete h;

}
