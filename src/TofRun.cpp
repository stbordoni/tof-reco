#include "../include/TofRun.h"
#include "../include/TofEvent.h"
#include "../include/TofHit.h"
#include "../include/TofSignal.h"

// Constructor
TofRun::TofRun(std::string software, std::string run_full_path){
    
    // Save run software
    RunSoftware = software;
    if (RunSoftware == "linux"){
        std::cout << "Linux. Creating TofRun object... ";
    } else if (RunSoftware == "windows"){
        std::cout << "Windows. Creating TofRun object... ";
    } else {
        std::string this_error;
        this_error = "Wrong software input, has to be linux or windows. TofRun object not created\n";
        std::cerr << this_error << std::endl;
        RunErrorsList.push_back(this_error);
        RunPrintErrors();
        exit(1);
    }

    // Save run number and other info in the run address
    RunPath = run_full_path;
    std::vector <std::string> split_run_path = SplitString(RunPath, '/');
    std::cout << "RunPath: " << RunPath << std::endl;
    RunAddress = split_run_path.at(split_run_path.size()-1); // filename for linux, folder for windows
    std::vector <std::string> split_run_address = SplitString(RunAddress, '_');

    size_t start, end;
    start = RunAddress.find("run") + 3;
    if (start != std::string::npos) {

        std::string run_number_and_extension = RunAddress.substr(start);
        end = run_number_and_extension.find(".txt");
        if (end != std::string::npos) {
            std::cout << run_number_and_extension.substr(0, end) << std::endl;
            RunNumber = std::stoi(run_number_and_extension.substr(0, end)); // linux
        } else {
            RunNumber = std::stoi(run_number_and_extension); // windows, no extension
        }

    } else {

        std::string this_error;
        this_error = "Wrong input filename, Run number not found.\n";
        std::cerr << this_error << std::endl;
        RunErrorsList.push_back(this_error);
        RunPrintErrors();
        exit(1);

    }

    // This can be uncommented to read the date from the filename
    // Declare the following string variables in one line
    std::string run_date_day, run_date_month, run_date_year, run_date_hours, run_date_minutes;
    
    if (RunSoftware == "linux"){
        
        // in linux the format is sampic_YYYYMMDD_HHMM_runX.txt
        RunDate = std::stoi(split_run_address.at(1));
        RunTime = std::stoi(split_run_address.at(2));

    } else if (RunSoftware == "windows"){

        // in windows the format is sampic_MMDDYYYY_HHhMMm_runX  
        std::string run_date_month = split_run_address.at(1).substr(0,2);
        std::string run_date_day = split_run_address.at(1).substr(2,2);
        std::string run_date_year = split_run_address.at(1).substr(4,4);
        std::cout << run_date_year + run_date_month + run_date_day << std::endl;
        RunDate = std::stoi(run_date_year + run_date_month + run_date_day);

        std::string run_date_hours = split_run_address.at(2).substr(0,2);
        std::string run_date_minutes = split_run_address.at(2).substr(3,2);
        RunTime = std::stoi(run_date_hours+run_date_minutes);

    }

    std::cout << "Built object TofRun for run number " << RunNumber << std::endl;
}

