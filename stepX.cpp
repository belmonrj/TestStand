//////////////////////////////////////////////////////////////////////////
// Stepper Code for Column Scans
//
// Author:		Sebastian Seeds
// Date:		2.24.15
// Use:			For use with Zaber stepper motors for transport and
//				positioning of a radioactive source above static
//				scintillating tiles. Includes sample ZABER code for manual
//				repositioning. Made for the SPHENIX HCal upgrade for use
//				with QA testing.
//
//////////////////////////////////////////////////////////////////////////

// Sample Code Documentation
/*------------------------------------------------------------------------
  Module:        MAIN.C
  Author:        ZABER
  Project:
  State:
  Creation Date: 27 June 2001
  Description:   Example program for communicating with
  Teckmo devices on a serial port
  Language : C
  Platform : Win32
  Serial   : Polled Mode Operation
  ------------------------------------------------------------------------*/
//FOR BASIC OPERATION
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include "pserial.c"  // Polled Serial API

//FOR SCOPE CODE
#include <sicl.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

//OPTION INCLUDES
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TVirtualStreamerInfo.h>
#include <TPluginManager.h>

//FOR STEPPER
static unsigned char Unit;
static unsigned char Command;
static long Data;

//FOR WHY?
using std::cout;
using std::endl;

//------------------------------------------------------------------------
//FUNCTIONS FOR SCOPE CODE

//Read in 16 bit data from the return buffer.
//The short array named buffer is 2 bytes wide, but iread reads
//one byte at a time.  Therefore, the pointer into buffer is explicitly
//casted as a character type in the iread function.

INST oscillo;

unsigned long ReadWord (short *buffer, unsigned long BytesToRead)
{
  unsigned long BytesRead;
  int reason;

  iread(oscillo, (char *) buffer, BytesToRead, &reason, &BytesRead);

  return BytesRead;

}

unsigned long ReadByte (char *buffer, unsigned long BytesToRead)
{
  unsigned long BytesRead;
  int reason;

  iread(oscillo,buffer,BytesToRead,&reason,&BytesRead);

  return BytesRead;
}

void WriteIO(char *buffer)
{
  unsigned long actualcnt;
  unsigned long BytesToWrite;
  int send_end=1;
  char temp[50];

  BytesToWrite = strlen(buffer)+1;
  strcpy_s(temp, buffer);
  strcat_s(temp, "\n");
  iwrite(oscillo, temp, BytesToWrite, send_end, &actualcnt);

}
void ReadIO(char *buffer)
{
  unsigned long actualcnt;
  unsigned long BytesToWrite;
  int send_end;
  char temp[50];

  BytesToWrite = strlen(buffer)+1;
  strcpy_s(temp, buffer);
  strcat_s(temp, "\n");
  iread(oscillo, temp, BytesToWrite, &send_end, &actualcnt);

}

void ReadDouble(double *buffer)
{
  iscanf(oscillo, "%lf", buffer);
}

void ReadChannel(char* source, double magnitude[], unsigned long &BytesToRead, int &success)
{
  /// printf(source);
  /// printf("\n");
  double yref = 0;
  double yorg = 0;
  double yinc = 0;
  char cData;
  //unsigned long BytesToRead;
  unsigned long BytesRead;
  char header_str[8];
  char Term;
  short waveform[70000];
  int iloop = 0;

  WriteIO(":WAV:BYTeorder LSBFIRST");
  WriteIO(source);
  WriteIO(":WAVeform:FORMat WORD");

  //Query scaling information for translating to real voltages and times
  WriteIO(":WAV:YREFerence?");
  ReadDouble(&yref);
  WriteIO(":WAV:YORigin?");
  ReadDouble(&yorg);
  WriteIO(":WAV:YINCrement?");
  ReadDouble(&yinc);
  //Request waveform data
  WriteIO(":WAV:DATA?");

  //Look for the # character
  do{
    ReadByte( &cData,1L );
    iloop++;

    if(iloop > 1)
      {
	/// printf("Failed to find data, trying again...\n");
	success = -1;
	return;
      }



  } while (cData != '#');

  //Read in the character following the # character
  ReadByte(&cData, 1L);//The 1L is a 1 casted to a long int
  BytesToRead = cData - '0';//Convert the character into a long int
  //printf("Reading %d bytes\n", BytesToRead);
  BytesRead=ReadByte(header_str, BytesToRead);
  header_str[BytesRead] = '\0';//Add null end of string character
  BytesToRead = atoi(header_str);//Extract number of bytes to read
  if(BytesToRead > 10000){
    cout<<BytesToRead<<" Too many points from scope"<<endl;
    return;
  }

  //Read in waveform data
  ReadWord(waveform, BytesToRead);
  ReadByte(&Term, 1L);

  for(int ii = 1; ii <= (signed) BytesToRead/2; ii++)
    {
      //Calculate voltages
      magnitude[ii-1] = (((waveform[ii-1])-yref)*yinc)+yorg;

    }
  /// printf("Read channel command succesful!\n");

  success = 1;
}


