// include needed libraries for the code below
#include "./include/TofHit.h"
#include "./include/TofSignal.h"
#include "./include/TofEvent.h"
#include "./include/TofRun.h"
#include "./include/TofObjectsDict.h"

int main(int argc, char *argv[]){

    std::string software = argv[1];
    std::cout  << "Software " << software << std::endl;
    std::string run_full_path = argv[2];
    std::cout << "Run path " << run_full_path << std::endl;

    // extract run number from run_full_path
    std::string run_number_string = run_full_path.substr(run_full_path.find_last_of("/")+1);
    run_number_string = run_number_string.substr(3, run_number_string.find_last_of("."));
    std::cout << "Run number " << run_number_string << std::endl;
    int run_number = std::stoi(run_number_string);

    std::string input_file = run_full_path;
    std::cout << "reading file: " << input_file << std::endl;
    TFile *f = new TFile(input_file.c_str(), "READ");
    if (f->IsZombie()) {
        std::cerr << "Error: failed to open file " << input_file << std::endl;
        return 1;
    }
    TTree *t = (TTree*)f->Get(Form("TreeTofRun%i", run_number));
    TofRun *run = new TofRun(software);
    // t->SetBranchAddress("TofRun", &run);
    int status = t->SetBranchAddress("TofRun", &run);
    if (status != 0) {
        std::cerr << "Error: SetBranchAddress failed with status " << status << std::endl;
        return 1;
    }
    std::cout << "Number of entries: " << t->GetEntries() << std::endl;
    t->GetEntry(0); // only one run in this file

    // Select desired constant fraction to print, 1 = 10%
    int cf_index = 1;

    std::cout << "Run number: " << run->RunNumber << std::endl;
    std::cout << "Run address: " << run->RunAddress << std::endl;
    std::cout << "Number of events: " << run->RunEventsList.size() << std::endl;
    for (int i = 0; i < run->RunEventsList.size(); i++){

        std::cout << " Event " << i << std::endl;

        for (int j = 0; j < run->RunEventsList.at(i).EventHitsList.size(); j++){

            // skipping if the fit was not successful, could also use the bool HitFitSuccess
            if (run->RunEventsList.at(i).EventHitsList.at(j).HitCfTimeFromFit.size() == 0){
                std::cout << "  Hit " << j << ": unsuccesful fit, skipping.\n";
                continue;
            } 
            std::cout << "  Hit " << j << " has " << run->RunEventsList.at(i).EventHitsList.at(j).HitCfTimeFromFit.at(cf_index) << "ns time at CF ";
            std::cout << run->RunEventsList.at(i).EventHitsList.at(j).HitPeakFraction.at(cf_index);
            std::cout << ".  CfTime - Cell0Time = ";
            std::cout << run->RunEventsList.at(i).EventHitsList.at(j).HitCfTimeFromFit.at(cf_index) - run->RunEventsList.at(i).EventHitsList.at(j).HitCell0Time;
            std::cout <<  "ns" << std::endl;
        }
    }
    return 0;
}