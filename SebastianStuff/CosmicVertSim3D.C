void CosmicsVertSim3D()
{

  TCanvas *c1 = new TCanvas();
  c1->SetLogy();

  /////////// Defining Simulation Variables to be Used Later//////////////

  const int Statistics = 100000;
  const double StepSize = 0.02;
  const double MeVperCM = 1.82; // MeV/cm
  const double PEperMeV = 31.6; // PE/MeV
  const double TriggerSep = 30;
  const double TriggerLength = 5;
  const double TriggerWidth = 1;

  /////////// Defining Bottom Trigger Area //////////////////////////////
  double LowerTrigCorner[][];
  LowerTrigCorner[0][0]=TriggerLength;
  LowerTrigCorner[0][1]=TriggerWidth;
  LowerTrigCorner[1][0]=TriggerLenght;
  LowerTrigCorner[1][1]=0.0;
  LowerTrigCorner[2][0]=0.0;
  LowerTrigCorner[2][1]=0.0;
  LowerTrigCorner[3][0]=0.0;
  LowerTrigCorner[3][1]=TriggerWidth;

  /////////// Defining Upper Trigger Area ////////////////////////////////
  double UpperTrigCorner[][];
  UpperTrigCorner[0][0]=TriggerLength;
  UpperTrigCorner[0][1]=TriggerLength;
  UpperTrigCorner[1][0]=TriggerLength;
  UpperTrigCorner[1][1]=0.0;
  UpperTrigCorner[2][0]=TriggerLength-TriggerWidth;
  UpperTrigCorner[2][1]=0.0;
  UpperTrigCorner[3][0]=TriggerLength-TriggerWidth;
  UpperTrigCorner[3][1]=TriggerWidth;

  ////////////// Begin Simmulation Loop ///////////////////////////////////

  for (int i = 0; i < Statistics;i++)
    {
      /////////// Getting Random Start Possition in Top Pannel ////////////////
      double XPos = gRandom->Uniform(UpperTrigCorner[2][0],UpperTrigCorner[1][0]);
      double YPos = gRandom->Uniform(UpperTrigCorner[1][1],UpperTrigCorner[0][1]);
      double Ray[][];
      double Slope[];
      Ray[0][0] = XPos - LowerTrigCorner[0][0];
      Ray[0][1] = YPos - LowerTrigCorner[0][1];
      Ray[3][0] = XPos - LowerTrigCorner[3][0];
      Ray[3][1] = YPos - LowerTrigCorner[3][1];
      Ray[1][0] = XPos - LowerTrigCorner[1][0];
      Ray[1][1] = YPos - LowerTrigCorner[1][1];
      Ray[2][0] = XPos - LowerTrigCorner[2][0];
      Ray[2][1] = YPos - LowerTrigCorner[2][1];

      for(int i = 0; i<4; i++)
	{
	  Slope[i]=(Ray[i][1])/(Ray[i][0])
	}


    } //End sim for loop

}










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
