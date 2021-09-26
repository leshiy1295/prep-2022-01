#!/usr/bin/env bash

PROG_PATH=${1}  # ./main.out

if [ -z "$1" ]; then
    echo "Usage: run_btests.sh /path/to/main.out"
    exit 1
fi

for test in ./btests/*.tst; do
    source ./btests/_default
    source ${test}

    EXPECTED=${OUT}
    RECEIVED=$(echo "${IN}" | eval "${PROG_PATH} ${KEYS}")
    REC_STATUS=$?
    STATUS=${STATUS:-0}

    if [ "${REC_STATUS}" != "${STATUS}" ]; then
        echo -e "TEST ${test} FAILED. INVALID EXIT STATUS"\
                "\nEXPECTED:\n${STATUS}"\
                "\nRECEIVED:\n${REC_STATUS}"
        exit 1
    fi

    if [ -n "${EXPECTED}" ] && [ "${EXPECTED}" != "${RECEIVED}" ]; then
        echo -e "TEST ${test} FAILED"\
                "\nEXPECTED (${#EXPECTED} symbols):\n${EXPECTED}"\
                "\nRECEIVED (${#RECEIVED} symbols):\n${RECEIVED}"
        exit 1
    fi
    echo -n "."
done

echo "SUCCESS"
