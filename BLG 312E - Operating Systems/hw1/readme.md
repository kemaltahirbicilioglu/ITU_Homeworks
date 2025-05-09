# Preemptive Priority-Based Scheduler

## What This Is
This project is a preemptive, priority-based process scheduler. It picks which process runs based on when it arrives, its priority, and how much time it has left. It uses `fork`, `exec`, and signals (`SIGSTOP`, `SIGCONT`, `SIGTERM`). Works on Linux and can also run inside a Docker container if needed.

## What You Need
- **Compiler:** GCC (with debug symbols enabled)
- **OS:** Linux (e.g., Ubuntu)
- **Docker:** Optional. If using Docker, make sure it has GCC and make installed.

## How to Build
1. Open a terminal in the project folder (where the Makefile is).
2. Run `make`.
3. Executables will be created in the `build/` folder.
4. `logs/` folder is also set up.

## How to Run
- Run `make run`.

## Debugging
- **Using Make** `make debug_run`
- **Using VSCode Debugger** User can put a breakpoint at some point and after running `make debug` pressing f5 on keyboard. 
- **Using GDB:** Run the scheduler with `gdb ./build/scheduler`.
- **Using VSCode:** There are debugging configs in `.vscode/launch.json` and other related files.

## Cleaning Up
- Run `make clean` to delete build files and logs.

## Running in Docker
- Use the Dockerfile to make a container with GCC and make.
- Copy the project into the container.
- Run `make` to build.
- Run `make run` to start the scheduler.
