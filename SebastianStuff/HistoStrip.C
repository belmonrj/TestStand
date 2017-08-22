void HistoStrip()
{
  TCanvas* Front = new TCanvas();  
  TCanvas* Mid = new TCanvas();  
  TCanvas* Back = new TCanvas();  
  TLegend* legFront = new TLegend(0.8,0.7,1,1);
  TLegend* legMid = new TLegend(0.8,0.7,1,1);
  TLegend* legBack = new TLegend(0.8,0.7,1,1);

  Front->SetTicks();
  Mid->SetTicks();
  Back->SetTicks();

  TGraph* GraphFront1 = new TGraph();
  TGraph* GraphMid1 = new TGraph();
  TGraph* GraphBack1 = new TGraph();

  
  TGraph* GraphFront2 = new TGraph();
  TGraph* GraphMid2 = new TGraph();
  TGraph* GraphBack2 = new TGraph();

  
  TGraph* GraphFront3 = new TGraph();
  TGraph* GraphMid3 = new TGraph();
  TGraph* GraphBack3 = new TGraph();

  
  TGraph* GraphFront4 = new TGraph();
  TGraph* GraphMid4 = new TGraph();
  TGraph* GraphBack4 = new TGraph();

  
  TGraph* GraphFront5 = new TGraph();
  TGraph* GraphMid5 = new TGraph();
  TGraph* GraphBack5 = new TGraph();

  
  TGraph* GraphFront6 = new TGraph();
  TGraph* GraphMid6 = new TGraph();
  TGraph* GraphBack6 = new TGraph();

  
  TGraph* GraphFront7 = new TGraph();
  TGraph* GraphMid7 = new TGraph();
  TGraph* GraphBack7 = new TGraph();


  ifstream datafile1;
  ifstream datafile2;
  ifstream datafile3;
  ifstream datafile4;
  ifstream datafile5;
  ifstream datafile6;
  ifstream datafile7;
  datafile1.open("Data_Txt_Files/20160512-1656_VMIN_SIPM2_OH146.txt");
  datafile2.open("Data_Txt_Files/20160513-1025_VMIN_SIPM2_OH147.txt");
  datafile3.open("Data_Txt_Files/20160509-1029_VMIN_SIPM2_OH12.txt");
  datafile4.open("Data_Txt_Files/20160510-1635_VMIN_SIPM2_OH13.txt");
  datafile5.open("Data_Txt_Files/20160429-1354_VMIN_SIPM2_OH246.txt");
  datafile6.open("Data_Txt_Files/20160511-1541_VMIN_SIPM2_OH247.txt");
  datafile7.open("Data_Txt_Files/20160427-1312_VMIN_SIPM2_OH26.txt");


  vector<double> vec_vmin1;
  vector<double> vec_vmin2;
  vector<double> vec_vmin3;
  vector<double> vec_vmin4;
  vector<double> vec_vmin5;
  vector<double> vec_vmin6;
  vector<double> vec_vmin7;
  vector<double> y_pos;
  double Vmin = 0.0;

  while(datafile1>>Vmin)
    {
      vec_vmin1.push_back(-Vmin*1000);
    } 
  while(datafile2>>Vmin)
    {
      vec_vmin2.push_back(-Vmin*1000);
    } 
  while(datafile3>>Vmin)
    {
      vec_vmin3.push_back(-Vmin*1000);
    } 
  while(datafile4>>Vmin)
    {
      vec_vmin4.push_back(-Vmin*1000);
    } 
  while(datafile5>>Vmin)
    {
      vec_vmin5.push_back(-Vmin*1000);
    } 
  while(datafile6>>Vmin)
    {
      vec_vmin6.push_back(-Vmin*1000);
    } 
  while(datafile7>>Vmin)
    {
      vec_vmin7.push_back(-Vmin*1000);
    } 
  
  int front = 24;
  int mid = 87;
  int back = 150;
  int row = 54;
  double SumEnd1=0;
  double Sum51=0;
  double SumEnd2=0;
  double Sum52=0;
  double SumEnd3=0;
  double Sum53=0;
  double SumEnd4=0;
  double Sum54=0;
  double SumEnd5=0;
  double Sum55=0;
  double SumEnd6=0;
  double Sum56=0;
  double SumEnd7=0;
  double Sum57=0;

  //  int possition = (collumn-1)*row;
  
  for (int i = 0; i < row; i++)
    {
      y_pos.push_back(i/2.0);
    } 

  for (int i = 0; i < row; i++)
    {
      GraphFront1->SetPoint(i,y_pos[i],vec_vmin1[i+(front-1)*row]);
      GraphMid1->SetPoint(i,y_pos[i],vec_vmin1[i+(mid-1)*row]);
      GraphBack1->SetPoint(i,y_pos[i],vec_vmin1[i+(back-1)*row]);

      GraphFront2->SetPoint(i,y_pos[i],vec_vmin2[i+(front-1)*row]);
      GraphMid2->SetPoint(i,y_pos[i],vec_vmin2[i+(mid-1)*row]);
      GraphBack2->SetPoint(i,y_pos[i],vec_vmin2[i+(back-1)*row]);

      GraphFront3->SetPoint(i,y_pos[i],vec_vmin3[i+(front-1)*row]);
      GraphMid3->SetPoint(i,y_pos[i],vec_vmin3[i+(mid-1)*row]);
      GraphBack3->SetPoint(i,y_pos[i],vec_vmin3[i+(back-1)*row]);

      GraphFront4->SetPoint(i,y_pos[i],vec_vmin4[i+(front-1)*row]);
      GraphMid4->SetPoint(i,y_pos[i],vec_vmin4[i+(mid-1)*row]);
      GraphBack4->SetPoint(i,y_pos[i],vec_vmin4[i+(back-1)*row]);

      GraphFront5->SetPoint(i,y_pos[i],vec_vmin5[i+(front-1)*row]);
      GraphMid5->SetPoint(i,y_pos[i],vec_vmin5[i+(mid-1)*row]);
      GraphBack5->SetPoint(i,y_pos[i],vec_vmin5[i+(back-1)*row]);

      GraphFront6->SetPoint(i,y_pos[i],vec_vmin6[i+(front-1)*row]);
      GraphMid6->SetPoint(i,y_pos[i],vec_vmin6[i+(mid-1)*row]);
      GraphBack6->SetPoint(i,y_pos[i],vec_vmin6[i+(back-1)*row]);

      GraphFront7->SetPoint(i,y_pos[i],vec_vmin7[i+(front-1)*row]);
      GraphMid7->SetPoint(i,y_pos[i],vec_vmin7[i+(mid-1)*row]);
      GraphBack7->SetPoint(i,y_pos[i],vec_vmin7[i+(back-1)*row]);

      SumEnd1 += vec_vmin1[i+(back+8)*row];
      Sum51 += vec_vmin1[i+(back-1)*row];
      SumEnd2 += vec_vmin2[i+(back+8)*row];
      Sum52 += vec_vmin2[i+(back-1)*row];
      SumEnd3 += vec_vmin3[i+(back+8)*row];
      Sum53 += vec_vmin3[i+(back-1)*row];
      SumEnd4 += vec_vmin4[i+(back+8)*row];
      Sum54 += vec_vmin4[i+(back-1)*row];
      SumEnd5 += vec_vmin5[i+(back+8)*row];
      Sum55 += vec_vmin5[i+(back-1)*row];
      SumEnd6 += vec_vmin6[i+(back+8)*row];
      Sum56 += vec_vmin6[i+(back-1)*row];
      SumEnd7 += vec_vmin7[i+(back+8)*row];
      Sum57 += vec_vmin7[i+(back-1)*row];
    } 


  Front->cd();

  GraphFront1->SetTitle("Strip 5cm from SiPM");
  GraphFront1->GetXaxis()->SetTitle("Scan Position (cm)");
  GraphFront1->GetYaxis()->SetTitle("SiPM Response (mV)");
  GraphFront1->GetXaxis()->CenterTitle(true);
  GraphFront1->GetYaxis()->CenterTitle(true);
  //line color
  GraphFront2->SetLineColor(kBlue);
  GraphFront3->SetLineColor(kRed);
  GraphFront4->SetLineColor(kGreen);
  GraphFront5->SetLineColor(kBlue);
  GraphFront6->SetLineColor(kRed);
  //line style
  GraphFront5->SetLineStyle(7);
  GraphFront6->SetLineStyle(7);
  GraphFront7->SetLineStyle(7);
  //drawing
  GraphFront1->Draw("al");
  GraphFront2->Draw("same");
  GraphFront3->Draw("same");
  GraphFront4->Draw("same");
  GraphFront5->Draw("same");
  GraphFront6->Draw("same");
  GraphFront7->Draw("same");
  //legend
  legFront->AddEntry(GraphFront3,"OH-1-2","l");
  legFront->AddEntry(GraphFront4,"OH-1-3","l");
  legFront->AddEntry(GraphFront1,"OH-1-46","l");
  legFront->AddEntry(GraphFront2,"OH-1-47","l");
  legFront->AddEntry(GraphFront7,"OH-2-6","l");
  legFront->AddEntry(GraphFront5,"OH-2-46","l");
  legFront->AddEntry(GraphFront6,"OH-2-47","l");
  legFront->Draw();

  Mid->cd();
  
  GraphMid1->SetTitle("Strip in Tile Center");
  GraphMid1->GetXaxis()->SetTitle("Scan Position (cm)");
  GraphMid1->GetYaxis()->SetTitle("SiPM Response (mV)");
  GraphMid1->GetXaxis()->CenterTitle(true);
  GraphMid1->GetYaxis()->CenterTitle(true);
  //line color
  GraphMid2->SetLineColor(kBlue);
  GraphMid3->SetLineColor(kRed);
  GraphMid4->SetLineColor(kGreen);
  GraphMid5->SetLineColor(kBlue);
  GraphMid6->SetLineColor(kRed);
  //line style
  GraphMid5->SetLineStyle(7);
  GraphMid6->SetLineStyle(7);
  GraphMid7->SetLineStyle(7);
  //drawing
  GraphMid1->Draw("al");
  GraphMid2->Draw("same");
  GraphMid3->Draw("same");
  GraphMid4->Draw("same");
  GraphMid5->Draw("same");
  GraphMid6->Draw("same");
  GraphMid7->Draw("same");
  //legend
  legMid->AddEntry(GraphMid3,"OH-1-2","l");
  legMid->AddEntry(GraphMid4,"OH-1-3","l");
  legMid->AddEntry(GraphMid1,"OH-1-46","l");
  legMid->AddEntry(GraphMid2,"OH-1-47","l");
  legMid->AddEntry(GraphMid7,"OH-2-6","l");
  legMid->AddEntry(GraphMid5,"OH-2-46","l");
  legMid->AddEntry(GraphMid6,"OH-2-47","l");
  legMid->Draw();


  cout << "Pannel end: OH 1 46: " << SumEnd1/row << endl;
  cout << "Pannel end: OH 1 47: " << SumEnd2/row << endl;
  cout << "Pannel end: OH 1 2: " << SumEnd3/row << endl;
  cout << "Pannel end: OH 1 3: " << SumEnd4/row << endl;
  cout << "Pannel end: OH 2 46: " << SumEnd5/row << endl;
  cout << "Pannel end: OH 2 47: " << SumEnd6/row << endl;
  cout << "Pannel end: OH 2 6: " << SumEnd7/row << endl;
  cout << endl;
  cout << "Pannel 5cm: OH 1 46: " << Sum51/row << endl;
  cout << "Pannel 5cm: OH 1 47: " << Sum52/row << endl;
  cout << "Pannel 5cm: OH 1 2: " << Sum53/row << endl;
  cout << "Pannel 5cm: OH 1 3: " << Sum54/row << endl;
  cout << "Pannel 5cm: OH 2 46: " << Sum55/row << endl;
  cout << "Pannel 5cm: OH 2 47: " << Sum56/row << endl;
  cout << "Pannel 5cm: OH 2 6: " << Sum57/row << endl;



  Back->cd();

  GraphBack1->SetTitle("Strip 5cm from Tile End");
  GraphBack1->GetXaxis()->SetTitle("Scan Position (cm)");
  GraphBack1->GetYaxis()->SetTitle("SiPM Response (mV)");
  GraphBack1->GetXaxis()->CenterTitle(true);
  GraphBack1->GetYaxis()->CenterTitle(true);
  //line color
  GraphBack2->SetLineColor(kBlue);
  GraphBack3->SetLineColor(kRed);
  GraphBack4->SetLineColor(kGreen);
  GraphBack5->SetLineColor(kBlue);
  GraphBack6->SetLineColor(kRed);
  //line style
  GraphBack5->SetLineStyle(7);
  GraphBack6->SetLineStyle(7);
  GraphBack7->SetLineStyle(7);
  //drawing
  GraphBack1->Draw("al");
  GraphBack2->Draw("same");
  GraphBack3->Draw("same");
  GraphBack4->Draw("same");
  GraphBack5->Draw("same");
  GraphBack6->Draw("same");
  GraphBack7->Draw("same");
  //legend
  legBack->AddEntry(GraphBack3,"OH-1-2","l");
  legBack->AddEntry(GraphBack4,"OH-1-3","l");
  legBack->AddEntry(GraphBack1,"OH-1-46","l");
  legBack->AddEntry(GraphBack2,"OH-1-47","l");
  legBack->AddEntry(GraphBack7,"OH-2-6","l");
  legBack->AddEntry(GraphBack5,"OH-2-46","l");
  legBack->AddEntry(GraphBack6,"OH-2-47","l");
  legBack->Draw();


}