void TofRun::RunSaveSettings (){
    
    if (RunSoftware == "linux"){
        std::cout << "Linux. Saving Run parameters... ";
        
        // hard coding for now, then there should be a settings file dumped from MIDAS
        RunNFebs = 4;
        RunNSamplesToRead = 64;
        RunSamplingFrequency = 3200; // MHz
        RunSampleLength = 1e-3 / RunSamplingFrequency; // ns
        for (int i=0; i<nFebsMax; i++) for (int j=0; j<nSampicsPerFeb; j++){
            RunBaseline[i][j] = 0.150; // V
            RunPostTrig[i][j] = 2;
        }
        for (int i=0; i<nChannels; i++) RunTrigThr[i] = 0.020;
        std::cout << "Run parameters saved.\n";
    }
    else if (RunSoftware == "windows"){

        std::cout << "Windows. Saving Run parameters... ";
        // ifstream RunInputFile(RunFile.c_str(), ios_base::in);
        std::string RunSettingsFile = RunPath + "/Run_Settings.txt";
        std::ifstream RunSettingsStream;
        RunSettingsStream.open(RunSettingsFile.c_str());
    
        std::string linedump;
        std::cout << "enter runsettingstream\n";


        while(!RunSettingsStream.eof())
        {

            getline(RunSettingsStream, linedump);

            if(linedump.find("UnixTime") != std::string::npos)
            {
            std::string RunUnixTime_stringStart = "UnixTime = ";
            std::string RunUnixTime_stringStop = " date";
            int RunUnixTime_startPosition = linedump.find(RunUnixTime_stringStart) + RunUnixTime_stringStart.length();
            int RunUnixTime_length = linedump.find(RunUnixTime_stringStop) - RunUnixTime_startPosition;
            std::string RunUnixTime_string = linedump.substr(RunUnixTime_startPosition, RunUnixTime_length);
            RunUnixTime = std::stod(RunUnixTime_string);

            }
            else if(linedump.find("Total Nb of 64-ch FE-Boards:") != std::string::npos)
            {
            std::string RunNFebs_stringStart = "FE-Boards: ";
            std::string RunNFebs_stringStop = " ===";
            int RunNFebs_startPosition = linedump.find(RunNFebs_stringStart) + RunNFebs_stringStart.length();
            int RunNFebs_length = linedump.find(RunNFebs_stringStop) - RunNFebs_startPosition;
            std::string RunNFebs_string = linedump.substr(RunNFebs_startPosition, RunNFebs_length);
            std::cout << "RunNFebs_string: " << RunNFebs_string << std::endl;
            RunNFebs = std::stoi(RunNFebs_string);
                std::cout << "RunNFebs: " << RunNFebs << std::endl;

            }
            else if(linedump.find("64-ch Front-End Board") != std::string::npos)
            {
            std::string Feb_stringStart = "64-ch Front-End Board[";
            std::string Feb_string = linedump.substr(linedump.find(Feb_stringStart) + Feb_stringStart.length(), 1);
            
            int Feb = std::stoi(Feb_string); // not needed
            
            std::string RunFebSerialNumber_stringStart = "SerNum: ";
            std::string RunFebSerialNumber_stringStop = ", Ctrl-FPGA";
            int RunFebSerialNumber_startPosition = linedump.find(RunFebSerialNumber_stringStart) + RunFebSerialNumber_stringStart.length();
            int RunFebSerialNumber_length = linedump.find(RunFebSerialNumber_stringStop) - RunFebSerialNumber_startPosition;
            std::string RunFebSerialNumber_string = linedump.substr(RunFebSerialNumber_startPosition, RunFebSerialNumber_length);
            RunFebSerialNumber.push_back(RunFebSerialNumber_string);
            }
            else if(linedump.find("SamplingFrequency:") != std::string::npos)
            {
            std::string RunSamplingFrequency_stringStart = "SamplingFrequency: ";
            std::string RunSamplingFrequency_stringStop = " MS/s";
            int RunSamplingFrequency_startPosition = linedump.find(RunSamplingFrequency_stringStart) + RunSamplingFrequency_stringStart.length();
            int RunSamplingFrequency_length = linedump.find(RunSamplingFrequency_stringStop) - RunSamplingFrequency_startPosition;
            std::string RunSamplingFrequency_string = linedump.substr(RunSamplingFrequency_startPosition, RunSamplingFrequency_length);
            std::cout << "RunSamplingFrequency_string: " << RunSamplingFrequency_string << std::endl;
            RunSamplingFrequency = std::stoi(RunSamplingFrequency_string);
            RunSampleLength = 1e3 / RunSamplingFrequency; // ns
            std::cout << "RunSamplingFrequency: " << RunSamplingFrequency << std::endl;
                std::cout << "RunSampleLength: " << RunSampleLength << std::endl;
            }
            else if(linedump.find("Baseline:") != std::string::npos)
            {
            std::string Feb_stringStart = "FE-BOARD[";
            std::string Feb_string = linedump.substr(linedump.find(Feb_stringStart) + Feb_stringStart.length(), 1);

            std::string Sampic_stringStart = "SAMPIC[";
            std::string Sampic_string = linedump.substr(linedump.find(Sampic_stringStart) + Sampic_stringStart.length(), 1);

            std::string Baseline_stringStart = "Baseline: ";
            std::string Baseline_stringV = linedump.substr(linedump.find(Baseline_stringStart) + Baseline_stringStart.length(), 1);
            std::string Baseline_stringmV = linedump.substr(linedump.find(Baseline_stringStart) + Baseline_stringStart.length() + 2, 3);

            int Feb = std::stoi(Feb_string);

            int Sampic = std::stoi(Sampic_string);

            double Baseline = std::stoi(Baseline_stringV) + 1E-3*std::stoi(Baseline_stringmV);

            RunBaseline[Feb][Sampic] = Baseline;

            }
            else if(linedump.find("==== FE-BOARD") != std::string::npos && linedump.find("SAMPIC") != std::string::npos && linedump.find("CHANNEL") != std::string::npos)
            {
                std::string Feb_stringStart = "FE-BOARD[";
                std::string Feb_string = linedump.substr(linedump.find(Feb_stringStart) + Feb_stringStart.length(), 1);

                std::string Sampic_stringStart = "SAMPIC[";
                std::string Sampic_string = linedump.substr(linedump.find(Sampic_stringStart) + Sampic_stringStart.length(), 1);

                std::string Channel_stringStart = "CHANNEL[";
                std::string Channel_string = linedump.substr(linedump.find(Channel_stringStart) + Channel_stringStart.length(), 1);

                std::string Enabled_stringStart = "Enabled: ";
                std::string Enabled_string = linedump.substr(linedump.find(Enabled_stringStart) + Enabled_stringStart.length(), 1);

                std::string TriggerMode_stringStart = "TriggerMode: '";
                std::string TriggerMode_stringEnd = "'";
                std::string TriggerMode_string = linedump.substr(linedump.find(TriggerMode_stringStart) + TriggerMode_stringStart.length(),
                                                                linedump.find(TriggerMode_stringEnd) - linedump.find(TriggerMode_stringStart) - TriggerMode_stringStart.length());

                std::string EnabledForCentralTrigger_stringStart = "EnabledForCentralTrigger: ";
                std::string EnabledForCentralTrigger_string = linedump.substr(linedump.find(EnabledForCentralTrigger_stringStart) + EnabledForCentralTrigger_stringStart.length(), 1);

                int length_of_stringV = 2;
                std::string InternalTriggerTreshold_stringStart = "InternalTriggerTreshold: ";
                std::string InternalTriggerTreshold_stringV = linedump.substr(linedump.find(InternalTriggerTreshold_stringStart) + InternalTriggerTreshold_stringStart.length(), 1);
                // if the value is negative
                if (InternalTriggerTreshold_stringV == "-")
                {
                // then add another digit
                    InternalTriggerTreshold_stringV = linedump.substr(linedump.find(InternalTriggerTreshold_stringStart) + InternalTriggerTreshold_stringStart.length(), 2);
                    length_of_stringV++;
                }
                std::string InternalTriggerTreshold_stringmV = linedump.substr(linedump.find(InternalTriggerTreshold_stringStart) + InternalTriggerTreshold_stringStart.length() + length_of_stringV, 3);

                // std::cout << "Feb_string: " << Feb_string << std::endl;
                // std::cout << "Sampic_string: " << Sampic_string << std::endl;
                // std::cout << "Channel_string: " << Channel_string << std::endl;
                // std::cout << "Enabled_string: " << Enabled_string << std::endl;
                // std::cout << "TriggerMode_string: " << TriggerMode_string << std::endl;
                // std::cout << "EnabledForCentralTrigger_string: " << EnabledForCentralTrigger_string << std::endl;
                // std::cout << "InternalTriggerTreshold_stringV: " << InternalTriggerTreshold_stringV << std::endl;
                // std::cout << "InternalTriggerTreshold_stringmV: " << InternalTriggerTreshold_stringmV << std::endl;

                int feb = std::stoi(Feb_string);
                int sampic = std::stoi(Sampic_string);
                int channel = std::stoi(Channel_string);
                // int enabled = std::stoi(Enabled_string);
                int enabled_for_central_trigger = std::stoi(EnabledForCentralTrigger_string);
                double InternalTriggerTreshold_string = std::stoi(InternalTriggerTreshold_stringV) + 1E-3*std::stoi(InternalTriggerTreshold_stringmV);

                RunTrigThr [feb*nChannelsPerFeb + sampic*nChannelsPerSampic + channel] = InternalTriggerTreshold_string;
                // std::cout << "channel " << feb*RunNFebs*nChannelsPerFeb + sampic*nChannelsPerSampic + channel << RunTrigThr[feb*RunNFebs*nChannelsPerFeb + sampic*nChannelsPerSampic + channel] << std::endl;

                std::string trigger_mode = TriggerMode_string;
                
                // print 
                // std::cout << "Feb: " << feb << std::endl;
                // std::cout << "Sampic: " << sampic << std::endl;
                // std::cout << "Channel: " << channel << std::endl;
                // std::cout << "Enabled: " << enabled_for_central_trigger << std::endl;
                // std::cout << "TriggerMode: " << trigger_mode << std::endl;
                // std::cout << "InternalTriggerTreshold: " << InternalTriggerTreshold_string << std::endl;




            }

            RunNSamplesToRead = 62;
        }
    }
    std::cout << "Run parameters saved.\n";

}


