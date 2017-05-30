
void file()
{

  TCanvas *C1 = new TCanvas();
  //  C1->SetLogy();

  cout << "Hello World" << endl;

  ofstream fout;
  fout.open("NameTest.txt");

  for(int i = 0; i<10;i++)
    {
      fout << i << " "  << i*i << endl;
    }

  fout.close();

  ifstream fin;
  fin.open("NameTest.txt");

  float xvar[10];
  float yvar[10];

  float exvar[10];
  float eyvar[10];

  float x2var[10];
  float y2var[10];

  float ex2var[10];
  float ey2var[10];

  for(int i=0;i<10;i++)
    {
      int x,y;
      fin >> x >> y;
      cout<< x << " " << y <<endl;
      xvar[i]=x;
      yvar[i]=y;
      exvar[i]=0;
      //exvar[i]=0.1+0.1*x;
      eyvar[i]=0.1+0.1*y;
      x2var[i]=x;
      y2var[i]= y + gRandom->Gaus(0,1.5*eyvar[i]);
      ex2var[i]=0;
      ey2var[i]=eyvar[i];
    }

  TGraph *TG1 = new TGraph(10,xvar,yvar);
  TG1->SetMarkerStyle(kFullCircle);
  TG1->SetMarkerColor(kBlack);
  TG1->Draw("ap");//a means access and p means point
  TG1->SetTitle("Graph Title");
  TG1->GetXaxis()->SetTitle("Title of X Axis");
  TG1->GetYaxis()->SetTitle("Title of Y Axis");
  TG1->GetXaxis()->SetLimits(-1,11);//ctrl space to start, select final column and ctrl x + "rk"
  //  TG1->GetYaxis()->SetLimits(-1,100);
  TG1->SetMinimum(-1);
  TG1->SetMaximum(100);

  TG1->GetYaxis()->SetTitle("Title of Y Axis");

  TF1 *Fun1 = new TF1("Fun1","pol2",0,10);//pol2 is a second order polynomial  
  TF1 *Fun2 = new TF1("Fun2","[0]*x*x",0,10);//0 gives the 0ths pweramter
  TF1 *Fun3 = new TF1("Fun3","[0]*TMath::Power(x,[1])",0,10);//now the poly order is being fit as well

  TG1->Fit(Fun1,"R");
  TLegend *Leg = new TLegend(0.2,0.6,0.4,0.8);//arguments are xlow,ylow,xhigh,yhigh
  Leg->AddEntry(TG1, "Data","p");
  Leg->AddEntry(Fun1, "Fit","l");
  Leg->Draw();

  TGraphErrors *eTG1 = new TGraphErrors(10,xvar,yvar,exvar,eyvar);
  eTG1->SetMarkerStyle(kFullCircle);
  eTG1->SetMarkerColor(kBlack);
  eTG1->Draw("ap");//a means access and p means point
  // eTG1->Draw("p");//p just draws on top of what you have already
  eTG1->SetTitle("Graph Title");
  eTG1->GetXaxis()->SetTitle("Title of X Axis");
  eTG1->GetYaxis()->SetTitle("Title of Y Axis");
  eTG1->GetXaxis()->SetLimits(-1,11);//ctrl space to start, select final column and ctrl x + "rk"
  //  eTG1->GetYaxis()->SetLimits(-1,100);
  eTG1->SetMinimum(-1);
  eTG1->SetMaximum(100);
  
  eTG1->Fit(Fun1,"R");//R means fit using the specified range

  TGraphErrors *eTG2 = new TGraphErrors(10,x2var,y2var,ex2var,ey2var);
  eTG2->SetMarkerStyle(kFullCircle);
  eTG2->SetMarkerColor(kBlack);
  eTG2->Draw("ap");//a means access and p means point
  // eTG2->Draw("p");//p just draws on top of what you have already
  eTG2->SetTitle("Graph Title");
  eTG2->GetXaxis()->SetTitle("Title of X Axis");
  eTG2->GetYaxis()->SetTitle("Title of Y Axis");
  eTG2->GetXaxis()->SetLimits(-1,11);//ctrl space to start, select final column and ctrl x + "rk"
  //  eTG2->GetYaxis()->SetLimits(-1,100);
  eTG2->SetMinimum(-1);
  eTG2->SetMaximum(100);
  
  eTG2->Fit(Fun1,"R");//R means fit using the specified range

  //shift esc 5 give find and replace func, with shift ! is yes to all
 
}
