#include <algorithm> // for min_element, max_element



void simplecosmics()
{

  // --- read in the data and create a vector with all the values
  // --- note that this code requires an duplicates to have already been cleaned out
  // --- this is automatically fixed with the new version of the DAQ/stepper code
  // --- but the user would do well do double check all output files anyway
  ifstream fin1("TEMP/20150930-1720_Unaveraged_VMin1.txt");
  double content;
  vector<double> voltage;
  while(fin1>>content)
    {
      voltage.push_back(content);
    }
  fin1.close();
  cout << voltage.size() << endl;

  // --- get the number of entries and the min and max
  int number = voltage.size();
  double max = *max_element(voltage.begin(),voltage.end());
  double min = *min_element(voltage.begin(),voltage.end());
  cout << max << endl;
  cout << min << endl;
  // --- use the min and max to calculate a range for the histogram
  double newmax = min*-0.95;
  double newmin = max*-1.05 - newmax*0.1;
  // --- create the new histogram
  TH1D *h1 = new TH1D("h1","",50,newmin,newmax);
  // --- loop over the vector to fill the histogram
  for(int i=0; i<number; i++)
    {
      content = -1*voltage[i];
      h1->Fill(content);
    }

  // --- make a canvas and draw the histogram
  TCanvas *c1 = new TCanvas("c1","",800,600);
  h1->Draw();
  // c1->Print("uglydata.png");
  c1->SetLogy();
  // c1->Print("uglydatalog.png");
  c1->SetLogy(0);
  // h1->SetMaximum(120);
  // c1->Print("uglydatanotlog.png");

  // --- rescale the histograms from volts to photoelectrons
  double peconvert = 0.00502; // volts per photoelectrion
  h1->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  h1->Draw();
  // c1->Print("uglydata.png");
  c1->SetLogy();
  // c1->Print("uglydatalog.png");
  c1->SetLogy(0);
  h1->SetMaximum(120);
  // c1->Print("uglydatanotlog.png");

  // --- define Landau function and draw
  // --- don't fit yet because the data have tons of ugly low voltage background
  double height = 600;
  double mu = 23;
  double sigma = 3;
  TF1 *fun = new TF1("fun","[0]*TMath::Landau(x,[1],[2])",newmin/peconvert,newmax/peconvert);
  fun->SetParameter(0,height);
  fun->SetParameter(1,mu);
  fun->SetParameter(2,sigma);
  fun->Draw("same");
  c1->Print("uglydatanotlogfit.png");
  c1->SetLogy(1);
  c1->Print("uglydatalogfit.png");




}
