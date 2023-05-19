#!/bin/bash

# change if you desire to use a different input directory
inputDirectory="../../TofRootFiles/" 

echo "Moving into build directory"
cd build;

# Compile the code
echo "Compiling the code"
cmake ..
make


# Check if the user has selected a run number
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
  echo "inputDirectory: "$inputDirectory
  # ls $inputDirectory
  runFullPath=$(find $inputDirectory$"run"$runit".root")
  echo "runFullPath: "$runFullPath

  # Stop execution if the selected run is not present in the input directory
  if [ "$runFullPath" == "" ]
  then
    echo "Run "$runit " not found. Stopping execution."
    continue
  fi

  if [ $runit -ge 1000 ]
  then
    software="linux"
  else
    software="windows"
  fi
  
  echo "./ReadRootFile $software $runFullPath"
  ./ReadRootFile $software $runFullPath

done