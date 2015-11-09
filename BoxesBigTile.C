void BoxesBigTile()
{

  TCanvas *c1 = new TCanvas("c1","c1", 1800, 522);

  gStyle->SetOptStat(0);

  TFile *file = TFile::Open("Data/BigPanel/20151026-1704_VMIN_SIPM1_meanHistSub.root");
  TH2D *meanHistSub = (TH2D *)file->Get("meanHistSub");

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);

  c1->SetRightMargin(0.15);
  meanHistSub->GetXaxis()->SetTitle("Position Y (cm)");
  meanHistSub->GetYaxis()->SetTitle("Position X (cm)");
  meanHistSub->GetZaxis()->SetTitle("Pulse Min Value (photoelectrons)");
  meanHistSub->Draw("colz");
  c1->Print(Form("Figures/Burn/boxes_meanHistSub.png"));
  c1->Print(Form("Figures/Burn/boxes_meanHistSub.pdf"));

  double xsize = 83.0;
  double ysize = 24.0;
  double xoff = 2.0;

  TBox *box1 = new TBox(xsize*0.2+xoff,ysize*0.6,xsize*0.23+xoff,ysize*0.7);
  box1->SetLineColor(kBlack);
  box1->SetFillStyle(0);
  box1->SetLineWidth(2);
  box1->SetLineStyle(1);
  box1->Draw("same");

  double xlo = xsize*0.2+xoff;
  double ylo = ysize*0.6;
  double xhi = xsize*0.23+xoff;
  double yhi = ysize*0.7;

  int xbinlo = int(xlo*2.0);
  int ybinlo = int(ylo*2.0);
  int xbinhi = int(xhi*2.0);
  int ybinhi = int(yhi*2.0);

  double content = 0;
  int counter = 0;
  for(int ix = xbinlo; ix < xbinhi; ix++)
    {
      for(int iy = ybinlo; iy < ybinhi; iy++)
	{
	  content += meanHistSub->GetBinContent(ix+1,iy+1);
	  counter++;
	}
    }
  content /= counter;
  cout << content << endl;

  TLatex *tex1 = new TLatex(xhi,yhi,Form("%.1f",content));
  tex1->SetTextSize(0.05);
  tex1->Draw();
  TLatex *tex1c = new TLatex(xhi,ylo-1.0,Form("26.2#pm0.4"));
  tex1c->SetTextSize(0.05);
  tex1c->Draw();

  TBox *box2 = new TBox(xsize*0.5+xoff,ysize*0.4,xsize*0.53+xoff,ysize*0.5);
  box2->SetLineColor(kBlack);
  box2->SetFillStyle(0);
  box2->SetLineWidth(2);
  box2->SetLineStyle(1);
  box2->Draw("same");

  xlo = xsize*0.5+xoff;
  ylo = ysize*0.4;
  xhi = xsize*0.53+xoff;
  yhi = ysize*0.5;

  xbinlo = int(xlo*2.0);
  ybinlo = int(ylo*2.0);
  xbinhi = int(xhi*2.0);
  ybinhi = int(yhi*2.0);

  content = 0;
  counter = 0;
  for(int ix = xbinlo; ix < xbinhi; ix++)
    {
      for(int iy = ybinlo; iy < ybinhi; iy++)
	{
	  content += meanHistSub->GetBinContent(ix+1,iy+1);
	  counter++;
	}
    }
  content /= counter;
  cout << content << endl;

  TLatex *tex2 = new TLatex(xhi,yhi,Form("%.1f",content));
  tex2->SetTextSize(0.05);
  tex2->Draw();
  TLatex *tex2c = new TLatex(xhi,ylo-1.0,Form("21.6#pm0.4"));
  tex2c->SetTextSize(0.05);
  tex2c->Draw();

  TBox *box3 = new TBox(xsize*0.06+xoff,ysize*0.2,xsize*0.09+xoff,ysize*0.3);
  box3->SetLineColor(kBlack);
  box3->SetFillStyle(0);
  box3->SetLineWidth(2);
  box3->SetLineStyle(1);
  box3->Draw("same");

  xlo = xsize*0.06+xoff;
  ylo = ysize*0.2;
  xhi = xsize*0.09+xoff;
  yhi = ysize*0.3;

  xbinlo = int(xlo*2.0);
  ybinlo = int(ylo*2.0);
  xbinhi = int(xhi*2.0);
  ybinhi = int(yhi*2.0);

  content = 0;
  counter = 0;
  for(int ix = xbinlo; ix < xbinhi; ix++)
    {
      for(int iy = ybinlo; iy < ybinhi; iy++)
	{
	  content += meanHistSub->GetBinContent(ix+1,iy+1);
	  counter++;
	}
    }
  content /= counter;
  cout << content << endl;

  TLatex *tex3 = new TLatex(xhi,yhi,Form("%.1f",content));
  tex3->SetTextSize(0.05);
  tex3->Draw();
  TLatex *tex3c = new TLatex(xhi,ylo-1.0,Form("19.8#pm0.7"));
  tex3c->SetTextSize(0.05);
  tex3c->Draw();

  TBox *box4 = new TBox(xsize*0.06+xoff,ysize*0.667,xsize*0.09+xoff,ysize*0.767);
  box4->SetLineColor(kBlack);
  box4->SetFillStyle(0);
  box4->SetLineWidth(2);
  box4->SetLineStyle(1);
  box4->Draw("same");

  xlo = xsize*0.06+xoff;
  ylo = ysize*0.667;
  xhi = xsize*0.09+xoff;
  yhi = ysize*0.767;

  xbinlo = int(xlo*2.0);
  ybinlo = int(ylo*2.0);
  xbinhi = int(xhi*2.0);
  ybinhi = int(yhi*2.0);

  content = 0;
  counter = 0;
  for(int ix = xbinlo; ix < xbinhi; ix++)
    {
      for(int iy = ybinlo; iy < ybinhi; iy++)
	{
	  content += meanHistSub->GetBinContent(ix+1,iy+1);
	  counter++;
	}
    }
  content /= counter;
  cout << content << endl;

  TLatex *tex4 = new TLatex(xhi,yhi,Form("%.1f",content));
  tex4->SetTextSize(0.05);
  tex4->Draw();
  TLatex *tex4c = new TLatex(xhi,ylo-1.0,Form("27.8#pm0.5"));
  tex4c->SetTextSize(0.05);
  tex4c->Draw();


  TBox *box5 = new TBox(xsize*0.86+xoff,ysize*0.35,xsize*0.89+xoff,ysize*0.45);
  box5->SetLineColor(kBlack);
  box5->SetFillStyle(0);
  box5->SetLineWidth(2);
  box5->SetLineStyle(1);
  box5->Draw("same");

  xlo = xsize*0.86+xoff;
  ylo = ysize*0.35;
  xhi = xsize*0.89+xoff;
  yhi = ysize*0.45;

  xbinlo = int(xlo*2.0);
  ybinlo = int(ylo*2.0);
  xbinhi = int(xhi*2.0);
  ybinhi = int(yhi*2.0);

  content = 0;
  counter = 0;
  for(int ix = xbinlo; ix < xbinhi; ix++)
    {
      for(int iy = ybinlo; iy < ybinhi; iy++)
	{
	  content += meanHistSub->GetBinContent(ix+1,iy+1);
	  counter++;
	}
    }
  content /= counter;
  cout << content << endl;

  TLatex *tex5 = new TLatex(xhi,yhi,Form("%.1f",content));
  tex5->SetTextSize(0.05);
  tex5->Draw();
  TLatex *tex5c = new TLatex(xhi,ylo-1.0,Form("22.4#pm0.6"));
  tex5c->SetTextSize(0.05);
  tex5c->Draw();

  c1->Print(Form("Figures/Burn/bboxes_meanHistSub.png"));
  c1->Print(Form("Figures/Burn/bboxes_meanHistSub.pdf"));

  delete c1;

}
