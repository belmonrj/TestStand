#include <algorithm> // for min_element, max_element

void source()
{

  double height = 1.0;
  double mu = 0.1;
  double sigma = 0.05;
  double min = mu - sigma*5;
  double max = mu + sigma*25;


  ifstream fin1("TEMP/20150921-1152_Unaveraged_VMin1.txt");
  double content;
  vector<double> voltage1;
  while(fin1>>content)
    {
      voltage1.push_back(content);
    }
  fin1.close();
  cout << voltage1.size() << endl;

  // --- do the same for SiPM2
  ifstream fin2("TEMP/20150921-1152_Unaveraged_VMin2.txt");
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
  const int nbins = 50;
  TH1D *h1 = new TH1D("h1","",nbins,newmin,newmax);
  TH1D *h2 = new TH1D("h2","",nbins,newmin,newmax);
  TH1D *hsum = new TH1D("hsum","",nbins,2*newmin,2*newmax);
  TH2D *hh1v2 = new TH2D("hh1v2","",2*nbins,newmin,newmax,2*nbins,newmin,newmax); // SiPM1 vs SiPM2
  TH2D *hhSvA = new TH2D("hhSvA","",2*nbins,2*newmin,2*newmax,2*nbins,-1,1); // Sum vs Asymmetry
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
      hsum->Fill(tempsum);
      hhSvA->Fill(tempsum,tempasym);
      sum.push_back(tempsum);
      asym.push_back(tempasym);
    }


  h2->SetLineColor(kRed);
  h1->SetLineColor(kBlue);
  h2->SetLineWidth(2);
  h1->SetLineWidth(2);
  double max2 = h2->GetMaximum();
  double max1 = h1->GetMaximum();

  TLegend *leg = new TLegend(0.68,0.73,0.88,0.88);
  leg->AddEntry(h1,"SiPM1","l");
  leg->AddEntry(h2,"SiPM2","l");


  // --- now do PE conversion
  double peconvert = 0.00502;
  h1->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  h2->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  h1->GetXaxis()->SetTitle("Number of photoelectrons");
  h2->GetXaxis()->SetTitle("Number of photoelectrons");
  if(max2>max1)
    {
      h2->Draw();
      h1->Draw("same");
    }
  else
    {
      h1->Draw();
      h2->Draw("same");
    }
  leg->Draw();
  c1->Print("Source/source_pe.pdf");
  c1->Print("Source/source_pe.png");




  c1->SetLogy(0);
  c1->Clear();
  hh1v2->Draw("colz");
  hh1v2->GetXaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  hh1v2->GetYaxis()->SetLimits(newmin/peconvert,newmax/peconvert);
  hh1v2->GetXaxis()->SetTitle("#pe SiPM1");
  hh1v2->GetYaxis()->SetTitle("#pe SiPM2");
  c1->SetLogz(0);
  c1->Print("Source/source_1v2.png");
  c1->Print("Source/source_1v2.pdf");
  c1->SetLogz(1);
  c1->Print("Source/source_1v2_log.png");
  c1->Print("Source/source_1v2_log.pdf");

  hhSvA->Draw("colz");
  hhSvA->GetXaxis()->SetLimits(2*newmin/peconvert,2*newmax/peconvert);
  hhSvA->GetXaxis()->SetTitle("Sum");
  hhSvA->GetYaxis()->SetTitle("Asymmetry");
  c1->SetLogz(0);
  c1->Print("Source/source_SvA.png");
  c1->Print("Source/source_SvA.pdf");
  c1->SetLogz(1);
  c1->Print("Source/source_SvA_log.png");
  c1->Print("Source/source_SvA_log.pdf");



  hsum->GetXaxis()->SetLimits(2*newmin/peconvert,2*newmax/peconvert);
  hsum->GetXaxis()->SetTitle("Number of photoelectrons SiPM1+SiPM2");
  hsum->Draw();
  c1->Print(Form("Source/source_sum.png"));
  c1->Print(Form("Source/source_sum.pdf"));
  c1->SetLogy();
  c1->Print(Form("Source/source_sumlog.png"));
  c1->Print(Form("Source/source_sumlog.pdf"));


}
