# NEW Reconstruction code for SAMPIC data

## Instructions to set up the environment

After cloning this repository in your machine, do `mkdir build; cd build; cmake ..; make`, to see if your system meets the requirements.
You need to have ROOT installed in the standard location, meaning `/usr/local/`, and cmake (minimum 3.20) and c++ (gcc version at least 11) meeting the requirements. 
If that's not the case, update them or create an environment.
You also need to have nlohmann/json installed, instructions \hyperlink{https://github.com/nlohmann/json}.

The first time you use the code, you have to initialize the use of the submodules with `git submodule init` and `git submodule update`.

To update the code to the latest version, do `git pull origin master` in the main folder. 
Also regularly update the submodules with `git submodule update --init --recursive --remote`.


## Generate ROOT file
An easy way to generate the ROOT file for a run is by running `./GenerateRootFile.sh run_number`, where `run_number` clearly is the number of the run to analyze.
The standard folder where the run is is `../TofData`, and the results are in `../TofRootFiles`.
If you want to set different folders, change the path in `GenerateRootFile.sh`.

In `AnalysisSettings.json` there are some parameters to be changed during the analysis. 
For now, the only one to care about is `RunMaxHitsToLoad`, since some runs are very long.

## Read ROOT file
To read the ROOT file the method is basically the same:
run `./ReadRootFile.sh run_number`. 
You can change what is printed or do other operations changing `ReadRootFile.cpp`.
A file `run*_plots.root` is generated, containing all the histograms produced in the code.


# Description of the classes

There are getters for all the variablesm which are set to private. 
To know in detail what they do, check in `include/` and `src/`, but here there are some words about the key points.

## TofHit
This contains all the information about the hit. 

In particular note that the mapping for HitPlane is U=0, D=1, T=2, B=3, L=4, R=5, while HitBar = 0,...,19 and HitEdge=0,1, where for the moment 0 is left and 1 is right.

In ReadRootFile, use as condition `if(GetHitFitSuccess)`, to be sure that the waveform was good and the fit was successful. 
For the TOT, use `GetTotValue`.
To get the time associated to a certain CF, use `HitComputeCfTime()`.
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

The output file contains a `TofRun` object itself. 

<!-- 
THE FOLLOWING OPTIONS ARE NOT HERE YET FOR THE NEW FORMAT, BUT WILL BE

## Display waveforms
The program `WFDisplay.C` displays the waveforms of a specific Run, of a specific channel. It can be made smarter to accept more than one channel at the time.
To run it, there is a simple bash script in this case as well: `DisplayWaveforms.sh`.
It is necessary to have generated the file `*_events.root`, and to write its mother directory in the script.
`DisplayWaveforms.sh` accepts two parameters from command line:

- argv[1] is the Run to analyse (`int`)
- argv[2] is the Channel of which waveforms will be displayed (`int`).

One can also set these two parameters directly inside the script and launch it without parameters passed from command line.
If the selected channel has triggered in the selected run, all waveforms will be displayed one at the time in a Canvas. 
The amplitude of the waveforms and the number of hits in that events will be printed out in the terminal.
The Y range can be set inside `WFDisplay.C`. 
Once the program is in execution, double click on the Canvas or single click on the axis to go to the following Waveform
You can also change the value of a bool to avoid having to click to display the following waveform; all will be shown quickly. 
This is useful to store all the waveforms of a channel in a quick way. They are then stored in `wfs.csv`.

## Display Events
The program `EventDisplay.C` displays the bars thata are triggered in a specific Run, in a specific Event. 
The option to display more events changing by clicking on the canvas will be added
To run it, there is a simple bash script in this case as well: `DisplayEvents.sh`.
It is necessary to have generated the file `*_events.root`, and to write its mother directory in the script.
`DisplayEvent.sh` accepts two parameters from command line:

- argv[1] is the Run to analyse (`int`)
- argv[2] is the Event to displat (`int`).

One can also set these two parameters directly inside the script and launch it without parameters passed from command line. -->