#!/bin/bash

IMAGE_NAME=nouts/yakforms
YAKFORMS_VERSION=v1.1

docker build -t $IMAGE_NAME:$YAKFORMS_VERSION --build-arg VERSION=$YAKFORMS_VERSION -f Dockerfile .
docker build -t $IMAGE_NAME:$YAKFORMS_VERSION-fpm --build-arg VERSION=$YAKFORMS_VERSION -f Dockerfile_fpm .
docker build -t $IMAGE_NAME:$YAKFORMS_VERSION-fpm-alpine --build-arg VERSION=$YAKFORMS_VERSION -f Dockerfile_fpm_alpine .

#docker push $IMAGE_NAME:$YAKFORMS_VERSION $IMAGE_NAME:$YAKFORMS_VERSION-fpm $IMAGE_NAME:$YAKFORMS_VERSION-fpm-alpine
