// Gabi Heuser
// pinfo.c
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sched.h>
#include <stdlib.h>
#include <errno.h>

void printProcess(pid_t process);

int main(int argc, char* argv[])
{
    if (argc == 1){
        pid_t current = getpid();
        printProcess(current);
    } else if (argc == 2){
        char *end;
        errno = 0;
        long value = strtol(argv[1], &end, 10);
        if (errno != 0 || *end != '\0' || value <= 0) {
            printf("Invalid process id: %s\n", argv[1]);
            return 1;
        }
        printProcess((pid_t)value);
    } else {
        printf("Invalid number of arguments\n");
        return 1;
    }


    return 0;
}

void printProcess(pid_t process){
    errno = 0; //clearing errno
    int priority = getpriority(PRIO_PROCESS, process);

    //check for error in priority
    if (errno != 0){
        perror("getpriority failed");
        exit(1);
    }

    int policy = sched_getscheduler(process);
    if (policy == -1){
        perror("sched_getscheduler failed");
        exit(1);
    }

    printf("Process ID: %d\n", process);
    printf("Priority: %d\n", priority);
    printf("Scheduling method: ");
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