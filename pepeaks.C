//#include <numeric> // for accumulate, which isn't working with root for some reason...

void pepeaks()
{

  // --- my installation of ROOT does not support the nice C++11 initialization syntax :(
  // --- I guess I should really fix that...
  vector<double> diffs;
  diffs.push_back(5.00);
  diffs.push_back(5.00);
  diffs.push_back(5.35);
  diffs.push_back(4.65);
  diffs.push_back(5.95);
  diffs.push_back(5.00);
  diffs.push_back(5.17);
  diffs.push_back(5.17);
  diffs.push_back(4.83);

  // --- this isn't work for some stupid reason
  //double sumdiffs = accumulate(diffs.begin(), diffs.end(), 0);

  // --- calculate the sum and the mean
  double sum = 0;
  for(int i=0; i<diffs.size(); i++) sum += diffs[i];
  double mean = sum/diffs.size();
  cout<<"mean is "<<mean<<endl;

  // --- calculate the variance and standard deviation
  double variance = 0;
  for(int i=0; i<diffs.size(); i++) variance += (diffs[i]-mean)**2;
  variance /= diffs.size();
  double stddev = sqrt(variance);
  cout<<"variance is "<<variance<<endl;
  cout<<"stddev is "<<stddev<<endl;

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
  for(int i=0; i<diffs_trimmed.size(); i++) variance_trimmed += (diffs_trimmed[i]-mean_trimmed)**2;
  variance_trimmed /= diffs_trimmed.size();
  double stddev_trimmed = sqrt(variance_trimmed);
  cout<<"trimmed variance is "<<variance_trimmed<<endl;
  cout<<"trimmed stddev is "<<stddev_trimmed<<endl;

  // --- check to see if any are outside of 2-sigma
  // --- for of order 10 elements, you might expect 0 or 1
  int counter = 0;
  for(int i=0; i<diffs_trimmed.size(); i++)
    {
      if ( fabs(diffs_trimmed[i]-mean_trimmed) > 2*stddev_trimmed )
	counter++;
      cout << (diffs_trimmed[i]-mean_trimmed)/stddev_trimmed << endl;
    }
  cout<<counter<<" found outside trimmed distribution"<<endl;

}
