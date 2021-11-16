#!/usr/bin/env bash

set -o xtrace

RUN_CLIENT_COMMAND=${1}
RUN_SERVER_COMMAND=${2}

SERVER_STARTED_PATTERN="Listening on port"

WORKING_DIRECTORY=${3}

MEMCHECK_FLAG="--memcheck"
WITH_MEMCHECK=${4-}

CLIENT_VALGRIND_LOG=${5-}
SERVER_VALGRIND_LOG=${6-}

HEAP_SUMMARY_PATTERN="HEAP SUMMARY:"
NO_LOST_PATTERN="All heap blocks were freed -- no leaks are possible"
NO_ERROR_PATTERN="ERROR SUMMARY: 0 errors"

CURRENT_SCRIPT_NAME=$(basename ${0})
RUNNING_SERVERS_PIDS=$(ps aux | grep '[s]erver.out' | grep -v "${CURRENT_SCRIPT_NAME}" | awk '{print $2}' | tr '\n' ' ')
echo ${RUNNING_SERVERS_PIDS}

if [ ! -z "${RUNNING_SERVERS_PIDS}" ]
then
    kill -9 ${RUNNING_SERVERS_PIDS} || true
fi
find "${WORKING_DIRECTORY}" -type f -name "task-*" -exec rm {} \; || true
find "${WORKING_DIRECTORY}" -type p -name "named-pipe-*" -exec rm {} \; || true

eval "${RUN_SERVER_COMMAND} &"
SERVER_PID=$!

echo "Waiting server to start (Looking for message in log \"${SERVER_STARTED_PATTERN}\"...)"

TIMEOUT=60

WAITED=0

while true
do
    if [ ${WAITED} -eq ${TIMEOUT} ]
    then
        echo "Server not started after ${TIMEOUT} seconds. Exiting"
        exit 1
    fi

    if grep -q -E "${SERVER_STARTED_PATTERN}" *server.out.log
    then
        break
    fi

    sleep 5
    WAITED=$((WAITED+5))
done

echo "****SERVER STARTED. RUNNING CLIENT TESTS*****"

eval ${RUN_CLIENT_COMMAND}

if [ $? -ne 0 ]
then
    echo "Client failed. Exiting"
    kill -s SIGINT ${SERVER_PID}
    exit 1
fi

echo "****CLIENT TESTS PASSED*****"

echo "Checking server stopping via SIGINT"
kill -s SIGINT ${SERVER_PID}

WAITED=0

while true
do
    if [ $WAITED -eq ${TIMEOUT} ]
    then
        echo "Server not stopped after ${TIMEOUT} seconds. Exiting"
        exit 1
    fi

    if ! ps -p ${SERVER_PID} > /dev/null
    then
        break
    fi

    sleep 5
    WAITED=$((WAITED+5))
done

echo "Checking working directory"
if find "${WORKING_DIRECTORY}" -type f -name "task-*" | grep -v 'Permission denied'
then
    echo "Task files remained unremoved. Exiting"
    find "${WORKING_DIRECTORY}" -type f -name "task-*" -exec rm {} \; | grep -v 'Permission denied'
    exit 1
fi

if find "${WORKING_DIRECTORY}" -type p -name "named-pipe-*" | grep -v 'Permission denied'
then
    echo "Task files remained unremoved. Exiting"
    find "${WORKING_DIRECTORY}" -type p -name "named-pipe-*" -exec rm {} \; | grep -v 'Permission denied'
    exit 1
fi

echo "Checking running server processes"
ps aux | grep '[s]erver.out' | grep -v "${CURRENT_SCRIPT_NAME}"
RUNNING_SERVERS=$(ps aux | grep '[s]erver.out' | grep -v "${CURRENT_SCRIPT_NAME}")
if [ -n "${RUNNING_SERVERS}" ]
then
    RUNNING_SERVERS_PIDS=$(echo ${RUNNING_SERVERS} | awk '{print $2}' | tr '\n' ' ')
    echo $RUNNING_SERVERS_PIDS
    kill -9 ${RUNNING_SERVERS_PIDS} || true
    echo "Running server processes detected. Exiting"
    exit 1
fi

echo "Checking zombies"
ps -o pid,ppid,s | grep -E 'Z'
ZOMBIES=$(ps -o pid,ppid,s | grep -E 'Z')
if [ -n "${ZOMBIES}" ]
then
    RUNNING_ZOMBIES=$(echo ${ZOMBIES} | awk '{print $2}' | tr '\n' ' ')
    kill -9 ${RUNNING_ZOMBIES} || true
    echo "Running zombies processes detected. Exiting"
    exit 1
fi

if [[ "${WITH_MEMCHECK}" == "${MEMCHECK_FLAG}" ]]
then
    echo "SERVER_VALGRIND_LOG = ${SERVER_VALGRIND_LOG}"

    HEAP_SUMMARY_COUNT=$(grep "${HEAP_SUMMARY_PATTERN}" "${SERVER_VALGRIND_LOG}" | wc -l)
    NO_LOST_COUNT=$(grep "${NO_LOST_PATTERN}" "${SERVER_VALGRIND_LOG}" | wc -l)
    NO_ERROR_COUNT=$(grep "${NO_ERROR_PATTERN}" "${SERVER_VALGRIND_LOG}" | wc -l)

    echo "SERVER VALGRIND -> Found ${HEAP_SUMMARY_COUNT} reports"

    if [[ "${HEAP_SUMMARY_COUNT}" != "${NO_LOST_COUNT}" ]] || [[ "${HEAP_SUMMARY_COUNT}" != "${NO_ERROR_COUNT}" ]]
    then
        echo "Server valgrind failed. Exiting"
        cat "${SERVER_VALGRIND_LOG}";
        exit 1;
    fi

    echo "CLIENT_VALGRIND_LOG = ${CLIENT_VALGRIND_LOG}"

    HEAP_SUMMARY_COUNT=$(grep "${HEAP_SUMMARY_PATTERN}" "${CLIENT_VALGRIND_LOG}" | wc -l)
    NO_LOST_COUNT=$(grep "${NO_LOST_PATTERN}" "${CLIENT_VALGRIND_LOG}" | wc -l)
    NO_ERROR_COUNT=$(grep "${NO_ERROR_PATTERN}" "${CLIENT_VALGRIND_LOG}" | wc -l)

    echo "CLIENT VALGRIND -> Found ${HEAP_SUMMARY_COUNT} reports"

    if [[ "${HEAP_SUMMARY_COUNT}" != "${NO_LOST_COUNT}" ]] || [[ "${HEAP_SUMMARY_COUNT}" != "${NO_ERROR_COUNT}" ]]
    then
        echo "Client valgrind failed. Exiting"
        cat "${CLIENT_VALGRIND_LOG}";
        exit 1;
    fi
fi

echo "****TESTS SUCCEED*****"
