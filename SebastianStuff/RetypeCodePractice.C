void RetypeCodePractice()
{
  
  TCanvas *C1 = new TCanvas();// Makes a new TCanvas Object

  cout << "Hello World" << endl;

  ofstream fout;
  fout.open("NameTest.txt");

  for(int i = 0; i<10; i++)
    {
      fout << i << " " << i*i << endl;//fills the file we made with two columns of data
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

  for (int i = 0; i<10;i++)
    {
      int x,y;
      fin >> x >> y;
      cout << x << " " << y << endl;
      xvar[i] = x;
      yvar[i] = y;
      exvar[i] = 0;//By convention for phenix, the error for x is soomehow incorportated into only y errors
      eyvar[i] = 0.1 + 0.1 *y;
      x2var[i] = x;
      y2var[i] = y + gRandom->Gaus(0,1.5*eyvar[i]);
      ex2var[i] = 0;
      ey2var[i] = eyvar[i];
    } 

  TGraph *TG1 = new TGraph (10, xvar, yvar);
  TG1->SetMarkerStyle(kFullCircle);
  TG1->SetMarkerColor(kBlack);
  TG1->Draw("ap"); // Draws as individual point
  TG1->SetTitle("Title of Graph");
  TG1->GetXaxis()->SetTitle("Title of X Axis");
  TG1->GetYaxis()->SetTitle("Title of Y Axis");
  TG1->GetXaxis()->SetLimits(-1,11);
  TG1->SetMinimum(-1);
  TG1->SetMaximum(110);


  TF1* Fun1 = new TF1("Fun1","pol2",0,10);
  TF1* Fun2 = new TF1("Fun2","[0]*x*x",0,10);
  TF1* Fun3 = new TF1("Fun3","[0]*TMath::Power(x,[1])",0,10);

  Fun3->SetParameter(0,1.0);// This gives it an initial set of paremters for a guess
  Fun3->SetParameter(1,2.0);

  TG1->Fit(Fun3,"R");//What is R?
  TLegend *Leg = new TLegend(0.2,0.6,0.4,0.8);
  Leg->AddEntry(TG1,"Data","p");
  Leg->AddEntry(Fun1, "Fit","l");
  Leg->Draw();

  TGraphErrors *eTG1 = new TGraphErrors(10,xvar,yvar,exvar,eyvar);

  eTG1->SetMarkerStyle(kFullCircle);
  eTG1->SetMarkerColor(kBlack);
  eTG1->Draw("ap");
  eTG1->SetTitle("G Title");
  eTG1->GetXaxis()->SetTitle("Title for X");
  eTG1->GetYaxis()->SetTitle("Title for Y");
  eTG1->GetXaxis()->SetLimits(-1,11);
  eTG1->SetMinimum(-1);
  eTG1->SetMaximum(110);

  TGraphErrors *eTG2 = new TGraphErrors(10,xvar,yvar,exvar,eyvar);

  eTG2->SetMarkerStyle(kFullCircle);
  eTG2->SetMarkerColor(kBlack);
  eTG2->Draw("ap");
  eTG2->SetTitle("G Title");
  eTG2->GetXaxis()->SetTitle("Title for X");
  eTG2->GetYaxis()->SetTitle("Title for Y");
  eTG2->GetXaxis()->SetLimits(-1,11);
  eTG2->SetMinimum(-1);
  eTG2->SetMaximum(110);

  eTG2->Fit(Fun3,"R");

}
