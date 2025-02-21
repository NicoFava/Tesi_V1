#ifndef FUNZIONI_H
#define FUNZIONI_H

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>
#include <numeric>

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
    double distance;
};

// Funzione per caricare i dati ROOT in un vettore
vector<muone> load_root_data(const string&);

// Funzione per stampare tutti i dati
void print_all_data(const vector<muone>&);

// Funzione per salvare i dati in un file
void save_all_data_to_file(const vector<muone>&, const string&);

// Funzione per creare un istogramma della carica
void create_PeSum_histogram(const vector<muone>&);

// Funzione per creare plot tridimensionale dei punti di entrata
void plot_3D_distribution(const vector<muone>&);

// Funzione per creare istrogramma Theta vs Punti di entrata
void plot_zenith_vs_inPos(const vector<muone>&);

// Funzione per calcolare il numero di bundle di muoni
int muon_bundle(const vector<muone>&);

// Funzione per contare il numero di eventi singolarmente
int Nevents(const vector<muone>&);

// Funzione per calcolare il tempo medio tra un evento e il successivo (inverso della frequenza)
float mean_delta_t(const vector<muone>&);
#endif