// END SCOPE FUNCTIONS ---------------------------------------------------

// BEGIN EXAMPLE CODE ----------------------------------------------------
//Comment
void gotoxy(int x, int y)
{
  static HANDLE h = NULL;
  if(!h)
    h = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD c = { x,y };
  SetConsoleCursorPosition(h,c);
}

void clrscr()
{
  system("cls");
}

// This is a helper command to redraw the screen
void Redraw (void)
{
  gotoxy(1,1);
  printf( "Unit#      Command#      Data" );
  gotoxy(1,21);
  printf( "To enter an instruction press space bar; to quit press 'q'.\n");
  printf( "                                             \n");
  printf( "                                             \n");
  printf( "                                             \n");
}

// This function reads in keyboard input and calls SendCommand
void Instruction(void)
{
  unsigned char Unit;
  unsigned char Command;
  long Data;
  char s[70];

  gotoxy(1,22);

  printf("Unit#: ");
  gets(s);							// Read in unit number
  Unit = atol(s);

  printf("Command#: ");
  gets(s);							// Read in command number
  Command = atol(s);

  printf("Data(bits): ");
  gets(s);
  Data = atol(s);				// Read in data

  Redraw();
  PSERIAL_Send(Unit, Command, Data);
}


/*------------------------------------------------------------------------
// This is the concept behind polled mode operation
// (A simplified pseudo-code version of the main function)

PSERIAL_Initialize();          // First, initialize the API
PSERIAL_Open();                // Open a serial port
InitializeChain();		     // Initializes the chain of teckmo devices

while ( NoQuit )               // Kernel loop
{
if ( PSERIAL_Receive() )
{
ProcessReceived();
}
// Read input from the keyboard or other sources
if ( kbhit() )
{
ProcessCommand();
}
}
PSERIAL_Close();

------------------------------------------------------------------------*/
// This is the main function.
int xmainx(void)
{
  char PortName[100];
  int NoQuit = 1;

  // Need to call this function before using the serial port API
  PSERIAL_Initialize();

  printf( "Make sure all Teckmo units are connected to the serial port\n" );
  printf( "and that they have power.\n");
  printf( "Then select your port [com1|com2|com3|com4]: " );
  gets( PortName ); // type in the name of the com port.

  // Attempt to open the serial port
  if (!PSERIAL_Open( PortName ))
    {
      // Unable to open port
      // - The port name could be incorrect (must be "com1", "com2", etc...)
      // - The port could be currently used
      printf("Cannot open port.\n");
      printf("Please check that you typed in the correct name,\n");
      printf("and check that the requested port is not currently being used.\n");
      printf("press any key to exit...");
      while (!kbhit() )
	{
	  // Wait until keystroke
	}
      return 0;
    }

  // Now, Initailize the Teckmo chain by renumbering
  PSERIAL_Send( 0,2,0 ); // All units renumber
  printf("Initializing chain...\n");

  // This is the initial delay after renumbering
  // to allow the units to return their device numbers
  Sleep(1500);

  clrscr(); // Clear the screen before refreshing with up to date information
  Redraw();
  PSERIAL_Send( 0,10,0 ); // All units to return to their current positions

  // this is the kernel loop
  while ( NoQuit )
    {
      // Try to receive data
      if ( PSERIAL_Receive(&Unit, &Command, &Data) )
	{
	  // for this demo program, screen only can accomodate 20 lines, or 20 units
	  if (Unit >= 1 && Unit <= 20)
	    {
	      gotoxy(1, Unit+1); // print from line 2 to line 21
	      printf("%i          %i             %i                  ", Unit, Command, Data);
	    }
	}
      // Scan keyboard
      if ( kbhit() )
	{
	  switch(getch())
	    {
	    case ' ':
	      Instruction();
	      // PSERIAL_Send() is called in the Instruction() subroutine
	      break;
	    case 'q':
	      printf("quitting\n");
	      NoQuit = 0;
	      break;
	    default:
	      // do nothing
	      break;
	    }
	}
    }

  // Finished with the serial port.
  PSERIAL_Close();

  // Exit program
  return 0;
}


