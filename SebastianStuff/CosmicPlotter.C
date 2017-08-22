#include <algorithm>
//const double PE = -0.005;
const double PE = 1.0;

void CosmicPlotter()
{

  TCanvas *c1 = new TCanvas("c1");
  TCanvas *c2 = new TCanvas("c2");
  c1->Divide(1,2);

  ifstream CosmicTxt1;
  ifstream CosmicTxt2;
  CosmicTxt1.open("Data_Txt_Files/Slant_Edge_OH-1-2.txt");
  CosmicTxt2.open("Data_Txt_Files/Straight_Edge_OH-1-2.txt");

  double Vout1;
  double Vout2;
  vector<double>VMin1;
  vector<double>VMin2;
  double possition[41];

  while(CosmicTxt1>>Vout1)
    {
      double vout1=Vout1/PE;
      VMin1.push_back(vout1);
    }
  while(CosmicTxt2>>Vout2) 
    {
      double vout2=Vout2/PE;
      VMin2.push_back(vout2);
    }
  for(int i=0;i<41;i++)
    {
      possition[i] = i*2;
    }

  int Entries = VMin1.size();
  cout << "Entires is " << Entries << endl;
  double max1 = *max_element(VMin1.begin(),VMin1.end());
  cout << "max1 is " << max1 << endl;
  double min1 = *min_element(VMin1.begin(),VMin1.end());
  cout << "min1 is " << min1 << endl;
  double max2 = *max_element(VMin2.begin(),VMin2.end());
  cout << "max2 is " << max2 << endl;
  double min2 = *min_element(VMin2.begin(),VMin2.end());
  cout << "max2 is " << min2 << endl;

  cout<< "Filling Histos"<<endl;

  //TH1D *CosmicV1 = new TH1D("CosmicV1","Log Plot of Cosmic Distribution",Entries*10,min1,max1);
  //TH1D *CosmicV2 = new TH1D("CosmicV2","Log Plot of Cosmic Distribution",Entries*10,min1,max1);
  TH1D *CosmicV1 = new TH1D("CosmicV1","Slant Edge OH-2-47",50,8,9);
  TH1D *CosmicV2 = new TH1D("CosmicV2","Straight Edge OH-2-47",50,8,9);
  TGraph *TG1 = new TGraph();
  TGraph *TG2 = new TGraph();
  //  TNtuple *CosmicNtup = TNtuple("CosmicNtup","nt","Q1:Q2");


  for(int i = 0; i<Entries;i++)
    {
      CosmicV1->Fill(VMin1[i]);
      CosmicV2->Fill(VMin2[i]);
      TG1->SetPoint(i,possition[i],VMin1[i]);
      TG2->SetPoint(i,possition[i],VMin2[i]);
      //  nt->Fill(VMin1[i],VMin2[i]);
    }

  // TFile *CosmicDataMarchPanels = new TFile("CosmicDataMarchPanels","RECREATE");
  // CosmicNtup->Write("CosmicNtup");
  // CosmicDataMarchPanels->Close();

  leg = new TLegend(0.1,0.7,0.3,0.9);
  leg->AddEntry(TG1,"Slant Edge","p");
  leg->AddEntry(TG2,"Straight Edge","p");

  c1->cd(1);
  //gPad->SetLogy();
  CosmicV1->GetXaxis()->SetTitle("Thickness (mm)");
  CosmicV1->GetYaxis()->SetTitle("Counts");
  CosmicV1->GetXaxis()->CenterTitle(true);
  CosmicV1->GetYaxis()->CenterTitle(true);
  CosmicV2->SetLineColor(kBlue);
  CosmicV1->SetLineColor(kRed);
  CosmicV1->Draw();
  c1->cd(2);
  CosmicV2->GetXaxis()->SetTitle("Thickness (mm)");
  CosmicV2->GetYaxis()->SetTitle("Counts");
  CosmicV2->GetXaxis()->CenterTitle(true);
  CosmicV2->GetYaxis()->CenterTitle(true);
  CosmicV2->Draw();
  c1->Print("CosmicsMarch.png");

  c2->cd();
  TG1->SetMarkerStyle(kFullCircle);
  TG1->SetMarkerColor(kBlue);
  TG1->SetTitle("Thickness(x) OH-1-2");
  TG1->GetXaxis()->SetTitle("Position (cm)");
  TG1->GetYaxis()->SetTitle("Thickness (mm)");
  TG1->GetXaxis()->CenterTitle(true);
  TG1->GetYaxis()->CenterTitle(true);
  TG1->GetYaxis()->SetTitleOffset(1.2);
  TG1->SetMaximum(9);
  TG1->SetMinimum(7);
  TG1->Draw("ALP");

  TG2->SetMarkerStyle(kFullCircle);
  TG2->SetMarkerColor(kRed);
  TG2->SetTitle("Thickness(x) Edge OH-2-47");
  TG2->GetXaxis()->SetTitle("Position (cm)");
  TG2->GetYaxis()->SetTitle("Thickness (mm)");
  TG2->GetXaxis()->CenterTitle(true);
  TG2->GetYaxis()->CenterTitle(true);
  TG2->GetYaxis()->SetTitleOffset(1.2);
  TG2->Draw("LP,same");

  leg->Draw();

  //c1->cd(1);
  //CosmicV1->GetXaxis()->SetRangeUser(0,100);
  //c1->cd(2);
  //CosmicV2->GetXaxis()->SetRangeUser(0,100);
  //c1->cd();

  //c1->Print("CosmicsMarchZoom.png");


}
