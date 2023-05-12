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

############################################################
# Verify the configuration file content is valid           #
############################################################
verify_configuration_file()
{
    if [ ! -f $1 ]; then
        die "Configuration file '$1' not found!"
    fi

    source $1
    invalid_config=0

    if [ -z "$GATEWAY_SVC_CONFIG" ] || [ ! -f $GATEWAY_SVC_CONFIG ] ; then
        warn "GATEWAY_SVC_CONFIG configuration item missing or file invalid"
        invalid_config=1
    fi

    if [ -z "$ACCOUNTS_SVC_CONFIG" ] || [ ! -f $ACCOUNTS_SVC_CONFIG ] ; then
        warn "ACCOUNTS_SVC_CONFIG configuration item missing or file invalid"
        invalid_config=1
    fi

    if [ -z "$PORTAL_SVC_CONFIG" ] || [ ! -f $PORTAL_SVC_CONFIG ] ; then
        warn "PORTAL_SVC_CONFIG configuration item missing or file invalid"
        invalid_config=1
    fi

    if [ $invalid_config -eq 1 ]; then
        die "Required configuration item(s) missing"
    fi

    msg "Configuration file is valid"
}

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
           die 'Invalid command line option'
    esac
done

msg "Configuration file : $config_file"

if [ "$pull_images" ] && [ -z "$release" ]; then
    die "Release tag required with pull images option"
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

verify_configuration_file $config_file