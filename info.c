// Gabi Heuser
// info.c
#include <stdio.h>
#include <time.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <sys/sysinfo.h>

int main(int argc, char* argv[]){

    struct timespec ts;
    struct utsname unamedata;

    if (clock_gettime(CLOCK_REALTIME, &ts) != 0){
        perror("clock_gettime failed");
    } else {
        long long nano = (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
        printf("Current Time: %lld nanoseconds\n", nano);
    }

    if (uname(&unamedata) != 0){
        perror("uname failure");
    } else {
        if ((unamedata.nodename)[0] != '\0') {
            printf("Network Name: %s\n", unamedata.nodename);
        }

        if ((unamedata.sysname)[0] != '\0') {
            printf("Operating System Name: %s\n", unamedata.sysname);
        }

        if ((unamedata.release)[0] != '\0') {
            printf("Operating System Release: %s\n", unamedata.release);
        }

        if ((unamedata.version)[0] != '\0') {
            printf("Operating System Version: %s\n", unamedata.version);
        }

        if ((unamedata.machine)[0] != '\0') {
            printf("Hardware Type: %s\n", unamedata.machine);
        }
    }

    printf("Number of CPUs: %d\n", get_nprocs());

    //get size of one page and multiply by number of pages from sysconf
    long long pageSize = getpagesize();
    long long totalMem = sysconf(_SC_PHYS_PAGES) * pageSize;
    long long freeMem = sysconf(_SC_AVPHYS_PAGES) * pageSize;

    printf("Total Bytes of Physical Memory: %lld\n", totalMem);
    printf("Free Bytes of Physical Memory: %lld\n", freeMem);


    




    return 0;
}