// Ionization 
// -------------------------------------------------------------------
// Simulate particles through gas and calculate the primary ionization clusters
// and their size (secondary electrons)
//
// -------------------------------------------------------------------
// M. Posik
// posik@temple.edu
// Nov. 2022 

#include <iostream>
#include <fstream>
#include <cmath>

#include <TCanvas.h>
#include <TROOT.h>
#include <TApplication.h>
#include <TH1F.h>
#include <TF1.h>
#include <TMath.h>
#include <TString.h>

#include "Garfield/TrackHeed.hh"
#include "Garfield/MediumMagboltz.hh"
#include "Garfield/SolidBox.hh"
#include "Garfield/GeometrySimple.hh"
#include "Garfield/ComponentConstant.hh"
#include "Garfield/Sensor.hh"
#include "Garfield/FundamentalConstants.hh"
#include "Garfield/Random.hh"
#include "Garfield/Plotting.hh"

bool DEBUG = false;

using namespace Garfield;

int main(int argc, char * argv[]) {

  TApplication *app = new TApplication("app", &argc, argv);
  plottingEngine.SetDefaultStyle();

  // Make a gas medium.
  MediumMagboltz* gas = new MediumMagboltz("Ar", 80.0, "CO2", 20.0);
  gas->SetTemperature(293.15);
  gas->SetPressure(AtmosphericPressure);
  
  constexpr double gap = 0.3; // cm
  constexpr double length = 10.0; // cm
  //3 placement and 3 half dimension arguments
  SolidBox* box = new SolidBox(0, 0, 0, 0.5*length,0.5*length,0.5*gap);

  // Combine gas and box to a simple geometry.
  GeometrySimple* geo = new GeometrySimple();
  geo->AddSolid(box, gas);

  // Make a component with constant electric field.
  ComponentConstant* field = new ComponentConstant();
  field->SetGeometry(geo);
  //field->SetElectricField(0., 0., 1000.); 

  // Make a sensor.
  Sensor* sensor = new Sensor();
  sensor->AddComponent(field);
  
  // Use Heed for simulating the particles passing through gas volume.
  TrackHeed* track = new TrackHeed();
  track->SetParticle("mu-");
  track->SetMomentum(150.0e9); //eV
  track->SetSensor(sensor);
  //track->EnableElectricField();
 
  
  //Initial track coordinates of the mip.
  const double track_x = 0.0;
  const double track_y = 0.0;
  const double track_z = 0.5*gap;
  const double track_t = 0.0;
  //track direction
  double track_dx = 0.0;
  double track_dy = 0.0;
  double track_dz = -1.0;
  //cluster info
  double clust_x = 0.0; 
  double clust_y = 0.0; 
  double clust_z = 0.0; 
  double clust_t = 0.0;
  double clust_e = 0.0; 
  double extra = 0.0;
  int ne_tot = 0; //total number of electrons summed over all clusters
  double ne_tot_avg = 0.0; //average number of total electrons

  //Define some histograms
  TH1F* hClust = new TH1F("hClust", "Number of Clusters", 41, 0, 40);
  TH1F* hElec  = new TH1F("hElec", "Number of Electrons/Cluster", 100, 0, 100);

  //setup event loop
  const int nEvents = 10000;
  for (unsigned int i = 0; i < nEvents; ++i) {
    //make track    
    track->NewTrack(track_x,track_y,track_z,track_t,track_dx, track_dy, track_dz);
    
    //number of clusters
    int clust_count = 0;
    //number of electrons in the cluster
    int clust_size = 0;

    if (i % 100 == 0) std::cout << i << "/" << nEvents << "\n";
    
    //Loop over clusters
    while(track->GetCluster(clust_x,clust_y,clust_z,clust_t,clust_size,clust_e,extra)){
      //cluster found and electrons in it counted
      hElec->Fill(clust_size);
      ne_tot += clust_size;
      //increase its cluster counts
      clust_count++;
      if (DEBUG){ std::cout << Form("Cluster: %d, cluster size: %d, cluster loc (x,y,z): %f, %f, %f \n",
		      clust_count ,clust_size, clust_x, clust_y, clust_z);
      }
    }//end cluster loop
    if (DEBUG) std::cout << Form("Total Clusters found: %d \n",clust_count); 
    //fill cluster histogram
    hClust->Fill(clust_count);
    //if(clust_count !=0) hClust->Fill(clust_count);
  }//end event loop

  ne_tot_avg = ( (double)ne_tot ) / ( (double)(nEvents) );
  std::cout << Form("total electrons: %d \t average electrons per event: %f \n", ne_tot, ne_tot_avg);

  //Poisson fit function
  TF1* poisson_fit = new TF1("poission_fit","[0]*TMath::Poisson(x,[1])",0,40);
  poisson_fit->SetParameter(0,hClust->Integral());
  poisson_fit->SetParameter(1,hClust->GetMean());
  poisson_fit->SetParName(0,"Normailization");
  poisson_fit->SetParName(1,"#mu"); 
  poisson_fit->SetLineColor(kRed);

  TCanvas* c1 = new TCanvas("c", "", 800, 600);
  gStyle->SetOptFit(1);
  c1->Divide(2,1);
  c1->cd(1);
  hClust->SetLineColor(kBlue+2);
  hClust->GetXaxis()->SetTitle("Number of Clusters");
  hClust->Draw();
  hClust->Fit(poisson_fit);

  c1->cd(2);
  c1->SetLogy(1);
  hElec->SetLineColor(kBlue+2);
  hElec->SetTitle("Number Electrons per Clusters");
  hElec->GetXaxis()->SetTitle("Electrons/Cluster");
  hElec->Draw();
  c1->SetLogy(1);

  app->Run(true);
}
