void LED_Efficiency(const int PhotonNumber = 1000, const int NumPixels = 9000)
{
  
  TCanvas *c1 = new TCanvas();
  double FracFired[PhotonNumber];
  int Fired[NumPixels];
  TGraph *Eff = new TGraph();    
 
  for (int i = 0;i<PhotonNumber;i++)
    {
      int Stat = 100*(i+1);
      for (int k = 0;k<NumPixels;k++)
 	{
 	  Fired[k]=0;
 	} ///////////// Clearing Fired /////////////      
      
      int NumbFired = 0; 
      for( int j = 0; j < Stat; j++)
	{
	  int PixelFired = gRandom->Uniform(0,NumPixels);
  
	  if(Fired[PixelFired] == 0)
	    {
	      Fired[PixelFired] = 1;
	      NumbFired++;
	    } ///////////// End Chincking to see if pixel already fired ////////////
	} ///////////// End firing pixels ////////////////////
      FracFired[i]=(double)NumbFired/(double)Stat;
      
      Eff->SetPoint(i,(double)Stat/((double)NumPixels),FracFired[i]);
    } /////////////////// End Looping over number of photons fired /////////////////

  Eff->Draw(); 


}
