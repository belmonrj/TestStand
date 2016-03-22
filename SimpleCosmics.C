#include<algorithm>

void ReadSingleFile(const char*, const char*);

void SimpleCosmics()
{

  ReadSingleFile("Data/20160315-2359_Unaveraged_VMin1.txt","OH-1-3");
  ReadSingleFile("Data/20160315-2359_Unaveraged_VMin2.txt","OH-2-6");

  ReadSingleFile("Data/20160316-1220_Unaveraged_VMin1.txt","OH-1-47");
  ReadSingleFile("Data/20160316-1220_Unaveraged_VMin2.txt","OH-1-46");

  ReadSingleFile("Data/20160318-1911_Unaveraged_VMin1.txt","OH-1-2");


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

  c1->Print(Form("Cosmics_%s.png",tileid));

  delete histo;
  delete c1;

}
