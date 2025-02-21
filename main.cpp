#include "funzioni.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <file.root>" << endl;
        return 1;
    }
    TApplication app("app", 0, 0);
    string filename = argv[1];
    vector<muone> eventi = load_root_data(filename);

    if (eventi.empty()) {
        cerr << "Errore: Nessun evento caricato!" << endl;
        return 1;
    }
    create_PeSum_histogram(eventi);
    print_all_data(eventi);
    save_all_data_to_file(eventi, "output.txt");
    plot_3D_distribution(eventi);
    plot_zenith_vs_inPos(eventi);
    int bundle = muon_bundle(eventi);
    int nevents = Nevents(eventi);
    double delta_t = mean_delta_t(eventi);
    cout << "Il numero di eventi singoli registrati è: " << nevents << endl;
    cout << "Il nuemero di muoni bundle (muoni per traccia > 2) sono: " << bundle << " e rappresentano il " << (double) bundle/nevents*100 << "%." << endl;
    cout << "Il numero di tracce toatli è:" << nevents+bundle << endl;
    cout << "Il tempo medio tra un evento e un altro è: " << delta_t*1e-9 << " s" << endl;
    cout << "La frequenza dei muoni è: " << 1.0/((double)delta_t*1e-9) << " Hz" << endl;
    app.Run();
    return 0;
}