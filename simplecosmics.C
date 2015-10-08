#include <algorithm> // for min_element, max_element



void simplecosmics()
{

  // --- read in the data and create a vector with all the values
  // --- note that this code requires an duplicates to have already been cleaned out
  // --- this is automatically fixed with the new version of the DAQ/stepper code
  // --- but the user would do well do double check all output files anyway
  ifstream fin1("TEMP/20150930-1720_Unaveraged_VMin1.txt");
  double content;
  vector<double> voltage1;
  while(fin1>>content)
    {
      voltage1.push_back(content);
    }
  fin1.close();
  cout << voltage1.size() << endl;

  // --- do the same for SiPM2
  ifstream fin2("TEMP/20150930-1720_Unaveraged_VMin2.txt");
  vector<double> voltage2;
  while(fin2>>content)
    {
      voltage2.push_back(content);
    }
  fin2.close();
  cout << voltage2.size() << endl;

  // --- get the number of entries and the min and max
  int number = voltage1.size();
  double max = *max_element(voltage1.begin(),voltage1.end());
  double min = *min_element(voltage1.begin(),voltage1.end());
  cout << max << endl;
  cout << min << endl;
  // --- use the min and max to calculate a range for the histogram
  double newmax = min*-0.95;
  double newmin = max*-1.05 - newmax*0.1;
  // --- create the new histogram
  TH1D *h1 = new TH1D("h1","",50,newmin,newmax);
  TH1D *h2 = new TH1D("h2","",50,newmin,newmax);
  TH2D *hh1v2 = new TH2D("hh1v2","",100,newmin,newmax,100,newmin,newmax); // SiPM1 vs SiPM2
  TH2D *hhSvA = new TH2D("hhSvA","",100,2*newmin,2*newmax,100,-1,1); // Sum vs Asymmetry
  vector<double> sum;
  vector<double> asym;
  // --- loop over the vector to fill the histogram
  for(int i=0; i<number; i++)
    {
      // --- SiPM1
      h1->Fill(-voltage1[i]);
      // --- SiPM2
      h2->Fill(-voltage2[i]);
      // --- SiPM1 vs SiPM2
      hh1v2->Fill(-voltage1[i],-voltage2[i]);
      // --- sum vs asymmetry
      double tempsum = -voltage1[i] + -voltage2[i];
      double tempasym = (voltage1[i] - voltage2[i]) / (-voltage1[i] + -voltage2[i]);
      hhSvA->Fill(tempsum,tempasym);
      sum.push_back(tempsum);
      asym.push_back(tempasym);
    }

  // --- make a canvas and draw the histogram
  TCanvas *c1 = new TCanvas("c1","",800,600);
  h1->Draw();
  // c1->Print("Cosmics/uglydata.png");
  c1->SetLogy();
  // c1->Print("Cosmics/uglydatalog.png");
  c1->SetLogy(0);
  // h1->SetMaximum(120);
  // c1->Print("Cosmics/uglydatanotlog.png");

  // --- rescale the histograms from volts to photoelectrons
  double peconvert = 0.00502; // volts per photoelectrion
  h1->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  h1->GetXaxis()->SetTitle("Number of photoelectrons");
  h1->GetYaxis()->SetTitle("Counts");
  h1->Draw();
  // c1->Print("Cosmics/uglydata.png");
  c1->SetLogy();
  // c1->Print("Cosmics/uglydatalog.png");
  c1->SetLogy(0);
  h1->SetMaximum(210);
  // c1->Print("Cosmics/uglydatanotlog.png");

  // --- define Landau function and draw
  // --- don't fit yet because the data have tons of ugly low voltage1 background
  double height = 1049;
  double mu = 23;
  double sigma = 3;
  TF1 *fun = new TF1("fun","[0]*TMath::Landau(x,[1],[2])",newmin/peconvert,newmax/peconvert);
  fun->SetParameter(0,height);
  fun->SetParameter(1,mu);
  fun->SetParameter(2,sigma);
  fun->Draw("same");
  fun->SetLineColor(kGreen+2);
  fun->SetLineWidth(1);
  c1->Print("Cosmics/uglydatanotlogfit.png");
  c1->Print("Cosmics/uglydatanotlogfit.pdf");
  c1->SetLogy(1);
  c1->Print("Cosmics/uglydatalogfit.png");
  c1->Print("Cosmics/uglydatalogfit.pdf");

  // --- now draw SiPM2 on top
  h1->SetLineColor(kRed);
  h2->SetLineColor(kBlue);
  h2->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  h2->Draw("same");
  c1->SetLogy(0);
  c1->Print("Cosmics/uglydatabothnotlogfit.png");
  c1->Print("Cosmics/uglydatabothnotlogfit.pdf");
  c1->SetLogy(1);
  c1->Print("Cosmics/uglydatabothlogfit.png");
  c1->Print("Cosmics/uglydatabothlogfit.pdf");

  h1->SetMaximum(1.15*h1->GetBinContent(h1->GetMaximumBin()));

  c1->SetLogy(0);
  c1->Print("Cosmics/fuglydatabothnotlogfit.png");
  c1->Print("Cosmics/fuglydatabothnotlogfit.pdf");
  c1->SetLogy(1);
  c1->Print("Cosmics/fuglydatabothlogfit.png");
  c1->Print("Cosmics/fuglydatabothlogfit.pdf");

  double bgscale = 650; // guess...
  TF1 *bun = new TF1("bun","[0]*TMath::Exp([1]*x) + [2]*TMath::Exp([3]*x)",newmin/peconvert,newmax/peconvert);
  bun->SetParameter(0,bgscale*7.17020);
  bun->SetParameter(1,-5.10223e-1);
  bun->SetParameter(2,bgscale*1.22363e-1);
  bun->SetParameter(3,-8.84094e-2);
  bun->SetLineColor(kGreen+2);
  bun->SetLineWidth(1);
  bun->Draw("same");

  c1->SetLogy(0);
  c1->Print("Cosmics/buglydatabothnotlogfit.png");
  c1->Print("Cosmics/buglydatabothnotlogfit.pdf");
  c1->SetLogy(1);
  c1->Print("Cosmics/buglydatabothlogfit.png");
  c1->Print("Cosmics/buglydatabothlogfit.pdf");



  TF1 *ultrafun = new TF1("ultrafun","[0]*TMath::Exp([1]*x) + [2]*TMath::Exp([3]*x) + [4]*TMath::Landau(x,[5],[6])",newmin/peconvert,newmax/peconvert);
  ultrafun->SetParameter(0,bgscale*7.17020);
  ultrafun->SetParameter(1,-5.10223e-1);
  ultrafun->SetParameter(2,bgscale*1.22363e-1);
  ultrafun->SetParameter(3,-8.84094e-2);
  ultrafun->SetParameter(4,height);
  ultrafun->SetParameter(5,mu);
  ultrafun->SetParameter(6,sigma);
  ultrafun->SetLineColor(kBlack);
  ultrafun->Draw("same");

  TLegend *leg = new TLegend(0.48,0.68,0.88,0.88);
  leg->AddEntry(h1,"SiPM1","l");
  leg->AddEntry(h2,"SiPM2","l");
  leg->AddEntry(fun,"Signal (Landau)","l");
  leg->AddEntry(bun,"Background (double expo)","l");
  leg->AddEntry(ultrafun,"Combined functional fit","l");
  leg->Draw();

  c1->SetLogy(0);
  c1->Print("Cosmics/bhuglydatabothnotlogfit.png");
  c1->Print("Cosmics/bhuglydatabothnotlogfit.pdf");
  c1->SetLogy(1);
  c1->Print("Cosmics/bhuglydatabothlogfit.png");
  c1->Print("Cosmics/bhuglydatabothlogfit.pdf");

  h1->Fit(ultrafun,"R");
  bun->SetParameter(0,ultrafun->GetParameter(0));
  bun->SetParameter(1,ultrafun->GetParameter(1));
  bun->SetParameter(2,ultrafun->GetParameter(2));
  bun->SetParameter(3,ultrafun->GetParameter(3));
  fun->SetParameter(0,ultrafun->GetParameter(4));
  fun->SetParameter(1,ultrafun->GetParameter(5));
  fun->SetParameter(2,ultrafun->GetParameter(6));
  c1->SetLogy(0);
  c1->Print("Cosmics/bhfuglydatabothnotlogfit.png");
  c1->Print("Cosmics/bhfuglydatabothnotlogfit.pdf");
  c1->SetLogy(1);
  c1->Print("Cosmics/bhfuglydatabothlogfit.png");
  c1->Print("Cosmics/bhfuglydatabothlogfit.pdf");

  h1->SetMaximum(210);
  c1->SetLogy(0);
  c1->Print("Cosmics/pfuglydatabothnotlogfit.png");
  c1->Print("Cosmics/pfuglydatabothnotlogfit.pdf");
  c1->SetLogy(1);
  c1->Print("Cosmics/pfuglydatabothlogfit.png");
  c1->Print("Cosmics/pfuglydatabothlogfit.pdf");



  // ---

  h1->SetMaximum(1.15*h1->GetBinContent(h1->GetMaximumBin()));
  h1->Draw();
  h2->Draw("same");

  TF1 *pain = new TF1("pain","[0]*7.17020*TMath::Exp([1]*x) + [0]*1.22363*TMath::Exp([2]*x) + [3]*TMath::Landau(x,[4],[5])",newmin/peconvert,newmax/peconvert);
  pain->SetParameter(0,bgscale);
  pain->FixParameter(1,-5.10223e-1);
  pain->FixParameter(2,-8.84094e-2);
  pain->SetParameter(3,height);
  pain->SetParameter(4,mu);
  pain->SetParameter(5,sigma);
  pain->SetLineColor(kBlack);
  pain->Draw("same");

  h1->Fit(pain,"R");
  c1->SetLogy(0);
  c1->Print("Cosmics/iampain.png");
  c1->Print("Cosmics/iampain.pdf");
  c1->SetLogy(1);
  c1->Print("Cosmics/iampain_log.png");
  c1->Print("Cosmics/iampain_log.pdf");




  c1->SetLogy(0);
  c1->Clear();
  hh1v2->Draw("colz");
  hh1v2->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  hh1v2->GetYaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  hh1v2->GetXaxis()->SetTitle("#pe SiPM1");
  hh1v2->GetYaxis()->SetTitle("#pe SiPM2");
  c1->SetLogz(0);
  c1->Print("Cosmics/cosmics_1v2.png");
  c1->Print("Cosmics/cosmics_1v2.pdf");
  c1->SetLogz(1);
  c1->Print("Cosmics/cosmics_1v2.png");
  c1->Print("Cosmics/cosmics_1v2.pdf");

  hhSvA->Draw("colz");
  hhSvA->GetXaxis()->SetLimits(2*newmin/peconvert,2*newmax/peconvert);
  hhSvA->GetXaxis()->SetTitle("Sum");
  hhSvA->GetYaxis()->SetTitle("Asymmetry");
  c1->SetLogz(0);
  c1->Print("Cosmics/cosmics_SvA.png");
  c1->Print("Cosmics/cosmics_SvA.pdf");
  c1->SetLogz(1);
  c1->Print("Cosmics/cosmics_SvA.png");
  c1->Print("Cosmics/cosmics_SvA.pdf");


}
