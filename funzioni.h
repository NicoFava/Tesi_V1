#ifndef FUNZIONI_H
#define FUNZIONI_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>

// Librerie di ROOT
#include "TH1F.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TPaveStats.h"
#include "TAxis.h"
#include "TH2.h"
#include "TF1.h"
#include "TGraph2D.h"
#include "TStyle.h"
#include "TColor.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;

struct muone {
    int eventID;
    int trackID;
    int fSec;
    int fNanosec;
    float PeSum;
    float entry_x, entry_y, entry_z;
    float exit_x, exit_y, exit_z;
    float ux, uy, uz;
    float quality;
    float entry_time, exit_time;
};

// Funzione per caricare i dati ROOT in un vettore
vector<muone> load_root_data(const string&);

// Funzione per Stampare tutti i dati
void print_all_data(const vector<muone>&);

// Funzione per creare un istogramma ROOT
void create_histogram(const vector<muone>&);
// Funzione per creare plot tridimensionale dei punti di entrata
void plot_3D_distribution(const vector<muone>&);

// Funzione per creare istrogramma Theta vs Punti di entrata
void plot_zenith_vs_inPos(const vector<muone>&);

#endif