#include "ReadAndReconstructSingleRun.h"

void HitAnalysis(std::string InputFile)
{
  int LogLevel = 1;
  bool ReadHitWaveform = true;
  bool ReadWaveformInterpolation = false;

  //Input file
  TFile *fInputFile = new TFile(InputFile.c_str(), "READ");


  TTree *RunInfoTree = (TTree*)fInputFile->Get("RunInfoTree");
  
  //Run info
  double RunUnixTime;
  int RunNFEBs;
  std::vector<std::string> *RunFEBSerialNumber = NULL;;
  double RunSetBaseline[NFEBsMax][NSAMPICsPerFEB];
  int RunNSamplesPerHit;
  double RunSamplingFrequency;
  double RunSampleLength;
  int RunFirstSampleHitBaseline;
  int RunNSamplesHitBaseline;
  std::vector<double> *RunHitPeakFraction = NULL;


  RunInfoTree->SetBranchAddress("RunUnixTime", &RunUnixTime);
  RunInfoTree->SetBranchAddress("RunNFEBs", &RunNFEBs);
  RunInfoTree->SetBranchAddress("RunFEBSerialNumber", &RunFEBSerialNumber);
  RunInfoTree->SetBranchAddress("RunSetBaseline", &RunSetBaseline);  
  RunInfoTree->SetBranchAddress("RunNSamplesPerHit", &RunNSamplesPerHit);
  RunInfoTree->SetBranchAddress("RunSamplingFrequency", &RunSamplingFrequency);
  RunInfoTree->SetBranchAddress("RunSampleLength", &RunSampleLength);
  RunInfoTree->SetBranchAddress("RunFirstSampleHitBaseline", &RunFirstSampleHitBaseline);
  RunInfoTree->SetBranchAddress("RunNSamplesHitBaseline", &RunNSamplesHitBaseline);
  RunInfoTree->SetBranchAddress("RunHitPeakFraction", &RunHitPeakFraction);

  RunInfoTree->GetEntry(0);

  if(LogLevel > 0)
  {
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "RunUnixTime: " << RunUnixTime << std::endl;
    std::cout << "RunNFEBs: " << RunNFEBs << std::endl;

    for(int febit = 0; febit < RunFEBSerialNumber->size(); febit++)
    {
      std::cout << "RunFEBSerialNumber->at(febit): " << RunFEBSerialNumber->at(febit) << std::endl;
    }

    for(int febit = 0; febit < RunNFEBs; febit++)
    {
      for(int sampicit = 0; sampicit < NSAMPICsPerFEB; sampicit++)
      {
        std::cout << "RunSetBaseline[" << febit << "][" << sampicit << "]: " << RunSetBaseline[febit][sampicit] << std::endl;
      }
    }

    std::cout << "RunNSamplesPerHit: " << RunNSamplesPerHit << std::endl;
    std::cout << "RunSamplingFrequency: " << RunSamplingFrequency << std::endl;
    std::cout << "RunSampleLength: " << RunSampleLength << std::endl;
    std::cout << "RunFirstSampleHitBaseline: " << RunFirstSampleHitBaseline << std::endl;
    std::cout << "RunNSamplesHitBaseline: " << RunNSamplesHitBaseline << std::endl;

    for(int runhitpeakfractionit = 0; runhitpeakfractionit < RunHitPeakFraction->size(); runhitpeakfractionit++)
    {
      // std::cout << "RunHitPeakFraction->at(" << runhitpeakfractionit << "): " << RunHitPeakFraction->at(runhitpeakfractionit) << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }


  //Hit info
  TTree *HitInfoTree = (TTree*)fInputFile->Get("HitInfoTree");
  int NHits = HitInfoTree->GetEntries();

  //Hit info from data file
  int HitID = 0;
  int HitFEB;
  int HitSAMPIC;
  int HitChannelOnFEB;
  int HitChannelGlobal;
  //double HitUnixTime;
  double HitCell0Time;
  double HitTime;
  double HitTOTValue;
  double HitRawTOTValue;
  double HitRawPeak;
  std::vector<double> *HitWaveform = NULL;;

  HitInfoTree->SetBranchAddress("HitID", &HitID);
  HitInfoTree->SetBranchAddress("HitFEB", &HitFEB);
  HitInfoTree->SetBranchAddress("HitSAMPIC", &HitSAMPIC);
  HitInfoTree->SetBranchAddress("HitChannelOnFEB", &HitChannelOnFEB);
  HitInfoTree->SetBranchAddress("HitChannelGlobal", &HitChannelGlobal);
  //HitInfoTree->SetBranchAddress("HitUnixTime", &HitUnixTime);
  HitInfoTree->SetBranchAddress("HitCell0Time", &
  HitCell0Time);
  HitInfoTree->SetBranchAddress("HitTime", &HitTime);
  HitInfoTree->SetBranchAddress("HitTOTValue", &HitTOTValue);
  HitInfoTree->SetBranchAddress("HitRawTOTValue", &HitRawTOTValue);
  HitInfoTree->SetBranchAddress("HitRawPeak", &HitRawPeak);
  
  if(ReadHitWaveform)
  {
    HitInfoTree->SetBranchAddress("HitWaveform", &HitWaveform);
  }

  //Reconstructed hit info
  double HitVoltageIntegral;
  double HitPeak;
  int HitPeakSample;  
  double HitPeakTime;
  double HitBaseline;

  std::vector<double> *HitPeakFractionSampleLinearInt = NULL;
  std::vector<double> *HitPeakFractionTimeLinearInt = NULL;
  std::vector<double> *HitPeakFractionSampleSplineInt = NULL;
  std::vector<double> *HitPeakFractionTimeSplineInt = NULL;

  HitInfoTree->SetBranchAddress("HitBaseline", &HitBaseline);
  HitInfoTree->SetBranchAddress("HitVoltageIntegral", &HitVoltageIntegral);
  HitInfoTree->SetBranchAddress("HitPeak", &HitPeak);
  HitInfoTree->SetBranchAddress("HitPeakSample", &HitPeakSample);
  HitInfoTree->SetBranchAddress("HitPeakTime", &HitPeakTime);
  HitInfoTree->SetBranchAddress("HitPeakFractionSampleLinearInt", &HitPeakFractionSampleLinearInt);
  HitInfoTree->SetBranchAddress("HitPeakFractionTimeLinearInt", &HitPeakFractionTimeLinearInt);
  HitInfoTree->SetBranchAddress("HitPeakFractionSampleSplineInt", &HitPeakFractionSampleSplineInt);
  HitInfoTree->SetBranchAddress("HitPeakFractionTimeSplineInt", &HitPeakFractionTimeSplineInt);


  std:: vector <double> v_HitCell0Time;

   for(int hitit = 0; hitit < NHits; hitit++)
  {
    HitInfoTree->GetEntry(hitit);
    v_HitCell0Time.push_back(HitCell0Time);  
  }
  cout << "lenght vector " << v_HitCell0Time.size() << std::endl;
  
  

  std::vector <int> eventhits;  
  std::vector < std::vector <int>> eventslist;  

  for (int it =0; it < v_HitCell0Time.size(); ){

    eventhits.clear();    
  
    eventhits.push_back(it); //store the hitid --> it would be better to store HitID
    int tmp_it = it;
    // std::cout << "now looking Hit : " << tmp_it << std::endl;
    
  

  int it2 = it+1;
 
    while( ( it2 < v_HitCell0Time.size()  ) && (  abs(v_HitCell0Time.at(it) - v_HitCell0Time.at(it2)) <100 ) )     {

      // std::cout << " it " << tmp_it << "  it2 " << it2 << std::endl;
    
      // std::cout<< "v_HitCell0Time.at(it) " << v_HitCell0Time.at(tmp_it) << " v_HitCell0Time.at(it2)  " << v_HitCell0Time.at(it2) <<  "  abs(v_HitCell0Time.at(it) - v_HitCell0Time.at(it2))  " << abs(v_HitCell0Time.at(tmp_it) - v_HitCell0Time.at(it2)) <<std::endl;
    
      // std::cout << " pushing the hit into the event " << std::endl;
      eventhits.push_back(it2);
      // std::cout << "eventhits.size() " << eventhits.size() << std::endl;    
      it2++;
    
      
    }  
  
  
  // std::cout << "size of event hits "<< eventhits.size() << std::endl;  
  // std::cout << "the event contains the following hits:";
    
  // for (int iev=0; iev<eventhits.size(); iev++){
  //   std::cout << " " << eventhits.at(iev) << std::endl;
  // }

  it = it2;
  eventslist.push_back(eventhits);
  // std::cout << "the new value of iterator it is : " << it << std::endl;
  }
  
  // std::cout<< " or here ? " << std::endl;
  // std::cout << " eventlist size " << eventslist.size() << std::endl;

  // for (int iev=0; iev< eventslist.size(); iev++){
    
  //   std::cout <<eventslist.at(iev).size() << std::endl;  

  // }


  // abort();

  // plots
  TH1F *h_baselines = new TH1F("h_baselines", "all baselines ", 200, 0.180, 0.250);
  
  for(int hitit = 0; hitit < NHits; hitit++)
  {

    HitInfoTree->GetEntry(hitit);

    if(LogLevel > 1)
    {
      std::cout << std::endl;
      std::cout << "HitID: " << HitID << std::endl;
      std::cout << "HitFEB: " << HitFEB << std::endl;
      std::cout << "HitSAMPIC: " << HitSAMPIC << std::endl;
      std::cout << "HitChannelOnFEB: " << HitChannelOnFEB << std::endl;
      std::cout << "HitChannelGlobal: " << HitChannelGlobal << std::endl;
      //std::cout << "HitUnixTime: " << HitUnixTime << std::endl;
      std::cout << "HitCell0Time: " << std::setprecision(20) << HitCell0Time << std::endl;
      std::cout << "HitTime: " << HitTime << std::endl;
      std::cout << "HitTOTValue: " << HitTOTValue << std::endl;
      std::cout << "HitRawTOTValue: " << HitRawTOTValue << std::endl;
      std::cout << "HitRawPeak: " << HitRawPeak << std::endl;

    
    


      if(ReadHitWaveform)
      {
        for(int sampleit = 0; sampleit < HitWaveform->size(); sampleit++)
        {
          // std::cout << "HitWaveform->at(" << sampleit << "): " << HitWaveform->at(sampleit) << std::endl;   
        }
      }

      std::cout << "HitBaseline: " << HitBaseline << std::endl;
      std::cout << "HitVoltageIntegral: " << HitVoltageIntegral << std::endl;
      std::cout << "HitPeak: " << HitPeak << std::endl;
      std::cout << "HitPeakSample: " << HitPeakSample << std::endl;
      std::cout << "HitPeakTime: " << HitPeakTime << std::endl;

      cout << "Hitbaseline " << HitBaseline << endl;
      h_baselines->Fill(HitBaseline);

      if(ReadWaveformInterpolation)
      {
        for(int runhitpeakfractionit = 0; runhitpeakfractionit < RunHitPeakFraction->size(); runhitpeakfractionit++)
        {
          std::cout << "HitPeakFractionSampleLinearInt->at(" << runhitpeakfractionit << "): " << HitPeakFractionSampleLinearInt->at(runhitpeakfractionit) << std::endl;
          std::cout << "HitPeakFractionTimeLinearInt->at(" << runhitpeakfractionit << "): " << HitPeakFractionTimeLinearInt->at(runhitpeakfractionit) << std::endl;
          std::cout << "HitPeakFractionSampleSplineInt->at(" << runhitpeakfractionit << "): " << HitPeakFractionSampleSplineInt->at(runhitpeakfractionit) << std::endl;
          std::cout << "HitPeakFractionTimeSplineInt->at(" << runhitpeakfractionit << "): " << HitPeakFractionTimeSplineInt->at(runhitpeakfractionit) << std::endl;
        }
      }
      std::cout << std::endl;
    }

  }

  TCanvas * c_baselines = new TCanvas("c_baselines", "baselines");
  c_baselines->cd();
  h_baselines->Draw("HIST");
  c_baselines->Update();

}


