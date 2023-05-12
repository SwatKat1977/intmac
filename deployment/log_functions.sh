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
