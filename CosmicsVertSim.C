// --- Primary author: Sebastian Vazquez-Torres
// --- Secondary euthor: Ron Belmont

void CosmicsVertSim()
{

  /////////// Defining Variables to be Used Later////////////////
  TCanvas *c1 = new TCanvas();
  c1->SetLogy();

  const int Statistics = 200000;
  const int nsteps = 100;
  const double StepSize = 0.1;
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
  TF1 *gumbel = new TF1("gumbel","([0]/sqrt(6.28))*TMath::Exp(-0.5*((x-[1])/[2] + TMath::Exp(-(x-[1])/[2])))",0,200);
  gumbel->SetParameter(0,1.0);
  gumbel->SetParameter(1,mpv);
  gumbel->SetParameter(2,ksi);



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
	}



      //double Theta = gRandom->Uniform(ThetaLeft,ThetaRight);
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
      double fromgumbel = gumbel->GetRandom();
      double shift = fromgumbel-mpv;
      shift *= PEperMeV; // yes
      //shift *= Dist; // maybe??? // no...
      SmearedEnergy.push_back(EnergyDep+shift);

    } // end for

  cout << "out of bounds counter " << counter_OOB << endl;
  cout << "fraction of throws out of bounds = " << (counter_OOB/(float)Statistics)*100 << "%" << endl;

  double MaxEnergy = *max_element(Energy.begin(),Energy.end());
  double MinEnergy = *min_element(Energy.begin(),Energy.end());

  // --- weird periodicity, reason not yet understood...
  // TH1D *histo_Energy = new TH1D("histo_Energy","Vertical Pannel Energy Spectrum",300,0,1500); // weird periodicity...
  // for (int i=0; i<=Statistics; i++) histo_Energy->Fill(Energy[i]);
  // histo_Energy->SetTitle("Expected Energies for a Vertical Cosmic Scan");
  // histo_Energy->GetXaxis()->SetTitle("Average dep. energy, no smearing (PE)");
  // histo_Energy->GetYaxis()->SetTitle("Counts");
  // // histo_Energy->GetXaxis()->CenterTitle(true);
  // // histo_Energy->GetYaxis()->CenterTitle(true);
  // histo_Energy->Draw();
  // c1->Print("plot_Energy.png");

  double distance_low = 0;
  double distance_high = 30;
  TH1D *histo = new TH1D("histo","Vertical Pannel Distance Spectrum",300,distance_low,distance_high);
  for (int i=0; i<=Statistics; i++) histo->Fill(Distance[i]);
  histo->SetTitle("Expected Distance for a Vertical Cosmic Scan");
  histo->GetXaxis()->SetTitle("Distance (cm)");
  histo->GetYaxis()->SetTitle("Counts");
  // histo->GetXaxis()->CenterTitle(true);
  // histo->GetYaxis()->CenterTitle(true);
  histo->Draw();
  c1->Print("plot_Distance.png");
  histo->GetXaxis()->SetLimits(distance_low*MeVperCM*PEperMeV,distance_high*MeVperCM*PEperMeV);
  histo->GetXaxis()->SetTitle("Energy, no smearing (cm)");
  c1->Print("plot_Energy.png");

  TH1D *histoSmear = new TH1D("histoSmear","Vertical Pannel Distance Spectrum",300,distance_low*MeVperCM*PEperMeV,distance_high*MeVperCM*PEperMeV);
  for (int i=0; i<=Statistics; i++) histoSmear->Fill(SmearedEnergy[i]);
  histoSmear->SetTitle("Expected Distance for a Vertical Cosmic Scan");
  histoSmear->GetXaxis()->SetTitle("Energy with Landau smearing (PE)");
  histoSmear->GetYaxis()->SetTitle("Counts");
  // histoSmear->GetXaxis()->CenterTitle(true);
  // histoSmear->GetYaxis()->CenterTitle(true);
  histoSmear->Draw();
  c1->Print("plot_SmearedEnergy.png");

}
