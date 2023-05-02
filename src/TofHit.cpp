#include "../include/TofRun.h"
#include "../include/TofEvent.h"
#include "../include/TofHit.h"
#include "../include/TofSignal.h"

std::vector<std::string> SplitString(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {elems.push_back(item);}
    return elems;
}

Double_t FitFunction(Double_t *x,Double_t *parameters){ // why x is a pointer 

    Double_t x_relative = 0;
    Double_t sigma = 0;

    if( x[0] < parameters[1] ) 
        sigma = TMath::Abs(parameters[2])*(x[0]-parameters[1])+parameters[3]; 
    else 
        sigma = TMath::Abs(parameters[4])*(x[0]-parameters[1])+parameters[3]; 

    x_relative = (x[0] - parameters[1]);
    Double_t fit_value = parameters[0]*TMath::Exp(-0.5*pow(x_relative/sigma,2))+parameters[5];

    return fit_value;

}

TofHit::TofHit(){
    
}


void TofHit::HitFitWaveform(){

    HitFitParameter[0] = HitPeak;
    HitFitParameter[1] = HitPeakSample;
    HitFitParameter[2] = 0.2; // WidthDep1
    HitFitParameter[3] = 13.; // Width
    HitFitParameter[4] = 0.2; // WidthDep2
    HitFitParameter[5] = 0.0; // Baseline

    double fit_peak = -1;
    double fit_peakSample = -1;
    double fit_widthDep1 = -1;
    double fit_width = -1;
    double fit_widthDep2 = -1;
    double fit_baseLine = -1;
    
    TF1 fit_function("fit_function",FitFunction,0,HitWaveform.size(),6);        
    fit_function.SetParameter(0,HitFitParameter[0]);
    fit_function.SetParameter(1,HitFitParameter[1]);
    fit_function.SetParameter(2,HitFitParameter[2]);
    fit_function.SetParameter(3,HitFitParameter[3]);
    fit_function.SetParameter(4,HitFitParameter[4]);
    fit_function.SetParameter(5,HitFitParameter[5]);
    
    // create appo canvas just to not create a dedfault one during execution. Look for a workaround

    TH1D * h_waveform = new TH1D(Form("Channel%i, HitId%i",HitDaqChannel, HitId),Form("Waveform Ch%i", HitDaqChannel),HitWaveform.size(),-0.5,HitWaveform.size()-0.5);
    h_waveform->GetXaxis()->SetTitle("Sample");
    h_waveform->GetYaxis()->SetTitle("Amplitude(V)"); 
    for(int j =0; j < HitWaveform.size();j++ ) {
        h_waveform->SetBinContent(j+1, HitWaveform[j]);
        h_waveform->SetBinError(j+1,1.e-4); // check this
    }

    // disable error messages during execution
    gROOT->ProcessLine("gErrorIgnoreLevel = kFatal;"); // PRETTY EXTEME, ONLY FOR DEBUGGING
    int fit_min = 2; // HitPeakSample-10;
    int fit_max = 60; // HitPeakSample+10; // check this
    int fit_status = h_waveform->Fit("fit_function","Q","",  fit_min, fit_max);

    // continue the loop if fit fails
    if (fit_status!= 0){
        // std::cout << "fit failed" << std::endl;
        HitFitSuccess = false;
        h_waveform->Delete();
        return;
    }
    
    HitFitSuccess = true;

    fit_peak = TMath::Abs(fit_function.GetParameter(0));
    fit_peakSample = fit_function.GetParameter(1);
    fit_widthDep1 = fit_function.GetParameter(2);
    fit_width = fit_function.GetParameter(3);
    fit_widthDep2 = fit_function.GetParameter(4);
    fit_baseLine = fit_function.GetParameter(5);


    if (fit_peakSample < HitWaveform.size() && fit_peakSample > 0. && fit_peak > 0.){
        for (int i = 0; i < HitPeakFraction.size(); i++){
            
            // std::cout << "argument of getX " << HitPeakFraction.at(i)*fit_peak + fit_baseLine << std::endl;
            double fit_value = fit_function.GetX(HitPeakFraction.at(i)*fit_peak + fit_baseLine);

            if (std::isnan(fit_value) || fit_value < fit_min || fit_value > fit_max){
                HitFitSuccess = false;
                // std::cerr << "Error: Grid search failed to find a root. Fit not succesfull, moving to next.\n";
                return;
            }
            else
                HitCfTimeFromFit.push_back(fit_value * HitSampleLength + HitCell0Time ); // +  HitCell0Time
        }
    }

    // to print on a canvas the fit function, change to true 
    if (false){
            gROOT->SetBatch(kFALSE);
            TCanvas *c_appo = new TCanvas("c_appo","c_appo",800,600);

            std::cout << "Drawing " << h_waveform->GetName() << std::endl;
            c_appo->cd();
            h_waveform->Draw();
            fit_function.Draw("same");
            c_appo->Update();
            c_appo->Modified();
            c_appo->WaitPrimitive();
            c_appo->Clear();
    }

    h_waveform->Delete();
    // c_appo->Close();

}

