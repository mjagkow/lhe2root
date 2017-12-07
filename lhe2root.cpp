#include "pugixml/src/pugixml.hpp"
#include <iostream>
#include <sys/stat.h>
#include <string>
#include <sstream>
#include <cstring>
#include <chrono>


#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"

using namespace std;


struct Event {
    const static size_t MAXSIZE = 16;

    Int_t NUP;
    Int_t IDPRUP;
    Float_t XWGTUP;
    Float_t SCALUP;
    Float_t AQEDUP;
    Float_t AQCDUP;

    Int_t IDUP[MAXSIZE];
    Int_t ISTUP[MAXSIZE];
    Int_t MOTHUP_1[MAXSIZE];
    Int_t MOTHUP_2[MAXSIZE];
    Int_t ICOLUP_1[MAXSIZE];
    Int_t ICOLUP_2[MAXSIZE];
    Float_t PUP_1[MAXSIZE];
    Float_t PUP_2[MAXSIZE];
    Float_t PUP_3[MAXSIZE];
    Float_t PUP_4[MAXSIZE];
    Float_t PUP_5[MAXSIZE];
    Float_t VTIMUP[MAXSIZE];
    Float_t SPINUP[MAXSIZE];

    TLorentzVector *P4;

    void print() {
        cout <<   "  n_part  : " << NUP << "\n      ID  : " << IDPRUP << "\n  weight  : " << XWGTUP << "\n   scale  : " << SCALUP << "\n   a_QED  : " << AQEDUP << "\n   a_QCD  : " << AQCDUP << '\n';
        cout << "\n  PDG ID[]: "; for (int i = 0; i < NUP; i ++) cout << IDUP[i] << '\t';
        cout << "\n  status[]: "; for (int i = 0; i < NUP; i ++) cout << ISTUP[i] << '\t';
        cout << "\nmother 1[]: "; for (int i = 0; i < NUP; i ++) cout << MOTHUP_1[i] << '\t';
        cout << "\nmother 2[]: "; for (int i = 0; i < NUP; i ++) cout << MOTHUP_2[i] << '\t';
        cout << "\ncolour 1[]: "; for (int i = 0; i < NUP; i ++) cout << ICOLUP_1[i] << '\t';
        cout << "\ncolour 2[]: "; for (int i = 0; i < NUP; i ++) cout << ICOLUP_2[i] << '\t';
        cout << "\n      px[]: "; for (int i = 0; i < NUP; i ++) cout << PUP_1[i] << '\t';
        cout << "\n      py[]: "; for (int i = 0; i < NUP; i ++) cout << PUP_2[i] << '\t';
        cout << "\n      pz[]: "; for (int i = 0; i < NUP; i ++) cout << PUP_3[i] << '\t';
        cout << "\n  energy[]: "; for (int i = 0; i < NUP; i ++) cout << PUP_4[i] << '\t';
        cout << "\n    mass[]: "; for (int i = 0; i < NUP; i ++) cout << PUP_5[i] << '\t';
        cout << "\nlifetime[]: "; for (int i = 0; i < NUP; i ++) cout << VTIMUP[i] << '\t';
        cout << "\n    spin[]: "; for (int i = 0; i < NUP; i ++) cout << SPINUP[i] << '\t';
        cout << endl;
    }

    void clear() {
        TLorentzVector *tmp = P4;
        memset(this, 0, sizeof(Event));
        P4 = tmp;
    }

    Event() {
        P4 = new TLorentzVector[MAXSIZE];
    }

    ~Event() {
        delete[] P4;
    }

