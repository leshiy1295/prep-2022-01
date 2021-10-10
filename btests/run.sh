#!/usr/bin/env bash

PROG_PATH=${1}  # ./main.out
ROOT=$(dirname $(realpath $0))

CHECK_MEM_ARG="--memcheck"

VALGRIND_LOG="valgrind.log"
NO_LOST_PATTERN="All heap blocks were freed -- no leaks are possible"
NO_ERROR_PATTERN="ERROR SUMMARY: 0 errors"

RECORDS_F_NAME="record.dat"
TRANSACTIONS_F_NAME="transaction.dat"
BLACKRECORDS_F_NAME="blackrecord.dat"

function check_mem() {
	RECEIVED=$(echo "${1}" | eval "valgrind --tool=memcheck --leak-check=summary --log-file=${VALGRIND_LOG} ${PROG_PATH}")

	NO_LOST=$(grep "${NO_LOST_PATTERN}" ${VALGRIND_LOG})
	NO_ERROR=$(grep "${NO_ERROR_PATTERN}" ${VALGRIND_LOG})
	if [ -z "${NO_LOST}" ] || [ -z "${NO_ERROR}" ]; then
		echo -e "TEST ${test} FAILED\n"

		cat ${VALGRIND_LOG}
		echo -e "\n\nRUN: valgrind --tool=memcheck --leak-check=full ./main.out\n\n"

		exit 1
	fi
}


rm -f $RECORDS_F_NAME $TRANSACTIONS_F_NAME $BLACKRECORDS_F_NAME
touch $RECORDS_F_NAME $TRANSACTIONS_F_NAME $BLACKRECORDS_F_NAME

IN="1
1
name_1
surname_1
address_1
tel_1
80
150
20
2
name_2
surname_2
address_2
tel_2
100
200
59"

if [[ ${2} == ${CHECK_MEM_ARG} ]]; then
	check_mem "${IN}"
else
	echo "${IN}" | eval "${PROG_PATH}"
fi

IN="2
2
85
1
24"

if [[ ${2} == ${CHECK_MEM_ARG} ]]; then
	check_mem "${IN}"
else
	echo "${IN}" | eval "${PROG_PATH}"
fi

IN="3"

if [[ ${2} == ${CHECK_MEM_ARG} ]]; then
	check_mem "${IN}"
else
	echo "${IN}" | eval "${PROG_PATH}"
fi

for file in $RECORDS_F_NAME $TRANSACTIONS_F_NAME $BLACKRECORDS_F_NAME; do
	diff -uN $file $ROOT/${file}.gold
	if [[ "$?" != "0" ]]; then
		echo "Files mismatches"
		exit 1
	fi
done

echo "***** TEST SUCCEED *****"
