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
        evento.distance = sqrt(pow(evento.exit_x - evento.entry_x, 2) + pow(evento.exit_y - evento.entry_y, 2) + pow(evento.exit_z - evento.entry_z, 2));
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
        cout << "Distanza percorsa: "<< ev.distance << " mm" << endl;
        cout << "--------------------------------------------" << endl;
    }
}

void save_all_data_to_file(const vector<muone>& eventi, const string& filename = "output.txt"){
    ofstream file(filename);
    if (!file) {
        cerr << "Errore: impossibile aprire il file " << filename << " per la scrittura!" << endl;
        return;
    }

    file << "============================================\n";
    file << " STAMPA COMPLETA DI TUTTI GLI EVENTI \n";
    file << "============================================\n";

    for (const auto& ev : eventi) {
        file << "Evento ID: " << ev.eventID << "\n";
        file << "Traccia: " << ev.trackID << "\n";
        file << "Tempo Secondi: " << ev.fSec << " | Nanosecondi: " << ev.fNanosec << "\n";
        file << "Energia PeSum: " << ev.PeSum << "\n";
        file << "Ingresso (x,y,z): (" << ev.entry_x << ", " << ev.entry_y << ", " << ev.entry_z << ")\n";
        file << "Uscita (x,y,z): (" << ev.exit_x << ", " << ev.exit_y << ", " << ev.exit_z << ")\n";
        file << "Direzione (ux,uy,uz): (" << ev.ux << ", " << ev.uy << ", " << ev.uz << ")\n";
        file << "Qualità Posizione: " << ev.quality << "\n";
        file << "Tempo Entrata: " << ev.entry_time << " | Tempo Uscita: " << ev.exit_time << "\n";
        file << "Distanza percorsa: " << ev.distance << " mm\n";
        file << "--------------------------------------------\n";
    }

    file.close();
    cout << "Dati salvati con successo in " << filename << "!" << endl;
}

void create_PeSum_histogram(const vector<muone>& eventi) {
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
    TCanvas *c3D = new TCanvas("c3D", "Distribuzione 3D dei punti di entrata", 800, 600);
    TGraph2D *g3D = new TGraph2D();

    for (const auto &ev : eventi) {
        double x = ev.entry_x;
        double y = ev.entry_y;
        double z = ev.entry_z;
        g3D->SetPoint(g3D->GetN(), x, y, z);
    }

    gStyle->SetPalette(kRainBow);
    gStyle->SetOptStat(0);
    g3D->SetTitle("Distribuzione 3D dei punti di entrata");
    g3D->SetMarkerStyle(21);
    g3D->SetMarkerSize(1.1);
    g3D->Draw("PCOL");

    g3D->GetXaxis()->SetTitle("X [mm]");
    g3D->GetYaxis()->SetTitle("Y [mm]");
    g3D->GetZaxis()->SetTitle("Z [mm]");

    c3D->Update();
}

void plot_zenith_vs_inPos(const vector<muone>& eventi){
    TCanvas *ctheta = new TCanvas("ctheta", "Distribuzione dei punti di entrata vs Angolo di Zenith", 800, 600);
    TH1F *zenith = new TH1F("histoz", "Distribuzione Angolare", 100, 100, 100);
    zenith->StatOverflows(kTRUE);
    ctheta->SetGrid();
    for(const auto& e:eventi){
        double r = sqrt(e.entry_x*e.entry_x+e.entry_y*e.entry_y+e.entry_z*e.entry_z);
        if(r == 0) continue;

        double theta = acos(-e.uz); // Messo negativo in sono "verso il basso"
        zenith->Fill(cos(theta));
    }
    zenith->SetTitle("cos(#theta) vs inPos");
    zenith->SetLineColor(kBlue);
    zenith->SetLineWidth(2);
    zenith->SetFillColorAlpha(kBlue, 0.3);
    
    zenith->GetXaxis()->SetTitle("cos(#theta)");
    zenith->GetYaxis()->SetTitle("Counts [a.u.]");
    zenith->Draw("HIST");
    ctheta->SaveAs("theta_vs_inPos.png");
}

int muon_bundle(const vector<muone>& eventi){
    int bundle = 0;
    int last_entry_time = 0;
    for(const auto& e : eventi){
        if(e.trackID > 0 && e.fSec+e.fNanosec!= last_entry_time){
            last_entry_time = e.fSec+e.fNanosec;
            bundle++;// Conto il bundle una sola volta per tempo
        }
    }
    return bundle;
}

int Nevents(const vector<muone>& eventi){
    int nevents = 0;
    int last_entry_time = 0;
    for(const auto& e : eventi){
        if(e.fSec+e.fNanosec != last_entry_time){
            last_entry_time = e.fSec+e.fNanosec;
            nevents++;
        }
    }
    return nevents;
}

float mean_delta_t(const vector<muone>& eventi){
    double mean_delta_t_= 0;
    int last_entry_time = 0;
    vector<float> intervalli;
    for (size_t i = 1; i < eventi.size(); i++) {
        if(eventi[i].fSec+eventi[i].fNanosec!=last_entry_time){
            // Converti il tempo in nanosecondi e calcola la differenza
            float t1 = eventi[i - 1].fSec * 1e9 + eventi[i - 1].fNanosec;
            float t2 = eventi[i].fSec * 1e9 + eventi[i].fNanosec;
            float delta_t = t2 - t1;

            // Se l'intervallo è negativo, c'è un errore nell'ordine degli eventi
            if (delta_t < 0) {
                cerr << "Attenzione: Intervallo di tempo negativo rilevato tra due eventi!" << endl;
                cout << "aaaa" <<  eventi[i].eventID << endl;
                cerr << delta_t << endl;
            } else {
                intervalli.push_back(delta_t);
            }
        }
    }

    // Calcola la media
    double somma = accumulate(intervalli.begin(), intervalli.end(), 0.0);
    double media = somma / intervalli.size();

    return media;
}
