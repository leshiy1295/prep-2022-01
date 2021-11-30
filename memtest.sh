#!/usr/bin/env bash

VALGRIND_LOG="valgrind.log"
NO_LOST_PATTERN="All heap blocks were freed -- no leaks are possible"
NO_ERROR_PATTERN="ERROR SUMMARY: 0 errors"

valgrind --tool=memcheck --leak-check=summary --log-file=${VALGRIND_LOG} "$@"
NO_LOST=$(grep "${NO_LOST_PATTERN}" "${VALGRIND_LOG}")
NO_ERROR=$(grep "${NO_ERROR_PATTERN}" "${VALGRIND_LOG}")
if [ -z "${NO_LOST}" ] || [ -z "${NO_ERROR}" ]; then cat "${VALGRIND_LOG}"; exit 1; fi
