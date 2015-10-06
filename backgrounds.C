#include <algorithm> // for min_element, max_element


void backgrounds()
{

  ifstream fin3("TEMP/20151005-1352_Unaveraged_VMin1.txt");
  double content;
  vector<double> voltage3;
  while(fin3>>content)
    {
      voltage3.push_back(content);
    }
  fin3.close();
  cout << voltage3.size() << endl;

  ifstream fin4("TEMP/20151005-1352_Unaveraged_VMin2.txt");
  vector<double> voltage4;
  while(fin4>>content)
    {
      voltage4.push_back(content);
    }
  fin4.close();
  cout << voltage4.size() << endl;

  // --- get the max, min of the vector
  double max = *max_element(voltage3.begin(),voltage3.end());
  double min = *min_element(voltage3.begin(),voltage3.end());
  cout << max << endl;
  cout << min << endl;
  // --- calculate the limits for the histograms
  double newmax = min*-0.95;
  double newmin = max*-1.05 - newmax*0.1;

  int numberB = voltage3.size();

  TH1D *h3 = new TH1D("h3","",100,newmin,newmax);
  TH1D *h4 = new TH1D("h4","",100,newmin,newmax);
  for(int i=0; i<numberB; i++)
    {
      // --- SiPM1
      content = -1*voltage3[i];
      h3->Fill(content);
      // --- SiPM2
      content = -1*voltage4[i];
      h4->Fill(content);
    }

  h3->SetLineColor(kRed);
  h3->SetLineWidth(2);
  h4->SetLineColor(kBlue);
  h4->SetLineWidth(2);
  // --- adjust limits
  double peconvert = 0.00502; // volts per photoelectrion
  h3->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  h4->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  // --- draw
  h3->Scale(1.0/numberB);
  h4->Scale(1.0/numberB);
  h3->Draw("");
  h4->Draw("same");
  // --- print figures
  c1->SetLogy(0);
  c1->Print("Backgrounds/backgrounds_part1.png");
  c1->SetLogy(1);
  c1->Print("Backgrounds/backgrounds_part1_log.png");

  //20150915-1612


  // ------------------------------------------------------


  ifstream fin5("TEMP/20150915-1612_Unaveraged_VMin1.txt");
  double content;
  vector<double> voltage5;
  while(fin5>>content)
    {
      voltage5.push_back(content);
    }
  fin5.close();
  cout << voltage5.size() << endl;

  ifstream fin6("TEMP/20150915-1612_Unaveraged_VMin2.txt");
  vector<double> voltage6;
  while(fin6>>content)
    {
      voltage6.push_back(content);
    }
  fin6.close();
  cout << voltage6.size() << endl;

  int numberC = voltage5.size();

  TH1D *h5 = new TH1D("h5","",100,newmin,newmax);
  TH1D *h6 = new TH1D("h6","",100,newmin,newmax);
  for(int i=0; i<numberC; i++)
    {
      // --- SiPM1
      content = -1*voltage5[i];
      h5->Fill(content);
      // --- SiPM2
      content = -1*voltage6[i];
      h6->Fill(content);
    }

  h5->SetLineColor(kRed);
  h5->SetLineWidth(2);
  h6->SetLineColor(kBlue);
  h6->SetLineWidth(2);
  // --- adjust limits
  double peconvert = 0.00502; // volts per photoelectrion
  h5->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  h6->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  // --- draw
  h5->Scale(1.0/numberC);
  h6->Scale(1.0/numberC);
  h5->Draw("same");
  h6->Draw("same");

  c1->SetLogy(0);
  c1->Print("Backgrounds/backgrounds_part2.png");
  c1->SetLogy(1);
  c1->Print("Backgrounds/backgrounds_part2_log.png");



  // ------------------------------------------------------

  ifstream fin7("TEMP/20151005-1452_Unaveraged_VMin1.txt");
  double content;
  vector<double> voltage7;
  while(fin7>>content)
    {
      voltage7.push_back(content);
    }
  fin7.close();
  cout << voltage7.size() << endl;

  ifstream fin8("TEMP/20151005-1452_Unaveraged_VMin2.txt");
  vector<double> voltage8;
  while(fin8>>content)
    {
      voltage8.push_back(content);
    }
  fin8.close();
  cout << voltage8.size() << endl;

  int numberD = voltage7.size();

  TH1D *h7 = new TH1D("h7","",100,newmin,newmax);
  TH1D *h8 = new TH1D("h8","",100,newmin,newmax);
  for(int i=0; i<numberD; i++)
    {
      // --- SiPM1
      content = -1*voltage7[i];
      h7->Fill(content);
      // --- SiPM2
      content = -1*voltage8[i];
      h8->Fill(content);
    }

  h7->SetLineColor(kRed);
  h7->SetLineWidth(2);
  h8->SetLineColor(kBlue);
  h8->SetLineWidth(2);
  // --- adjust limits
  double peconvert = 0.00502; // volts per photoelectrion
  h7->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  h8->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  // --- draw
  h7->Scale(1.0/numberD);
  h8->Scale(1.0/numberD);
  h7->Draw("same");
  h8->Draw("same");

  c1->SetLogy(0);
  c1->Print("Backgrounds/backgrounds_part3.png");
  c1->SetLogy(1);
  c1->Print("Backgrounds/backgrounds_part3_log.png");



  // h7->Scale(numberD);
  // h8->Scale(numberD);
  h7->Draw();
  h8->Draw("same");

  TF1 *fun = new TF1("fun","expo",0,150);
  h7->Fit(fun,"","",15,100);

  TF1 *fun2 = (TF1 *)fun->Clone("fun2");
  fun2->SetLineColor(kBlack);
  fun2->Draw("same");

  TF1 *funl = new TF1("funl","expo",0,150);
  h7->Fit(funl,"","",6,15);

  TF1 *funl2 = (TF1 *)funl->Clone("funl2");
  funl2->SetLineColor(kBlack);
  funl2->Draw("same");

  c1->SetLogy(0);
  c1->Print("Backgrounds/backgrounds_part4_fit.png");
  c1->SetLogy(1);
  c1->Print("Backgrounds/backgrounds_part4_fit_log.png");

  // ---

  TF1 *ffun = new TF1("ffun","[0]*TMath::Exp([1]*x)",0,150);
  ffun->SetParameter(0,TMath::Exp(fun->GetParameter(0)));
  ffun->SetParameter(1,fun->GetParameter(1));
  h7->Fit(ffun,"","",15,100);

  TF1 *ffun2 = (TF1 *)ffun->Clone("ffun2");
  ffun2->SetParameter(0,TMath::Exp(fun2->GetParameter(0)));
  ffun2->SetParameter(1,fun2->GetParameter(1));
  ffun2->SetLineColor(kBlack);
  ffun2->Draw("same");

  TF1 *ffunl = new TF1("ffunl","[0]*TMath::Exp([1]*x)",0,150);
  ffunl->SetParameter(0,TMath::Exp(funl->GetParameter(0)));
  ffunl->SetParameter(1,funl->GetParameter(1));
  h7->Fit(ffunl,"","",6,15);

  TF1 *ffunl2 = (TF1 *)ffunl->Clone("ffunl2");
  ffunl2->SetParameter(0,TMath::Exp(funl2->GetParameter(0)));
  ffunl2->SetParameter(1,funl2->GetParameter(1));
  ffunl2->SetLineColor(kBlack);
  ffunl2->Draw("same");

  c1->SetLogy(0);
  c1->Print("Backgrounds/backgrounds_part5_fit.png");
  c1->SetLogy(1);
  c1->Print("Backgrounds/backgrounds_part5_fit_log.png");



  TF1 *superfun = new TF1("superfun","[0]*TMath::Exp([1]*x) + [2]*TMath::Exp([3]*x)",0,150);
  superfun->SetParameter(0,0.8*ffunl->GetParameter(0));
  superfun->SetParameter(1,ffunl->GetParameter(1));
  superfun->SetParameter(2,ffun->GetParameter(0));
  superfun->SetParameter(3,ffun->GetParameter(1));
  superfun->Draw("same");

  c1->SetLogy(0);
  c1->Print("Backgrounds/backgrounds_part6_fit.png");
  c1->SetLogy(1);
  c1->Print("Backgrounds/backgrounds_part6_fit_log.png");



  TF1 *superfun2 = new TF1("superfun2","[0]*TMath::Exp([1]*x) + [2]*TMath::Exp([3]*x)",0,150);
  superfun2->SetParameter(0,0.8*ffunl->GetParameter(0));
  superfun2->SetParameter(1,ffunl->GetParameter(1));
  superfun2->SetParameter(2,ffun->GetParameter(0));
  superfun2->SetParameter(3,ffun->GetParameter(1));
  h7->Draw();
  h8->Draw("same");
  h7->Fit(superfun2,"","",6,100);

  c1->SetLogy(0);
  c1->Print("Backgrounds/backgrounds_part7_fit.png");
  c1->SetLogy(1);
  c1->Print("Backgrounds/backgrounds_part7_fit_log.png");



  TF1 *superfun3 = new TF1("superfun3","expo(0)+expo(2)",0,150);
  superfun3->SetParameter(0,0.8*funl->GetParameter(0));
  superfun3->SetParameter(1,funl->GetParameter(1));
  superfun3->SetParameter(2,fun->GetParameter(0));
  superfun3->SetParameter(3,fun->GetParameter(1));
  h7->Draw();
  h8->Draw("same");
  h7->Fit(superfun3,"","",6,100);

  c1->SetLogy(0);
  c1->Print("Backgrounds/backgrounds_part8_fit.png");
  c1->SetLogy(1);
  c1->Print("Backgrounds/backgrounds_part8_fit_log.png");



}