void TofHit::HitQualityCheck(){
    
    if (HitEdge != 0 && HitEdge != 1){
        std::string this_error;
        this_error = "HitEdge variable has incorrect value,  should be 0 or 1\n";
        std::cerr << this_error << std::endl;
        HitErrorsList.push_back(this_error);
    }

}

void TofHit::HitMatchDaqChToTofCh(){
    // bulk bars: all except 0,1,18,19
    // dictionary is U=0, D=1, T=2, B=3, L=4, R=5
    if (HitFeb == 0 and (HitSampic == 0 or HitSampic == 1)){
        HitPlane = 2;
        HitBar = HitFebChannel%16 + 2; // this is when connecting bulk to FEB 0,1,2 and external bars to FEB3. Otherwise, it's channel/2
        HitChannelOnPlane = HitBar*2 + HitSampic%2;
    }
    if (HitFeb == 0 and (HitSampic == 2 or HitSampic == 3)){
        HitPlane = 1;
        HitBar = (HitFebChannel - 32)%16 + 2;
        HitChannelOnPlane = HitBar*2 + HitSampic%2;
    }
    if (HitFeb == 1 and (HitSampic == 0 or HitSampic == 1)){
        HitPlane = 3;
        HitBar = HitFebChannel%16 + 2;
        HitChannelOnPlane = HitBar*2 + HitSampic%2;
    }
    if (HitFeb == 1 and (HitSampic == 2 or HitSampic == 3)){
        HitPlane = 0;
        HitBar = (HitFebChannel - 32)%16 + 2;
        HitChannelOnPlane = HitBar*2 + HitSampic%2;
    }
    if (HitFeb == 2 and (HitSampic == 0 or HitSampic == 1)){
        HitPlane = 4;
        HitBar = HitFebChannel%16 + 2;
        HitChannelOnPlane = HitBar*2 + HitSampic%2;
    }
    if (HitFeb == 2 and (HitSampic == 2 or HitSampic == 3)){
        HitPlane = 5;
        HitBar = (HitFebChannel - 32)%16 + 2;
        HitChannelOnPlane = HitBar*2 + HitSampic%2;
    }

    //external bars
    if (HitFeb == 3){
    if (HitSampic == 0){
        if (HitFebChannel == 0) {HitPlane = 0; HitBar = 0;}
        if (HitFebChannel == 1) {HitPlane = 0; HitBar = 1;}
        if (HitFebChannel == 2) {HitPlane = 0; HitBar = 18;}
        if (HitFebChannel == 3) {HitPlane = 0; HitBar = 19;}
        if (HitFebChannel == 4) {HitPlane = 2; HitBar = 0;}
        if (HitFebChannel == 5) {HitPlane = 2; HitBar = 1;}
        if (HitFebChannel == 6) {HitPlane = 2; HitBar = 18;}
        if (HitFebChannel == 7) {HitPlane = 2; HitBar = 19;}
        if (HitFebChannel == 8) {HitPlane = 1; HitBar = 0;}
        if (HitFebChannel == 9) {HitPlane = 1; HitBar = 1;}
        if (HitFebChannel == 10) {HitPlane = 1; HitBar = 18;}
        if (HitFebChannel == 11) {HitPlane = 1; HitBar = 19;}
        if (HitFebChannel == 12) {HitPlane = 5; HitBar = 0;}
        if (HitFebChannel == 13) {HitPlane = 5; HitBar = 1;}
        if (HitFebChannel == 14) {HitPlane = 5; HitBar = 18;}
        if (HitFebChannel == 15) {HitPlane = 5; HitBar = 19;}
        HitChannelOnPlane = HitBar*2 + 1;
    }
    if (HitSampic == 1){
        if (HitFebChannel == 16) {HitPlane = 2; HitBar = 0;}
        if (HitFebChannel == 17) {HitPlane = 2; HitBar = 1;}
        if (HitFebChannel == 18) {HitPlane = 2; HitBar = 18;}
        if (HitFebChannel == 19) {HitPlane = 2; HitBar = 19;}
        if (HitFebChannel == 20) {HitPlane = 0; HitBar = 0;}
        if (HitFebChannel == 21) {HitPlane = 0; HitBar = 1;}
        if (HitFebChannel == 22) {HitPlane = 0; HitBar = 18;}
        if (HitFebChannel == 23) {HitPlane = 0; HitBar = 19;}
        // if (HitFebChannel == 24) {HitPlane = 3; HitBar = 0;}
        if (HitFebChannel == 25) {HitPlane = 3; HitBar = 1;}
        if (HitFebChannel == 26) {HitPlane = 3; HitBar = 18;}
        // if (HitFebChannel-16 == 27) {HitPlane = 3; HitBar = 19;}
        if (HitFebChannel == 28) {HitPlane = 4; HitBar = 0;}
        if (HitFebChannel == 29) {HitPlane = 4; HitBar = 1;}
        if (HitFebChannel == 30) {HitPlane = 4; HitBar = 18;}
        if (HitFebChannel == 31) {HitPlane = 4; HitBar = 19;}
        HitChannelOnPlane = HitBar*2;
    }
    if (HitSampic == 2){
        if (HitFebChannel == 32) {HitPlane = 4; HitBar = 0; HitChannelOnPlane = HitBar*2 + 1;}
        if (HitFebChannel == 33) {HitPlane = 4; HitBar = 1; HitChannelOnPlane = HitBar*2 + 1;}
        if (HitFebChannel == 34) {HitPlane = 4; HitBar = 18; HitChannelOnPlane = HitBar*2 + 1;}
        if (HitFebChannel == 35) {HitPlane = 4; HitBar = 19; HitChannelOnPlane = HitBar*2 + 1;}
        // if (HitFebChannel == 36) {HitPlane = 3; HitBar = 0; HitChannelOnPlane = HitBar*2 + 1;}
        if (HitFebChannel == 37) {HitPlane = 3; HitBar = 1; HitChannelOnPlane = HitBar*2 + 1;}
        if (HitFebChannel == 38) {HitPlane = 3; HitBar = 18; HitChannelOnPlane = HitBar*2 + 1;}
        // if (HitFebChannel-32 == 39) {HitPlane = 3; HitBar = 19; HitChannelOnPlane = HitBar*2 + 1;}
        if (HitFebChannel == 40) {HitPlane = 5; HitBar = 0; HitChannelOnPlane = HitBar*2;}
        if (HitFebChannel == 41) {HitPlane = 5; HitBar = 1; HitChannelOnPlane = HitBar*2;}
        if (HitFebChannel == 42) {HitPlane = 5; HitBar = 18; HitChannelOnPlane = HitBar*2;}
        if (HitFebChannel == 43) {HitPlane = 5; HitBar = 19; HitChannelOnPlane = HitBar*2;}
        if (HitFebChannel == 44) {HitPlane = 1; HitBar = 0; HitChannelOnPlane = HitBar*2;}
        if (HitFebChannel == 45) {HitPlane = 1; HitBar = 1; HitChannelOnPlane = HitBar*2;}
        if (HitFebChannel == 46) {HitPlane = 1; HitBar = 18; HitChannelOnPlane = HitBar*2;}
        if (HitFebChannel == 47) {HitPlane = 1; HitBar = 19; HitChannelOnPlane = HitBar*2;}
    }
    }
    // std::cout << "This is (plane, bar): " << HitPlane << HitBar << "   ";
    HitEdge = HitChannelOnPlane%2;

    // singlebar
    if (HitDaqChannel == 216) {HitPlane = -1; HitBar = -1;}
    if (HitDaqChannel == 219) {HitPlane = -1; HitBar = -1;}
    if (HitDaqChannel == 228) {HitPlane = -1; HitBar = -1;}
    if (HitDaqChannel == 231) {HitPlane = -1; HitBar = -1;}
    if (HitDaqChannel == 242) {HitPlane = -1; HitBar = -1;}
    if (HitDaqChannel == 243) {HitPlane = -1; HitBar = -1;}
}

