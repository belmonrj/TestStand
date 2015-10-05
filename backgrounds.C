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

  TH1D *h3 = new TH1D("h3","",50,newmin,newmax);
  TH1D *h4 = new TH1D("h4","",50,newmin,newmax);
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
  h3->Draw("");
  h4->Draw("same");
  // --- print figures
  c1->SetLogy(0);
  c1->Print("backgrounds_part1.png");
  c1->SetLogy(1);
  c1->Print("backgrounds_part1_log.png");



}
