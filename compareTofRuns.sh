#!/bin/bash

inputDirectory="../../../../TofRootFiles/"

mkdir -p build
echo "Moving into build directory"
cd build;

# Compile the code
echo "Compiling the code"
cmake ..
make
cd src/app; # executable is here


# Check if the user has selected a run number. Need to move from just an interval to a list of runs to compare
if [ $# -eq 0 ]
then
  echo "Please select a run number (or an interval) from command line to create summary plots and compare. Usage: 
  $ ./compareTofRuns.sh run_number. or $ ./compareTofRuns.sh first_run_number last_run_number."
  exit 0
elif [ $# -eq 1 ]
then
  firstRun=$1
  lastRun=$1
elif [ $# -eq 2 ]
then
  firstRun=$1
  lastRun=$2
else
  echo "Please select a run number (or an interval) from command line to convert the data into ROOT format. Usage:
  $ ./compareTofRuns.sh run_number. or $ ./compareTofRuns.sh first_run_number last_run_number."
  exit 0
fi

# Iterate over all the selected runs
for ((runit = $firstRun; runit <= $lastRun; runit++ ))
do
  echo "runit: "$runit

  runFullPath=$(find $inputDirectory -name "run"$runit"_plots.root")
 
  echo "inputDirectory: "$inputDirectory
  echo "runFullPath: "$runFullPath

  # Stop execution if the selected run is not present in the input directory
  if [ "$runFullPath" == "" ]
  then
    echo "Run "$runit " not found. Stopping execution."
    continue
  fi

  mkdir -p $inputDirectory

  echo ""
  echo "Currently we are in"
  pwd
  echo ""
  
  ./compareTofRuns -r $runFullPath -o $inputDirectory

done