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

//FOR C++ io
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
  iread(oscillo, (char*)buffer, BytesToRead, &reason, &BytesRead);
  return BytesRead;
}



unsigned long ReadByte (char *buffer, unsigned long BytesToRead)
{
  unsigned long BytesRead;
  int reason;
  iread(oscillo, buffer, BytesToRead, &reason, &BytesRead);
  return BytesRead;
}



void WriteIO(char *buffer)
{
  unsigned long actualcnt;
  unsigned long BytesToWrite;
  int send_end=1;
  char temp[50];
  // ---
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
  // ---
  BytesToWrite = strlen(buffer)+1;
  strcpy_s(temp, buffer);
  strcat_s(temp, "\n");
  iread(oscillo, temp, BytesToWrite, &send_end, &actualcnt);

}



void ReadDouble(double *buffer)
{
  iscanf(oscillo, "%lf", buffer);
}


// All commands follow the structure "PSERIAL_Send( X,Y,Z )" where X is the
// target drive (for several daisy-chained), Y is the Zaber command (20 is
// move absolute, for instance), and Z is data (microsteps to move for
// command 20, for instance). All daisy-chained drives must be numbered
// for coherent commands. This is accomplished with zaber command 2 (already
// present in code). Wait times are essential to prevent data collision.
// Data value "64" denotes that the value is irrelevant for the command.



