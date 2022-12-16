#include <vector>
#include <fstream>
#include "TString.h"

Int_t GetData(TString file, vector<Double_t> &e, vector<Double_t> &v, vector<Double_t> &dl, vector<Double_t> &dt, vector<Double_t> &alpha);
TGraph * GetGraph(vector<Double_t> x, vector<Double_t> y, Int_t lc, Int_t lw, Int_t ls); //plot x vs y, line color, line width, line style

void PlotTransport()
{
  vector<TGraph*> vgraph; vector<TString> labels; 
  vector<TGraph*> dlgraph, dtgraph, alphagraph;

  vector<Double_t> e0, v0, dl0, dt0, alpha0;
  GetData("Ar_80.000000_CO2_20.000000_0.000000T_ang90.000000.dat",e0, v0, dl0, dt0, alpha0);
  //for(Int_t i = 0 ; i <  e0.size(); i++) cout << e0[i] << endl;
  TGraph *vg0 = GetGraph(e0,v0, kGreen, 2, 1);  
  TGraph *dlg0 = GetGraph(e0,dl0, kGreen, 2, 1);  
  TGraph *dtg0 = GetGraph(e0,dt0, kGreen, 2, 1);  
  vgraph.push_back(vg0); labels.push_back("ArCO2: 80/20");
  dlgraph.push_back(dlg0); 
  dtgraph.push_back(dtg0); 

  vector<Double_t> e1, v1, dl1, dt1, alpha1;
  GetData("Ar_80.000000_CO2_20.000000_1.700000T_ang90.000000.dat",e1, v1, dl1, dt1, alpha1);
  TGraph *vg1 = GetGraph(e1,v1, kRed, 2, 1);  
  TGraph *dlg1 = GetGraph(e1,dl1, kRed, 2, 1);  
  TGraph *dtg1 = GetGraph(e1,dt1, kRed, 2, 1);  
  vgraph.push_back(vg1); labels.push_back("ArCO2: 80/20");
  dlgraph.push_back(dlg1);
  dtgraph.push_back(dtg1);
  
  vector<Double_t> e2, v2, dl2, dt2, alpha2;
  GetData("Xe_80.000000_CO2_20.000000_0.000000T_ang90.000000.dat",e2, v2, dl2, dt2, alpha2);
  TGraph *vg2 = GetGraph(e2,v2, kBlue, 2, 1);  
  TGraph *dlg2 = GetGraph(e2,dl2, kBlue, 2, 1);  
  TGraph *dtg2 = GetGraph(e2,dt2, kBlue, 2, 1);  
  vgraph.push_back(vg2); labels.push_back("XeCO2: 80/20");
  dlgraph.push_back(dlg2);
  dtgraph.push_back(dtg2);

  vector<Double_t> e3, v3, dl3, dt3, alpha3;
  GetData("Ar_80.000000_C4H10_20.000000_0.000000T_ang90.000000.dat",e3, v3, dl3, dt3, alpha3);
  //for(Int_t i = 0 ; i <  e0.size(); i++) cout << e0[i] << endl;
  TGraph *vg3 = GetGraph(e3,v3, kGreen, 2, 2);  
  TGraph *dlg3 = GetGraph(e3,dl3, kGreen, 2, 2);  
  TGraph *dtg3 = GetGraph(e3,dt3, kGreen, 2, 2);  
  vgraph.push_back(vg3); labels.push_back("ArC4H10: 80/20");
  dlgraph.push_back(dlg3); 
  dtgraph.push_back(dtg3); 

  vector<Double_t> e4, v4, dl4, dt4, alpha4;
  GetData("Kr_80.000000_CO2_20.000000_0.000000T_ang90.000000.dat",e4, v4, dl4, dt4, alpha4);
  TGraph *vg4 = GetGraph(e4,v4, kRed, 2, 2);  
  TGraph *dlg4 = GetGraph(e4,dl4, kRed, 2, 2);  
  TGraph *dtg4 = GetGraph(e4,dt4, kRed, 2, 2);  
  vgraph.push_back(vg4); labels.push_back("KrC4H10: 80/20");
  dlgraph.push_back(dlg4);
  dtgraph.push_back(dtg4);

  vector<Double_t> e5, v5, dl5, dt5, alpha5;
  GetData("Xe_80.000000_C4H10_20.000000_0.000000T_ang90.000000.dat",e5, v5, dl5, dt5, alpha5);
  TGraph *vg5 = GetGraph(e5,v5, kBlue, 2, 2);  
  TGraph *dlg5 = GetGraph(e5,dl5, kBlue, 2, 2);  
  TGraph *dtg5 = GetGraph(e5,dt5, kBlue, 2, 2);  
  vgraph.push_back(vg5); labels.push_back("XeC4H10: 80/20");
  dlgraph.push_back(dlg5);
  dtgraph.push_back(dtg5);

  //Plot curves
  TMultiGraph *mg1 = new TMultiGraph();
    mg1->SetTitle("");
  TLegend *leg1 = new TLegend();
  TCanvas *c1 = new TCanvas("c1","drift vel");
    c1->cd(1);  
  for(Int_t j = 0; j < vgraph.size(); j++)
    { 
      mg1->Add(vgraph[j],"l"); leg1->AddEntry(vgraph[j],labels[j],"l");
    }
  mg1->Draw("ac");
  mg1->SetTitle("");
  mg1->GetXaxis()->SetTitle("E (V/cm)");
  mg1->GetYaxis()->SetTitle("Drift Veleocity (cm/#mus)");
  leg1->Draw("same");

  //Long. Diff.
  TMultiGraph *mg2 = new TMultiGraph();
    mg2->SetTitle("");
  TCanvas *c2 = new TCanvas("c2","long diff");
    c2->cd(1);  
  for(Int_t j = 0; j < dlgraph.size(); j++)
    { 
      mg2->Add(dlgraph[j],"l");
    }
  mg2->Draw("ac");
  mg2->SetTitle("");
  mg2->GetXaxis()->SetTitle("E (V/cm)");
  mg2->GetYaxis()->SetTitle("Longitudinal Diffusion Coef. (#sqrt{cm})");
  leg1->Draw("same");

  //Trans. Diff.
  TMultiGraph *mg3 = new TMultiGraph();
    mg3->SetTitle("");
  TCanvas *c3 = new TCanvas("c3","trans diff");
    c3->cd(1);  
  for(Int_t j = 0; j < dtgraph.size(); j++)
    { 
      mg3->Add(dtgraph[j],"l");
    }
  mg3->Draw("ac");
  mg3->SetTitle("");
  mg3->GetXaxis()->SetTitle("E (V/cm)");
  mg3->GetYaxis()->SetTitle("Transverse Diffusion Coef. (#sqrt{cm})");
  leg1->Draw("same");
}


