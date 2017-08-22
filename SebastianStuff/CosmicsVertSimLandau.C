// --- Primary author: Sebastian Vazquez-Torres
// --- Secondary author: Ron Belmont

void CosmicsVertSimLandau(bool SimAndData = true)
{

  TCanvas *c1 = new TCanvas();
  c1->SetLogy();

  /////////// Defining Simulation Variables to be Used Later////////////////

  const int Statistics = 100000;
  const double StepSize = 0.02;
  const double MeVperCM = 1.82; // MeV/cm
  const double PEperMeV = 31.6; // PE/MeV
  const double TriggerSep = 30;
  const double FingerWidth = 1;
  const double TriggerLeft = 0.0;
  const double TriggerRight = TriggerLeft+FingerWidth;
  const double TriggerBottom = 0.0;
  const double TriggerTop = TriggerBottom+TriggerSep;
  const double PannelWidth = .7;
  const double PannelHight = 20;
  const double PannelLeftEdge = (FingerWidth-PannelWidth)/2.0;
  const double PannelRightEdge = PannelLeftEdge + PannelWidth;
  const double PannelBotEdge = (TriggerSep-PannelHight)/2.0;
  const double PannelTopEdge = PannelBotEdge+PannelHight;

  vector<double> Distance;
  vector<double> Energy;
  vector<double> SmearedEnergy;

  int counter_OOB = 0;

  TF1 *funangle = new TF1("funangle","[0]*(cos([1]*x))*(cos([1]*x))",-1,1);
  funangle->SetParameter(0,1.0);
  funangle->SetParameter(1,1.0);

  const double mpv = MeVperCM;
  const double ksi = 0.385;
  TF1 *fgumbel = new TF1("fgumbel","([0]/sqrt(6.28))*TMath::Exp(-0.5*((x-[1])/[2] + TMath::Exp(-(x-[1])/[2])))",0,200);
  fgumbel->SetParameter(0,1.0);
  fgumbel->SetParameter(1,mpv);
  fgumbel->SetParameter(2,ksi);
  TF1 *flandau = new TF1("flandau","landau",0,200);
  flandau->SetParameter(0,1.0);
  flandau->SetParameter(1,mpv);
  flandau->SetParameter(2,ksi);

  /////////// Defining Data Variables to be Used Later////////////////
 
 const double NPE = 0.005/14;
  
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
  
  TH1D* HistoData = new TH1D("HistoData","Cosmic Scan Energies",200,(min-0.1)/NPE,(max+0.1)/NPE);
  
  for(int i=0;i<entries1;i++)
    {
      
      double npe = (vec_volts1[i]+vec_volts2[i])/NPE;
      HistoData->Fill(npe);
      
    }
  
  ////////////// End of Real Data Analysis ///////////////////////
  ////////////// Begin Simulation Loops //////////////////////////
  
  for (int i = 0; i < Statistics; i++)
    {
      
      /////////// Getting Random Start Possition for X //////////////
      double Xpos = gRandom->Uniform(TriggerLeft,TriggerRight);
      double Zpos = TriggerBottom;
      double XDist = 0;
      double ZDist = 0;

      /////////// Calculating Max Acceptance Angle //////////////////
      double ThetaRight = TMath::ATan((FingerWidth-Xpos)/TriggerSep);
      double ThetaLeft = -1*TMath::ATan(Xpos/TriggerSep);
      double ThetaMax = 0;

      if (ThetaRight<ThetaLeft)
	{
	  cout << "angle error" << endl;
	} // end if

      if(ThetaLeft>0)
	{
	  cout << "Why is theta left positive???" << endl;
	} // end if

      double Theta = funangle->GetRandom(ThetaLeft,ThetaRight);
      double XStepSize = StepSize*(TMath::Sin(Theta));
      double ZStepSize = StepSize*(TMath::Cos(Theta));
      int counter = 0;
      while ( Xpos >= TriggerLeft && Xpos <= TriggerRight && Zpos >= TriggerBottom && Zpos <= TriggerTop )
	{
	  counter++;
	  Xpos = Xpos + XStepSize;
	  Zpos = Zpos + ZStepSize;
	  if ( Xpos >= PannelLeftEdge && Xpos <= PannelRightEdge && Zpos >= PannelBotEdge && Zpos <= PannelTopEdge )
	    {
	      XDist = XDist + XStepSize;
	      ZDist = ZDist + ZStepSize;
	    } // end if
	} // end while

      if ( counter > 0 && ( Xpos <= TriggerLeft || Xpos >= TriggerRight ) )
	{
	  counter_OOB++;
	  XDist = 0;
	  ZDist = 0;
	}


      ////////// Path length and energy deposited////////////

      double Dist = TMath::Sqrt(XDist*XDist + ZDist*ZDist);
      Distance.push_back(Dist);
      double EnergyDep = Dist*MeVperCM*PEperMeV;
      Energy.push_back(EnergyDep);
      //double shifted = fgumbel->GetRandom();
      double shifted = flandau->GetRandom();
      shifted -= mpv;
      shifted *= MeVperCM*PEperMeV;
      shifted += EnergyDep;
      if ( EnergyDep == 0 ) shifted = 0;
      SmearedEnergy.push_back(shifted);

    } // end for

  cout << "out of bounds counter " << counter_OOB << endl;
  cout << "fraction of throws out of bounds = " << (counter_OOB/(float)Statistics)*100 << "%" << endl;

  double MaxEnergy = *max_element(Energy.begin(),Energy.end());
  double MinEnergy = *min_element(Energy.begin(),Energy.end());
  double distance_low = 0;
  double distance_high = 30;
  TH1D *histo = new TH1D("histo","Vertical Pannel Distance Spectrum",300,distance_low,distance_high);

  for (int i=0; i<=Statistics; i++) histo->Fill(Distance[i]);

  histo->SetTitle("Expected Distance for a Vertical Cosmic Scan");
  histo->GetXaxis()->SetTitle("Distance (cm)");
  histo->GetYaxis()->SetTitle("Counts");
  histo->GetXaxis()->CenterTitle(true);
  histo->GetYaxis()->CenterTitle(true);
  histo->SetLineColor(kRed);
  HistoData->SetLineColor(kBlue);
  histo->Scale(1.0/(histo->GetMaximum()));
  HistoData->Scale(1.0/(HistoData->GetMaximum()));
  histo->Draw();
  HistoData->Draw("same");
  c1->Print("plot_Distance.png");
  histo->GetXaxis()->SetLimits(distance_low*MeVperCM*PEperMeV,distance_high*MeVperCM*PEperMeV);
  histo->GetXaxis()->SetTitle("Energy, no smearing (cm)");
  c1->Print("plot_Energy.png");

  TH1D *histoSmear = new TH1D("histoSmear","Vertical Pannel Distance Spectrum",300,distance_low*MeVperCM*PEperMeV,distance_high*MeVperCM*PEperMeV+600);

  for (int i=0; i<=Statistics; i++) histoSmear->Fill(SmearedEnergy[i]);
  histoSmear->SetTitle("Simulated Energies for a Vertical Cosmic Scan");
  histoSmear->GetXaxis()->SetTitle("Energy with Landau smearing (PE)");
  histoSmear->GetYaxis()->SetTitle("Histograms Arbitrarily Scaled");
  histoSmear->GetXaxis()->CenterTitle(true);
  histoSmear->GetYaxis()->CenterTitle(true);
  histoSmear->SetLineColor(kRed);
  HistoData->SetLineColor(kBlue);
  //histoSmear->Scale(1.0/(histoSmear->GetBinContent(histoSmear->FindBin(1150))));
  histoSmear->Scale(1.0/(histoSmear->Integral()));
  HistoData->Scale(0.5/(HistoData->Integral()));
  histoSmear->Draw();
  // HistoData->Draw("same");
  c1->Print("plot_SmearedEnergy.png");

  // leg = new TLegend(0.1,0.7,0.3,0.9);
  // leg->AddEntry(histoSmear,"Simulated","l");
  // leg->AddEntry(HistoData,"Experimental","l");
  // leg->Draw("same"); 



}

