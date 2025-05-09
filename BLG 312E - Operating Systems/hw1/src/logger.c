#include "logger.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static char *log_path = "logs/scheduler.log";

// Logs an event with the current simulation time formatted as HH:MM:SS.
void log_entry(t_logType type, t_logMessage message, char *job_name, int pid) {
    FILE *log_file = fopen(log_path, "a");
    if (!log_file) {
        perror("Unable to open log file");
        exit(EXIT_FAILURE);
    }
    // Convert simulation time counter into HH:MM:SS.
    int hours = g_current_time / 3600;
    int minutes = (g_current_time % 3600) / 60;
    int seconds = g_current_time % 60;
    char timestamp[9];
    sprintf(timestamp, "%02d:%02d:%02d", hours, minutes, seconds);
    
    switch (message) {
        case FORK:
            if (type == INFO)
                fprintf(log_file, "[%s] [INFO] Forking new process for %s\n", timestamp, job_name);
            else
                fprintf(log_file, "[%s] [ERROR] Failed to fork new process for %s. Error: %s\n",
                        timestamp, job_name, strerror(errno));
            break;
        case EXEC:
            if (type == INFO)
                fprintf(log_file, "[%s] [INFO] Executing %s (PID: %d) using exec\n", timestamp, job_name, pid);
            else
                fprintf(log_file, "[%s] [ERROR] Failed to execute %s (PID: %d) using exec. Error: %s\n",
                        timestamp, job_name, pid, strerror(errno));
            break;
        case STOP:
            if (type == INFO)
                fprintf(log_file, "[%s] [INFO] %s ran for %d seconds. Time slice expired - Sending SIGSTOP\n",
                        timestamp, job_name, g_time_slice);
            else
                fprintf(log_file, "[%s] [ERROR] Failed to stop %s (PID: %d) - Error: %s\n",
                        timestamp, job_name, pid, strerror(errno));
            break;
        case CONT:
            if (type == INFO)
                fprintf(log_file, "[%s] [INFO] Resuming %s (PID: %d) - SIGCONT\n", timestamp, job_name, pid);
            else
                fprintf(log_file, "[%s] [ERROR] Failed to resume %s (PID: %d) - Error: %s\n",
                        timestamp, job_name, pid, strerror(errno));
            break;
        case COMPLETE:
            if (type == INFO)
                fprintf(log_file, "[%s] [INFO] %s completed execution. Terminating (PID: %d)\n", timestamp, job_name, pid);
            else
                fprintf(log_file, "[%s] [ERROR] Failed to terminate %s (PID: %d) after completion. Error: %s\n",
                        timestamp, job_name, pid, strerror(errno));
            break;
        case NO_JOB:
            if (type == INFO)
                fprintf(log_file, "[%s] [INFO] No job at the moment!\n", timestamp);
            else
                fprintf(log_file, "[%s] [ERROR] No job at the moment! - Error: %s\n", timestamp, strerror(errno));
            break;
    }
    fclose(log_file);
}
