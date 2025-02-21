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
    create_histogram(eventi);
    print_all_data(eventi);
    plot_3D_distribution(eventi);
    plot_zenith_vs_inPos(eventi);
    int bundle = muon_bundle(eventi);
    int nevents = Nevents(eventi);
    cout << "Il numero di eventi singoli registrati è: " << nevents << endl;
    cout << "Il nuemero di muoni bundle (muoni per traccia > 2) sono: " << bundle << " e rappresentano il " << (double) bundle/nevents*100 << "%." << endl;
    app.Run();
    return 0;
}