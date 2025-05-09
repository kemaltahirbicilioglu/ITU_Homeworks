#ifndef LOGGER_H
#define LOGGER_H

// Log type enum.
typedef enum logType {
    INFO,
    ERROR
} t_logType;

// Log message enum.
typedef enum logMessage {
    FORK,      // Process fork event.
    EXEC,      // Process exec event.
    STOP,      // Process preemption (SIGSTOP).
    CONT,      // Process resume (SIGCONT).
    COMPLETE,  // Process completion.
    NO_JOB     // No eligible job found.
} t_logMessage;

// Logs a scheduler event with simulation time.
void log_entry(t_logType type, t_logMessage message, char *job_name, int pid);

#endif // LOGGER_H
