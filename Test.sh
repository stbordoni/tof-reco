#!/bin/bash

# inputDirectory="/home/emanuele/Documents/ToF/linuxData/"
# inputDirectory="/home/emanuele/Documents/ToF/sampic/data/" # CHANGE LATER
outputDirectory="/home/emanuele/Documents/ToF/sampic/NEWresults/"

echo "Compiling and linking the program..."

make clean; # comment this line if you don't want to clean the directory
make;

if [ $# -eq 0 ]
then
  echo "Please select a run number from command line to convert the data into ROOT format. Usage: 
  $ ./GenerateRootFile.sh run_number."
  exit 0
fi

firstRun=$1
lastRun=$1

# Iterate over all the selected runs
for ((runit = $firstRun; runit <= $lastRun; runit++ ))
do
  echo "runit: "$runit

  if [ $runit -ge 1000 ]
  then
    software="linux"
    inputDirectory="/home/emanuele/Documents/ToF/linuxData/"
    runFullPath=$(find $inputDirectory$"linux_run"$runit".txt")
  else
    software="windows"
    inputDirectory="/home/emanuele/Documents/ToF/sampic/data/"
    runFullPath=$(find $inputDirectory -type d -name "sampic_*_*h*min_run"$runit)
  fi
  
  echo "inputDirectory: "$inputDirectory
  echo "This run was taken using the "$software" software"
  echo "runFullPath: "$runFullPath
  echo "outputDirectory: "$outputDirectory

  # Stop execution if the selected run is not present in the input directory
  if [ "$runFullPath" == "" ]
  then
    echo "Run "$runit " not found. Stopping execution."
    continue
  fi

  eventsFile=$outputDirectory"run"$DataName"_events.root"
  mkdir -p $outputDirectory
  ./main $software $runFullPath $outputDirectory

done