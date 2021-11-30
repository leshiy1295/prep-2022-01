TST_TARGET = ./tests.out
VALGRIND_LOG = "valgrind.log"

HDRS = \
		src/list.hpp

TST_SRCS = \
           test/test.cpp

.PHONY: all check build test memtest rebuild clean

all: clean check build test memtest

check:
	./run_linters.sh

build: $(TST_TARGET)

test: $(TST_TARGET)
	$(TST_TARGET)
	echo "****TEST PASSED****"

memtest: $(TST_TARGET)
	./memtest.sh ${TST_TARGET}
	echo "****MEMTEST PASSED****"

rebuild: clean build

$(TST_TARGET): $(TST_SRCS) $(HDRS)
	$(CXX) -std=c++17 -g -fdiagnostics-color -O0 -Wall -Wextra -Werror -I. -o $(TST_TARGET) $(TST_SRCS)

clean:
	rm -f $(TST_TARGET) ${VALGRIND_LOG}
