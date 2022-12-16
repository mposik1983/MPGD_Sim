#include <iostream>
#include <fstream>
#include <cmath>

#include "Garfield/MediumMagboltz.hh"
#include "Garfield/FundamentalConstants.hh"

using namespace Garfield;

int main(int argc, char * argv[]) {

  MediumMagboltz gas;
  const std::string path = "/home/mposik/MPGD_Sim/Gas/GasFile/GeneratedTables/";
  const std::string gasFile = "Xe_80.000000_CO2_20.000000_1.700000T_ang90.000000";
  gas.LoadGasFile(path + gasFile + ".gas");
  
  const std::string outpath = "/home/mposik/MPGD_Sim/Gas/GasFile/PlotTables/";
  std::ofstream fout;
  fout.open(outpath + gasFile + ".csv");

  std::vector<double> efields;
  std::vector<double> bfields;
  std::vector<double> angles;
  gas.GetFieldGrid(efields, bfields, angles);
  const auto nE = efields.size();
  const auto nB = bfields.size();
  const auto nA = angles.size();

  for (size_t j = 0; j < nB; ++j) {
    for (size_t k = 0; k < nA; ++k) {
      std::cout << "B = " << bfields[j] << " T, theta = "
                << angles[k] * RadToDegree << " degree\n";
      std::cout << "   E [V/cm]     vE [cm/us]    Long. Diff. [sqrt(cm)]     Trans. Diff. [sqrt(cm)]     alpha [1/cm]\n";
      fout << "   E [V/cm],     vE [cm/us],    Long. Diff. [sqrt(cm)],     Trans. Diff. [sqrt(cm)],     alpha [1/cm]\n";
      for (size_t i = 0; i < nE; ++i) {
        double ve = 0.;
        gas.GetElectronVelocityE(i, j, k, ve);
        // Convert from cm/ns to cm/us.
        ve *= 1.e3;
        double alpha = 0.;
        gas.GetElectronTownsend(i, j, k, alpha);
        alpha = exp(alpha);
	//Get diffusions
        double dl = 0.0;
        gas.GetElectronLongitudinalDiffusion(i, j, k, dl);
        double dt = 0.0;
        gas.GetElectronTransverseDiffusion(i, j, k, dt);

        std::printf("%10.3f    %10.3f    %10.3f     %10.3f     %10.3f\n", efields[i], ve, dl, dt, alpha);
        //fout << std::printf("%10.3f    %10.3f    %10.3f     %10.3f     %10.3f\n", efields[i], ve, dl, dt, alpha);
	fout << efields[i] << "," << ve << "," << dl << "," << dt << "," << alpha << std::endl;
        //std::printf("%10.3f    %10.3f    %10.3f\n", efields[i], ve, dl);
      }
    }
  } 
 fout.close(); 
  
}
