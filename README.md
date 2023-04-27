# NEW Reconstruction code for SAMPIC data

We are now using a proper framework. 
After cloning this repository in your machine, do `mkdir build; cd build; cmake ..; make`, to see if your system meets the requirements.
You need to have ROOT installed in the standard location, meaning `/usr/local/`, and cmake and c++ meeting the requirements. 
If that's not the case, update them. 

## Generate ROOT file
An easy way to generate the ROOT file for a run is by running `./GenerateRootFile.sh run_number`, where `run_number` clearly is the number of the run to analyze.
The standard folder where the run is is `../TofData`, and the results are in `../TofRootFiles`.
If you want to set different folders, change the path in `GenerateRootFile.sh`.

## Read ROOT file
To read the ROOT file the method is basically the same:
run `./ReadRootFile.sh run_number`. 
You can change what is printed or do other operations changing `ReadRootFile.cpp`.


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