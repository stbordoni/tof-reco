#!/bin/bash

InputDirectory="/home/emanuele/Documents/ToF/sampic/results/" # set your own
# InputDirectory="$HOME/ResearchActivities/ND280Upgrade/ToF/Data/"

# Indicate Run and CH through command line, or write them here
if [ $# -eq 0 ]
then
  RunID=1
  ch=1
else
  RunID=$1
  ch=$2
fi

echo "RunID: "$RunID
echo "Channel:"$ch

InputFile=$(find $InputDirectory -type f -name "sampic_*_run"$RunID"_events.root")

echo "InputFile: "$InputFile

# Stop execution if the selected run is not present in the input directory
if [ "$InputFile" == "" ]
then
  echo "Run "$RunID " not found. Stopping execution."
else
  root -l 'WFDisplay.C("'$InputFile'", "'$RunID'", "'$ch'")'
fi