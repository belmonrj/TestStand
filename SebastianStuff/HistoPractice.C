const double NPE = 0.005/14;

void HistoPractice()
{

  TCanvas *c1 = new TCanvas();

  ifstream fin1;
  ifstream fin2;
  fin1.open("20160129-1800_Unaveraged_VMin1.txt");
  fin2.open("20160129-1800_Unaveraged_VMin2.txt");

  double volts1;
  vector<double>vec_volts1;
  double volts2;
  vector<double>vec_volts2;

  while(fin1>>volts1)
    {
      vec_volts1.push_back(-volts1);

    }
  while(fin2>>volts2)
    {
      vec_volts2.push_back(-volts2);

    }
  int entries1 = vec_volts1.size();
  int entries2 = vec_volts2.size();
  if (entries1!=entries2)
    {
      cout<<"Lists of Different Length"<<endl;
    }

  double max = *max_element(vec_volts1.begin(),vec_volts1.end());
  double min = *min_element(vec_volts1.begin(),vec_volts1.end());

  TH1D* histo = new TH1D("histo","Cosmic Scan Energies",200,(min-0.1)/NPE,(max+0.1)/NPE);
  TH1D* histo2 = new TH1D("histo2","Cosmic Scan Energies",200,(min-0.1)/NPE,(max+0.1)/NPE);
  TNtuple* nt = new TNtuple("nt","nt","Q1:Q2");


  for(int i=0;i<entries1;i++)
    {

      double npe = (vec_volts1[i])/NPE;//+vec_volts2[i])/NPE;
      double npe1 = (vec_volts1[i])/NPE;
      double npe2 = (vec_volts2[i])/NPE;
      histo->Fill(npe);
      nt->Fill(npe1,npe2);

    }
  for(int i=0;i<entries1;i++)
    {

      double npe = (vec_volts2[i])/NPE;//+vec_volts2[i])/NPE;
      histo2->Fill(npe);

    }

  TFile* CosmicVertData = new TFile("CosmicVertData.root","RECREATE");
  nt->Write("nt");
  CosmicVertData->Close();

  histo->GetXaxis()->SetTitle("Energy (PE)");
  histo->GetYaxis()->SetTitle("Counts");
  histo->GetXaxis()->CenterTitle(true);
  histo->GetYaxis()->CenterTitle(true);
  histo->SetLineColor(kBlue);
  histo2->SetLineColor(kRed);

  histo->Draw();
  histo2->Draw("same");
  c1->Print("linear.png");
  c1->SetLogy();
  c1->Print("Log.png");


}