    Event(const Event &r) {
        memcpy(
        P4 = new TLorentzVector[MAXSIZE];
        P4
    }
};


void print_usage() {
    cout << "Usage:\n"
            "  lhe2root <input.txt> [output.root]" << endl;
}

int main(int argc, char **argv) {

    if (argc < 2) {
        cout << "Too few parameters, please specify LHE file path.\n";
        print_usage();
        return 1;
    }



    {
        struct stat buffer;   
        if ( stat(argv[1], &buffer) != 0 ) {
            cout << "Error: cannot open file '" << argv[1] << "', check path and permissions.\n";
            return 1;
        }
    }



    string outputFileName;
    if (argc >= 3)
        outputFileName = argv[2];
    else {
        outputFileName = argv[1];
        auto pos = outputFileName.find_last_of('.');
        if (pos == string::npos) 
            outputFileName += ".root";
        else {
            auto slashPos = outputFileName.find_last_of('/');
            if (slashPos == string::npos)
                slashPos = 0;
            else
                slashPos ++;
            outputFileName = outputFileName.substr(slashPos, pos - slashPos) + ".root";
        }
    }


    cout << "Loading LHE file... " << flush;
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(argv[1]);
    if (result.status != pugi::xml_parse_status::status_ok) {
        cout << "XML parsing failed: " << result.description() << endl;
    }
    cout << "Done" << endl;


    
    TFile *f = new TFile(outputFileName.c_str(), "recreate");
    TTree *t = new TTree("LHE", "LHE event data");
    Event e;
    t->Branch("n_part", &e.NUP, "n_part/I");
    t->Branch("procID", &e.IDPRUP, "procID/I");
    t->Branch("weight", &e.XWGTUP, "weight/F");
    t->Branch("scale", &e.SCALUP, "scale/F");
    t->Branch("a_QED", &e.AQEDUP, "a_QED/F");
    t->Branch("a_QCD", &e.AQCDUP, "a_QCD/F");
    t->Branch("PDGID", e.IDUP, "PDGID[n_part]/I");
    t->Branch("status", e.ISTUP, "status[n_part]/I");
    t->Branch("mother_1", e.MOTHUP_1, "mother_1[n_part]/I");
    t->Branch("mother_2", e.MOTHUP_2, "mother_2[n_part]/I");
    t->Branch("colour_1", e.ICOLUP_1, "colour_1[n_part]/I");
    t->Branch("colour_2", e.ICOLUP_2, "colour_2[n_part]/I");
    t->Branch("p4", &e.P4, "p4[n_part]/TLorentzVector");
    t->Branch("px", e.PUP_1, "px[n_part]/F");
    t->Branch("py", e.PUP_2, "py[n_part]/F");
    t->Branch("pz", e.PUP_3, "pz[n_part]/F");
    t->Branch("energy", e.PUP_4, "energy[n_part]/F");
    t->Branch("mass", e.PUP_5, "mass[n_part]/F");
    t->Branch("lifetime", e.VTIMUP, "lifetime[n_part]/F");
    t->Branch("spin", e.SPINUP, "spin[n_part]/F");


    auto root = doc.first_child();
    if (string(root.name()) != "LesHouchesEvents") {
        cout << "root name: " << root.name() << '\n';
        cout << "Error: incorrect XML format, missing root \"LesHouchesEvents\"" << endl;
        return 1;
    }


    int c = 0;
    istringstream iss;
    pugi::xpath_node_set events = doc.select_nodes("/LesHouchesEvents/event");
    for (const auto &node : events) {
        static auto last_time = chrono::steady_clock::now();

        e.clear();

        auto text = stringstream(node.node().child_value());
        string line;

        // Omit first line as it only contains whitespaces
        std::getline(text, line, '\n');


        // Read compulsory event data in format
        // NUP    -- number of particles
        // IDPRUP -- UNKNOWN (process ID?)
        // XWGTUP -- event weight
        // SCALUP -- scale
        // AQEDUP -- QED constant \alpha_{em}
        // AQCDUP -- QCD constant \alpha_{s}
        std::getline(text, line, '\n');
        iss.clear();
        iss.str(line);
        iss >> e.NUP >> e.IDPRUP >> e.XWGTUP >> e.SCALUP >> e.AQEDUP >> e.AQCDUP;


        if (e.NUP > Event::MAXSIZE) {
            e.NUP = Event::MAXSIZE;

            static int eventSizeWarningCounter = 0;

            if (eventSizeWarningCounter < 3) {
                cout << "Warning: too many particles in the event: " << e.NUP << 
                      "\n         Event will be truncated. Consider changing Event::MAXSIZE constant." << endl;
                eventSizeWarningCounter ++;
                if (eventSizeWarningCounter == 3) {
                    cout << "         This warning will not show up any more." << endl;
                }
            }
        }

        // Read optional particle data
        // IDUP   -- PDG ID
        // ISTUP  -- status (guessed: -1 for initial state, 1 for final state)
        // MOTHUP -- mothers(1-2)
        // ICOLUP -- colours(1-2)
        // PUP    -- 3-momentum(1-3), energy(4) and mass(5)
        // VTIMUP -- life time, may be not used
        // SPINUP -- spin (9 for unknown)
        for (int i = 0; i < e.NUP; i ++) {
            std::getline(text, line, '\n');
            iss.clear();
            iss.str(line);
            iss >> e.IDUP[i] >> e.ISTUP[i] >> e.MOTHUP_1[i] >> e.MOTHUP_2[i] >> e.ICOLUP_1[i] >> e.ICOLUP_2[i];
            iss >> e.PUP_1[i] >> e.PUP_2[i] >> e.PUP_3[i] >> e.PUP_4[i] >> e.PUP_5[i] >> e.VTIMUP[i] >> e.SPINUP[i];
            e.P4[i].SetPxPyPzE(e.PUP_1[i], e.PUP_2[i], e.PUP_3[i], e.PUP_4[i]);
        }
        t->Fill();
        c ++;
        
        if (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - last_time).count() > 200) {
            last_time = chrono::steady_clock::now();
            cout << "\rParsing: " << c << flush;
        }

    }
    cout << "\rParsing: " << c << "  Done" << endl;

    //cout << "Writing TTree... " << flush;
    t->Write();
    //cout << "Done" << endl << "Writing TFile... " << flush;
    f->Write();
    //cout << "Done" << endl << "Closing TFile... " << flush;
    f->Close();
    //cout << "Done" << endl << "Deleting TFile... " << flush;
    delete f;
    //cout << "Done" << endl;


    return 0;
}
