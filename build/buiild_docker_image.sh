#!/bin/bash

helpFunction()
{
   echo "Usage: $0 -b <branch> -c <compose file> -t <image tag>"
   echo "\t-b GIT Branch"
   echo "\t-c Componant"
   exit 1
}

while getopts ":b:c:" opt
do
   case "$opt" in
      b ) gitBranch="$OPTARG" ;;
      c ) componant="$OPTARG" ;;
      : ) echo "Invalid option: -$OPTARG requires an argument" 1>&2
          exit 1 ;;
      * ) helpFunction ;;
   esac
done

if [ -z "$gitBranch" ]
then
   echo "[ERROR] gitBranch (-b) parameter was not specified";
   helpFunction
fi

if [ -z "$componant" ]
then
   echo "[ERROR] componant (-c) parameter was not specified";
   helpFunction
fi

composeFile="docker/Dockerfile.$componant"
if [ ! -f $composeFile ]; then
   echo "[ERROR] Docker compose file $composeFile does not exist"
   exit 1
fi

version=$(python3 build/find_branch.py $gitBranch)
if [ -z "$version" ]
then
   echo "[ERROR] Unable to make docker tag"
   exit 1
fi

dockerItem="items-$componant"
fullTag="paulmorriss/$dockerItem:$version"

echo "Building docker image..."
echo "Build Parameters:"
echo "\tcompose file    : $composeFile"
echo "\tgit branch      : $gitBranch"
echo "\tfull docker tag : $fullTag"
echo "\tdocker item     : $dockerItem"

docker build \
   --file $composeFile \
   --label items.git_branch="$gitBranch" \
   --label items.git_commit="GIT_COMMIT" \
   --tag "$fullTag" \
   .

echo "CREDS = $registryCredential"
docker push $fullTag
