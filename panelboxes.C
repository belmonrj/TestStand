void panelboxes()
{

  int linewidth = 5;

  TCanvas *c1 = new TCanvas("c1","",480,800);
  c1->Draw();

  TLine *line1 = new TLine((1.0/6.0),0.0,(1.0/6.0),1.0);
  line1->SetLineColor(kGreen);
  line1->SetLineWidth(linewidth);
  line1->Draw();

  TLine *line2 = new TLine((3.0/6.0),0.0,(3.0/6.0),1.0);
  line2->SetLineColor(kGreen);
  line2->SetLineWidth(linewidth);
  line2->Draw();

  TLine *line3 = new TLine((4.0/6.0),0.0,(4.0/6.0),1.0);
  line3->SetLineColor(kGreen);
  line3->SetLineWidth(linewidth);
  line3->Draw();

  TLine *line4 = new TLine((5.0/6.0),0.0,(5.0/6.0),1.0);
  line4->SetLineColor(kGreen);
  line4->SetLineWidth(linewidth);
  line4->Draw();

  TBox *box1 = new TBox(0.001,0.001,0.999,0.999);
  box1->SetLineColor(kBlack);
  box1->SetFillStyle(0);
  box1->SetLineWidth(2);
  box1->SetLineStyle(1);
  box1->Draw("same");

  c1->Print("PanelFigures/panel2_empty.png");
  c1->Print("PanelFigures/panel2_empty.pdf");

  TLine *linescan = new TLine((0.2/6.0),0.0,(0.2/6.0),1.0);
  linescan->SetLineWidth(linewidth);
  linescan->SetLineStyle(2);
  linescan->Draw();
  c1->Print("PanelFigures/panel2_scanatminus20cm.png");
  c1->Print("PanelFigures/panel2_scanatminus20cm.pdf");

  delete linescan;
  TLine *linescan = new TLine((0.4/6.0),0.0,(0.4/6.0),1.0);
  linescan->SetLineWidth(linewidth);
  linescan->SetLineStyle(2);
  linescan->Draw();
  c1->Print("PanelFigures/panel2_scanatminus15cm.png");
  c1->Print("PanelFigures/panel2_scanatminus15cm.pdf");

  delete linescan;
  TLine *linescan = new TLine((0.6/6.0),0.0,(0.6/6.0),1.0);
  linescan->SetLineWidth(linewidth);
  linescan->SetLineStyle(2);
  linescan->Draw();
  c1->Print("PanelFigures/panel2_scanatminus10cm.png");
  c1->Print("PanelFigures/panel2_scanatminus10cm.pdf");

  delete linescan;
  TLine *linescan = new TLine((0.8/6.0),0.0,(0.8/6.0),1.0);
  linescan->SetLineWidth(linewidth);
  linescan->SetLineStyle(2);
  linescan->Draw();
  c1->Print("PanelFigures/panel2_scanatminus05cm.png");
  c1->Print("PanelFigures/panel2_scanatminus05cm.pdf");

  delete linescan;
  TLine *linescan = new TLine((1.0/6.0),0.0,(1.0/6.0),1.0);
  linescan->SetLineWidth(linewidth);
  linescan->SetLineStyle(2);
  linescan->Draw();
  c1->Print("PanelFigures/panel2_scanatminus00cm.png");
  c1->Print("PanelFigures/panel2_scanatminus00cm.pdf");

  delete linescan;
  TLine *linescan = new TLine((1.0/6.0),0.0,(1.0/6.0),1.0);
  linescan->SetLineWidth(linewidth);
  linescan->SetLineStyle(2);
  linescan->Draw();
  c1->Print("PanelFigures/panel2_scanatplus00cm.png");
  c1->Print("PanelFigures/panel2_scanatplus00cm.pdf");

  delete linescan;
  TLine *linescan = new TLine((1.2/6.0),0.0,(1.2/6.0),1.0);
  linescan->SetLineWidth(linewidth);
  linescan->SetLineStyle(2);
  linescan->Draw();
  c1->Print("PanelFigures/panel2_scanatplus05cm.png");
  c1->Print("PanelFigures/panel2_scanatplus05cm.pdf");

  delete linescan;
  TLine *linescan = new TLine((1.4/6.0),0.0,(1.4/6.0),1.0);
  linescan->SetLineWidth(linewidth);
  linescan->SetLineStyle(2);
  linescan->Draw();
  c1->Print("PanelFigures/panel2_scanatplus10cm.png");
  c1->Print("PanelFigures/panel2_scanatplus10cm.pdf");

  delete linescan;
  TLine *linescan = new TLine((1.6/6.0),0.0,(1.6/6.0),1.0);
  linescan->SetLineWidth(linewidth);
  linescan->SetLineStyle(2);
  linescan->Draw();
  c1->Print("PanelFigures/panel2_scanatplus15cm.png");
  c1->Print("PanelFigures/panel2_scanatplus15cm.pdf");

  delete linescan;
  TLine *linescan = new TLine((1.8/6.0),0.0,(1.8/6.0),1.0);
  linescan->SetLineWidth(linewidth);
  linescan->SetLineStyle(2);
  linescan->Draw();
  c1->Print("PanelFigures/panel2_scanatplus20cm.png");
  c1->Print("PanelFigures/panel2_scanatplus20cm.pdf");

  delete linescan;
  TLine *linescan = new TLine((2.0/6.0),0.0,(2.0/6.0),1.0);
  linescan->SetLineWidth(linewidth);
  linescan->SetLineStyle(2);
  linescan->Draw();
  c1->Print("PanelFigures/panel2_scanatplus25cm.png");
  c1->Print("PanelFigures/panel2_scanatplus25cm.pdf");

}

