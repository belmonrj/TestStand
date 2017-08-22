void CosmicsVertSim()
{

  /////////// Defining Variables to be Used Later////////////////
  TCanvas *c1 = new TCanvas(); 
  c1->SetLogy();

  const int Statistics = 200000;
  const double StepSize = 0.1;
  const double EnergyPerCM = 1;
  const double TriggerSep = 30;
  const double FingerWidth = 1;
  const double PannelWidth = .7; 
  const double PannelHight = 20; 
  const double PannelLeftEdge = (FingerWidth-PannelWidth)/2;
  const double PannelRightEdge = PannelLeftEdge + PannelWidth;
  const double PannelBotEdge = (TriggerSep-PannelHight)/2;
  const double PannelTopEdge = PannelBotEdge+PannelHight;
  vector<double> Energy;
 
  for (int i = 0; i < Statistics; i++)
    {

      /////////// Getting Random Start Possition for X //////////////
      double Xpos = gRandom->Uniform(0,FingerWidth);
      double Zpos = 0;
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
      
      /////////// Giving Cosmic Random Incident Angle in Acceptance Range ////////////

      //double Theta = gRandom->Gaus(0,ThetaMax/3.0);
      double Theta = gRandom->Uniform(ThetaLeft,ThetaRight);

      /////////// Making sure theta is in acceptance range ///////////////////////////

      //       if ( fabs(Theta) > ThetaMax )
      // 	{
      // 	  //cout << "theta out of bounds, rejecting " << endl;
      // 	  continue;
      // 	}

      double XStepSize = StepSize*(TMath::Sin(Theta));
      double ZStepSize = StepSize*(TMath::Cos(Theta));     
            
      /////////// Stepping Cosmic through and checking if still within pannel //////////
      
      while( Xpos<=FingerWidth && Zpos<=TriggerSep )
	{
	  Xpos = Xpos + XStepSize;
	  Zpos = Zpos + ZStepSize;


	  if (Xpos>=PannelLeftEdge && Xpos<=PannelRightEdge && Zpos>=PannelBotEdge && Zpos<=PannelTopEdge)
	    {
	      XDist = XDist + XStepSize;
	      ZDist = ZDist + ZStepSize;

	    } // end if

	} // end while
      
      ////////// Path length and energy deposited////////////

      double Dist = TMath::Sqrt(XDist*XDist + ZDist*ZDist);
      double EnergyDep = Dist*EnergyPerCM;
      Energy.push_back(EnergyDep);   
      
      
    } // end for

  double MaxEnergy = *max_element(Energy.begin(),Energy.end());
  double MinEnergy = *min_element(Energy.begin(),Energy.end());

  TH1D *histo = new TH1D("histo","Vertical Pannel Energy Spectrum",300,0,30);
  
  for (int i=0; i<=Statistics; i++)
    {
      double erg = Energy[i];
      histo->Fill(erg);
    
    } // end for
  histo->SetTitle("Expected Energies for a Vertical Cosmic Scan");
  histo->GetXaxis()->SetTitle("Energy Deposited (PE)");
  histo->GetYaxis()->SetTitle("Counts");
  histo->GetXaxis()->CenterTitle(true);
  histo->GetYaxis()->CenterTitle(true);
  histo->Draw();  
  
}
