#!/bin/bash

helpFunction()
{
   echo "Usage: $0 -b <branch> -c <compose file> -t <image tag>"
   echo "\t-b GIT Branch"
   echo "\t-c Docker compose file"
   exit 1
}

while getopts ":b:c:" opt
do
   case "$opt" in
      b ) gitBranch="$OPTARG" ;;
      c ) composeFile="$OPTARG" ;;
      : ) echo "Invalid option: -$OPTARG requires an argument" 1>&2
          exit 1 ;;
      * ) helpFunction ;;
   esac
done

if [ -z "$gitBranch" ]
then
   echo "gitBranch (-b) parameter was not specified";
   helpFunction
fi

if [ -z "$composeFile" ]
then
   echo "composeFile (-c) parameter was not specified";
   helpFunction
fi

va=$(python build/find_branch.py $gitBranch)
echo "va is $va"

if [ -z "$va" ]
then
      echo "\$va is empty"
      exit 1
fi
