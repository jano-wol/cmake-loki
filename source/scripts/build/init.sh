#!/bin/bash
set -ex
if [[ $1 != "debug" ]] && [[ $1 != "debug_thread" ]] && [[ $1 != "release" ]]; then
    echo "First command line argument should be debug or debug_thread or release"
    exit 1
fi	
CMAKE_BUILD_TYPE=$1
SCRIPT_FOLDER=$(dirname "${0}") 
WORKSPACE_FOLDER=$(readlink -e "${SCRIPT_FOLDER}/../../")
BUILD_FOLDER="${WORKSPACE_FOLDER}/build/${CMAKE_BUILD_TYPE}"
CMAKE_CXX_COMPILER=g++
CMAKE_MAKE_PROGRAM=ninja
