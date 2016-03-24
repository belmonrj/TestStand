#include<algorithm>

void ReadSingleFile(const char*, const char*);

void SimpleCosmics()
{

  ReadSingleFile("Data/20160314-1215_Unaveraged_VMin1.txt","OH-2-46");
  ReadSingleFile("Data/20160314-1215_Unaveraged_VMin2.txt","OH-2-47");

  // ReadSingleFile("Data/20160315-1448_Unaveraged_VMin1.txt","OH-1-3");
  // ReadSingleFile("Data/20160315-1448_Unaveraged_VMin2.txt","OH-2-6");

  ReadSingleFile("Data/20160316-1220_Unaveraged_VMin1.txt","OH-1-47");
  ReadSingleFile("Data/20160316-1220_Unaveraged_VMin2.txt","OH-1-46");

  ReadSingleFile("Data/20160318-1911_Unaveraged_VMin1.txt","OH-1-2");
  ReadSingleFile("Data/20160318-1911_Unaveraged_VMin2.txt","OH-1-46");

  // ReadSingleFile("Data/20160323-1647_Unaveraged_VMin1.txt","OH-1-3");
  // ReadSingleFile("Data/20160323-1647_Unaveraged_VMin2.txt","OH-2-6");

  ReadSingleFile("Data/20160323-2359_Unaveraged_VMin1.txt","OH-1-3"); // combined
  ReadSingleFile("Data/20160323-2359_Unaveraged_VMin2.txt","OH-2-6"); // combined



}

void ReadSingleFile(const char* filename, const char* tileid)
{

  TCanvas *c1 = new TCanvas();

  ifstream fin(filename);

  vector<double> signal;
  double single;

  while ( fin >> single ) signal.push_back( single / -0.005);

  double max = *max_element(signal.begin(),signal.end());
  double min = *min_element(signal.begin(),signal.end());

  TH1D* histo = new TH1D("histo","",50,0,50);

  for ( auto it = signal.begin(); it != signal.end(); ++it ) histo->Fill(*it);

  histo->Draw();

  TF1 *fgumbel = new TF1("fgumbel","([0]/sqrt(6.28))*TMath::Exp(-0.5*((x-[1])/[2] + TMath::Exp(-(x-[1])/[2])))",0,50);
  fgumbel->SetParameter(0,100);
  fgumbel->SetParameter(1,5.0);
  fgumbel->SetParameter(2,5.0);
  fgumbel->SetParLimits(0,0,1e10);
  fgumbel->SetParLimits(1,0,1e2);
  fgumbel->SetParLimits(2,0,1e2);

  histo->Fit(fgumbel,"","",5,35);

  c1->Print(Form("Cosmics_%s.png",tileid));

  delete histo;
  delete c1;

}
