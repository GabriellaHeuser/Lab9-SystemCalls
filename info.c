/*********************************************************************************
 * Filename: info.c
 * Author: Gabi Heuser
 * Assignment: Lab 9 - System Calls
 * Section: CPE2600 121
 * Executable: ./info
 * Purpose: Uses info maintenance system calls to print details about system
 *              current time, network name, OS name, OS release/version, 
 *              hardware type, CPU count, and physical memory (total and free)
 *          Blank values are not printed.
 *********************************************************************************/
#include <stdio.h>
#include <time.h>          // clock_gettime, struct timespec
#include <sys/utsname.h>   // uname, struct utsname
#include <unistd.h>        // sysconf, getpagesize
#include <sys/sysinfo.h>   // get_nprocs

int main(int argc, char* argv[]){

    //structure declarations
    //system calls fill in a struct declared in the program
    struct timespec ts;
    struct utsname unamedata;

    //clock_gettime puts time in ts 
    //returns 0 on success or -1 on error. 
    // CLOCK_REALTIME is time measured as seconds + nanoseconds since January 1, 1970
    // localtime(&ts.tv_sec) will get the local time
    if (clock_gettime(CLOCK_REALTIME, &ts) != 0){
        perror("clock_gettime failed");
    } else {
        struct tm *localTime = localtime(&ts.tv_sec); //local time broken into hours, minutes, seconds
        if (localTime == NULL){
            printf("localtime failed\n");
            return 1;
        }
        long long secondsSinceMid = localTime->tm_hour * 3600 + localTime->tm_min * 60 + localTime->tm_sec;
        long long nano = secondsSinceMid * 1000000000LL + ts.tv_nsec;
        printf("Current (Local) Time: %lld nanoseconds since midnight\n", nano);
    }

    //uname fills struct with system id strings
    //returns 0 on success, -1 on error
    //provides the network name, OS name, release, version, and hardware type.
    //checking for blank - must check if first character is '\0'
    //skipping blank values in this assignment
    if (uname(&unamedata) != 0){
        perror("uname failure");
    } else {
        //nodename - system name on the network
        //could also use gethostname
        if ((unamedata.nodename)[0] != '\0') {
            printf("Network Name: %s\n", unamedata.nodename);
        }

        //sysname - operating system name
        if ((unamedata.sysname)[0] != '\0') {
            printf("Operating System Name: %s\n", unamedata.sysname);
        }

        //release - OS release
        if ((unamedata.release)[0] != '\0') {
            printf("Operating System Release: %s\n", unamedata.release);
        }

        //version - OS version 
        if ((unamedata.version)[0] != '\0') {
            printf("Operating System Version: %s\n", unamedata.version);
        }

        //machine - hardware type
        if ((unamedata.machine)[0] != '\0') {
            printf("Hardware Type: %s\n", unamedata.machine);
        }
    }

    //get_nprocs returns # of processors online
    printf("Number of CPUs: %d\n", get_nprocs());

    //getpagesize returns the size of one memory page in bytes
    long long pageSize = getpagesize();

    //sysconf(_SC_PHYS_PAGES) and sysconf(_SC_AVPHYS_PAGES) 
    // return # of pages
    // To get bytes - multiply pages by bytes per page
    //sysconf returns -1 on error
    long physPages = sysconf(_SC_PHYS_PAGES);
    long freePages = sysconf(_SC_AVPHYS_PAGES);

    if (physPages == -1){
        perror("sysconf(_SC_PHYS_PAGES) failed");
    } else {
        //cast to long long first so the multiplication cannot overflow
        long long totalMem = (long long)physPages * pageSize;
        printf("Total Bytes of Physical Memory: %lld\n", totalMem);
    }

    if (freePages == -1){
        perror("sysconf(_SC_AVPHYS_PAGES) failed");
    } else {
        long long freeMem = (long long)freePages * pageSize;
        printf("Free Bytes of Physical Memory: %lld\n", freeMem);
    }

    return 0;
}