/***********************************************************************************
 * Filename: pinfo.c
 * Author: Gabi Heuser
 * Assignment: Lab 9 - System Calls
 * Section: CPE2600 121
 * Executable: ./pinfo
 * Purpose: Prints priority (nice value) and scheduling method of a process
 *          Takes process id on the command line or uses current if none provided
 **********************************************************************************/

//_GNU_SOURCE must be defined before any #include
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>         // getpid
#include <sys/resource.h>   // getpriority, PRIO_PROCESS
#include <sched.h>          // sched_getscheduler, SCHED_* constants
#include <stdlib.h>         // strtol, exit
#include <errno.h>          // errno

//function declaration to simplify printing
void printProcess(pid_t process);

int main(int argc, char* argv[])
{

    if (argc == 1){ //no argument case
        //getpid returns process id of the calling process (never fails)
        pid_t current = getpid();
        printProcess(current);
    } else if (argc == 2){ //single argument (process id)
        char *end; //strtol sets to the first unparsable char
        errno = 0; //clearing errno so we can detect errors from strtol

        long value = strtol(argv[1], &end, 10); //convert string input to long

        //Error checks:
        //      errno != 0   -> issue in converting
        //      *end != '\0' -> non integer characters
        //      end == argv[1] -> no digits in string
        //      value <= 0   -> process ids are always positive
        if (errno != 0 || *end != '\0' || end == argv[1] || value <= 0) {
            fprintf(stderr, "Invalid process id: %s\n", argv[1]);
            return 1;
        }
        printProcess((pid_t)value);
    } else {
        fprintf(stderr, "Invalid number of arguments\n");
        return 1;
    }


    return 0;
}

//prints the priority and scheduling method of the pid
//if process does not exist, getpriority and sched_getscheduler fail w/ error messages and exitting accordingly

void printProcess(pid_t process){
    //getpriority returns the nice value (-20 highest priority to 19 lowest)
    //-1 is a valid nice value, so return value cannot indicate error. use errno instead.
    errno = 0; //clearing errno
    int priority = getpriority(PRIO_PROCESS, process);

    //check for error in priority
    if (errno != 0){
        perror("getpriority failed");
        exit(1);
    }

    //sched_getscheduler returns scheduling policy as integer
    //defined macro or -1 on error
    //translated to text for printing
    int policy = sched_getscheduler(process);
    if (policy == -1){
        perror("sched_getscheduler failed");
        exit(1);
    }

    printf("Process ID: %d\n", process);
    printf("Priority: %d\n", priority);
    printf("Scheduling method: ");

    //SCHED_OTHER: default time-sharing policy for normal processes
    //SCHED_FIFO / SCHED_RR: real-time policies (first-in-first-out, round robin)
    //SCHED_BATCH: for non-interactive, CPU-heavy work (Linux specific)
    //SCHED_IDLE: for very low priority background jobs (Linux specific)
    if (policy == SCHED_OTHER){
        printf("SCHED_OTHER\n");
    } else if (policy == SCHED_FIFO){
        printf("SCHED_FIFO\n");
    } else if (policy == SCHED_RR){
        printf("SCHED_RR\n");
    } else if (policy == SCHED_BATCH){
        printf("SCHED_BATCH\n");
    } else if (policy == SCHED_IDLE){
        printf("SCHED_IDLE\n");
    } else {
        printf("UNKNOWN\n");
    }
}