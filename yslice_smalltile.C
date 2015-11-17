void doyslice(const char*);
void ySlice(const char*);


void yslice_smalltile()
{

  doyslice("20151113-1313");

}


void doyslice(const char *basename)
{

  char name[200];
  sprintf(name,"%s_VMIN_SIPM1_meanHistSub",basename);
  ySlice(name);

}


void ySlice(const char *NAME)
{

  cout<<"NAME is "<<NAME<<endl;

  TFile* inFile = TFile::Open(Form("Data/SmallPanel/%s.root",NAME));
  if(inFile==NULL)
    {
      cout<<"cannot find file, exiting"<<endl;
      return;
    }
  //inFile->ls();

  TH2D *h1 = (TH2D*)inFile->Get(Form("meanHistSub"));
  if(h1==NULL)
    {
      cout<<"no histogram available, exiting"<<endl;
      return;
    }

  TCanvas *c1 = new TCanvas("c1","");

  TFile *fout = new TFile(Form("Data/SmallPanel/%s_projections.root",NAME),"recreate");

  int nybins = h1->GetNbinsY();
  for(int i=0; i<nybins; i++)
    {
      cout<<"in loop "<<i<<" NAME is "<<NAME<<endl;
      TH1D *hpx = (TH1D *)h1->ProjectionX(Form("hprojX_%d",NAME,i),i+1,i+1,"");
      //hpx->SetTitle(Form("%s ProjectionX %d",NAME,i));
      //hpx->SetTitle("");
      hpx->Draw();
      c1->Print(Form("Figures/Burn/%s_projectionX_%d.png",NAME,i));
      c1->Print(Form("Figures/Burn/%s_projectionX_%d.pdf",NAME,i));
    }

  int nxbins = h1->GetNbinsX();
  for(int i=0; i<nxbins; i++)
    {
      cout<<"in loop "<<i<<" NAME is "<<NAME<<endl;
      TH1D *hpy = (TH1D *)h1->ProjectionY(Form("hprojY_%d",NAME,i),i+1,i+1,"");
      //hpy->SetTitle(Form("%s ProjectionY %d",NAME,i));
      //hpy->SetTitle("");
      hpy->Draw();
      c1->Print(Form("Figures/Burn/%s_projectionY_%d.png",NAME,i));
      c1->Print(Form("Figures/Burn/%s_projectionY_%d.pdf",NAME,i));
    }

  fout->Write();
  fout->Close();

  delete fout;

}
