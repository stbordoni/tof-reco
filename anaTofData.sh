#!/bin/bash

inputDirectory="../../../../TofData/" # we are in tof-reco/build/src/app, so we have to go up some levels
outputDirectory="../../../../TofRootFiles/"

mkdir -p build
echo "Moving into build directory"
cd build;

# Compile the code
echo "Compiling the code"
cmake ..
make
cd src/app; # executable is here


# Check if the user has selected a run number
if [ $# -eq 0 ]
then
  echo "Please select a run number (or an interval) from command line to convert the data into ROOT format. Usage: 
  $ ./anaTofData.sh run_number. or $ ./anaTofData.sh first_run_number last_run_number."
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
  $ ./anaTofData.sh run_number. or $ ./anaTofData.sh first_run_number last_run_number."
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
    if [ "$runFullPath" == "" ]
    then
      echo "Run "$runit " is not in text format, looking for MIDAS format."
      runFullPath=$(find $inputDirectory -name "run_0"$runit".gz")
    fi
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
  
  ./anaTofData -s $software -r $runFullPath -o $outputDirectory

done