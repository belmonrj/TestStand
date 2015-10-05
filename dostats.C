#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath> // for sqrt
#include <numeric> // for accumulate

using std::cout;
using std::endl;
using std::vector;
using std::accumulate;



void dostats(vector<double>); // function prototype



int main()
{

  // --- this way works!  :)
  // vector<double> vec1 {5.0,5.0,5.35,4.65,5.95,5.0,5.17,5.17,4.83}; // C++11
  // dostats(vec1);
  // --- and so does this way!  :)
  dostats({5.0,5.0,5.35,4.65,5.95,5.0,5.17,5.17,4.83}); // C++11

  // --- SiPM2 -------------------------------------------
  dostats({5.35,5.17,5.35,4.82,4.91,5.17,4.66,5.86,5.34});

  cout<<"And now for something completely different!"<<endl;
  cout<<"Actually it's basically the exact same stuff"<<endl;

  // --- SiPM1, earlier
  dostats({5.17,4.65,5.69,4.83,4.83,5.34,5.18,4.48});

  // --- SiPM2, earlier
  dostats({4.83,5.35,6.03,4.31,4.83,5.34,5.18,4.48});

  cout<<"And now for something completely different!"<<endl;
  cout<<"Actually it's basically the exact same stuff"<<endl;

  // --- SiPM1, 20151005
  dostats({4.66,5.17,5.17,5.00,5.00,5.00,5.00,5.17,5.93,5.02});

  cout<<"And now for something completely different!"<<endl;
  cout<<"Actually it's basically the exact same stuff"<<endl;

  // --- SiPM1, 20151005
  dostats({5.35,5.00,5.69,4.82,4.66,5.00,5.00,5.35,5.00,4.83});

  return 0;

}



void dostats(vector<double> diffs)
{

  // --- calculate the sum and the mean
  double sum = accumulate(diffs.begin(), diffs.end(), 0.0);
  double mean = sum/diffs.size();
  cout<<"mean is "<<mean<<endl;


  // --- calculate the variance and standard deviation
  double variance = 0;
  for(int i=0; i<diffs.size(); i++) variance += pow((diffs[i]-mean),2);
  variance /= diffs.size();
  double stddev = sqrt(variance);
  cout<<"variance is "<<variance<<endl;
  cout<<"stddev is "<<stddev<<endl;


  // --- check to see if any are outside of 2-sigma
  // --- for of order 10 elements, you might expect 0 or 1
  int counter1 = 0;
  int counter2 = 0;
  for(int i=0; i<diffs.size(); i++)
    {
      if ( fabs(diffs[i]-mean) < 1*stddev )
	counter1++;
      if ( fabs(diffs[i]-mean) < 2*stddev )
	counter2++;
      //cout << (diffs[i]-mean)/stddev << endl;
    }
  cout<<counter1<<" found within 1 sigma, expectation "<<diffs.size()*0.6827<<endl;
  cout<<counter2<<" found within 2 sigma, expectation "<<diffs.size()*0.9545<<endl;


  // --- make a new vector rejecting entries outside of 2-sigma
  vector<double> diffs_trimmed;
  for(int i=0; i<diffs.size(); i++)
    {
      if ( fabs(diffs[i]-mean) < 2*stddev ) diffs_trimmed.push_back(diffs[i]);
      else cout<<"vector element "<<i<<" value "<<diffs[i] <<" outside 2 sigma ("<<fabs(diffs[i]-mean)/stddev<<" sigma), rejecting"<<endl;
    }


  // --- calculate a trimmed mean rejecting at the 2-sigma level
  double sum_trimmed = 0;
  for(int i=0; i<diffs_trimmed.size(); i++) sum_trimmed += diffs_trimmed[i];
  double mean_trimmed = sum_trimmed/diffs_trimmed.size();
  cout<<"trimmed mean is "<<mean_trimmed<<endl;


  // --- calculate the new variance and standard deviation
  double variance_trimmed = 0;
  for(int i=0; i<diffs_trimmed.size(); i++) variance_trimmed += pow((diffs_trimmed[i]-mean_trimmed),2);
  variance_trimmed /= diffs_trimmed.size();
  double stddev_trimmed = sqrt(variance_trimmed);
  cout<<"trimmed variance is "<<variance_trimmed<<endl;
  cout<<"trimmed stddev is "<<stddev_trimmed<<endl;


  // --- check to see if any are outside of 2-sigma
  // --- for of order 10 elements, you might expect 0 or 1
  counter1 = 0;
  counter2 = 0;
  for(int i=0; i<diffs_trimmed.size(); i++)
    {
      if ( fabs(diffs_trimmed[i]-mean_trimmed) < 1*stddev_trimmed )
	counter1++;
      if ( fabs(diffs_trimmed[i]-mean_trimmed) < 2*stddev_trimmed )
	counter2++;
      //cout << (diffs_trimmed[i]-mean_trimmed)/stddev_trimmed << endl;
    }
  cout<<counter1<<" found within 1 sigma, expectation "<<diffs_trimmed.size()*0.6827<<endl;
  cout<<counter2<<" found within 2 sigma, expectation "<<diffs_trimmed.size()*0.9545<<endl;

  cout<<endl;

}
