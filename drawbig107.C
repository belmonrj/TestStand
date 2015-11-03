void drawbig107()
{

  int linewidth = 2;

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
  int canvasnumber = int(1300.0/aspectratio);
  cout << canvasnumber << endl;

  TCanvas *c1 = new TCanvas("c1","",1300,canvasnumber);
  c1->Draw();

  double xlow = 0.001;
  double ylow = 0.003;
  double xhigh = 0.999;
  double yhigh = 0.999;
  double yhigh_left = yhigh;
  double yhigh_right = yhigh*shortratio;

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

  // lower right arc
  TEllipse *ellipse2 = new TEllipse(1-corner/longside,corner/shortside,radius/longside,radius/shortside,-90,0);
  ellipse2->SetLineColor(kGreen);
  ellipse2->SetLineWidth(linewidth);
  ellipse2->SetNoEdges();
  ellipse2->Draw();

  // upper right arc
  TEllipse *ellipse4 = new TEllipse(1-corner/longside,shorterupper/shortside,radius/longside,radius/shortside,0,90);
  ellipse4->SetLineColor(kGreen);
  ellipse4->SetLineWidth(linewidth);
  ellipse4->SetNoEdges();
  ellipse4->Draw();

  // upper left arc, inner
  TEllipse *ellipse5 = new TEllipse(corner/longside,shortupper/shortside,radius/longside,radius/shortside,180,270);
  ellipse5->SetLineColor(kGreen);
  ellipse5->SetLineWidth(linewidth);
  ellipse5->SetNoEdges();
  ellipse5->Draw();

  // upper left arc, outer
  TEllipse *ellipse6 = new TEllipse(0.010,shortupper/shortside,0.79*radius/longside,radius/shortside,0,90);
  ellipse6->SetLineColor(kGreen);
  ellipse6->SetLineWidth(linewidth);
  ellipse6->SetNoEdges();
  ellipse6->Draw();

  c1->Print("PanelFigures/bigpanel107_empty.png");
  c1->Print("PanelFigures/bigpanel107_empty.pdf");

  TBox *box1 = new TBox(0.2,0.6,0.23,0.7);
  box1->SetLineColor(kBlack);
  box1->SetFillStyle(0);
  box1->SetLineWidth(2);
  box1->SetLineStyle(1);
  box1->Draw("same");

  c1->Print("PanelFigures/bigpanel107_box1.png");
  c1->Print("PanelFigures/bigpanel107_box1.pdf");
  delete box1;

  TBox *box2 = new TBox(0.5,0.4,0.53,0.5);
  box2->SetLineColor(kBlack);
  box2->SetFillStyle(0);
  box2->SetLineWidth(2);
  box2->SetLineStyle(1);
  box2->Draw("same");

  c1->Print("PanelFigures/bigpanel107_box2.png");
  c1->Print("PanelFigures/bigpanel107_box2.pdf");
  delete box2;

  TBox *box3 = new TBox(0.06,0.2,0.09,0.3);
  box3->SetLineColor(kBlack);
  box3->SetFillStyle(0);
  box3->SetLineWidth(2);
  box3->SetLineStyle(1);
  box3->Draw("same");

  c1->Print("PanelFigures/bigpanel107_box3.png");
  c1->Print("PanelFigures/bigpanel107_box3.pdf");
  delete box3;

  TBox *box4 = new TBox(0.06,0.667,0.09,0.767);
  box4->SetLineColor(kBlack);
  box4->SetFillStyle(0);
  box4->SetLineWidth(2);
  box4->SetLineStyle(1);
  box4->Draw("same");

  c1->Print("PanelFigures/bigpanel107_box4.png");
  c1->Print("PanelFigures/bigpanel107_box4.pdf");
  delete box4;

  TBox *box5 = new TBox(0.86,0.35,0.89,0.45);
  box5->SetLineColor(kBlack);
  box5->SetFillStyle(0);
  box5->SetLineWidth(2);
  box5->SetLineStyle(1);
  box5->Draw("same");

  c1->Print("PanelFigures/bigpanel107_box5.png");
  c1->Print("PanelFigures/bigpanel107_box5.pdf");
  delete box5;

  int start = 4;
  int finish = 171;
  for ( int i=start; i<finish; i++ )
    {
      double xlow = (i-4)/166.0 ;
      double xhigh = (i-4)/166.0 ;
      double ylow = 0;
      double yhigh = 1;
      TLine *line = new TLine(xlow,ylow,xhigh,yhigh);
      line->SetLineWidth(3);
      line->SetLineStyle(2);
      line->Draw();
      c1->Print(Form("PanelFigures/bigpanel107_proj_%d.png",i));
      c1->Print(Form("PanelFigures/bigpanel107_proj_%d.pdf",i));
      //cout << xlow << endl;
      delete line;
    }

}

