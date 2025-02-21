#include "funzioni.h"

// Prendo i dati dal TTree e li inserisco in un vector di muone
vector<muone> load_root_data(const string& filename) {
    vector<muone> eventi;
    
    TFile *file = TFile::Open(filename.c_str(), "READ");
    if (!file) {
        cerr << "Errore: impossibile aprire il file ROOT!" << endl;
        return eventi;
    }

    TTree *tree = (TTree*)file->Get("MuonEvents");
    if (!tree) {
        cerr << "Errore: TTree 'MuonEvents' non trovato!" << endl;
        file->Close();
        return eventi;
    }

    muone evento;
    tree->SetBranchAddress("EvtID", &evento.eventID);
    tree->SetBranchAddress("TrackNumber", &evento.trackID);
    tree->SetBranchAddress("fSec", &evento.fSec);
    tree->SetBranchAddress("fNanoSec", &evento.fNanosec);
    tree->SetBranchAddress("PeSum", &evento.PeSum);
    tree->SetBranchAddress("inPos._0", &evento.entry_x);
    tree->SetBranchAddress("inPos._1", &evento.entry_y);
    tree->SetBranchAddress("inPos._2", &evento.entry_z);
    tree->SetBranchAddress("outPos._0", &evento.exit_x);
    tree->SetBranchAddress("outPos._1", &evento.exit_y);
    tree->SetBranchAddress("outPos._2", &evento.exit_z);
    tree->SetBranchAddress("Direction._0", &evento.ux);
    tree->SetBranchAddress("Direction._1", &evento.uy);
    tree->SetBranchAddress("Direction._2", &evento.uz);
    tree->SetBranchAddress("PosQuality", &evento.quality);
    tree->SetBranchAddress("InTime", &evento.entry_time);
    tree->SetBranchAddress("OutTime", &evento.exit_time);

    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; i++) {
        tree->GetEntry(i);
        eventi.push_back(evento);
    }

    file->Close();
    return eventi;
}

void print_all_data(const vector<muone>& eventi) {
    cout << "============================================" << endl;
    cout << " STAMPA COMPLETA DI TUTTI GLI EVENTI " << endl;
    cout << "============================================" << endl;

    for (const auto& ev : eventi) {
        cout << "Evento ID: " << ev.eventID << endl;
        cout << "Traccia: " << ev.trackID << endl;
        cout << "Tempo Secondi: " << ev.fSec << " | Nanosecondi: " << ev.fNanosec << endl;
        cout << "Energia PeSum: " << ev.PeSum << endl;
        cout << "Ingresso (x,y,z): (" << ev.entry_x << ", " << ev.entry_y << ", " << ev.entry_z << ")" << endl;
        cout << "Uscita (x,y,z): (" << ev.exit_x << ", " << ev.exit_y << ", " << ev.exit_z << ")" << endl;
        cout << "Direzione (ux,uy,uz): (" << ev.ux << ", " << ev.uy << ", " << ev.uz << ")" << endl;
        cout << "Qualità Posizione: " << ev.quality << endl;
        cout << "Tempo Entrata: " << ev.entry_time << " | Tempo Uscita: " << ev.exit_time << endl;
        cout << "--------------------------------------------" << endl;
    }
}

void create_histogram(const vector<muone>& eventi) {
    TCanvas *canvas = new TCanvas("canvas", "Istogramma Energia", 800, 600);
    TH1F *charge = new TH1F("histo", "Distribuzione Energia", 100, 0, 1000);
    charge->StatOverflows(kTRUE);
    for (const auto& ev : eventi) {
        charge->Fill(ev.PeSum);
    }
    charge->GetXaxis()->SetTitle("Charge [p.e.]");
    charge->GetYaxis()->SetTitle("Counts [a.u.]");
    charge->Draw();
    canvas->SaveAs("PeSum_plot.png");
}

void plot_3D_distribution(const vector<muone>& eventi){
    TCanvas *c3D = new TCanvas("c3D", "Distribuzione 3D dei punti di entrata", 800, 800);
    TGraph2D *g3D = new TGraph2D();

    for (const auto &ev : eventi) {
        double x = ev.entry_x;
        double y = ev.entry_y;
        double z = ev.entry_z;
        g3D->SetPoint(g3D->GetN(), x, y, z);
    }

    // Stile e visualizzazione
    gStyle->SetPalette(kRainBow);
    gStyle->SetOptStat(0);
    g3D->SetTitle("Distribuzione 3D dei punti di entrata");
    g3D->SetMarkerStyle(21);
    g3D->SetMarkerSize(1.1);
    g3D->Draw("PCOL");

    // Titoli assi
    g3D->GetXaxis()->SetTitle("X [mm]");
    g3D->GetYaxis()->SetTitle("Y [mm]");
    g3D->GetZaxis()->SetTitle("Z [mm]");

    c3D->Update();
}

void plot_zenith_vs_inPos(const vector<muone>& eventi){
    TCanvas *ctheta = new TCanvas("ctheta", "Distribuzione dei punti di entrata vs Angolo di Zenith", 1000, 1000);
    TH1F *zenith = new TH1F("histoz", "Distribuzione Angolare", 100, 100, 100);
    zenith->StatOverflows(kTRUE);
    for(const auto& e:eventi){
        double r = sqrt(e.entry_x*e.entry_x+e.entry_y*e.entry_y+e.entry_z*e.entry_z);
        if(r == 0) continue;

        double theta = acos(e.entry_z/r)*180.0/M_PI;
        zenith->Fill(theta);
    }
    zenith->SetTitle("#Theta vs inPos");
    zenith->SetLineColor(kBlue);
    zenith->SetLineWidth(2);
    zenith->SetFillColorAlpha(kBlue, 0.3);
    
    zenith->GetXaxis()->SetTitle("#theta[#circ]");
    zenith->GetYaxis()->SetTitle("Counts [a.u.]");
    zenith->Draw();
    ctheta->SaveAs("theta_vs_inPos.png");
}

int muon_bundle(const vector<muone>& eventi){
    int bundle = 0;
    string last_entry_time = "";
    for(const auto& e : eventi){
        if(e.trackID > 0 && e.entry_time!= last_entry_time){
            last_entry_time = e.entry_time;
            bundle++;// Conto il bundle una sola volta per tempo
        }
    }
    return bundle;
}

int Nevents(const vector<muone>& eventi){
    int nevents = 0;
    string last_entry_time = "";
    for(const auto& e : eventi){
        if(e.entry_time != last_entry_time){
            last_entry_time = e.entry_time;
            nevents++;
        }
    }
    return nevents;
}


