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

  TH1D *h3 = new TH1D("h3","",100,newmin,newmax); // SiPM1 distribution
  TH1D *h4 = new TH1D("h4","",100,newmin,newmax); // SiPM2 distribution
  for(int i=0; i<numberB; i++)
    {
      // --- SiPM1
      h3->Fill(-voltage3[i]);
      // --- SiPM2
      h4->Fill(-voltage4[i]);
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
      h5->Fill(-voltage5[i]);
      // --- SiPM2
      h6->Fill(-voltage6[i]);
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
  TH2D *hh1v2 = new TH2D("hh1v2","",100,newmin,newmax,100,newmin,newmax); // SiPM1 vs SiPM2
  TH2D *hhSvA = new TH2D("hhSvA","",100,2*newmin,2*newmax,100,-1,1); // Sum vs Asymmetry
  vector<double> sum;
  vector<double> asym;
  for(int i=0; i<numberD; i++)
    {
      // --- SiPM1
      h7->Fill(-voltage7[i]);
      // --- SiPM2
      h8->Fill(-voltage8[i]);
      // --- SiPM1 vs SiPM2
      hh1v2->Fill(-voltage7[i],-voltage8[i]);
      // --- sum vs asymmetry
      double tempsum = -voltage7[i] + -voltage8[i];
      double tempasym = (voltage7[i] - voltage8[i]) / (-voltage7[i] + -voltage8[i]);
      hhSvA->Fill(tempsum,tempasym);
      sum.push_back(tempsum);
      asym.push_back(tempasym);
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

  h7->GetYaxis()->SetTitle("Counts");
  h7->GetXaxis()->SetTitle("Number of photoelectrons");



  TF1 *superfun3 = new TF1("superfun3","expo(0)+expo(2)",0,150);
  superfun3->SetParameter(0,0.8*funl->GetParameter(0));
  superfun3->SetParameter(1,funl->GetParameter(1));
  superfun3->SetParameter(2,fun->GetParameter(0));
  superfun3->SetParameter(3,fun->GetParameter(1));
  superfun3->SetLineColor(kBlack);
  h7->SetLineWidth(1);
  h8->SetLineWidth(1);
  h7->Draw();
  h8->Draw("same");
  h7->Fit(superfun3,"","",6,100);
  funl->SetParameter(0,superfun3->GetParameter(0));
  funl->SetParameter(1,superfun3->GetParameter(1));
  fun->SetParameter(0,superfun3->GetParameter(2));
  fun->SetParameter(1,superfun3->GetParameter(3));
  funl->SetLineColor(kGreen+2);
  fun->SetLineColor(kGreen+2);
  funl->SetLineWidth(1);
  fun->SetLineWidth(1);
  funl->Draw("same");
  fun->Draw("same");
  TLegend *leg = new TLegend(0.48,0.68,0.88,0.88);
  leg->AddEntry(h7,"SiPM1","l");
  leg->AddEntry(h8,"SiPM2","l");
  leg->AddEntry(funl,"low exponential","l");
  leg->AddEntry(fun,"high exponential","l");
  leg->AddEntry(superfun3,"Combined bg (double expo)","l");
  leg->Draw();
  c1->SetLogy(0);
  c1->Print("Backgrounds/backgrounds_part8_fit.png");
  c1->Print("Backgrounds/backgrounds_part8_fit.pdf");
  c1->SetLogy(1);
  c1->Print("Backgrounds/backgrounds_part8_fit_log.png");
  c1->Print("Backgrounds/backgrounds_part8_fit_log.pdf");


  h7->Scale(numberD);
  h8->Scale(numberD);
  h7->Fit(superfun3,"","",6,100);
  funl->SetParameter(0,superfun3->GetParameter(0));
  funl->SetParameter(1,superfun3->GetParameter(1));
  fun->SetParameter(0,superfun3->GetParameter(2));
  fun->SetParameter(1,superfun3->GetParameter(3));
  superfun3->Draw("same");
  funl->Draw("same");
  fun->Draw("same");
  c1->SetLogy(0);
  c1->Print("Backgrounds/backgrounds_part9_fit.png");
  c1->Print("Backgrounds/backgrounds_part9_fit.pdf");
  c1->SetLogy(1);
  c1->Print("Backgrounds/backgrounds_part9_fit_log.png");
  c1->Print("Backgrounds/backgrounds_part9_fit_log.pdf");




  c1->SetLogy(0);
  c1->Clear();
  hh1v2->Draw("colz");
  hh1v2->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  hh1v2->GetYaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  hh1v2->GetXaxis()->SetTitle("#pe SiPM1");
  hh1v2->GetYaxis()->SetTitle("#pe SiPM2");
  c1->SetLogz(0);
  c1->Print("Backgrounds/backgrounds_1v2.png");
  c1->Print("Backgrounds/backgrounds_1v2.pdf");
  c1->SetLogz(1);
  c1->Print("Backgrounds/backgrounds_1v2_log.png");
  c1->Print("Backgrounds/backgrounds_1v2_log.pdf");

  hhSvA->Draw("colz");
  hhSvA->GetXaxis()->SetLimits(2*newmin/peconvert,2*newmax/peconvert);
  hhSvA->GetXaxis()->SetTitle("Sum");
  hhSvA->GetYaxis()->SetTitle("Asymmetry");
  c1->SetLogz(0);
  c1->Print("Backgrounds/backgrounds_SvA.png");
  c1->Print("Backgrounds/backgrounds_SvA.pdf");
  c1->SetLogz(1);
  c1->Print("Backgrounds/backgrounds_SvA_log.png");
  c1->Print("Backgrounds/backgrounds_SvA_log.pdf");

}
