CALC_SRCS=calc/*.cpp
CALC_HEADERS=calc/*.hpp
UTILS_HEADERS=net/common/*.hpp
SERVER_SRCS=net/server/*.cpp
SERVER_HEADERS=net/server/*.hpp
CLIENT_SRCS=net/client/*.cpp
CLIENT_HEADERS=net/client/*.hpp
CALC_TEST_GENERATOR_SRCS=tests/tests_generator.cpp
CALC_TEST_GENERATOR_HEADERS=tests/tests_generator.hpp
CLIENT_TEST_SRCS=tests/test_client.cpp
SERVER_TEST_SRCS=tests/test_server.cpp

SERVER_TARGET=./server.out
CLIENT_TARGET=./client.out

TEST_CLIENT_TARGET=./test_client.out
TEST_SERVER_TARGET=./test_server.out

TEST_EXTRA_CLIENT_TARGET=./test_extra_client.out
TEST_EXTRA_SERVER_TARGET=./test_extra_server.out

VALGRIND=valgrind --tool=memcheck --leak-check=summary --leak-check=full --show-leak-kinds=all --track-origins=yes

CLIENT_VALGRIND_LOG='client.valgrind.log'
EXTRA_CLIENT_VALGRIND_LOG='extra.client.valgrind.log'
SERVER_VALGRIND_LOG='server.valgrind.log'
EXTRA_SERVER_VALGRIND_LOG='extra.server.valgrind.log'

SERVER_PORT ?= 8080
WORKING_DIRECTORY ?= /tmp
MAX_WORKERS_COUNT ?= 1

EXTRA_MAX_WORKERS_COUNT=5

.PHONY: clean check build build_tests build_extra_tests build_client run_client build_server run_server build_test_client run_test_client build_test_server run_test_server build_extra_test_client run_extra_test_client build_extra_test_server run_extra_test_server run_tests run_extra_tests run_memtests run_extra_memtests

all: clean check build build_tests run_tests run_memtests build_extra_tests run_extra_tests run_extra_memtests

run_server: build_server
	${SERVER_TARGET} ${SERVER_PORT} ${WORKING_DIRECTORY} ${MAX_WORKERS_COUNT} 2>&1 | tee ${SERVER_TARGET}.log

build_server: ${SERVER_TARGET}

${SERVER_TARGET}: ${SERVER_SRCS} ${SERVER_HEADERS} ${CALC_SRCS} ${CALC_HEADERS} ${UTILS_HEADERS}
	g++ -DDEBUG -std=c++17 -g -fdiagnostics-color -Wall -Wextra -Wpedantic -Werror ${SERVER_SRCS} ${CALC_SRCS} -I. -pthread -lstdc++fs -o ${SERVER_TARGET}

build_client: ${CLIENT_TARGET}

run_client: build_client
	${CLIENT_TARGET} 2>&1 | tee ${CLIENT_TARGET}.log

${CLIENT_TARGET}: ${CLIENT_SRCS} ${CLIENT_HEADERS} ${CALC_TEST_GENERATOR_SRCS} ${CALC_TEST_GENERATOR_HEADERS} ${UTILS_HEADERS}
	g++ -DDEBUG -std=c++17 -g -fdiagnostics-color -Wall -Wextra -Wpedantic -Werror ${CLIENT_SRCS} ${CALC_TEST_GENERATOR_SRCS} -I. -pthread -lstdc++fs -o ${CLIENT_TARGET}

build_test_client: ${TEST_CLIENT_TARGET}

run_test_client: build_test_client
	${TEST_CLIENT_TARGET} 2>&1 | tee ${TEST_CLIENT_TARGET}.log

build_extra_test_client: ${TEST_EXTRA_CLIENT_TARGET}

run_extra_test_client: build_extra_test_client
	${TEST_EXTRA_CLIENT_TARGET} 2>&1 | tee ${TEST_EXTRA_CLIENT_TARGET}.log

${TEST_EXTRA_CLIENT_TARGET}: ${CLIENT_TEST_SRCS} ${CALC_TEST_GENERATOR_SRCS} ${CALC_TEST_GENERATOR_HEADERS} ${CALC_SRCS} ${CALC_HEADERS} ${CLIENT_SRCS} ${CLIENT_HEADERS} ${UTILS_HEADERS}
	g++ -DEXTRA -DTESTS_ENABLED -DDEBUG -std=c++17 -g -fdiagnostics-color -Wall -Wextra -Wpedantic -Werror ${CLIENT_TEST_SRCS} ${CALC_TEST_GENERATOR_SRCS} ${CALC_SRCS} ${CLIENT_SRCS} -I. -pthread -lstdc++fs -o ${TEST_CLIENT_TARGET}

${TEST_CLIENT_TARGET}: ${CLIENT_TEST_SRCS} ${CALC_TEST_GENERATOR_SRCS} ${CALC_TEST_GENERATOR_HEADERS} ${CALC_SRCS} ${CALC_HEADERS} ${CLIENT_SRCS} ${CLIENT_HEADERS} ${UTILS_HEADERS}
	g++ -DTESTS_ENABLED -DDEBUG -std=c++17 -g -fdiagnostics-color -Wall -Wextra -Wpedantic -Werror ${CLIENT_TEST_SRCS} ${CALC_TEST_GENERATOR_SRCS} ${CALC_SRCS} ${CLIENT_SRCS} -I. -pthread -lstdc++fs -o ${TEST_CLIENT_TARGET}

build_test_server: ${TEST_SERVER_TARGET}

run_test_server: build_test_server
	${TEST_SERVER_TARGET} ${SERVER_PORT} ${WORKING_DIRECTORY} ${MAX_WORKERS_COUNT} 2>&1 | tee ${TEST_SERVER_TARGET}.log

build_extra_test_server: ${TEST_EXTRA_SERVER_TARGET}

run_extra_test_server: build_extra_test_server
	${TEST_EXTRA_SERVER_TARGET} ${SERVER_PORT} ${WORKING_DIRECTORY} ${EXTRA_MAX_WORKERS_COUNT} 2>&1 | tee ${TEST_EXTRA_SERVER_TARGET}.log

${TEST_EXTRA_SERVER_TARGET}: ${SERVER_TEST_SRCS} ${SERVER_SRCS} ${SERVER_HEADERS} ${CALC_SRCS} ${CALC_HEADERS} ${UTILS_HEADERS}
	g++ -DEXTRA -DTESTS_ENABLED -DDEBUG -std=c++17 -g -fdiagnostics-color -Wall -Wextra -Wpedantic -Werror ${SERVER_TEST_SRCS} ${SERVER_SRCS} ${CALC_SRCS} -I. -pthread -lstdc++fs -o ${TEST_EXTRA_SERVER_TARGET}

${TEST_SERVER_TARGET}: ${SERVER_TEST_SRCS} ${SERVER_SRCS} ${SERVER_HEADERS} ${CALC_SRCS} ${CALC_HEADERS} ${UTILS_HEADERS}
	g++ -DTESTS_ENABLED -DDEBUG -std=c++17 -g -fdiagnostics-color -Wall -Wextra -Wpedantic -Werror ${SERVER_TEST_SRCS} ${SERVER_SRCS} ${CALC_SRCS} -I. -pthread -lstdc++fs -o ${TEST_SERVER_TARGET}

run_tests: clean build_tests
	./run.sh "${TEST_CLIENT_TARGET} >${TEST_CLIENT_TARGET}.log 2>&1" "${TEST_SERVER_TARGET} ${SERVER_PORT} ${WORKING_DIRECTORY} ${MAX_WORKERS_COUNT} >${TEST_SERVER_TARGET}.log 2>&1" ${WORKING_DIRECTORY}

run_extra_tests: clean build_extra_tests
	./run.sh "${TEST_EXTRA_CLIENT_TARGET} >${TEST_EXTRA_CLIENT_TARGET}.log 2>&1" "${TEST_EXTRA_SERVER_TARGET} ${SERVER_PORT} ${WORKING_DIRECTORY} ${EXTRA_MAX_WORKERS_COUNT} >${TEST_EXTRA_SERVER_TARGET} 2>&1" ${WORKING_DIRECTORY}

run_memtests: clean build_tests
	./run.sh "${VALGRIND} --log-file=${CLIENT_VALGRIND_LOG} ${TEST_CLIENT_TARGET} >${TEST_CLIENT_TARGET}.log 2>&1" "${VALGRIND} --log-file=${SERVER_VALGRIND_LOG}  ${TEST_SERVER_TARGET} ${SERVER_PORT} ${WORKING_DIRECTORY} ${MAX_WORKERS_COUNT} >${TEST_SERVER_TARGET}.log 2>&1" ${WORKING_DIRECTORY} --memcheck ${CLIENT_VALGRIND_LOG} ${SERVER_VALGRIND_LOG}

run_extra_mrmtests: clean build_extra_tests
	./run.sh "${VALGRIND} --log-file=${EXTRA_CLIENT_VALGRIND_LOG} ${TEST_EXTRA_CLIENT_TARGET} >${TEST_EXTRA_CLIENT_TARGET}.log 2>&1" "${VALGRIND} --log-file=${EXTRA_SERVER_VALGRIND_LOG}  ${TEST_EXTRA_SERVER_TARGET} ${SERVER_PORT} ${WORKING_DIRECTORY} ${EXTRA_MAX_WORKERS_COUNT} >${TEST_EXTRA_SERVER_TARGET} 2>&1" ${WORKING_DIRECTORY} --memcheck ${EXTRA_CLIENT_VALGRIND_LOG} ${EXTRA_SERVER_VALGRIND_LOG}

clean:
	rm *.log *.out || true

check:
	./run_linters.sh

build: build_client build_server

build_tests: build_test_client build_test_server

build_extra_tests: build_extra_test_client build_extra_test_server

