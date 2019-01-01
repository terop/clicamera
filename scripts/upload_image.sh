#!/bin/sh
# 2018-2019 Tero Paloheimo

# Script for uploading an image to a remote host.

set -e

usage() {
    cat >&2 <<EOF
This script uploads the given image to a remote host.
NOTE! The image is removed after uploading.

Settings are read from a file named 'upload_image_conf.sh' which must exist in the
same directory as this file.

Usage: $0 [-h] <image filename>
Flags:
-h    Print this message and exit
EOF
}

image=

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

image=$1
if [ -z "${image}" ]; then
    echo 'Image name must be provided, exiting.' >&2
    exit 1
fi

if [ ! -f "${image}" ]; then
    echo "Image file '${image}' not found, exiting." >&2
    exit 1
fi

# Check configuration file existence
conf_file='upload_image_conf.sh'
if [ ! -e ${conf_file} ]; then
    echo "Configuration file '${conf_file}' not found, exiting." >&2
    exit 1
fi
. ./${conf_file}

upload_image() {
    day_directory="${target_directory}/$(date +%Y-%m-%d)"
    conn_string="${target_user}@${target_host}"

    ssh ${conn_string} "mkdir -p ${day_directory}"

    image_date="$(basename ${image} .jpg)-$(date -Iminutes).jpg"
    scp "${image}" "${conn_string}:${day_directory}/${image_date}" 1>/dev/null
    if [ $? -ne 0 ]; then
        echo 'Image upload failed, exiting.' >&2
    fi
    rm "${image}"

    # Add image name to env-logger database
    if [ ${env_logger_integration} -ne 0 ]; then
        curl -s -S --data-urlencode "image-name=${image_date}" --data-urlencode "code=${auth_code}" \
             "${env_logger_url}/image"
    fi
}

upload_image
