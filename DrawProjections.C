void DrawProjections()
{

  drawproj("A4_LED_Assymetry_VMin_SiPM1_meanHistSub");
  drawproj("A4_LED_Assymetry_VMin_SiPM1_meanHistSub");

}

void drawproj(const char* basename)
{

  TFile *file = TFile::Open(Form("Data/ROOT/%s_projections.root",basename));

  TH1D *hp0 = (TH1D*)file->Get(Form("%s_hproj_0",basename));
  TH1D *hp1 = (TH1D*)file->Get(Form("%s_hproj_1",basename));
  TH1D *hp2 = (TH1D*)file->Get(Form("%s_hproj_2",basename));
  TH1D *hp3 = (TH1D*)file->Get(Form("%s_hproj_3",basename));
  TH1D *hp4 = (TH1D*)file->Get(Form("%s_hproj_4",basename));
  TH1D *hp5 = (TH1D*)file->Get(Form("%s_hproj_5",basename));
  TH1D *hp6 = (TH1D*)file->Get(Form("%s_hproj_6",basename));
  TH1D *hp7 = (TH1D*)file->Get(Form("%s_hproj_7",basename));
  TH1D *hp8 = (TH1D*)file->Get(Form("%s_hproj_8",basename));

  hp5->SetLineColor(kBlack);
  hp5->SetLineWidth(2);
  hp5->Draw(); // directly over fiber (THIS MAY CHANGE FROM SCAN SCAN TO SCAN, NEED TO BE CAREFUL)
  hp5->SetMinimum(0.0);
  hp5->GetXaxis()->SetTitle("distance (cm)");
  hp5->GetYaxis()->SetTitle("Number of photoelectrons");
  //hp5->GetYaxis()->SetTitle("Estimated number of photoelectrons");
  // hp4->Add(hp6);
  // hp4->Scale(0.5); // average 4 and 6
  hp4->SetLineColor(kRed);
  hp4->SetLineWidth(2);
  hp4->Draw("same"); // 0.5 cm away from fiber
  hp3->Add(hp7);
  hp3->Scale(0.5); // average 3 and 7
  hp3->SetLineColor(kBlue);
  hp3->SetLineWidth(2);
  hp3->Draw("same"); // 1.0 cm away from fiber
  hp2->Add(hp8);
  hp2->Scale(0.5); // average 2 and 8
  hp2->SetLineColor(kGreen+2);
  hp2->SetLineWidth(2);
  hp2->Draw("same"); // 1.5 cm away from fiber
  hp1->Add(hp0);
  hp1->Scale(0.5); // average 0 (2.5 cm away) and 1 (2.0 cm away)
  hp1->SetLineColor(kMagenta);
  hp1->SetLineWidth(2);
  // hp1->Draw("same"); // 2.25 cm away from fiber, doesn't look nice

  TLegend *leg = new TLegend(0.56,0.68,0.88,0.88);
  leg->AddEntry(hp5,"over fiber","l");
  leg->AddEntry(hp4,"0.5 cm off fiber","l");
  leg->AddEntry(hp3,"1.0 cm off fiber","l");
  leg->AddEntry(hp2,"1.5 cm off fiber","l");
  leg->SetTextSize(0.05);
  leg->Draw();

  c1->Print(Form("Figures/%s_projections.png",basename));
  c1->Print(Form("Figures/%s_projections.pdf",basename));

  double Ihp1 = hp1->Integral();
  double Ihp2 = hp2->Integral();
  double Ihp3 = hp3->Integral();
  double Ihp4 = hp4->Integral();
  double Ihp5 = hp5->Integral();


  hp5->Scale(1/Ihp5);
  hp4->Scale(1/Ihp4);
  hp3->Scale(1/Ihp3);
  hp2->Scale(1/Ihp2);
  hp1->Scale(1/Ihp1);
  hp5->Draw();
  hp5->GetYaxis()->SetTitleOffset(1.3);
  hp2->Draw("same");
  hp3->Draw("same");
  hp4->Draw("same");

  TLegend *leg2 = new TLegend(0.33,0.68,0.88,0.88);
  leg2->AddEntry(hp5,Form("over fiber, #times %f",1/Ihp1),"l");
  leg2->AddEntry(hp4,Form("0.5 cm off fiber, #times %f",1/Ihp2),"l");
  leg2->AddEntry(hp3,Form("1.0 cm off fiber, #times %f",1/Ihp3),"l");
  leg2->AddEntry(hp2,Form("1.5 cm off fiber, #times %f",1/Ihp4),"l");
  leg2->SetTextSize(0.05);
  //leg2->Draw();

  TLegend *leg3 = new TLegend(0.43,0.63,0.88,0.88);
  leg3->AddEntry(hp5,"over fiber","l");
  leg3->AddEntry(hp4,"0.5 cm off fiber","l");
  leg3->AddEntry(hp3,"1.0 cm off fiber","l");
  leg3->AddEntry(hp2,"1.5 cm off fiber","l");
  leg3->AddEntry((TObject*)0x0,"Normalized to integral","");
  leg3->SetTextSize(0.05);
  leg3->Draw();

  c1->Print(Form("Figures/%s_projectionsSC.png",basename));
  c1->Print(Form("Figures/%s_projectionsSC.pdf",basename));

}
