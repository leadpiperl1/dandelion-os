CC = gcc
CFLAGS = -std=c99 -O3 -Wall -Wextra -Wconversion -Werror

all: build/dandelion_mtp_assist_v4_1_1

build/dandelion_mtp_assist_v4_1_1: src/dandelion_mtp_assist_v4_1_1.c src/ternary_raft.c src/fox_daemon_core.c
	@mkdir -p build
	  -c src/ternary_raft.c -o build/ternary_raft.o
	  -c src/fox_daemon_core.c -o build/fox_daemon_core.o
	  src/dandelion_mtp_assist_v4_1_1.c build/ternary_raft.o build/fox_daemon_core.o -o build/dandelion_mtp_assist_v4_1_1

test: all
	./build/dandelion_mtp_assist_v4_1_1

clean:
	rm -rf build
