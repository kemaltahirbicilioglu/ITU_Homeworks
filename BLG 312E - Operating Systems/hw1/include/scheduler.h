#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <sys/types.h>

#define MAX_JOBS 100 

// Structure representing a job to be scheduled.
typedef struct {
    char name[50];
    int arrival_time;
    int priority;
    int exec_time;
    int remaining_time;
    pid_t pid;
    int started; // 0 if not started; 1 if started.
} Job;

// Structure holding job list and scheduling parameters.
typedef struct {
    int time_slice;
    int job_count;
    Job jobs[MAX_JOBS];
} JobList;

// Reads job definitions from a file.
void read_jobs(const char *filename, JobList *job_list);

#endif // SCHEDULER_H
