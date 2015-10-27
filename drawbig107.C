void drawbig107()
{

  int linewidth = 3;

  double radius = 27.5;
  double corner = 57.4;
  double shortmiddleU = 88.2;
  double shortmiddleL = 97.2;
  double shortupper = 175.7;
  double shortmost = 204.0;
  double longupper = 88.3;
  double shorterupper = 114.9;
  double longside = 828.9;
  double shortside = 240.54;
  double shorterside = 166.2;
  double shortratio = shorterside/shortside;
  double aspectratio = longside/shortside;
  cout << shortratio << endl;
  cout << aspectratio << endl;
  int canvasnumber = int(1000.0/aspectratio);
  cout << canvasnumber << endl;

  TCanvas *c1 = new TCanvas("c1","",1000,canvasnumber);
  c1->Draw();

  // TLine *line1 = new TLine((1.0/6.0),0.0,(1.0/6.0),1.0);
  // line1->SetLineColor(kGreen);
  // line1->SetLineWidth(linewidth);
  // line1->Draw();

  double xlow = 0.005;
  double ylow = 0.005;
  double xhigh = 0.999;
  double yhigh_left = 0.999;
  double yhigh_right = 0.999*shortratio;

  TLine *outline_left = new TLine(xlow,ylow,xlow,yhigh_left);
  outline_left->SetLineColor(kBlack);
  outline_left->SetLineWidth(linewidth);
  outline_left->Draw();

  TLine *outline_bottom = new TLine(xlow,ylow,xhigh,ylow);
  outline_bottom->SetLineColor(kBlack);
  outline_bottom->SetLineWidth(linewidth);
  outline_bottom->Draw();

  TLine *outline_right = new TLine(xhigh,ylow,xhigh,yhigh_right);
  outline_right->SetLineColor(kBlack);
  outline_right->SetLineWidth(linewidth);
  outline_right->Draw();

  TLine *outline_top = new TLine(xlow,yhigh_left,xhigh,yhigh_right);
  outline_top->SetLineColor(kBlack);
  outline_top->SetLineWidth(linewidth);
  outline_top->Draw();

  // lowest
  TLine *line1 = new TLine(corner/longside,(corner-radius)/shortside,1-(corner/longside),(corner-radius)/shortside);
  line1->SetLineColor(kGreen);
  line1->SetLineWidth(linewidth);
  line1->Draw();

  // second lowest
  TLine *line2 = new TLine(corner/longside,(corner+radius)/shortside,1-(shortmiddleL/longside),(shortmiddleU-radius)/shortside);
  line2->SetLineColor(kGreen);
  line2->SetLineWidth(linewidth);
  line2->Draw();

  // second highest
  TLine *line3 = new TLine(corner/longside,(shortupper-radius)/shortside,1-(shortmiddleL/longside),(shortmiddleU+radius)/shortside);
  line3->SetLineColor(kGreen);
  line3->SetLineWidth(linewidth);
  line3->Draw();

  // highest (long part)
  TLine *line4 = new TLine(longupper/longside,shortmost/shortside,1-(corner/longside),(shorterupper+radius)/shortside);
  line4->SetLineColor(kGreen);
  line4->SetLineWidth(linewidth);
  line4->Draw();

  // highest (short part)
  TLine *line5 = new TLine(xlow,shortmost/shortside,longupper/longside,shortmost/shortside);
  line5->SetLineColor(kGreen);
  line5->SetLineWidth(linewidth);
  line5->Draw();

  // rightmost
  TLine *line6 = new TLine(1-((corner-radius)/longside),corner/shortside,1-((corner-radius)/longside),shorterupper/shortside);
  line6->SetLineColor(kGreen);
  line6->SetLineWidth(linewidth);
  line6->Draw();

  // lower left arc
  TEllipse *ellipse1 = new TEllipse(corner/longside,corner/shortside,radius/longside,radius/shortside,90,270);
  ellipse1->SetLineColor(kGreen);
  ellipse1->SetLineWidth(linewidth);
  ellipse1->SetNoEdges();
  ellipse1->Draw();

  // middle right arc
  TEllipse *ellipse3 = new TEllipse(1-shortmiddleL/longside,shortmiddleU/shortside,radius/longside,radius/shortside,-90,90);
  ellipse3->SetLineColor(kGreen);
  ellipse3->SetLineWidth(linewidth);
  ellipse3->SetNoEdges();
  ellipse3->Draw();

  // TBox *box1 = new TBox(0.001,0.001,0.999,0.999);
  // box1->SetLineColor(kBlack);
  // box1->SetFillStyle(0);
  // box1->SetLineWidth(2);
  // box1->SetLineStyle(1);
  // box1->Draw("same");

  c1->Print("PanelFigures/bigpanel107_empty.png");
  c1->Print("PanelFigures/bigpanel107_empty.pdf");

  // TLine *linescan = new TLine((0.2/6.0),0.0,(0.2/6.0),1.0);
  // linescan->SetLineWidth(linewidth);
  // linescan->SetLineStyle(2);
  // linescan->Draw();
  // c1->Print("PanelFigures/panel2_scanatminus20cm.png");
  // c1->Print("PanelFigures/panel2_scanatminus20cm.pdf");
  // delete linescan;
}

