#include <algorithm> // for min_element, max_element


void singleslice()
{

  TCanvas *c1 = new TCanvas();

  double peconvert = 0.00502;

  ifstream fin1("SingleSlice/20151111-1552_VMIN_SIPM1.txt");
  double content;
  vector<double> signal1;
  while(fin1>>content)
    {
      signal1.push_back(-content/peconvert);
    }
  fin1.close();
  cout << signal1.size() << endl;


  ifstream fin2("SingleSlice/20151111-1552_VMIN_SIPM2.txt");
  vector<double> signal2;
  while(fin2>>content)
    {
      signal2.push_back(-content/peconvert);
    }
  fin2.close();
  cout << signal2.size() << endl;

  int size1 = signal1.size();
  double min1 = *min_element(signal1.begin(),signal1.end());
  double max1 = *max_element(signal1.begin(),signal1.end());
  int size2 = signal2.size();
  double min2 = *min_element(signal2.begin(),signal2.end());
  double max2 = *max_element(signal2.begin(),signal2.end());

  if(size1!=size2) cout << "prepare to suffer immensely..." << endl;
  double size = size1;

  double stepsize = 0.5; // cm

  TH1D *th1d_sipm1 = new TH1D("th1d_sipm1","",size,0,size*stepsize);
  TH1D *th1d_sipm2 = new TH1D("th1d_sipm2","",size,0,size*stepsize);
  for(int i=0; i<size; i++)
    {
      th1d_sipm1->SetBinContent(i+1,signal1[i]);
      th1d_sipm2->SetBinContent(i+1,signal2[i]);
    }

  th1d_sipm2->SetLineColor(kRed);
  if(max1>max2)
    {
      th1d_sipm1->Draw();
      th1d_sipm2->Draw("same");
    }
  else
    {
      th1d_sipm2->Draw();
      th1d_sipm1->Draw("same");
    }
  c1->Print("ShortSideSlices/slice_sipm12.png");
  // ---
  double scale = max1/max2;
  th1d_sipm2->Scale(scale);
  th1d_sipm2->Draw();
  th1d_sipm1->Draw("same");
  c1->Print("ShortSideSlices/slice_sipm12scale.png");

  th1d_sipm1->Add(th1d_sipm2);
  th1d_sipm1->Draw();
  c1->Print("ShortSideSlices/slice_sipmB.png");

  TF1 *gengaus = new TF1("gengaus","[0]*TMath::Exp(-pow(TMath::Abs(x-[1]),[3])/[2])",0,16);
  gengaus->SetParameter(0,47);
  gengaus->SetParameter(1,12.3);
  gengaus->SetParameter(2,3.5);
  gengaus->SetParameter(3,1.1);
  th1d_sipm1->Fit(gengaus,"","",9,15);
  th1d_sipm1->SetMinimum(0);
  c1->Print("ShortSideSlices/slice_sipmB_fit.png");
  c1->Print("ShortSideSlices/slice_sipmB_fit.pdf");

  TFile *file = new TFile("sourcemiddle.root","recreate");
  file->cd();
  th1d_sipm1->Write();
  gengaus->Write();
  file->Write();
  file->Close();

}

///very good fit...
   // 1  p0           4.72007e+01   7.93397e+00   1.37453e-03   2.10777e-05
   // 2  p1           1.22838e+01   2.18691e-01   9.87666e-05   5.64875e-03
   // 3  p2           3.53249e+00   2.58431e+00   3.26703e-04  -1.57629e-03
   // 4  p3           1.08189e+00   8.77823e-01   1.80370e-04   2.99766e-03
