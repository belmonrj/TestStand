#include <algorithm> // for min_element, max_element

void pedestal()
{

  donewpart("20151030-1532",50);

}


void donewpart(const char *basename, int nbins)
{

  double content;

  ifstream fin7(Form("TEMP/%s_Unaveraged_VMin2.txt",basename));
  vector<double> voltage7;
  while(fin7>>content)
    {
      voltage7.push_back(content);
    }
  fin7.close();
  cout << voltage7.size() << endl;

  int numberD = voltage7.size();
  cout << numberD << endl;

  // --- get the max, min of the vector
  double max = *max_element(voltage7.begin(),voltage7.end());
  double min = *min_element(voltage7.begin(),voltage7.end());
  cout << max << endl;
  cout << min << endl;
  // --- calculate the limits for the histograms
  double newmax = min*-0.95;
  double newmin = max*-1.05 - newmax*0.1;

  TH1D *h7 = new TH1D("h7","",nbins,newmin,newmax);
  for(int i=0; i<numberD; i++)
    {
      // --- SiPM1
      h7->Fill(-voltage7[i]);
    }

  h7->SetLineColor(kRed);
  h7->SetLineWidth(2);
  // --- adjust limits
  double peconvert = 0.00502; // volts per photoelectrion
  h7->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  // --- draw
  // h7->Scale(1.0/numberD);
  h7->Draw();

  c1->SetLogy(0);
  c1->Print(Form("Pedestals/%s_pedestals_part3.png",basename));
  c1->Print(Form("Pedestals/%s_pedestals_part3.pdf",basename));
  c1->SetLogy(1);
  c1->Print(Form("Pedestals/%s_pedestals_part3_log.png",basename));
  c1->Print(Form("Pedestals/%s_pedestals_part3_log.pdf",basename));



  delete h7;

}

