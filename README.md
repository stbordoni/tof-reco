#

# Analysis and reconstruction from SAMPIC data

<a name ="analysis">

## Analyse a Run

With the script `RunSAMPICReconstructionAndAnalysis.sh` one can quickly analyse the data coming from a run:
- Set the parent directory and where to put the input and output files.
- Just change the number of the run to analyse inside the script. It is possible to analyse more than one consecutive run at the same time.
- Select if to run only the reconstruction or also the analysis. Three parameters can be passed from command line:

`./RunSAMPICAnalysisAndReconstruction argv[1] argv[2] argv[3]`

- argv[1] = 1 executes the hit reconstruction and reorders the hits in time with `ReadAndReconstructSingleRun.C` and `OrderHits.C`, while 0 or other values don't do anyting.
- argv[2] = 1 executes `GroupHits.C` (groups Hits in Events), while 0 or other values don't do anything. This is normally the longest step.
- argv[3] = 1 executes `ReadToFEvents.C`, that is the high level analysis of the events.

If no parameter is passed from command line, it automatically runs in 1 1 1. Note that once the reconstruction and the grouping are done, it is not necessary to redo them before executing the analysis (unless changes have been made). 

Check the last lines of the script to personalize the execution. The flag -b stops root display, -q quits root after the end of execution, so if one wants to see the root display they have to remove it.

<a name="wfdisplay">

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

One can also set these two parameters directly inside the script and launch it without parameters passed from command line.