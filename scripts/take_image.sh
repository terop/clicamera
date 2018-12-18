#!/bin/sh
# 2018 Tero Paloheimo

# Script for taking a image with clicamera over a SSH connection.

set -e

usage() {
    cat >&2 <<EOF
This script takes a image with clicamera and and stores it as a JPEG file
in the current working directory.
Prerequisites:
1) Passwordless SSH access
2) clicamera binary in the users home directory

Usage: $0 [-h] <device address for SSH> <filename>
Flags:
-h    Print this message and exit
EOF
}

device_address=
filename=

while getopts 'h' OPTION; do
    case "${OPTION}" in
    h)
        usage
        exit 1
        ;;
    *)
        echo "Invalid option: ${OPTION}"
        exit 1
  esac
done
shift "$((OPTIND - 1))"

device_address=$1
if [ -z "${device_address}" ]; then
    echo 'Device address must be provided, exiting.' >&2
    exit 1
fi

filename=$2
if [ -z "${filename}" ]; then
    echo 'Filename must be provided, exiting.' >&2
    exit 1
fi

if [ -z "$(ssh ${device_address} ls)" ]; then
    echo "Could not connect to device '${device_address}', exiting." >&2
    exit 1
fi

if [ -z "$(ssh ${device_address} 'ls clicamera')" ]; then
    echo 'clicamera not found on device, exiting.' >&2
    exit 1
fi

# Take image
ssh ${device_address} "./clicamera ${filename}" >/dev/null 2>&1
# Copy image from device
scp "${device_address}:${filename}" . >/dev/null
# Remove image on device
ssh ${device_address} "rm ${filename}" >/dev/null 2>&1
