#ifndef GLOBALS_H
#define GLOBALS_H

// Global simulation variables.
extern int g_current_time;  // Simulation time counter (in seconds), it represents the time but not the real time actually
extern int g_time_slice;    // Default time slice for scheduling, given by the jobs.txt

#endif // GLOBALS_H
