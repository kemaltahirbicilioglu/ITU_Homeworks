#include "scheduler.h"
#include "logger.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

// Reads the jobs and time slice from the specified file.
void read_jobs(const char *filename, JobList *job_list) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open jobs file");
        exit(EXIT_FAILURE);
    }
    char line[256];
    // Read first line: "TimeSlice <value>"
    if (fgets(line, sizeof(line), fp)) {
        sscanf(line, "TimeSlice %d", &job_list->time_slice);
    }
    job_list->job_count = 0;
    // Read each job: "job_name arrival_time priority exec_time".
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '\n')
            continue;
        Job *job = &job_list->jobs[job_list->job_count];
        sscanf(line, "%s %d %d %d", job->name, &job->arrival_time, &job->priority, &job->exec_time);
        job->remaining_time = job->exec_time;
        job->pid = -1;
        job->started = 0;
        job_list->job_count++;
    }
    fclose(fp);
}

// Returns 1 if all jobs are finished; 0 otherwise.
int all_jobs_finished(JobList *job_list) {
    for (int i = 0; i < job_list->job_count; i++) {
        if (job_list->jobs[i].remaining_time > 0)
            return 0;
    }
    return 1;
}

int main() {
    JobList job_list;
    read_jobs("jobs.txt", &job_list);
    
    g_current_time = 0;        // Start simulation time at 0.
    g_time_slice = job_list.time_slice; // Set default time slice.
    
    int last_job_index = -1;   // Tracks the last scheduled job.
    
    while (!all_jobs_finished(&job_list)) {
        int candidate_index = -1;
        // Select an eligible job based on arrival, priority, and remaining time.
        for (int i = 0; i < job_list.job_count; i++) {
            if (job_list.jobs[i].arrival_time <= g_current_time && job_list.jobs[i].remaining_time > 0) {
                if (candidate_index == -1) {
                    candidate_index = i;
                } else {
                    // Prefer an alternative if the current candidate equals last_job_index.
                    if (candidate_index == last_job_index && i != last_job_index) {
                        candidate_index = i;
                    }
                    // Otherwise, compare based on lower priority, earlier arrival, and less remaining time.
                    else if (candidate_index != last_job_index && i != last_job_index) {
                        Job *current = &job_list.jobs[i];
                        Job *candidate = &job_list.jobs[candidate_index];
                        if (current->priority < candidate->priority ||
                           (current->priority == candidate->priority && current->arrival_time < candidate->arrival_time) ||
                           (current->priority == candidate->priority && current->arrival_time == candidate->arrival_time &&
                            current->remaining_time < candidate->remaining_time)) {
                            candidate_index = i;
                        }
                    }
                }
            }
        }
        
        // If no eligible job is found, log the event and increment simulation time.
        if (candidate_index == -1) {
            log_entry(INFO, NO_JOB, "", 0);
            g_current_time++;
            continue;
        }
        
        Job *job = &job_list.jobs[candidate_index];
        
        // If the job has not been started, fork and execute it.
        if (!job->started) {
            pid_t pid = fork();
            if (pid == 0) {
                // Child process: execute job_exec (which runs an infinite loop).
                execl("./build/job_exec", "job_exec", (char *)NULL);
                perror("execl failed");
                exit(EXIT_FAILURE);
            } else {
                job->pid = pid;
                job->started = 1;
                log_entry(INFO, FORK, job->name, pid);
                log_entry(INFO, EXEC, job->name, pid);
            }
        } else {
            // Resume a previously started job.
            log_entry(INFO, CONT, job->name, job->pid);
            if (kill(job->pid, SIGCONT) < 0) {
                log_entry(ERROR, CONT, job->name, job->pid);
            }
        }
        
        // Determine simulated run time for this cycle.
        int run_time = (job->remaining_time < g_time_slice) ? job->remaining_time : g_time_slice;
        g_current_time += run_time;         // Update simulation time.
        job->remaining_time -= run_time;      // Decrease remaining time.
        
        if (job->remaining_time > 0) { // If job not finished, preempt it.
            log_entry(INFO, STOP, job->name, job->pid);
            if (kill(job->pid, SIGSTOP) < 0) {
                log_entry(ERROR, STOP, job->name, job->pid);
            }
        } else { // Terminate the job if finished.
            if (kill(job->pid, SIGTERM) < 0) {
                log_entry(ERROR, COMPLETE, job->name, job->pid);
            }
            int status;
            waitpid(job->pid, &status, 0);
            log_entry(INFO, COMPLETE, job->name, job->pid);
        }
        last_job_index = candidate_index;  // Update last scheduled job.
    }
    return 0;
}
