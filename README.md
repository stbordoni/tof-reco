# Reconstruction code for SAMPIC data
<!--comment -->

## Instructions to set up the environment

Follow these instructions to set up in your local machine. 
You need to have ROOT installed in the standard location, meaning `/usr/local/`, cmake (minimum 3.20) and c++ (gcc version at least 11, or a recent clang) meeting the requirements. 
If that's not the case, update them.

```
git clone https://git.t2k.org/tof_utils/tof-reco.git
cd tof-reco
./update.sh --up
```
Then, these operations are already done in the script to analyze the data, but if you just want to compile the code to check if it's ok on your machine, do:

```
mkdir build; 
cd build; 
cmake ..;
make;
```

## Analyze data, create ROOT output file
Run `./readTofData.sh run_number`
where `run_number` clearly is the number of the run to analyze.
The standard folder where the run is `../TofData`, and the results are in
`../TofRootFiles`.
If you want to set different folders (not recommended), change the path in `readTofData.sh`.

The data can be found at this link: https://cernbox.cern.ch/s/4G9V1T8z8YgbUdE, you can just download the whole folder or just some runs.

In `AnalysisSettings.json` there are some parameters to be changed during the analysis, names should be self-explicative.

The output will be in the format `runXXX_plots.root`, containing histos per channel and some plots for the whole run. 
To change or add more, go to `src/app/ReadTofData.cpp`.

# Description of the classes

There are getters for all the variablesm which are set to private. 
To know in detail what they do, check in `include/` and `src/`, but here there are some words about the key points.

## TofHit
This contains all the information about the hit. 

In particular note that the mapping for HitPlane is U=0, D=1, T=2, B=3, L=4, R=5, while HitBar = 0,...,19 and HitEdge=0,1, where for the moment 0 is left and 1 is right.

In ReadRootFile, use as condition `if(GetHitFitSuccess)`, to be sure that the waveform was good and the fit was successful. 
For the TOT, use `GetTotValue`.
To get the time associated to a certain CF, use `HitComputeCfTime()`. 
If `GetHitFitSuccess` is false, instead of a fit a linear interpolation is performed.
To select saturated hits, use `GetHitIsSaturated()`.

## TofSignal
This contains either one or two hits, called HitMin and HitMax (for the moment, HitMin is the L edge and HitMax is the R edge). 

There is a variable `SignalType`:

= 1: only HitMin 

= 2: only HitMax

= 3: both 

If `SignalType = 3` and `HitFitSuccess =  true` for both hits, a `SignalPosition` is computed. 
If `SignalPosition` falls within 0 and 220 cm, then also a `SignalTime`  is computed as         
``SignalTime = SignalHitMin.HitComputeCfTime(0.1) - SignalPosition/light_velocity;``. 
Inizialitation values are -1.

## TofEvent
This is a collection of Signals that fall within a 50ns coincidence window, matching the physics calculation and the DAQ features.

If the Event contains two Signals and for both there is a valid `SignalTime`, then `EventTimeOfFlight` is computed as the difference between the two SignalTimes. 
Inizialitation value is 0.

## TofRun
TofRun contains all the information regarding a run and does all the operations in the analysis process. 

There is a method `GetRunInfo()` that prints out the information about the run, if needed.