void TofRun::RunQualityCheck(){

    if (RunNFebs != nFebsMax){
        std::string this_error;
        this_error = "Instead of 4 Febs, we have "+std::to_string(RunNFebs)+" Febs.";
        std::cerr << this_error;
        RunErrorsList.push_back(this_error);
    }
    if (RunSamplingFrequency != 3200){
        std::string this_error;
        this_error = "Sampling frequency is set to "+std::to_string(RunSamplingFrequency)+" MHz instead of 3200 MHz.";
        std::cerr << this_error;
        RunErrorsList.push_back(this_error);    
    }  

    if (RunErrorsList.size() == 0) std::cout << "No errors were found when generating the TofRun object\n";
    else{
        for (int i = 0; i < RunErrorsList.size(); i++){
            std::cout << RunErrorsList.at(i) << std::endl;
        }
    }

}

void TofRun::RunGetInfo (){
    std::cout << "nFebsMax: " << nFebsMax << std::endl;
    std::cout << "nSampicsPerFeb: " << nSampicsPerFeb << std::endl;
    std::cout << "nChannelsPerSampic: " << nChannelsPerSampic << std::endl;
    std::cout << "nChannelsPerFeb: " << nChannelsPerFeb << std::endl;
    std::cout << "nChannels: " << nChannels << std::endl;

    std::cout << "RunSoftware: " << RunSoftware << std::endl;
    std::cout << "RunPath: " << RunPath << std::endl;
    std::cout << "RunAddress: " << RunAddress << std::endl;
    std::cout << "RunFebSerialNumber: ";
    for (auto sn : RunFebSerialNumber) std::cout << sn << " ";
    std::cout << std::endl;
    std::cout << "RunFebFirmwareVersion: ";
    for (auto fv : RunFebFirmwareVersion) std::cout << fv << " ";
    std::cout << std::endl;
    std::cout << "RunControllerBoardSerialNumber: " << RunControllerBoardSerialNumber << std::endl;
    std::cout << "RunControllerBoardFirmwareVersion: " << RunControllerBoardFirmwareVersion << std::endl;
    std::cout << "RunNumber: " << RunNumber << std::endl;
    std::cout << "RunDate: " << RunDate << std::endl;
    std::cout << "RunTime: " << RunTime << std::endl;
    std::cout << "RunUnixTime: " << RunUnixTime << std::endl;
    std::cout << "RunNFebs: " << RunNFebs << std::endl;
    std::cout << "RunNSamplesToRead: " << RunNSamplesToRead << std::endl;
    std::cout << "RunSamplingFrequency: " << RunSamplingFrequency << std::endl;
    std::cout << "RunSampleLength: " << RunSampleLength << std::endl;

    std::cout << "RunBaseline: " << std::endl;
    for (int i = 0; i < nFebsMax; i++) {
        for (int j = 0; j < nSampicsPerFeb; j++) {
            std::cout << "[" << i << "][" << j << "]: " << RunBaseline[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "RunPostTrig: " << std::endl;
    for (int i = 0; i < nFebsMax; i++) {
        for (int j = 0; j < nSampicsPerFeb; j++) {
            std::cout << "[" << i << "][" << j << "]: " << RunPostTrig[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "RunTrigThr: ";
    for (auto tt : RunTrigThr) {
        std::cout << tt << " ";
    }
    std::cout << std::endl;

    std::cout << "RunUnorderedHitsList: ";
    for (auto hit : RunUnorderedHitsList) {
        // print out info about the hit here
    }
    std::cout << std::endl;

    std::cout << "RunEventsList: ";
    for (auto event : RunEventsList) {
        // print out info about the event here
    }
    std::cout << std::endl;

    std::cout << "RunVerboseMode: " << RunVerboseMode << std::endl;
    std::cout << "RunSelectedAnalysisOptions: " << RunSelectedAnalysisOptions << std::endl;
    std::cout << "RunHitPeakFraction: ";
    for (const auto& fraction : RunHitPeakFraction) {
        std::cout << fraction << " ";
    }
    std::cout << std::endl;
    std::cout << "RunNSamplesInWaveform: " << RunNSamplesInWaveform << std::endl;
    std::cout << "RunNSamplesToExclude: " << RunNSamplesToExclude << std::endl;
    std::cout << "RunBaselineFirstSample: " << RunBaselineFirstSample << std::endl;
    std::cout << "RunBaselineNSamples: " << RunBaselineNSamples << std::endl;
    std::cout << "RunDeleteHitsList: " << RunDeleteHitsList << std::endl;
    std::cout << std::endl;

};

// this function only loads information from the file, almost no computation is done, except cell0time cycles
void TofRun::RunLoadHits(){

    double Cell0TimeOffset = 2.16e13; // reset after 6 hours
    // int maxLinesInFile = 1e10; // to limit very long Runs! move to analysis settings
    int NLinesInFile = 0, hitId_counter = 0;
    int Cell0TimeCycles = 0;
    double new_sample;
    double previous_cell0time = 0;
    std::string dump;
    TofHit new_Hit; // appo hit, then will be pushedback


    // first, load WFs and save ch number, unixtime

    if (RunSoftware == "windows"){
        // simplified, now it has to be one file per Feb. dont split files when saving
        for (int febit = 0; febit < RunNFebs; febit++){

            // print working directory
            // char buffer[FILENAME_MAX];
            // getcwd(buffer, FILENAME_MAX);
            // std::cout << "Current working directory: " << buffer << std::endl;

            std::string RunFebDataFile = "/feb" + std::to_string(febit) + "/";
            std::string full_filename = RunPath + RunFebDataFile + RunAddress + "_feb"+ std::to_string(febit) + ".txt";
            std::ifstream RunHitsFileStream;
            RunHitsFileStream.open(full_filename);

            if (!RunHitsFileStream.is_open()) {
                std::string this_error = "Error opening file " + full_filename;
                std::cerr << this_error << std::endl;
                RunErrorsList.push_back(this_error);
                return;
            }
            
            std::cout << "Loading hits from file " << full_filename << std::endl;

            std::ifstream count_lines_stream;
            count_lines_stream.open(full_filename);
            std::string linedump;
            while (!count_lines_stream.eof())
            {
                getline(count_lines_stream, linedump);
                if(linedump != "") NLinesInFile++;
                // std::cout << linedump << std::endl;
            }
            std::cout << "Hits in Feb[" << febit << "]: " << NLinesInFile/2 << std::endl;

            for (int lineit = 0; lineit < NLinesInFile; lineit++){
                
                // print progression of this ciycle in %
                if (lineit % (NLinesInFile/1000) == 0) {
                    std::cout << "Loading hits from file " << full_filename << " " << lineit/(NLinesInFile/100) << "%\r";
                    std::cout.flush();
                }

                new_Hit.SetHitFeb(febit);

                if(lineit%2 == 0){ //even lines
                    int hit_feb_channel = -1;
                    double hit_unix_time = -1;
                    double hit_cell0time = -1;
                    double hit_raw_TOT_value = -1;
                    double hit_TOT_value = -1;
                    RunHitsFileStream >> dump >> hit_feb_channel
                            >> dump >> hit_unix_time 
                            >> dump >> hit_cell0time 
                            >> dump >> hit_raw_TOT_value
                            >> dump >> hit_TOT_value 
                            >> dump >> dump //Time // computing from wf
                            >> dump >> dump //Baseline // computing from wf
                            >> dump >> dump //RawPeak // computing from wf
                            >> dump >> dump //Amplitude // computing from wf
                            >> dump >> dump; //DataSize; // not reliable
                    // std::cout << "Hitch " << new_Hit.HitFebChannel << std::endl;
                    new_Hit.SetHitFebChannel(hit_feb_channel);
                    new_Hit.SetHitCell0Time(hit_cell0time);
                    new_Hit.SetHitRawTOTValue(hit_raw_TOT_value);
                    new_Hit.SetHitTOTValue(hit_TOT_value);

                }
                else{

                    RunHitsFileStream >> dump; // before the waveform, there is a string "DataSamples"

                    // new_Hit.HitWaveform.clear(); // necessary?

                    for (int sampleit = 0; sampleit < RunNSamplesToExclude; sampleit++) RunHitsFileStream >> dump;
                    std::vector <double> this_wf;
                    for(int sampleit = 0; sampleit < RunNSamplesToRead; sampleit++){
                        RunHitsFileStream >> new_sample;
                        // std::cout << new_sample << std::endl;
                        this_wf.push_back(new_sample);
                    }
                    new_Hit.SetHitWaveform(this_wf);
                    new_Hit.SetHitId(hitId_counter);
                    //print hitidcounter
                    // std::cout << "HitId: " << new_Hit.HitId << " ";

                    if ((previous_cell0time - Cell0TimeCycles*Cell0TimeOffset) - new_Hit.GetHitCell0Time() > 2e13) {
                        Cell0TimeCycles++; 
                        std::cout << "Feb " << new_Hit.GetHitFeb() << ", Cell0Time cycle" << Cell0TimeCycles << std::endl;
                    }

                    new_Hit.SetHitCell0Time(new_Hit.GetHitCell0Time() + Cell0TimeCycles*Cell0TimeOffset);
                    previous_cell0time = new_Hit.GetHitCell0Time();
                    // std::cout << "Filling hit info... ";
                    
                    // std::cout << "Filled hit info. ";

                    RunFillHitInfo(new_Hit);
                    RunUnorderedHitsList.push_back(new_Hit);
                    // std::cout << "Hit added to this run. " ;
                    hitId_counter++;
                }
                if (hitId_counter > RunMaxHitsToLoad) break;
            }

            NLinesInFile = 0;

        }
    }


    if (RunSoftware == "linux"){

        std::string full_filename = RunPath;
        std::ifstream RunHitsFileStream; 
        RunHitsFileStream.open(full_filename);


        std::cout << "Loading hits from file " << full_filename << std::endl;

        std::ifstream count_lines_stream;
        count_lines_stream.open(full_filename);
        std::string linedump;
        while (!count_lines_stream.eof()){

            getline(count_lines_stream, linedump);
            if(linedump != "") NLinesInFile++;
            // std::cout << linedump << std::endl;

        }
        std::cout << "Hits in this Run: " << NLinesInFile/2 << std::endl;

        for (int lineit = 0; lineit < NLinesInFile; lineit++){

            // print progression of this ciycle in %
            if (lineit % (NLinesInFile/1000) == 0) {
                    std::cout << "Loading hits from file " << full_filename << " " << lineit/(NLinesInFile/100) << "%\r";
                    std::cout.flush();
            }

            if(lineit%2 == 0){ //even lines
                int hit_feb_channel = -1;
                double hit_feb = -1;
                double hit_cell0time = -1;
                RunHitsFileStream >> dump >> hit_feb_channel
                        >> dump >> hit_feb
                        >> dump >> hit_cell0time;
                        // >> dump >> new_Hit.HitRawTOTValue // should add
                        // >> dump >> new_Hit.HitTOTValue // should add
                // std::cout << "New hit channel " << new_Hit.HitFebChannel << " time " << new_Hit.HitCell0Time << std::endl;
                new_Hit.SetHitFebChannel(hit_feb_channel);
                new_Hit.SetHitFeb(hit_feb);
                new_Hit.SetHitCell0Time(hit_cell0time);
            } 
            else{
                RunHitsFileStream >> dump; // before the waveform, there is a string "DataSamples"
                // new_Hit.HitWaveform.clear(); // necessary?

                for (int sampleit = 0; sampleit < RunNSamplesToExclude; sampleit++) RunHitsFileStream >> dump;
                std::vector <double> this_wf;
                for(int sampleit = 0; sampleit < RunNSamplesInWaveform; sampleit++){
                    RunHitsFileStream >> new_sample;
                    this_wf.push_back(new_sample);
                    // std::cout << new_sample << std::endl;
                }
                new_Hit.SetHitWaveform(this_wf);
                new_Hit.SetHitId(hitId_counter);
                // print hit id counter
                // std::cout << "HitId: " << new_Hit.HitId << " ";

                if ((previous_cell0time - Cell0TimeCycles*Cell0TimeOffset) - new_Hit.GetHitCell0Time() > 2e13) {
                    Cell0TimeCycles++; 
                    std::cout << "Feb " << new_Hit.GetHitFeb() << ", Cell0Time cycle" << Cell0TimeCycles << std::endl;
                }

                new_Hit.SetHitCell0Time(new_Hit.GetHitCell0Time() + Cell0TimeCycles*Cell0TimeOffset);

                previous_cell0time = new_Hit.GetHitCell0Time();

                RunFillHitInfo(new_Hit);    // can do elsewhere
                if (RunVerboseMode == true) new_Hit.HitGetHitInfo();
                // std::cout << "Filled hit info. \n";

                RunUnorderedHitsList.push_back(new_Hit);
                hitId_counter++;
                // std::cout << "hitid_counter " << hitId_counter << std::endl;

            }
            if (hitId_counter > RunMaxHitsToLoad){
                std::cout << "\nReached max hits set in analysis settings\n";
                break;
            } 

        }

    }
}





void TofRun::RunFillHitInfo(TofHit &this_hit){
    // Set HitSampic using setter function
    this_hit.SetHitSampic(std::floor(this_hit.GetHitFebChannel()/nChannelsPerSampic));

    // Set HitDaqChannel using setter function
    this_hit.SetHitDaqChannel(nChannelsPerFeb*this_hit.GetHitFeb() + this_hit.GetHitFebChannel());

    std::vector <double> this_wf = this_hit.GetHitWaveform();

    // RawPeak is just max in wf, with no fit
    double raw_peak = *std::max_element(this_wf.begin() + RunBaselineFirstSample + RunBaselineNSamples, this_wf.end());
    auto max_it = std::max_element(this_wf.begin() + RunBaselineFirstSample + RunBaselineNSamples, this_wf.end());
    size_t hit_peak_sample = std::distance(this_wf.begin(), max_it);

    // Set HitRawPeak using setter function
    this_hit.SetHitRawPeak(raw_peak);

    // Set HitPeakSample using setter function
    this_hit.SetHitPeakSample(hit_peak_sample);

    // Calculate HitBaseline
    double baseline = 0;
    for(int sampleit = RunBaselineFirstSample; sampleit < RunBaselineFirstSample + RunBaselineNSamples; sampleit++)
        baseline += this_wf.at(sampleit);
    baseline /= RunBaselineNSamples;

    // Set HitBaseline using setter function
    this_hit.SetHitBaseline(baseline);

    // Subtract baseline from waveform
    for(int sampleit = 0; sampleit < RunNSamplesInWaveform; sampleit++)
        this_wf.at(sampleit) -= baseline;
    
    this_hit.SetHitWaveform(this_wf);

    // Make waveform positive in case it's negative (pmts). Dirty but ok for now
    // if (this_hit.HitDaqChannel == 216 || this_hit.HitDaqChannel == 219 || this_hit.HitDaqChannel == 228 || this_hit.HitDaqChannel == 231){
    //     for(int sampleit = 0; sampleit < RunNSamplesInWaveform; sampleit++)
    //         this_hit.HitWaveform.at(sampleit) *= -1;   
    // }

    // Calculate and set HitPeak using setter function
    double peak = raw_peak - baseline;
    this_hit.SetHitPeak(peak);

    // Calculate and set HitPeakTime using setter function
    double peak_time = hit_peak_sample*RunSampleLength; // add Cell0time
    this_hit.SetHitPeakTime(peak_time);

    // Calculate HitVoltageIntegral
    double voltage_integral = 0.; // fix
    for(int sampleit = RunBaselineFirstSample + RunBaselineNSamples; sampleit < RunNSamplesInWaveform; sampleit++){
        voltage_integral += this_wf.at(sampleit); // maybe better a method in Tofhit
    }

    // Set HitVoltageIntegral using setter function
    this_hit.SetHitVoltageIntegral(voltage_integral);

    // Set HitSampleLength using setter function
    this_hit.SetHitSampleLength(RunSampleLength); // give through struct

    // Call HitMatchDaqChToTofCh method
    this_hit.HitMatchDaqChToTofCh();

    // Clear HitCfTimeFromFit vector
    // this_hit.GeHitCfTimeFromFit.clear();

    // Call HitFitWaveform method
    this_hit.HitFitWaveform();

    // Print hit info if verbose mode is on
    if (RunVerboseMode == true) {
        this_hit.HitGetHitInfo();
    }
}


// void TofRun::RunFillHitInfo(TofHit this_hit){

//     // std::cout << "Filling hit info... ";

//     this_hit.HitSampic = std::floor(this_hit.HitFebChannel/nChannelsPerSampic);
//     this_hit.HitDaqChannel = nChannelsPerFeb*this_hit.HitFeb + this_hit.HitFebChannel;

//     this_hit.HitBaseline = 0;
//     for(int sampleit = RunBaselineFirstSample; sampleit < RunBaselineFirstSample + RunBaselineNSamples; sampleit++)
//         this_hit.HitBaseline += this_hit.HitWaveform.at(sampleit);        
//     this_hit.HitBaseline /= RunBaselineNSamples;


//     // subtract baseline to waveform
//     for(int sampleit = 0; sampleit < RunNSamplesInWaveform; sampleit++)
//         this_hit.HitWaveform.at(sampleit) -= this_hit.HitBaseline;

//     // Make waveform positive in case it's negative (pmts). Dirty but ok for now
//     if (this_hit.HitDaqChannel == 216 || this_hit.HitDaqChannel == 219 || this_hit.HitDaqChannel == 228 || this_hit.HitDaqChannel == 231){
//         for(int sampleit = 0; sampleit < RunNSamplesInWaveform; sampleit++)
//         this_hit.HitWaveform.at(sampleit) *= -1;   
//     }

//     // RawPeak is just max in wf, with no fit
//     this_hit.HitRawPeak = *std::max_element(this_hit.HitWaveform.begin(), this_hit.HitWaveform.end());
//     auto max_it = std::max_element(this_hit.HitWaveform.begin(), this_hit.HitWaveform.end());
//     size_t hit_peak_sample = std::distance(this_hit.HitWaveform.begin(), max_it);
//     this_hit.HitPeakSample = hit_peak_sample; // not super useful

//     this_hit.HitPeak = this_hit.HitRawPeak - this_hit.HitBaseline; // will be changed to use fit

//     this_hit.HitPeakTime = this_hit.HitPeakSample*RunSampleLength; // will be replaced by fit

//     this_hit.HitVoltageIntegral = 0.;
//     for(int sampleit = RunBaselineFirstSample + RunBaselineNSamples; sampleit < RunNSamplesInWaveform; sampleit++){
//         this_hit.HitVoltageIntegral += this_hit.HitWaveform.at(sampleit);
//     }

//     this_hit.HitPeakFraction = RunHitPeakFraction;
    
//     this_hit.HitSampleLength = RunSampleLength;
//     // std::cout << "Sample length: " << this_hit.HitSampleLength << std::endl;

//     this_hit.HitMatchDaqChToTofCh();

//     this_hit.HitCfTimeFromFit.clear(); // avoid memory issues

//     this_hit.HitFitWaveform();

//     // std::cout << "Filled hit info. ";
//     // //print hid id
//     // std::cout << "HitId: " << this_hit.HitId << " ";
//     if (RunVerboseMode == true) this_hit.HitGetHitInfo();

// }


void TofRun::RunOrderHits(){

    std::vector<std::pair<double, int>> pair_cell0times_hitid;

    for (int ihit = 0; ihit < RunUnorderedHitsList.size(); ihit++)
        pair_cell0times_hitid.push_back(std::make_pair(RunUnorderedHitsList.at(ihit).GetHitCell0Time(), ihit));

    // for (int ihit = 0; ihit < 200; ihit++)
    //     std::cout << pair_cell0times_hitid.at(ihit).first << " ";


    std::cout << "\nSorting hits basing on Cell0Time...";
    sort(pair_cell0times_hitid.begin(), pair_cell0times_hitid.end());
    // for (int ihit = 0; ihit < 200; ihit++)
    //     std::cout << pair_cell0times_hitid.at(ihit).first << " ";
    std::cout << "Done!";

    for (int ihit = 0; ihit < RunUnorderedHitsList.size(); ihit++)
        RunOrderedHitsList.push_back(RunUnorderedHitsList.at(pair_cell0times_hitid.at(ihit).second));
    
    RunUnorderedHitsList = {}; // free memory
    std::cout << "\nStored hits in RunOrderedHitsList and emptied RunUnorderedHitsList.\n";

}

void TofRun::RunCreateEvents(){
    double primitive_gate = 50., latency_gate = 70.; // move to proper location
    double extra = 20.;
    double coincidence_window = primitive_gate + latency_gate +extra; 
    double this_hit_time = 0;
    bool  create_new_event = true;
    TofEvent new_event;

    for (int ihit = 0; ihit < RunOrderedHitsList.size(); ihit++){

        if (create_new_event) {
            // std::cout << "Creating new event" << std::endl;
            this_hit_time = RunOrderedHitsList.at(ihit).GetHitCell0Time();
            new_event = TofEvent(); // reset
            // std::cout << "Create new event\n";
        }

        if (RunOrderedHitsList.at(ihit).GetHitCell0Time() - this_hit_time < coincidence_window){
            new_event.AddSignal(RunOrderedHitsList.at(ihit));
            create_new_event = false;
        }
        else{            
            // more operations on EventHits
            // std::cout << "Event " << RunEventsList.size() << " has " << new_event.EventHitsList.size() << " hits." << std::endl;
            new_event.EventCreateSignals();
            RunEventsList.push_back(new_event);
            create_new_event = true;
        }

    }
    
    std::cout << "Created " << RunEventsList.size() << " events." << std::endl;
    RunOrderedHitsList = {}; // free memory

}

void TofRun::RunSetAnalysisOptions (){   

    if (RunSelectedAnalysisOptions == true) return; // avoid double calls

    // for Linux as of april
    if (RunSoftware == "linux"){ 
        RunNSamplesInWaveform = 63;
        RunNSamplesToExclude = 1; // first is not to consider
        RunBaselineNSamples = 5;
        RunDeleteHitsList = false;
        RunMaxHitsToLoad = 1000;
        return;
    }    

    std::string RunAnalysisSettingsFile = "../AnalysisSettings.json"; // has to be in same folder for now
    std::ifstream RunAnalysisSettingsStream(RunAnalysisSettingsFile.c_str());
    if (RunAnalysisSettingsStream.good()) RunSelectedAnalysisOptions = true;

    if (!RunAnalysisSettingsStream.is_open()) {
        std::string this_error = "Failed to open " + RunAnalysisSettingsFile;
        std::cerr << this_error << std::endl;
        RunErrorsList.push_back(this_error);
        RunSelectedAnalysisOptions = false;
        return;
    }

    std::cout << "Reading analysis settings from " << RunAnalysisSettingsFile << std::endl;

    nlohmann::json analysis_settings_file;
    RunAnalysisSettingsStream >> analysis_settings_file;
    RunBaselineNSamples = analysis_settings_file["RunBaselineNSamples"];
    RunNSamplesToExclude = analysis_settings_file["RunNSamplesToExclude"];
    RunDeleteHitsList = analysis_settings_file["RunDeleteHitsList"];
    RunVerboseMode = analysis_settings_file["RunVerboseMode"];
    RunMaxHitsToLoad = analysis_settings_file["RunMaxHitsToLoad"];
    

    RunNSamplesInWaveform = RunNSamplesToRead- RunNSamplesToExclude;



    RunSelectedAnalysisOptions = true;


}

void TofRun::RunGetAnalysisOptions (){}


void TofRun::RunPrintErrors(){
    for (int i = 0; i < RunErrorsList.size(); i++){
        std::cout << "------------------------------------------\n";
        std::cout << "Errors encountered:\n";
        std::cout << RunErrorsList.at(i) << std::endl;
        std::cout << "------------------------------------------\n";
    }

    if (RunErrorsList.size() == 0) std::cout << "No errors encountered.\n";
    
}


void TofRun::RunGenerateOutputFile(std::string output_directory){

    std::string output_file_name = "run"+ std::to_string(RunNumber) + ".root";

    std::cout << "Generating output file " << output_file_name << " in " << output_directory << std::endl;
    TFile *output_file = new TFile(Form("%s%s", output_directory.c_str(), output_file_name.c_str()), "RECREATE");
    TTree *output_tree = new TTree(Form("TreeTofRun%d", RunNumber), "Tree contanining all TofRun information");

    output_tree -> Branch("TofRun", this);
    
    output_tree->Fill();
    output_file->Write();
    output_file->Close();

}
