#include <algorithm> // for min_element, max_element

void yslice_new_LEDscan()
{

  doyslice("20151105-1515");

}


void doyslice(const char *basename)
{

  char name1[200];
  sprintf(name1,"%s_VMIN_SIPM1_meanHistSub",basename);
  char name2[200];
  sprintf(name2,"%s_VMIN_SIPM2_meanHistSub",basename);
  ySlice(name1,name2);

}


void ySlice(const char *name1, const char *name2)
{

  cout<<"name1 is "<<name1<<endl;

  TFile *inFile1 = TFile::Open(Form("Data/NewLED/%s.root",name1));
  if(inFile1==NULL)
    {
      cout<<"cannot find file, exiting"<<endl;
      return;
    }
  TH2D *h1 = (TH2D*)inFile1->Get(Form("%s",name1));
  if(h1==NULL)
    {
      cout<<"no histogram available, exiting"<<endl;
      return;
    }
  // ---
  TFile *inFile2 = TFile::Open(Form("Data/NewLED/%s.root",name2));
  if(inFile2==NULL)
    {
      cout<<"cannot find file, exiting"<<endl;
      return;
    }
  TH2D *h2 = (TH2D*)inFile2->Get(Form("%s",name2));
  if(h2==NULL)
    {
      cout<<"no histogram available, exiting"<<endl;
      return;
    }


  TH2D *hnew = (TH2D *)h1->Clone();
  hnew->Add(h2);
  hnew->Draw("colz");
  c1->Print(Form("Figures/Burn/%s_new_LED_sum.png",name1));
  c1->Print(Form("Figures/Burn/%s_new_LED_sum.pdf",name1));

  TH1D *hmiddle = (TH1D *)h2->ProjectionY("hmiddle",27,27,"");
  TF1 *reggaus = new TF1("reggaus","gaus",0,16);
  hmiddle->Draw();
  hmiddle->Fit(reggaus,"","",3,13);
  c1->Print("middle_gaus.png");
  //TF1 *gengaus = new TF1("gengaus","[0]*TMath::Exp(-pow(fabs(x-[1]),[3])/[2])",0,16);
  TF1 *gengaus = new TF1("gengaus","[0]*TMath::Exp(-pow(TMath::Abs(x-[1]),[3])/[2])",0,16);
  gengaus->SetParameter(0,80);
  gengaus->SetParameter(1,7.6);
  gengaus->SetParameter(2,2.4);
  gengaus->SetParameter(3,1);
  //gengaus->FixParameter(3,0.5);
  hmiddle->Fit(gengaus,"","",1,15);
  c1->Print("middle_gengaus.png");

  TFile *fout = new TFile("middle.root","recreate");
  fout->cd();
  hmiddle->Write();
  reggaus->Write();
  gengaus->Write();
  fout->Write();
  fout->Close();
  delete fout;

  return;

  TFile *fout1 = new TFile(Form("Data/NewLED/%s_projections.root",name1),"recreate");
  TFile *fout2 = new TFile(Form("Data/NewLED/%s_projections.root",name2),"recreate");

  int nybins = h1->GetNbinsY();
  for(int i=0; i<nybins; i++)
    {
      cout<<"in loop "<<i<<" name1 is "<<name1<<endl;
      TH1D *hpx = (TH1D *)h1->ProjectionX(Form("hprojX_%d",name1,i),i+1,i+1,"");
      TH1D *h2px = (TH1D *)h2->ProjectionX(Form("hprojX_%d",name2,i),i+1,i+1,"");
      h2px->SetLineColor(kRed);
      // ---
      if(hpx->GetMaximum() > h2px->GetMaximum())
	{
	  hpx->Draw();
	  h2px->Draw("same");
	}
      else
	{
	  h2px->Draw();
	  hpx->Draw("same");
	}
      c1->Print(Form("Figures/Burn/%s_projectionX_%d.png",name1,i));
      c1->Print(Form("Figures/Burn/%s_projectionX_%d.pdf",name1,i));
    }


  int nxbins = h1->GetNbinsX();
  for(int i=0; i<nxbins; i++)
    {
      cout<<"in loop "<<i<<" name1 is "<<name1<<endl;
      TH1D *hpy = (TH1D *)h1->ProjectionY(Form("hprojY_%d",name1,i),i+1,i+1,"");
      TH1D *h2py = (TH1D *)h2->ProjectionY(Form("hprojY_%d",name2,i),i+1,i+1,"");
      h2py->SetLineColor(kRed);
      // ---
      if(hpy->GetMaximum() > h2py->GetMaximum())
	{
	  hpy->Draw();
	  h2py->Draw("same");
	}
      else
	{
	  h2py->Draw();
	  hpy->Draw("same");
	}
      c1->Print(Form("Figures/Burn/%s_projectionY_%d.png",name1,i));
      c1->Print(Form("Figures/Burn/%s_projectionY_%d.pdf",name1,i));
    }

  fout1->Write();
  fout1->Close();
  fout2->Write();
  fout2->Close();

  delete fout1;
  delete fout2;


}
