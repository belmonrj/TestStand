#include <algorithm>

void ReadSingleFile(const char*, const char*);

void SimpleCosmics()
{

  // ReadSingleFile("Data/BigPanel/20160316-1220_Unaveraged_VMin1.txt","OH-1-47");
  // ReadSingleFile("Data/BigPanel/20160316-1220_Unaveraged_VMin2.txt","OH-1-46");

  // ReadSingleFile("Data/BigPanel/20160318-1911_Unaveraged_VMin1.txt","OH-1-2");
  // ReadSingleFile("Data/BigPanel/20160318-1911_Unaveraged_VMin2.txt","OH-1-46");

  // ReadSingleFile("Data/BigPanel/20160323-2359_Unaveraged_VMin1.txt","OH-1-3"); // combined
  // ReadSingleFile("Data/BigPanel/20160323-2359_Unaveraged_VMin2.txt","OH-2-6"); // combined

  // ReadSingleFile("Data/BigPanel/20160324-2359_Unaveraged_VMin1.txt","OH-2-46"); // combined
  // ReadSingleFile("Data/BigPanel/20160324-2359_Unaveraged_VMin2.txt","OH-2-47"); // combined

  //ReadSingleFile("Data/BigPanel/20151023-1307_Unaveraged_VMin1.txt","Previous");
  ReadSingleFile("Data/BigPanel/20160516-1006_Unaveraged_VMin2.txt","OH-1-47");
  ReadSingleFile("Data/BigPanel/20160520-1710_Unaveraged_VMin2.txt","OH-1-46");
  ReadSingleFile("Data/BigPanel/20160602-1416_Unaveraged_VMin2.txt","OH-2-47");
  ReadSingleFile("Data/BigPanel/20160606-1400_Unaveraged_VMin2.txt","OH-2-46");
  ReadSingleFile("Data/BigPanel/20160614-1630_Unaveraged_VMin2.txt","OH-1-2");
  ReadSingleFile("Data/BigPanel/20160616-1731_Unaveraged_VMin2.txt","OH-1-3");
  ReadSingleFile("Data/BigPanel/20160624-1010_Unaveraged_VMin2.txt","OH-2-6");



}

void ReadSingleFile(const char* filename, const char* tileid)
{

  TCanvas *c1 = new TCanvas();
  gStyle->SetOptStat(0);

  ifstream fin(filename);

  vector<double> signal;
  double single;

  while ( fin >> single ) signal.push_back( single / -0.005);

  double max = *max_element(signal.begin(),signal.end());
  double min = *min_element(signal.begin(),signal.end());

  TH1D* histo = new TH1D("histo","",50,0,100);

  for ( auto it = signal.begin(); it != signal.end(); ++it ) histo->Fill(*it);

  histo->GetYaxis()->SetTitle("Counts");
  histo->GetXaxis()->SetTitle("Number of photoelectrons");
  histo->Draw();

  TF1 *fgumbel = new TF1("fgumbel","([0]/sqrt(6.28))*TMath::Exp(-0.5*((x-[1])/[2] + TMath::Exp(-(x-[1])/[2])))",0,100);
  fgumbel->SetParameter(0,100);
  fgumbel->SetParameter(1,5.0);
  fgumbel->SetParameter(2,5.0);
  fgumbel->SetParLimits(0,0,1e10);
  fgumbel->SetParLimits(1,0,1e2);
  fgumbel->SetParLimits(2,0,1e2);

  //histo->Fit(fgumbel,"Q","",15,100);
  histo->Fit(fgumbel,"Q","",20,100);
  fgumbel->Draw("same");

  TLatex *tex_tid = new TLatex(0.6,0.8,Form("Tile ID %s",tileid));
  tex_tid->SetNDC();
  tex_tid->Draw();

  TLatex *tex_mpv = new TLatex(0.6,0.75,Form("MPV = %.1f #pm %.1f",fgumbel->GetParameter(1),fgumbel->GetParError(1)));
  tex_mpv->SetNDC();
  tex_mpv->Draw();

  TLatex *tex_sig = new TLatex(0.6,0.7,Form("#sigma = %.1f #pm %.1f",fgumbel->GetParameter(2),fgumbel->GetParError(2)));
  tex_sig->SetNDC();
  tex_sig->Draw();

  TLatex *tex_chi = new TLatex(0.6,0.65,Form("#chi^{2}/ndf = %.1f/%d",fgumbel->GetChisquare(),fgumbel->GetNDF()));
  tex_chi->SetNDC();
  tex_chi->Draw();

  c1->Print(Form("Figures/Cosmics_%s.pdf",tileid));
  c1->Print(Form("Figures/Cosmics_%s.png",tileid));

  delete tex_tid;
  delete tex_mpv;
  delete tex_sig;
  delete tex_chi;
  delete histo;
  delete c1;

}
