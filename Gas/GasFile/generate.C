#include <iostream>

#include "Garfield/MediumMagboltz.hh"
#include "Garfield/FundamentalConstants.hh"
#include <string>
#include "TMath.h"

using namespace Garfield;
using namespace std;
int main(int argc, char * argv[]) {

  const double pressure = 1 * AtmosphericPressure;
  const double temperature = 293.15;
 
  // Setup the gas.
  string gc1 = "Ar"; //gas compoent 1
  double fgc1 = 80.0; //fraction of gas component 1 (in %)
  string gc2 = "CO2"; //gas compoent 2
  double fgc2 = 20.0; //fraction of gas component 2 (in %)

  cout << "Enter gas component 1\n"; cin >> gc1; cout << endl;
  cout << "Enter gas 1 fraction\n";  cin >> fgc1; cout << endl;
  cout << "Enter gas component 2\n"; cin >> gc2; cout << endl;
  cout << "Enter gas 2 fraction\n";  cin >> fgc2; cout << endl;
  cout << "Using gas: " << gc1 << fgc1 << gc2 << fgc2 << endl;
  if(fgc1+fgc2 !=100)
    {
      cout << "Gas mixture is " << fgc1 + fgc2 << endl;
      return -1;
    }

  MediumMagboltz gas(gc1, fgc1, gc2, fgc2);
  gas.SetTemperature(temperature);
  gas.SetPressure(pressure);
  //gas.EnableAutoEnergyLimit(false); //turn off auto upper limit
  //gas.SetMaxElectronEnergy(100.0);  //set upper limit
  // Set the field range to be covered by the gas table. 
  double b = 0.0;
  cout << "Enter B field\n";
  cin >> b;
  cout << "Using B Field of " << b << endl;
  const size_t nE = 25;
  const double emin =    10.0;
  const double emax =  6000.0;
  const size_t nB   = 1;
  const double bmin = b;
  const double bmax = b;
  double a = 90; //in degrees
  cout << "Enter B field angle relative to E field\n";
  cin >> a;
  cout << "Using B Field angle of " << a << endl;
  const double angle = a*TMath::Pi()/180; //convert to angle
  // Flag to request logarithmic spacing.
  constexpr bool useLog = true;
  gas.SetFieldGrid(emin, emax, nE, useLog, bmin, bmax, nB, angle); 
  const int ncoll = 15;
  // Run Magboltz to generate the gas table.
  gas.GenerateGasTable(ncoll);
  // Save the table. 
  string bs = to_string(b);
  string as = to_string(a);
  string fgc1s = to_string(fgc1);
  string fgc2s = to_string(fgc2);
  string gasfilename = gc1+"_"+fgc1s+"_"+gc2+"_"+fgc2s+"_"+bs+"T_ang"+as+".gas";
  gas.WriteGasFile(gasfilename);

}
