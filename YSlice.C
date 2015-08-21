void YSlice()
{

  // ySlice("A1_LED_ColTests_Fiber1_VMIN_SIPM1_meanHistSub");
  // ySlice("A1_LED_ColTests_Fiber2_VMIN_SIPM1_meanHistSub");
  // ySlice("A1_LED_ColTests_Fiber3_VMIN_SIPM1_meanHistSub");
  // ySlice("A1_LED_ColTests_Fiber4_VMIN_SIPM1_meanHistSub");

  // ySlice("A1_LED_ColTests_Fiber1_VMIN_SIPM2_meanHistSub");
  // ySlice("A1_LED_ColTests_Fiber2_VMIN_SIPM2_meanHistSub");
  // ySlice("A1_LED_ColTests_Fiber3_VMIN_SIPM2_meanHistSub");
  // ySlice("A1_LED_ColTests_Fiber4_VMIN_SIPM2_meanHistSub");

  ySlice("A3_LED_Assymetry_VMin_SiPM1_meanHistSub");
  ySlice("A3_LED_Assymetry_VMin_SiPM2_meanHistSub");

  ySlice("A3_LED_Assymetry_PannelBlock_VMin_SiPM1_meanHistSub");
  ySlice("A3_LED_Assymetry_PannelBlock_VMin_SiPM2_meanHistSub");

}


void ySlice(const char *NAME)
{

  TFile* inFile = TFile::Open(Form("Data/ROOT/%s.root",NAME));
  if(inFile==NULL)
    {
      cout<<"cannot find file, exiting"<<endl;
      return;
    }
  inFile->ls();

  TH2D *h1 = (TH2D*)inFile->Get(Form("%s",NAME));
  if(h1==NULL)
    {
      cout<<"no histogram available, exiting"<<endl;
      return;
    }

  TFile *fout = new TFile(Form("Data/ROOT/%s_projections.root",NAME),"recreate");

  int nybins = h1->GetNbinsY();
  for(int i=0; i<nybins; i++)
    {
      TH1D *hp = (TH1D *)h1->ProjectionX(Form("%s_hproj_%d",NAME,i),i+1,i+1,"");
      hp->SetTitle(Form("%s ProjectionX %d",NAME,i));
      hp->Draw();
      c1->Print(Form("Figures/%s_projection_%d.png",NAME,i));
      c1->Print(Form("Figures/%s_projection_%d.pdf",NAME,i));
    }

  fout->Write();
  fout->Close();

  delete fout;

}