char TofHit::HitGetPlaneId(){
    if (HitPlane == 0) return 'U';
    else if (HitPlane == 1) return 'D';
    else if (HitPlane == 2) return 'T';
    else if (HitPlane == 3) return 'B';
    else if (HitPlane == 4) return 'L';
    else if (HitPlane == 5) return 'R';
    else{
        std::string this_error;
        this_error = "HitPlane variable has incorrect value\n";
        std::cerr << this_error << std::endl;
        HitErrorsList.push_back(this_error);
        return '!';
    }
}


void TofHit::HitGetHitInfo(){
    std::cout << "HitId: " << HitId << std::endl;
    std::cout << "HitFeb: " << HitFeb << std::endl;
    std::cout << "HitSampic: " << HitSampic << std::endl;
    std::cout << "HitFebChannel: " << HitFebChannel << std::endl;
    std::cout << "HitDaqChannel: " << HitDaqChannel << std::endl;
    std::cout << "HitChannelOnPlane: " << HitChannelOnPlane << std::endl;
    std::cout << "HitPlane: " << HitPlane << std::endl;
    std::cout << "HitBar: " << HitBar << std::endl;
    std::cout << "HitEdge: " << HitEdge << std::endl;
    std::cout << "HitUnixTime: " << HitUnixTime << std::endl;
    std::cout << "HitCell0Time: " << HitCell0Time << std::endl;
    std::cout << "HitTOTValue: " << HitTOTValue << std::endl;
    std::cout << "HitRawTOTValue: " << HitRawTOTValue << std::endl;
    std::cout << "HitRawPeak: " << HitRawPeak << std::endl;

    std::cout << "HitWaveform: ";
    for (auto elem : HitWaveform) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    std::cout << "HitBaseline: " << HitBaseline << std::endl;
    std::cout << "HitVoltageIntegral: " << HitVoltageIntegral << std::endl;
    std::cout << "HitPeak: " << HitPeak << std::endl;
    std::cout << "HitPeakSample: " << HitPeakSample << std::endl;
    std::cout << "HitPeakTime: " << HitPeakTime << std::endl;
    std::cout << std::endl;
}
