#/bin/bash

source $(dirname $0)/log_functions.sh

############################################################
# Display Help information                                 #
############################################################
help()
{
   echo "You will need to supply a release flag."
   echo
   echo "Syntax: bootstrapDocker [-p|r|h]"
   echo "options:"
   echo "p     Pull images - requires 'r' option."
   echo "r     Release tag to use."
   echo "h     Print this help."
   echo
}

echo 'gateway svc config'
echo 'accounts svc config'
echo 'web portal svc config'

config_file='items_deployment.config'

while getopts c:phr: flag
do
    case "${flag}" in
        c) config_file=${OPTARG} ;;
        p) pull_images=1 ;;
        r) release=${OPTARG} ;;
        h) 
            help 
            exit ;;
        *)
           die '[ERROR] Invalid command line option'
    esac
done

echo "Configuration file : $config_file"

if [ "$pull_images" ] && [ -z "$release" ]; then
    die "[ERROR] Release tag required with pull images option"
fi

if [ "$pull_images" ]; then
    msg "Pulling Docker images from Docker Hub with release tag '$release'"

    ./$(dirname $0)/pullDockerImages.sh -r $release
    pull_return_value=$?
    if [ $pull_return_value -eq 1 ]; then
        die "Unable to pull Docker images, deployment aborted"
    fi

    msg "Docker images successfully pulled..."
fi