Int_t GetData(TString file, vector<Double_t> &e, vector<Double_t> &v, vector<Double_t> &dl, vector<Double_t> &dt, vector<Double_t> &alpha)
{
    Double_t ie, iv, idl, idt ,ialpha;
    TString path = "/home/mposik/MPGD_Sim/Gas/GasFile/PlotTables/";
    fstream INFILE;
    INFILE.open(path+file,ios::in);
    if(!INFILE)
     {
       cout << "GetData: Can't find file: " << file <<endl;
       return -1;
     }
    while(INFILE >> ie  >> iv  >> idl >> idt >> ialpha)
     {
	e.push_back(ie); 
	v.push_back(iv);
	dl.push_back(idl);
	dt.push_back(idt);
	alpha.push_back(ialpha);
        cout << Form("%f, %f, %f, %f, %f\n", ie, iv, idl, idt, ialpha); 
     }
   INFILE.close();
   //delete INFILE;
  return 1;
}

TGraph *GetGraph(vector<Double_t> x, vector<Double_t> y, Int_t lc = kBlack, Int_t lw = 2, Int_t ls = 1)
{
  TGraph *g = new TGraph(x.size(), &x[0], &y[0]);
    g->SetLineColor(lc);
    g->SetLineStyle(ls);
    g->SetLineWidth(lw);

  return g;
}

