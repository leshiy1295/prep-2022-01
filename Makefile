.PHONY: all build rebuild check test testextra memtest memtestextra clean

all: clean check build test memtest testextra memtestextra

clean:
	rm -rf build

build:
	./build.sh

rebuild: clean build

check:
	./run_linters.sh

test:
	./build.sh -DWITH_MEMCHECK=OFF
	./run_tests.sh

memtest:
	./build.sh -DWITH_MEMCHECK=ON
	./run_tests.sh --memcheck

testextra:
	./build.sh -DWITH_MEMCHECK=OFF
	./run_tests.sh --with-extra

memtestextra:
	./build.sh -DWITH_MEMCHECK=ON
	./run_tests.sh --with-extra --memcheck
