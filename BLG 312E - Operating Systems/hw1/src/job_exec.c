#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// Runs an infinite loop to simulate a job process.
int main() {
    while (1) {
        sleep(1); // symbolic sleep 1 second for the current job
    }
    return 0;
}
