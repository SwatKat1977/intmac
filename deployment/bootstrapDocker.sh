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
   echo "s     Clean up Docker before running"
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
    internal_db_file=`pwd`/items_accounts_svc.db

    if [ -z "$GATEWAY_SVC_CONFIG" ] || [ ! -f $GATEWAY_SVC_CONFIG ] ; then
        warn "GATEWAY_SVC_CONFIG configuration item missing or file invalid"
        invalid_config=1
    fi
    export GATEWAY_SVC_CONFIG=$GATEWAY_SVC_CONFIG

    if [ -z "$ACCOUNTS_SVC_CONFIG" ] || [ ! -f $ACCOUNTS_SVC_CONFIG ] ; then
        warn "ACCOUNTS_SVC_CONFIG configuration item missing or file invalid"
        invalid_config=1
    fi
    export ACCOUNTS_SVC_CONFIG=$ACCOUNTS_SVC_CONFIG

    if [ "$ACCOUNTS_SVC_INTERNAL_DB" ] ; then
        internal_db_file=$ACCOUNTS_SVC_INTERNAL_DB
    fi
    export ACCOUNTS_SVC_INTERNAL_DB=$internal_db_file

    if [ -z "$PORTAL_SVC_CONFIG" ] || [ ! -f $PORTAL_SVC_CONFIG ] ; then
        warn "PORTAL_SVC_CONFIG configuration item missing or file invalid"
        invalid_config=1
    fi

    if [ $invalid_config -eq 1 ]; then
        die "Required configuration item(s) missing"
    fi

    msg "Configuration file is valid"
}

sweep_docker()
{
    msg "Performing Docker sweep"
    msg "======================="

    msg "Checking if Docket network 'items_docker_network' exists..."
    if [ "$(docker network ls | grep items_docker_network)" ]; then
        msg "Removing Docket network 'items_docker_network'."
        docker network rm items_docker_network
    fi

    msg "Checking status of accounts service container"
    if [ ! "$(docker ps | grep items_accounts_svc)" ]; then
        if [ "$(docker ps -aq -f name=items_accounts_svc)" ]; then
            # cleanup
            msg "Removing Docker container 'items_accounts_svc'"
            docker rm items_accounts_svc
        fi

    else
        die "Svc already running. (TODO)"
    fi
}

start_docker_images()
{
    msg "Creating Docker network 'items_docker_network' ..."
    docker network create --driver bridge items_docker_network || exit 1

    export ITEMS_ACCOUNTS_SVC_IMAGE="intmac/nightly:accounts-svc-$1"

    docker compose up

    msg "Starting Web Portal Service..."

    msg "Starting Accounts Service..."

    die "before"

    docker run -d \
        --name items_accounts_svc \
        --net items_docker_network \
        -p 5050:5000 \
        --volume=$ACCOUNTS_SVC_CONFIG:/usr/local/items/accounts_svc.config \
        items_accounts_svc || exit 1
}

config_file='items_deployment.config'

while getopts c:phr:s flag
do
    case "${flag}" in
        c) config_file=${OPTARG} ;;
        p) pull_images=1 ;;
        r) release=${OPTARG} ;;
        s) sweep=1 ;;
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

sweep_docker

start_docker_images $release
