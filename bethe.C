const double mu_mass = 106; // MeV, 105.6583715...
const double e_mass = 0.511; // MeV, 0.510998910...
const double density = 1.060; // g/cm^3
const double e_radius = 2.81794; // fm
const double pi = 3.1415926535; // pi
const double N_A = 6.0221415e23; // Avogadro's number
//const double thickness = 0.762; // cm, thickness of panel
//const double thickness = 0.777; // cm, thickness of panel
const double thickness = 0.815; // cm, thickness of panel

void bethe()
{

  cout << "minimum muon" << endl;
  dobethe(318,mu_mass);
  //dobethe(367,mu_mass); // guessed min by process of elimination

  dobethe(1000.0,mu_mass);
  dobethe(10000.0,mu_mass);
  dobethe(100000.0,mu_mass);

  return;

  cout << "electron from Sr-90" << endl;
  dobethe(0.546,e_mass);

  cout << "electron from Y-90" << endl;
  dobethe(2.484,e_mass);


  dobethe(0.55,e_mass);
  dobethe(1.0,e_mass);
  dobethe(1.5,e_mass);
  dobethe(2.0,e_mass);
  dobethe(2.5,e_mass);
  dobethe(5.0,e_mass);
  dobethe(10.0,e_mass);
  dobethe(50.0,e_mass);
  dobethe(100.0,e_mass);
  dobethe(1000.0,e_mass);

  cout << endl;

  dobethe(110.0,mu_mass);
  dobethe(212.0,mu_mass);
  dobethe(318.0,mu_mass);
  dobethe(500.0,mu_mass);
  dobethe(1000.0,mu_mass);
  dobethe(10000.0,mu_mass);
  dobethe(100000.0,mu_mass);

}




double dobethe(double energy = 318, double mass = mu_mass)
{

  density *= thickness; // to account for panel thickness in calculations

  double gamma = energy/mass;
  double beta = calcbeta(gamma);

  //cout << gamma << " " << beta << " " << beta*gamma << endl;

  double answer = -9999;

  double ZoverA = 0.53768;
  double A = 11.1592; // average atomic weight
  double K = 0.307075; // K is some constant relating to electron density, MeV/g/cm^2
  double I = 68.7; // mean excitation energy, in eV
  I /= 1.0e6; // covert to MeV;
  double z = 1; // 1 for electrons, muons, protons, etc... 2 for alpha particles

  double r_e = e_radius/1.0e13;
  double calcK = 4*pi*N_A*r_e*r_e*e_mass;

  //cout << "K is " << K << endl;
  //cout << "calcK is " << calcK << endl; // very nice agreement

  double firstblock = (K*z*z*ZoverA)/(beta*beta);
  double Tmax (2*e_mass*beta*beta*gamma*gamma)/(1 + (2*gamma*e_mass/mass) + (e_mass/mass)*(e_mass/mass));
  double logargument = (2*e_mass*beta*beta*gamma*gamma*Tmax)/(I*I);

  //cout << "log argument is " << logargument << endl;
  //cout << "first block is  " << firstblock << endl;

  // Sternheimer, 1982
  double X = log10(beta*gamma); // variable
  double a = 0.3670; // par
  double m = 2.724; // par
  double X0 = 0.1647; // par
  double X1 = 2.2; // par
  double hnup = 21.75; // eV
  hnup /= 1e6; // convert to MeV (to match I above)
  double C = -3.3; // par
  double calcC = -2*log(I/hnup) - 1;

  // cout << "C is " << C << endl;
  // cout << "calcC is " << calcC << endl; // very nice agreement

  double delta = 0; // density correction
  if(X<X1 && X>X0) delta = 4.6025*X + a*pow((X1 - X),m) + C;
  if(X>X1) delta = 4.6025*X + C;

  //delta/= 2;
  // cout << "delta is " << delta << endl;

  double plasma = 21.75; //
  plasma /= 1e6;
  double deltaover2 = log(plasma/I) + log(beta*gamma) - 0.5;
  // cout << "other delta is " << deltaover2 << endl;

  answer = firstblock * (0.5 * log(logargument) - (beta*beta) - delta);

  answer *= density;

  // cout << "For gamma = " << gamma << " <dE/dx> = " << answer << endl;


  // --- now calculation for MPV, not mean...
  double xi = density*firstblock;
  // cout << "xi " << xi << endl;
  double ksi = density*(0.1536/(beta*beta))*ZoverA;// * 1e-3;
  // cout << "ksi " << ksi << endl;
  xi /= 2; // not sure why, but...

  cout << "xi " << xi <<  " ksi " << ksi << " xi/ksi = " << xi/ksi << endl;

  double logargument_k = (2*e_mass*beta*beta*gamma*gamma*ksi)/(I*I);
  double logargument_x = (2*e_mass*beta*beta*gamma*gamma*xi)/(I*I);

  double mpv = ksi*(log(logargument_k) - (beta*beta) + 1 - 0.577216);
  // cout << "mpv = " << mpv << endl;
  //double mpv2 = xi*(log(logargument_x) - (beta*beta) + 0.198);
  double mpv2 = xi*(log(logargument_x) - (beta*beta) + 1 - 0.577216);
  //double mpv2 = xi*(log(logargument_x) - (beta*beta) - 0.198);
  // cout << "mpv = " << mpv << endl;
  cout << "xi " << mpv2 <<  " ksi " << mpv << " xi/ksi = " << mpv2/mpv << endl;

  cout << "mpv = " << mpv << " mean = " << answer << " ratio = " << mpv/answer << endl;

  return answer;

}



double calcbeta(double gamma)
{
  // gamma = 1/sqrt(1-beta^2)
  double x = 1.0/(gamma*gamma); // = 1 - beta^2
  x *= -1; // beta^2 - 1;
  x += 1; // beta^2
  if(x>0) return(sqrt(x));
  else
    {
      return -9999;
    }
}
