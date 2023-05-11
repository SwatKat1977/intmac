#/bin/sh

############################################################
# Display information message (in green)                   #
############################################################
msg() {
    printf '\033[0;32m[INFO]\033[0m %s\n' "$*"
}

############################################################
# Display warning message (in yellow)                      #
############################################################
warn() {
        printf '\033[0;33m[WARN]\033[0m %s\n' "$*"
}

############################################################
# Display error message (in red) and exit)                 #
############################################################
die() {
    printf '\033[0;31m[ERROR]\033[0m %s\n' "$*" >&2
    exit 1
}

############################################################
# Display Help information                                 #
############################################################
help()
{
   echo "You will need to supply a release flag."
   echo
   echo "Syntax: pullDockerImages [-r|h]"
   echo "options:"
   echo "r     Release tag to use."
   echo "h     Print this help."
   echo
}

############################################################
# Pull Docker images                                       #
############################################################
function pull_docker_images() {
    msg "Pulling docker images from Docker Hub..."
    msg "=> Web Portal Service"
    docker pull --quiet intmac/nightly:web-portal-svc-$1 || exit 1
    msg "=> Gateway Service"
    docker pull --quiet intmac/nightly:gateway-svc-$1 || exit 1
    msg "=> Accounts Service"
    docker pull --quiet intmac/nightly:accounts-svc-$1 || exit 1
}

while getopts hr: flag
do
    case "${flag}" in
        r) release=${OPTARG} ;;
        h) 
            help 
            exit 0;;
        *)
           die '[ERROR] Invalid command line option'
    esac
done

if [ -z "$release" ]; then
    die "[ERROR] Release tag not supplied"
fi

msg "Using release tag '$release'"

pull_docker_images "$release"

exit 0