int main(void)
{

  // --- Use a time struct to get the current date and time to append to the output file names
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


  // --- Create and open the output text files for the data to be measured
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


  // ----------------------------------------------------------- //
  // ----------------------------------------------------------- //
  // --- NEVER CHANGE THESE NUMBERS!!!                       --- //
  // --- Total number of available microsteps for each drive --- //
  double xmicrosteptot = 8062992;                                //
  double ymicrosteptot = 4031496;                                //
  double stepspercm = xmicrosteptot/100;                         //
  // ----------------------------------------------------------- //
  // ----------------------------------------------------------- //


  // -----------------------------------------------------------------------------------------------------------------------------
  // --- Set the positions for the beginning of the scan
  // --- Check these regularly and update as needed, but keep old numbers here for reference
  // --- Note that since this is the beginning of the scan, the start position is behind the fiber so that the scan goes across it
  // --- y-position
  //double yorigincm = 33.0; // fiber #4, furthest back
  //double yorigincm = 35.5; // fiber #3, second furthest back
  //double yorigincm = 38.0; // fiber #2, second closest
  double yorigincm = 43.0; // fiber #1, closest
  // --- x-position
  double xorigincm = 9.0; // for radioactive source
  //double xorigincm = 9.0 + 1.905; // for LED in source housing
  // -----------------------------------------------------------------------------------------------------------------------------
  // --- Convert the origin positions from cm to steps
  double xorigin = stepspercm*xorigincm;
  double yorigin = stepspercm*yorigincm;
  // -----------------------------------------------------------------------------------------------------------------------------


  // -------------------------------------------------------------------------------------------------------------
  // --- Stepper motor will move xsteps in the x-direction and ysteps in the y-direction, taking data at each step
  int xsteps = 58;
  int ysteps = 9;
  //int xsteps = 58;
  //int ysteps = 3;
  // --- Distance between each step during the scan
  double xsteplengthcm = 0.5;
  double ysteplengthcm = 0.5;
  // --- For example, 58 steps in x with a step length of 0.5 produces a scan of 29 cm
  //                  9 steps in y with a step length of 0.5 produces a scan of 4.5 cm
  // --- The code current does 9 steps in y followed by 1 step in x
  // -------------------------------------------------------------------------------------------------------------
  // --- Conver the step length from cm to steps
  double xgeneralstep = stepspercm*xsteplengthcm;
  double ygeneralstep = stepspercm*ysteplengthcm;
  // -------------------------------------------------------------------------------------------------------------


  // --- Variable for recording current positions of drives.
  // --- Used to calibrate wait times.
  // --- Takes stage 860ms to go 8062992 steps.
  double xcurrentpos = 0.0;
  double ycurrentpos = 0.0;
  double sleepzero = 0.0;
  double sleeptime = 0.0;
  double sleeptime2 = 0.0;


  // --- Checks to see if parameters are within physics ranges of the stages
  if (xsteps*xgeneralstep+xorigin > xmicrosteptot || ysteps*ygeneralstep+yorigin > ymicrosteptot)
    {
      cout << "Settings take the source beyond the end of the drive. Please readjust." << endl;
      return 0;
    }


  // --------------------------------------------------------------------------------- //


  // --------------------------------------------------------------------------------- //
  // --- NOW BEGINS THE PART OF THE CODE THAT COMMUNICATES WITH THE STEPPER MOTORS --- //
  // --- IF YOU TOUCH IT, PREPARE FOR UNFATHOMABLE SUFFERING                       --- //
  // --------------------------------------------------------------------------------- //

  // Variables for PSERIAL_Receive command.
  unsigned char Unit;
  unsigned char Unit2;
  unsigned char Command;
  unsigned char Command2;
  long Data;
  long Data2;

  // --- Give a warning if step settings distort the scan grid
  if ( xgeneralstep > ygeneralstep + 1000 || xgeneralstep < ygeneralstep - 1000)
    {
      cout << "Warning: Check Aspect Ratio." << endl;
      cout << "Each step in X is " << xgeneralstep << "." << endl;
      cout << "Each step in Y is " << ygeneralstep << "." << endl;
      cout << "It might be desired to adjust xaxis and yaxis parameters to produce similar values" << endl;
      Sleep(10000);
    }

  // --- Initialize the stepper motors to interpret instructions
  PSERIAL_Initialize();

  // --- Rezero the drives (send them to the home position)
  // --- Question from Ron B. to Sebastian S. - is this necessary?
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
      if (PSERIAL_Receive (&Unit, &Command, &Data) && Unit == 1 && Command == 60)
	{
	  xcurrentpos = Data;
	  break;
	}
    }
  Sleep(1500);
  PSERIAL_Send( 2,60,64 );
  while (1)
    {
      if (PSERIAL_Receive (&Unit, &Command, &Data2) && Unit == 2 && Command == 60)
	{
	  ycurrentpos = Data2;
	  break;
	}
    }
  cout << "The position of the X stepper is " << xcurrentpos << "." << endl;
  cout << "The position of the Y stepper is " << ycurrentpos << "." << endl;

  if(xcurrentpos > ycurrentpos)
    {
      //Sleeptime if x > y
      sleepzero = 100*1000*(xcurrentpos/xmicrosteptot); //(length of drive in cm)*(ms/cm)*(fractional drive position)
    }
  else
    {
      sleepzero = 50*1000*(ycurrentpos/ymicrosteptot); //(length of drive in cm)*(ms/cm)*(fractional drive position)
    }
  //cout << sleepzero << endl;

  PSERIAL_Send(1, 20, 0);
  PSERIAL_Send(2, 20, 0);
  //Sleep(20000);
  Sleep(sleepzero);


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


  // --- Begin the scan
  for (int ii = 0; ii < xsteps; ii++)
    {

      PSERIAL_Send( 1,20, (xorigin + ii*xgeneralstep) );

      for (int jj = 0; jj < ysteps; jj++)
	{
	  //Setting the sleeptime for y to return
	  if (jj == 0 && ii != 0)
	    {
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
	  if(jj==0 && ii ==0)
	    {
	      if(xorigincm > yorigincm)
		{
		  sleeptime = 1000*xorigincm; //(ms/cm)*(origin)
		}else
		{
		  sleeptime = 1000*yorigincm; //(ms/cm)*(origin)
		}
	    }

	  //printf("\nMoving to column %i, row %i\n", ii, jj);
	  cout<<"Moving to column "<<ii<<", row "<<jj<<endl;
	  PSERIAL_Send(2, 20, (yorigin + jj*ygeneralstep));

	  if (jj == 0)
	    {
	      Sleep(sleeptime);
	    }
	  else
	    {
	      Sleep(1000*ysteplengthcm); // (ms/cm)*(cm/step)
	    }


	  // --------------------------------------------------------------------------------- //
	  // --- NOW BEGINS THE PART OF THE CODE THAT COMMUNICATES WITH THE STEPPER MOTORS --- //
	  // --- IF YOU TOUCH IT, PREPARE FOR UNFATHOMABLE SUFFERING                       --- //
	  // --------------------------------------------------------------------------------- //

	  // --- This part is deprecated, probably not to be used again...
	  // PULSE FIT INTEGRATION METHOD - For independently written scope code.
	  //oscillo = iopen("gpib1,7");
	  //system("scope.exe");
	  //NEW TEST -> INTEGRAL METHOD

	  // --- Open up the comms to the scope
	  oscillo = iopen("gpib1,7");
	  double area1 = 0.0;
	  double area2 = 0.0;
	  double vmin1 = 10.0; // Why 10?
	  double vmin2 = 10.0; // Why 10?
	  time_t rawtime;
	  clock_t t;
	  struct tm * timeinfo;
	  time ( &rawtime );
	  timeinfo = localtime ( &rawtime );
	  //double hour = 0.0;
	  double minute = 0.0;
	  double second = 0.0;
	  itimeout(oscillo, 2000000);

	  // --- This part is deprecated, probably not to be used again...
	  //Get sample pulses for calibration. Will have to be adjusted based on dimension.
	  /*
	    if (jj == (ysteps/2) && (ii == 8 || ii == xsteps - 8))
	    {
	    system("scope.exe");
	    }
	  */
	  //Integration on the scope method start.

	  // ---------------------------------------------------------------- //
	  // --- Set the parameters on the scope for the data acquisition --- //
	  // ---------------------------------------------------------------- //

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

	  // --- This part is deprecated, probably not to be used again
	  // --- Working Template
	  // while (area2 >= 0.0 && area4 >=0.0)
	  // {
	  // t = clock();
	  // WriteIO(":RUN");
	  // WriteIO(":ACQUIRE:AVERAGE:COUNT 1000");
	  // WriteIO(":ACQUIRE:AVERAGE ON");
	  // cout << "Averaging enabled" << endl;
	  // WriteIO(":MEASURE:AREA DISPLAY,CHANNEL2");
	  // WriteIO(":MEASURE:AREA DISPLAY,CHANNEL4");
	  // WriteIO(":MEASURE:AREA? DISPLAY,CHANNEL2");
	  // ReadDouble(&area2);
	  // WriteIO(":MEASURE:AREA? DISPLAY,CHANNEL4");
	  // ReadDouble(&area4);
	  // WriteIO(":ACQUIRE:AVERAGE OFF");
	  // //WriteIO(":SYSTem:TIME?");
	  // //ReadChar(&time);
	  // //ReadDouble(&hour);
	  // //ReadDouble(&minute);
	  // //ReadDouble(&second);
	  // WriteIO(":STOP");
	  // t = clock() - t;
	  // }

	  //while (area2 >= 0.0 && area1 >= 0.0)
	  //{
	  t = clock();
	  WriteIO(":RUN");
	  WriteIO(":MEASURE:SENDVALID ON");
	  WriteIO(":ACQUIRE:AVERAGE:COUNT 1500");
	  WriteIO(":ACQUIRE:AVERAGE ON");
	  //cout << "Averaging enabled" << endl;

	  // --- Measure the area for Channel 1
	  //WriteIO(":MEASURE:AREA DISPLAY,CHANNEL1");
	  //WriteIO(":MEASURE:AREA? DISPLAY,CHANNEL1");
	  //ReadDouble(&area1);
	  //cout <<"Area 1 "<< area1 << endl;
	  //iclose(oscillo);
	  //oscillo = iopen("gpib1,7");

	  // --- Measure the area for Channel 2
	  //WriteIO(":MEASURE:AREA DISPLAY,CHANNEL2");
	  //WriteIO(":MEASURE:AREA? DISPLAY,CHANNEL2");
	  //ReadDouble(&area2);
	  //cout <<"Area 2 "<< area2 << endl;
	  // comment out these two lines if only doing area 1 and area 2
	  //iclose(oscillo);
	  //oscillo = iopen("gpib1,7");

	  // --- Measure the VMin for Channel 1
	  WriteIO(":MEASURE:SOURCE CHANNEL1");
	  WriteIO(":MEASURE:VMIN");
	  WriteIO(":MEASURE:VMIN?");
	  ReadDouble(&vmin1);
	  cout <<"VMin 1 "<< vmin1 << endl;
	  iclose(oscillo);
	  oscillo = iopen("gpib1,7");

	  // --- Measure the VMin for Channel 2
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
	  //} // matches while (area2>=0 && area4>=0 above), deprecated

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

	  // --- End of data aquisition

	} // matches for (jj), inner loop of stepper motor control

    } // matches for (ii), outer loop of stepper motor

  // --- Now finished with data files, close them
  file_1.close();
  file_2.close();
  file_3.close();
  file_4.close();
  file_5.close();

  // --- Rezero the stepper motors
  cout<<"Returning to home positions"<<endl;;
  PSERIAL_Send( 1,1,64 );
  PSERIAL_Send( 2,1,64 );


  // ENABLE IF HODOSCOPE IS ATTACHED! ----------------------------------------
  /*
    Sleep(100000);
    PSERIAL_Send( 1,36,0 );
    PSERIAL_Send( 2,36,0 );
  */
  // -------------------------------------------------------------------------

  // --- Close comms to the stepper motors
  PSERIAL_Close();

  // --- All done!
  cout<<"All done!  Have a nice day!"<<endl;
  return 0;

} // matches int main(void)
