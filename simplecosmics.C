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
  // --- loop over the vector to fill the histogram
  for(int i=0; i<number; i++)
    {
      // --- SiPM1
      content = -1*voltage1[i];
      h1->Fill(content);
      // --- SiPM2
      content = -1*voltage2[i];
      h2->Fill(content);
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
  fun->SetLineColor(kBlack);
  c1->Print("Cosmics/uglydatanotlogfit.png");
  c1->SetLogy(1);
  c1->Print("Cosmics/uglydatalogfit.png");

  // --- now draw SiPM2 on top
  h1->SetLineColor(kRed);
  h2->SetLineColor(kBlue);
  h2->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  h2->Draw("same");
  c1->SetLogy(0);
  c1->Print("Cosmics/uglydatabothnotlogfit.png");
  c1->SetLogy(1);
  c1->Print("Cosmics/uglydatabothlogfit.png");

  h1->SetMaximum(1.15*h1->GetBinContent(h1->GetMaximumBin()));

  c1->SetLogy(0);
  c1->Print("Cosmics/fuglydatabothnotlogfit.png");
  c1->SetLogy(1);
  c1->Print("Cosmics/fuglydatabothlogfit.png");

  double bgscale = 650; // guess...
  TF1 *bun = new TF1("bun","[0]*TMath::Exp([1]*x) + [2]*TMath::Exp([3]*x)",newmin/peconvert,newmax/peconvert);
  bun->SetParameter(0,bgscale*7.17020);
  bun->SetParameter(1,-5.10223e-1);
  bun->SetParameter(2,bgscale*1.22363e-1);
  bun->SetParameter(3,-8.84094e-2);
  bun->SetLineColor(kBlack);
  bun->Draw("same");

  c1->SetLogy(0);
  c1->Print("Cosmics/buglydatabothnotlogfit.png");
  c1->SetLogy(1);
  c1->Print("Cosmics/buglydatabothlogfit.png");





}
