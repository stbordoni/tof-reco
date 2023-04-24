#include "TofRun.h"
#include "TofEvent.h"
#include "TofHit.h"
#include "TofSignal.h"

std::vector<std::string> SplitString(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {elems.push_back(item);}
    return elems;
}

TofHit::TofHit(){
    // initialize all variables to recognizable value
    HitId = -1;
    HitFeb = -1;
    HitSampic = -1;
    HitFebChannel = -1;
    HitDaqChannel = -1;
    HitChannelOnPlane = -1;
    HitPlane = -1;
    HitBar = -1;
    HitEdge = -1;
    HitPeakSample = -1;

    HitUnixTime = -1.0;
    HitCell0Time = -1.0;
    HitTOTValue = -1.0;
    HitRawTOTValue = -1.0;
    HitRawPeak = -1.0;
    HitBaseline = -1.0;
    HitVoltageIntegral = -1.0;
    HitPeak = -1.0;
    HitPeakTime = -1.0;

    HitWaveform = {};
    HitErrorsList = {};
}



double TofHit::HitFitWaveform(){

//  Double_t arg = 0;
//  double sigm = 0;

//  if( x[0] < HitFitParameter[1] ) 
//    sigm = TMath::Abs(HitFitParameter[2])*(x[0]-HitFitParameter[1])+HitFitParameter[3]; 
//  else 
//    sigm = TMath::Abs(HitFitParameter[4])*(x[0]-HitFitParameter[1])+HitFitParameter[3]; 
 
//  // sigm = HitFitParameter[2]*(x[0]-HitFitParameter[1])+HitFitParameter[3]; 
 
//  arg = (x[0] - HitFitParameter[1])/sigm;
//  Double_t fitval = HitFitParameter[0]*TMath::Exp(-0.5*arg*arg)+HitFitParameter[5];
//  return fitval;

}

void TofHit::HitComputeCf(){

//   double mx =0; 
//   int imx = 0;

//   for(int i =0; i < HitWaveform.size(); i++ ) {
//     if( h->GetBinContent(i+1) > mx ) {mx = h->GetBinContent(i+1); imx = i;}
//   }

//   TF1 func("fit",fitf,0,62,6);
      
//   func.SetParameter(0,mx);
//   func.SetParameter(1,imx);
//   func.SetParameter(2,0.2);
//   func.SetParameter(3,13.);
//   func.SetParameter(4,0.2);
//   func.SetParameter(5,0.0);
  
//   h->Fit("fit","","",imx-30,imx+10);

//   for( int i = 0; i < 6; i++ ) 
//     HitFitParameter[i] = func.GetParameter(i); 
  

//   return func.GetX(th*func.GetParameter(0)+func.GetParameter(5));
 
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
