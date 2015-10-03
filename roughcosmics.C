#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
//#include <cmath> // for sqrt
//#include <numeric> // for accumulate
#include <algorithm> // for min_element, max_element


#include "TROOT.h"
#include "TH1.h"
#include "TCanvas.h"


using namespace std;


//void roughcosmics()
int main()
{

  // read in the data and create a vector with all the values
  ifstream fin1("TEMP/20150930-1720_Unaveraged_VMin1.txt");
  double voltage;
  vector<double> voltage_full;
  while(fin1>>voltage)
    {
      voltage_full.push_back(voltage);
    }
  fin1.close();

  cout << voltage_full.size() << endl;


  int number_full = voltage_full.size();
  double max = *max_element(voltage_full.begin(),voltage_full.end());
  double min = *min_element(voltage_full.begin(),voltage_full.end());
  cout << max << endl;
  cout << min << endl;

  double newmax = min*-0.95;
  double newmin = max*-1.05 - newmax*0.1;

  TH1D *h1 = new TH1D("h1","",50,newmin,newmax);

  vector<double> voltage_rmdups;
  for(int i=0; i<number_full; i++)
    {
      voltage = -1*voltage_full[i];
      if(i<number_full-1)
	{
	  if(voltage_full[i] != voltage_full[i+1])
	    {
	      voltage_rmdups.push_back(voltage_full[i]);
	      h1->Fill(voltage);
	    }
	}
      else
	{
	  voltage_rmdups.push_back(voltage_full[i]);
	  h1->Fill(voltage);
	}
    }

  cout << voltage_rmdups.size() << endl;



  TCanvas *c1 = new TCanvas("c1","",800,600);
  h1->Draw();
  c1->Print("uglydata.png");
  c1->SetLogy();
  c1->Print("uglydatalog.png");
  c1->SetLogy(0);
  h1->SetMaximum(120);
  c1->Print("uglydatanotlog.png");


}
