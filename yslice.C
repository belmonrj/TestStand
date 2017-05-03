void yslice()
{

  // doyslice("20150820-1156_A4_LED_NoBlock");
  // doyslice("20150820-1625_A4_LED_PanelAir");
  // doyslice("20150821-1312_A4_LED_NoBlock");
  // doyslice("20150821-1431_A4_LED_Panel");
  // doyslice("20150821-1550_A4_LED_PanelAir");
  // doyslice("20150821-1651_A4_LED_PanelAirClad");
  // doyslice("20150824-1552_A4_LED_PanelAirClad");
  // doyslice("20150824-1701_A4_LED_Panel");
  // doyslice("20150824-1750_A4_LED_NoBlock");
  // doyslice("20150825-1133_A4_LED_NoBlock");
  // doyslice("20150825-1422_A4_LED_PanelAirClad");
  // doyslice("20150825-1515_A4_LED_PanelAirClad");
  // doyslice("20150825-1622_A4_LED_NoBlock");
  // doyslice("20150825-1733_A4_LED_Panel");
  // doyslice("20150826-0945_A4_LED_NoBlock");
  // doyslice("20150826-1055_A4_LED_Panel");
  // doyslice("20150826-1334_A4_LED_PanelAirClad");
  // doyslice("20150826-1505_A4_LED_PanelAir");
  // doyslice("20150827-0946_A4_LED_PanelAir");
  // doyslice("20150827-1402_A4_Source_PanelAir");
  // doyslice("20150828-1609_A4_Source_PanelAir");
  // doyslice("20150830-1452_A4_Source_PanelAir");
  // doyslice("20150831-1730_A1_Source");
  // doyslice("20150901-0950_A1_Source");
  // doyslice("20150901-1748_A1_LED");
  // doyslice("20150902-1005_A1_LED");
  // doyslice("20150909-1318_A1_LED");
  // doyslice("20150910-1215_A1_LED"); // looks bad, alignment problem (background subtraction problem)
  // doyslice("20150910-1736_A1_Source"); // looks bad, alignment problem (background subtraction problem)
  // doyslice("20150911-1004_A1_Source");
  doyslice("20150911-1150_A1_Source");
  doyslice("20150911-1328_A1_Source");
  doyslice("20150911-1607_A1_LED");
  doyslice("20150911-1700_A1_LED");
  doyslice("20150914-1138_A1_Source");
  //  doyslice("20150914-1750_A1_LED");

}


void doyslice(const char *basename)
{

  char name[200];
  sprintf(name,"%s_VMIN_SIPM1_meanHistSub",basename);
  ySlice(name);
  sprintf(name,"%s_VMIN_SIPM2_meanHistSub",basename);
  ySlice(name);

}


void ySlice(const char *NAME)
{

  cout<<"NAME is "<<NAME<<endl;

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
      cout<<"in loop "<<i<<" NAME is "<<NAME<<endl;
      TH1D *hp = (TH1D *)h1->ProjectionX(Form("%s_hprojX_%d",NAME,i),i+1,i+1,"");
      //hp->SetTitle(Form("%s ProjectionX %d",NAME,i));
      //hp->SetTitle("");
      hp->Draw();
      c1->Print(Form("Figures/Burn/%s_projectionX_%d.png",NAME,i));
      c1->Print(Form("Figures/Burn/%s_projectionX_%d.pdf",NAME,i));
    }

  int nxbins = h1->GetNbinsX();
  for(int i=0; i<nxbins; i++)
    {
      cout<<"in loop "<<i<<" NAME is "<<NAME<<endl;
      TH1D *hp = (TH1D *)h1->ProjectionY(Form("%s_hprojY_%d",NAME,i),i+1,i+1,"");
      //hp->SetTitle(Form("%s ProjectionY %d",NAME,i));
      //hp->SetTitle("");
      hp->Draw();
      c1->Print(Form("Figures/Burn/%s_projectionY_%d.png",NAME,i));
      c1->Print(Form("Figures/Burn/%s_projectionY_%d.pdf",NAME,i));
    }

  fout->Write();
  fout->Close();

  delete fout;

}
