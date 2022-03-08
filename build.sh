#!/usr/bin/env bash

set -e

mkdir -p build
cd build
cmake "$@" ..
cmake --build .
