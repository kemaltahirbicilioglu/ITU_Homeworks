CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g

SRC = src/scheduler.c src/logger.c src/globals.c
JOB_SRC = src/job_exec.c

all: build/scheduler build/job_exec

build:
	mkdir -p build
	mkdir -p logs

build/scheduler: build $(SRC)
	$(CC) $(CFLAGS) -o build/scheduler $(SRC)

build/job_exec: build $(JOB_SRC)
	$(CC) $(CFLAGS) -o build/job_exec $(JOB_SRC)

# Target to build a debug version (CFLAGS already include -g)
debug: all

# Run the scheduler normally
run: build/scheduler
	./build/scheduler

# Run the scheduler under gdb
debug_run: build/scheduler
	gdb ./build/scheduler

clean:
	rm -rf build logs/scheduler.log
