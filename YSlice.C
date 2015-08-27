void YSlice()
{

  // ---

  ySlice("20150824-1552_A3_LED_PanelAirClad_VMIN_SIPM1_meanHistSub");
  ySlice("20150824-1552_A3_LED_PanelAirClad_VMIN_SIPM2_meanHistSub");
  ySlice("20150824-1701_A3_LED_Panel_VMIN_SIPM1_meanHistSub");
  ySlice("20150824-1701_A3_LED_Panel_VMIN_SIPM2_meanHistSub");
  ySlice("20150824-1750_A3_LED_NoBlock_VMIN_SIPM1_meanHistSub");
  ySlice("20150824-1750_A3_LED_NoBlock_VMIN_SIPM2_meanHistSub");

  // ---

  ySlice("20150825-1133_A3_LED_NoBlock_VMIN_SIPM1_meanHistSub");
  ySlice("20150825-1133_A3_LED_NoBlock_VMIN_SIPM2_meanHistSub");
  ySlice("20150825-1422_A3_LED_PanelAirClad_VMIN_SIPM1_meanHistSub");
  ySlice("20150825-1422_A3_LED_PanelAirClad_VMIN_SIPM2_meanHistSub");
  ySlice("20150825-1515_A3_LED_PanelAirClad_VMIN_SIPM1_meanHistSub");
  ySlice("20150825-1515_A3_LED_PanelAirClad_VMIN_SIPM2_meanHistSub");
  ySlice("20150825-1622_A3_LED_NoBlock_VMIN_SIPM1_meanHistSub");
  ySlice("20150825-1622_A3_LED_NoBlock_VMIN_SIPM2_meanHistSub");
  ySlice("20150825-1733_A3_LED_Panel_VMIN_SIPM1_meanHistSub");
  ySlice("20150825-1733_A3_LED_Panel_VMIN_SIPM2_meanHistSub");

  // ---

  ySlice("20150826-0945_A3_LED_NoBlock_VMIN_SIPM1_meanHistSub");
  ySlice("20150826-0945_A3_LED_NoBlock_VMIN_SIPM2_meanHistSub");
  ySlice("20150826-1055_A3_LED_Panel_VMIN_SIPM1_meanHistSub");
  ySlice("20150826-1055_A3_LED_Panel_VMIN_SIPM2_meanHistSub");

}


void ySlice(const char *NAME)
{

  TFile* inFile = TFile::Open(Form("Data/ROOT/%s.root",NAME));
  if(inFile==NULL)
    {
      cout<<"cannot find file, exiting"<<endl;
      return;
    }
  //inFile->ls();

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
