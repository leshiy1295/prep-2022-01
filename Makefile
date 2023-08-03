TARGET = ./main.out
HDRS_DIR = project/include

SRCS = \
		project/src/main.c \
		project/src/read_write_mes_func.c \
		project/src/create_new_client.c \
		project/src/write_transaction.c \
		project/src/upd_base_client.c

TEST_TARGET = ./test.out
TEST_SRCS = \
		project/src/test.c \
		project/src/read_write_mes_func.c

.PHONY: all build rebuild check test memtest clean

all: clean check test memtest

$(TARGET): $(SRCS)
	$(CC) -Wpedantic -Wall -Wextra -Werror -I $(HDRS_DIR) -o $(TARGET) $(CFLAGS) $(SRCS)

build: $(TARGET)
$(TEST_TARGET): $(TEST_SRCS)
	$(CC) -Wpedantic -Wall -Wextra -Werror -I $(HDRS_DIR) -o $(TEST_TARGET) $(CFLAGS) $(TEST_SRCS)

build: $(TARGET) $(TEST_TARGET)

rebuild: clean build

check:
	./run_linters.sh

test: $(TARGET) $(TEST_TARGET)
	./btests/run.sh $(TARGET) && $(TEST_TARGET)

memtest: $(TARGET)
	./btests/run.sh $(TARGET) --memcheck

clean:
	rm -rf $(TARGET) $(TEST_TARGET) *.dat
