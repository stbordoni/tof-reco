#!/bin/bash

inputDirectory="../../TofData/" # CHANGE LATER
outputDirectory="../../TofRootFiles/"

mkdir -p build
echo "Moving into build directory"
cd build;

# Compile the code
echo "Compiling the code"
cmake ..
make


# Check if the user has selected a run number
if [ $# -eq 0 ]
then
  echo "Please select a run number (or an interval) from command line to convert the data into ROOT format. Usage: 
  $ ./readTofData.sh run_number. or $ ./readTofData.sh first_run_number last_run_number."
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
  $ ./readTofData.sh run_number. or $ ./readTofData.sh first_run_number last_run_number."
  exit 0
fi

# Iterate over all the selected runs
for ((runit = $firstRun; runit <= $lastRun; runit++ ))
do
  echo "runit: "$runit

  if [ $runit -ge 1000 ]
  then
    software="linux"
    # inputDirectory="/home/emanuele/Documents/ToF/linuxData/"
    runFullPath=$(find $inputDirectory -name "sampic_*_*_run"$runit".txt")
  else
    software="windows"
    # inputDirectory="/home/emanuele/Documents/ToF/sampic/data/"
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

  mkdir -p $outputDirectory
  
  echo ""
  echo "Currently we are in"
  pwd
  echo ""
  
  ./readTofData $software $runFullPath $outputDirectory

done