// END SAMPLE CODE --------------------------------------------------------

// BEGIN SCAN CODE --------------------------------------------------------

// All commands follow the structure "PSERIAL_Send( X,Y,Z )" where X is the
// target drive (for several daisy-chained), Y is the Zaber command (20 is
// move absolute, for instance), and Z is data (microsteps to move for
// command 20, for instance). All daisy-chained drives must be numbered
// for coherent commands. This is accomplished with zaber command 2 (already
// present in code). Wait times are essential to prevent data collision.
// Data value "64" denotes that the value is irrelevant for the command.

int main(void)

{

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  char timestamp[100];
  char daypart[100];
  char timepart[100];

  if(tm.tm_hour>9 && tm.tm_min>9) sprintf(timepart,"%d%d", tm.tm_hour, tm.tm_min);
  else if(tm.tm_hour>9 && tm.tm_min<=9) sprintf(timepart,"%d0%d", tm.tm_hour, tm.tm_min);
  else if(tm.tm_hour<=9 && tm.tm_min>9) sprintf(timepart,"0%d%d", tm.tm_hour, tm.tm_min);
  else if(tm.tm_hour<=9 && tm.tm_min<=9) sprintf(timepart,"0%d0%d", tm.tm_hour, tm.tm_min);

  if(tm.tm_mon>8 && tm.tm_mday>9) sprintf(daypart,"%d%d%d",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
  else if(tm.tm_mon>8 && tm.tm_mday<=9) sprintf(daypart,"%d%d0%d",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
  else if(tm.tm_mon<=8 && tm.tm_mday>9) sprintf(daypart,"%d0%d%d",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
  else if(tm.tm_mon<=8 && tm.tm_mday<=9) sprintf(daypart,"%d0%d0%d",tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

  sprintf(timestamp,"%s-%s",daypart,timepart);


  // Creating and opening a text file for pulse averaging method
  ofstream file_1;
  ofstream file_2;
  ofstream file_3;
  ofstream file_4;
  ofstream file_5;

  char filename1[100];
  char filename2[100];
  char filename3[100];
  char filename4[100];
  char filename5[100];

  sprintf(filename1,"%s_TIME.txt",timestamp);
  sprintf(filename2,"%s_AREA_SIPM1.txt",timestamp);
  sprintf(filename3,"%s_AREA_SIPM2.txt",timestamp);
  sprintf(filename4,"%s_VMIN_SIPM1.txt",timestamp);
  sprintf(filename5,"%s_VMIN_SIPM2.txt",timestamp);

  file_1.open(filename1);
  file_2.open(filename2);
  file_3.open(filename3);
  file_4.open(filename4);
  file_5.open(filename5);

  // Reports total number of available microsteps for each drive.
  double xmicrosteptot = 8062992;
  double ymicrosteptot = 4031496;
  double stepspercm = xmicrosteptot/100;
  // Sets origin for scan./////////////////////////////////////////////////

  //double xorigincm = 9.0;//signal bias fiber scan 9.0
  //double yorigincm = 43.0;
  //double yorigincm = 33.0; // fiber #4, furthest back
  //double yorigincm = 35.5; // fiber #3, second furthest back
  //double yorigincm = 38.0; // fiber #2, second closest
  double yorigincm = 43.0; // fiber #1, closest

  double xorigincm = 9.0; // for radioactive source
  //double xorigincm = 15.0; // for radioactive source
  //double xorigincm = 9.0 + 1.905; // for LED in source housing

  double xorigin = stepspercm*xorigincm;
  double yorigin = stepspercm*yorigincm;

  // Adjust for step length across both x and y by cm...///////////////////
  double xsteplengthcm = 0.5;
  double ysteplengthcm = 0.5;

  double xgeneralstep = stepspercm*xsteplengthcm;
  double ygeneralstep = stepspercm*ysteplengthcm;

  // Variable for recording current positions of drives. Used to calibrate
  // wait times. Takes stage 860ms to go 8062992 steps.
  double xcurrentpos = 0.0;
  double ycurrentpos = 0.0;
  double sleepzero = 0.0;
  double sleeptime = 0.0;
  double sleeptime2 = 0.0;

  // Adjust for number of steps across x and y ////////////////////////////

  int xsteps = 58;
  int ysteps = 9;
  //int xsteps = 58;
  //int ysteps = 3;
  /////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////


  // Checks to see if parameters are within physics ranges of the stages
  if (xsteps*xgeneralstep+xorigin > xmicrosteptot || ysteps*ygeneralstep+yorigin > ymicrosteptot)
    {
      cout << "Settings take the source beyond the end of the drive. Please readjust." << endl;
      return 0;
    }

  // Variables for PSERIAL_Receive command.
  unsigned char Unit;
  unsigned char Unit2;
  unsigned char Command;
  unsigned char Command2;
  long Data;
  long Data2;

  // Warns if step settings distort scan grid.
  if ( xgeneralstep > ygeneralstep + 1000 || xgeneralstep < ygeneralstep - 1000)
    {
      cout << "Warning: Check Aspect Ratio." << endl;
      cout << "Each step in X is " << xgeneralstep << "." << endl;
      cout << "Each step in Y is " << ygeneralstep << "." << endl;
      cout << "It might be desired to adjust xaxis and yaxis parameters to produce similar values" << endl;
      Sleep(10000);
    }

  // Initializes the stepper motors to interpret instructions.
  PSERIAL_Initialize();

  // Rezero drives ---------------------------------------------------------
  PSERIAL_Open("com3");
  PSERIAL_Send( 0,2,0);
  Sleep(1500);
  //PSERIAL_Send( 1,1,64 );
  //PSERIAL_Send( 2,1,64 );
  // Waiting for drives by current position
  // Returns current position
  PSERIAL_Send( 1,60,64 );
  while (1)
    {
      if (PSERIAL_Receive (&Unit,&Command,&Data) && Unit == 1 && Command == 60)
	{
	  xcurrentpos = Data;
	  break;
	}
    }
  Sleep(1500);
  PSERIAL_Send( 2,60,64 );
  while (1)
    {
      if (PSERIAL_Receive (&Unit,&Command,&Data2) && Unit == 2 && Command == 60)
	{
	  ycurrentpos = Data2;
	  break;
	}
    }
  cout << "The position of the X stepper is " << xcurrentpos << "." << endl;
  cout << "The position of the Y stepper is " << ycurrentpos << "." << endl;

  if(xcurrentpos > ycurrentpos){
    //Sleeptime if x > y
    sleepzero = 100*1000*(xcurrentpos/xmicrosteptot); //(length of drive in cm)*(ms/cm)*(fractional drive position)
  }
  else{
    sleepzero = 50*1000*(ycurrentpos/ymicrosteptot); //(length of drive in cm)*(ms/cm)*(fractional drive position)
  }
  //cout << sleepzero << endl;

  PSERIAL_Send( 1,20,0 );
  PSERIAL_Send( 2,20,0 );
  //Sleep(20000);
  Sleep(sleepzero);



  ///////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////
  // ENABLE IF HODOSCOPE IS ATTACHED! ---------------------------------------
  /*
    double hardstopx = 0.0;
    double hardstopy = 0.0;

    //Prevents adjustment of the hardstops from running the carriage into the
    //end of the drive.
    if (hardstopx > xmicrosteptot) hardstopx = xmicrosteptot;
    if (hardstopy > ymicrosteptot) hardstopy = ymicrosteptot;

    //Prevents the hodoscope from running into the test stand or light tight
    //box.
    PSERIAL_Send( 1,44,hardstopx );
    PSERIAL_Send( 2,44,hardstopy );
  */
  //-------------------------------------------------------------------------
  ///////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////



  // Begin Scan
  for (int ii = 0; ii < xsteps; ii++)
    {
      PSERIAL_Send( 1,20, (xorigin + ii*xgeneralstep) );

      for (int jj = 0; jj < ysteps; jj++)
	{
	  //Setting the sleeptime for y to return
	  if (jj == 0 && ii != 0){
	    PSERIAL_Send( 2,60,64 );
	    while (1)
	      {
		if (PSERIAL_Receive (&Unit2,&Command,&Data2) && Unit2 == 2 && Command == 60)
		  {
		    ycurrentpos = Data2;
		    break;
		  }
	      }
	    sleeptime = 50*1000*((ycurrentpos-yorigin)/ymicrosteptot); //(length of drive in cm)*(ms/cm)*(fractional drive position)
	  }
	  //wait time to send to the origin from zero
	  if(jj==0 && ii ==0){
	    if(xorigincm > yorigincm)
	      {
		sleeptime = 1000*xorigincm; //(ms/cm)*(origin)
	      }else
	      {
		sleeptime = 1000*yorigincm; //(ms/cm)*(origin)
	      }
	  }

	  printf("\nMoving to column %i, row %i\n", ii, jj);
	  PSERIAL_Send( 2,20,(yorigin + jj*ygeneralstep) );

	  if (jj == 0)
	    {
	      Sleep(sleeptime);
	    }else
	    {
	      Sleep(1000*ysteplengthcm); // (ms/cm)*(cm/step)
	    }


	  ///////////////////////////////////////////////////////////////////////////
	  ///////////////////////////////////////////////////////////////////////////

	  // Here include method for data aquisition ---------------------------

	  ///////////////////////////////////////////////////////////////////////////
	  ///////////////////////////////////////////////////////////////////////////


	  // PULSE FIT INTEGRATION METHOD - For independently written scope code.
	  //oscillo = iopen("gpib1,7");
	  //system("scope.exe");


	  ///////////////////////////////////////////////////////////////////////////
	  ///////////////////////////////////////////////////////////////////////////

	  /*
	  // AVERAGING METHOD

	  oscillo = iopen("gpib1,7");
	  double hits = 0.0;
	  double mean = 0.0;
	  itimeout(oscillo, 2000000);
	  WriteIO(":CDISPLAY");
	  WriteIO(":RUN");
	  WriteIO(":ACQuire:AVERAGE:COUNT 1000");
	  WriteIO(":ACQUIRE:AVERAGE ON");
	  //WriteIO(":HISTOGRAM:WINDOW:X1POSITION 85.0e-9");//for strt Y11MCS
	  //WriteIO(":HISTOGRAM:WINDOW:X2POSITION 125.0e-9");
	  WriteIO(":HISTOGRAM:WINDOW:X1POSITION 110.0e-9"); //for custom serpentine
	  WriteIO(":HISTOGRAM:WINDOW:X2POSITION 150.0e-9");
	  cout << "Averaging enabled" << endl;

	  while (hits < 400000)
	  {
	  Sleep(4500);
	  Sleep(4500);
	  Sleep(4500);
	  WriteIO(":MEASURE:HISTOGRAM:HITS?");
	  cout << hits << endl;
	  ReadDouble(&hits);
	  cout << hits << endl;
	  }

	  WriteIO(":ACQUIRE:AVERAGE OFF");
	  WriteIO(":STOP");
	  WriteIO(":MEASURE:HISTOGRAM:MEAN?");
	  ReadDouble(&mean);
	  iclose(oscillo);
	  cout << mean << endl;
	  file << mean << "\n";
	  printf("Data Collected");
	  Sleep(2000);
	  */

	  ///////////////////////////////////////////////////////////////////////////
	  ///////////////////////////////////////////////////////////////////////////

	  //NEW TEST -> INTEGRAL METHOD

	  oscillo = iopen("gpib1,7");
	  double area1 = 0.0; //New
	  double area2 = 0.0;
	  double vmin1 = 10.0;
	  double vmin2 = 10.0;
	  time_t rawtime;
	  clock_t t;
	  struct tm * timeinfo;
	  time ( &rawtime );
	  timeinfo = localtime ( &rawtime );
	  //double hour = 0.0;
	  double minute = 0.0;
	  double second = 0.0;
	  itimeout(oscillo, 2000000);

	  //Get sample pulses for calibration. Will have to be adjusted based on dimension.
	  /*
	    if (jj == (ysteps/2) && (ii == 8 || ii == xsteps - 8))
	    {
	    system("scope.exe");
	    }
	  */
	  //Integration on the scope method start.

	  // FOR LED TEST, CHECK EVERY TIME
	  //WriteIO(":CDISPLAY");
	  //WriteIO(":VIEW CHANNEL1");
	  //WriteIO(":VIEW CHANNEL2");
	  //WriteIO(":CHANNEL1:SCALE 200E-3"); //New
	  //WriteIO(":CHANNEL2:SCALE 200E-3"); //New
	  //WriteIO(":CHANNEL1:OFFSET -650E-3"); //New
	  //WriteIO(":CHANNEL2:OFFSET -650E-3"); //New
	  //WriteIO(":TIMEBASE:SCALE 20E-9"); //New
	  //WriteIO(":TIMEBASE:POSITION 110E-9"); //New

	  // FOR SOURCE TEST, CHECK EVERY TIME
	  WriteIO(":CDISPLAY");
	  WriteIO(":VIEW CHANNEL1");
	  WriteIO(":VIEW CHANNEL2");
	  WriteIO(":CHANNEL1:SCALE 20E-3"); //New
	  WriteIO(":CHANNEL2:SCALE 20E-3"); //New
	  WriteIO(":CHANNEL1:OFFSET -80E-3"); //New
	  WriteIO(":CHANNEL2:OFFSET -80E-3"); //New
	  WriteIO(":TIMEBASE:SCALE 20E-9"); //New
	  WriteIO(":TIMEBASE:POSITION -40E-9"); //New

	  /*  //Working Template
	      while (area2 >= 0.0 && area4 >=0.0)
	      {
	      t = clock();
	      WriteIO(":RUN");
	      WriteIO(":ACQUIRE:AVERAGE:COUNT 1000");
	      WriteIO(":ACQUIRE:AVERAGE ON");
	      cout << "Averaging enabled" << endl;
	      WriteIO(":MEASURE:AREA DISPLAY,CHANNEL2");
	      WriteIO(":MEASURE:AREA DISPLAY,CHANNEL4");
	      WriteIO(":MEASURE:AREA? DISPLAY,CHANNEL2");
	      ReadDouble(&area2);
	      WriteIO(":MEASURE:AREA? DISPLAY,CHANNEL4");
	      ReadDouble(&area4);
	      WriteIO(":ACQUIRE:AVERAGE OFF");
	      //WriteIO(":SYSTem:TIME?");
	      //ReadChar(&time);
	      //ReadDouble(&hour);
	      //ReadDouble(&minute);
	      //ReadDouble(&second);
	      WriteIO(":STOP");
	      t = clock() - t;
	      }
	  */
	  //while (area2 >= 0.0 && area1 >= 0.0)
	  //{
	  t = clock();
	  WriteIO(":RUN");
	  WriteIO(":MEASURE:SENDVALID ON");
	  WriteIO(":ACQUIRE:AVERAGE:COUNT 1500");
	  WriteIO(":ACQUIRE:AVERAGE ON");
	  //cout << "Averaging enabled" << endl;

	  // Measure the area for Channel 1
	  //WriteIO(":MEASURE:AREA DISPLAY,CHANNEL1");
	  //WriteIO(":MEASURE:AREA? DISPLAY,CHANNEL1");
	  //ReadDouble(&area1);
	  //cout <<"Area 1 "<< area1 << endl;
	  //iclose(oscillo);
	  //oscillo = iopen("gpib1,7");

	  // Measure the area for Channel 2
	  //WriteIO(":MEASURE:AREA DISPLAY,CHANNEL2");
	  //WriteIO(":MEASURE:AREA? DISPLAY,CHANNEL2");
	  //ReadDouble(&area2);
	  //cout <<"Area 2 "<< area2 << endl;
	  // comment out these two lines if only doing area 1 and area 2
	  //iclose(oscillo);
	  //oscillo = iopen("gpib1,7");

	  //// Measure the VMin for Channel 1
	  WriteIO(":MEASURE:SOURCE CHANNEL1");
	  WriteIO(":MEASURE:VMIN");
	  WriteIO(":MEASURE:VMIN?");
	  ReadDouble(&vmin1);
	  cout <<"VMin 1 "<< vmin1 << endl;
	  iclose(oscillo);
	  oscillo = iopen("gpib1,7");

	  // Measure the VMin for Channel 2
	  WriteIO(":MEASURE:SOURCE CHANNEL2");
	  WriteIO(":MEASURE:VMIN");
	  WriteIO(":MEASURE:VMIN?");
	  ReadDouble(&vmin2);
	  cout << "VMin 2 " << vmin2 << endl;

	  //WriteIO(":ACQUIRE:AVERAGE OFF");
	  //WriteIO(":SYSTem:TIME?");
	  //ReadChar(&time);
	  //ReadDouble(&hour);
	  //ReadDouble(&minute);
	  //ReadDouble(&second);
	  WriteIO(":STOP");
	  t = clock() - t;
	  //}

	  iclose(oscillo);
	  //cout << "Ch.1 integral: " << area1 << endl; //New
	  //cout << "Ch.2 integral: " << area2 << endl;
	  //cout << "Ch.4 integral: " << area4 << endl;
	  cout << "Time is: " << asctime (timeinfo) << endl;
	  printf ("It took me %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC);
	  cout << (float)t/CLOCKS_PER_SEC << endl;
	  //cout << "Time is: " << hour << ":" << minute << ":" << second << endl;
	  file_1 << (float)t/CLOCKS_PER_SEC << "\n";
	  //file_1 << hour << " " << minute << " " << second << "\n";
	  file_2 << area1 << "\n"; //Saved as SiPM 1
	  file_3 << area2 << "\n"; //New - Possibly confusing: file_1 contains time for data aquisition data, file_4 contains ave. pulse data from channel 1
	  file_4 << vmin1 << "\n";
	  file_5 << vmin2 << "\n";
	  printf("Data Collected");
	  Sleep(2000);



	  ///////////////////////////////////////////////////////////////////////////
	  ///////////////////////////////////////////////////////////////////////////

	  // End data aquisition -----------------------------------------------
	}

    }

  // Closing and rezeroing.
  file_1.close();
  file_2.close();
  file_3.close();
  file_4.close();
  file_5.close();
  printf("Returning to home positions");
  PSERIAL_Send( 1,1,64 );
  PSERIAL_Send( 2,1,64 );

  ////////////////////////////////////////////////////////////////////////////
  // ENABLE IF HODOSCOPE IS ATTACHED! ----------------------------------------
  /*
    Sleep(100000);
    PSERIAL_Send( 1,36,0 );
    PSERIAL_Send( 2,36,0 );
  */
  // -------------------------------------------------------------------------
  ////////////////////////////////////////////////////////////////////////////

  PSERIAL_Close();

  // Exiting program.
  return 0;
}
