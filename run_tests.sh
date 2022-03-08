#!/usr/bin/env bash

set -e

cd build || (echo "Run ./build.sh [<args>] at first" && exit 1)

WITH_EXTRA=0

while true; do
    case "$1" in
        --with-extra )
            WITH_EXTRA=1
            shift
            ;;
        -- )
            shift
            break
            ;;
        * )
            break
            ;;
    esac
done

echo "Running base tests"
ctest -V -R Base

if [[ "${WITH_EXTRA}" == "1" ]]; then
    echo "Running extra tests"
    ctest -V -R Extra
fi
