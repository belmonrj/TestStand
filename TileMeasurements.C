void argument(TString);

void TileMeasurements()
{

  argument("OH-1-2");
  argument("OH-1-3");
  argument("OH-1-46");
  argument("OH-1-47");
  argument("OH-2-6");
  argument("OH-2-46");
  argument("OH-2-47");

}

void argument(TString arg)
{

  TCanvas *c1 = new TCanvas();

  TString innerfile = "Data/";
  //innerfile += "Straight_Edge_";
  innerfile += "Slant_Edge_"; // Sebastian labeled them backwards
  innerfile += arg;
  innerfile += ".txt";

  TString outerfile = "Data/";
  //outerfile += "Slant_Edge_";
  outerfile += "Straight_Edge_"; // Sebastian labeled them backwards
  outerfile += arg;
  outerfile += ".txt";

  //cout << innerfile << " " << outerfile << endl;

  ifstream f_inner(innerfile);
  ifstream f_outer(outerfile);
  float position_inner[40];
  float thickness_inner[40];
  float position_outer[40];
  float thickness_outer[40];
  for ( int i = 0; i < 40; ++i )
    {
      // position_inner[i] = i*2;
      // position_outer[i] = i*2;
      position_inner[i] = 80 - i*2;
      position_outer[i] = 80 - i*2;
      f_inner >> thickness_inner[i];
      f_outer >> thickness_outer[i];
    }

  TGraph *tg_inner = new TGraph(40,position_inner,thickness_inner);
  tg_inner->SetMarkerColor(kBlue);
  tg_inner->SetMarkerStyle(kFullCircle);
  tg_inner->Draw("apl");
  tg_inner->SetMinimum(7.0);
  tg_inner->SetMaximum(9.0);
  tg_inner->GetXaxis()->SetLimits(-4,84);
  tg_inner->GetXaxis()->SetTitle("Position (cm)");
  tg_inner->GetYaxis()->SetTitle("Thickness (mm)");
  TGraph *tg_outer = new TGraph(40,position_outer,thickness_outer);
  tg_outer->SetMarkerColor(kRed);
  tg_outer->SetMarkerStyle(kFullCircle);
  tg_outer->Draw("pl");
  TLegend *leg = new TLegend(0.18,0.68,0.38,0.88);
  //TLegend *leg = new TLegend(0.68,0.18,0.88,0.38);
  //TLegend *leg = new TLegend(0.68,0.68,0.88,0.88);
  leg->AddEntry(tg_inner,"Inner Edge","p");
  leg->AddEntry(tg_outer,"Outer Edge","p");
  leg->Draw();
  TLatex *tex_tid = new TLatex(0.55,0.819,Form("Tile ID %s",(const char*)arg));
  tex_tid->SetNDC();
  tex_tid->Draw();

  c1->Print(Form("Figs/Thickness_%s.png",(const char*)arg));
  c1->Print(Form("Figs/Thickness_%s.pdf",(const char*)arg));



  delete tg_inner;
  delete tg_outer;
  delete leg;
  delete c1;

}
