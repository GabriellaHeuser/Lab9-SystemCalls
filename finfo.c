/**********************************************************************************
 * Filename: finfo.c
 * Author: Gabi Heuser
 * Assignment: Lab 9 - System Calls
 * Section: CPE2600 121
 * Executable: ./finfo
 * Purpose: Displays info about file from command line:
 *              file type, permissions, owner (user id), size in bytes, and 
 *              date/time of last modification
 *********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>   // lstat, struct stat, S_IS* macros, S_I* masks
#include <time.h>       // localtime, strftime

int main(int argc, char* argv[])
{
    //check there are two arguments (program name and filename)
    if (argc != 2) {
        fprintf(stderr, "Must enter in form: %s <filename>\n", argv[0]);
        return 1;
    }

    //lstat fills a struct stat with details about the file and 
    //  returns 0 on success and -1 on error. 
    // using lstat because
    // stat  - follows symbolic links
    //       - could never print "symbolic link"
    // fstat - needs a pre-open file
    //       - this program takes in a filename
    // lstat - takes a filename 
    //       - can report a symbolic link
    struct stat sb;
    if (lstat(argv[1], &sb) != 0) {
        perror("lstat failed");
        return 1;
    }

    //st_mode stores file type and permission bits
    // The S_IS* macros pull out the type
    // S_I* masks can grab permissions.
    printf("File type: ");
    if (S_ISREG(sb.st_mode)) {
        printf("regular file\n");
    } else if (S_ISDIR(sb.st_mode)) {
        printf("directory\n");
    } else if (S_ISLNK(sb.st_mode)) {
        printf("symbolic link\n");
    } else if (S_ISCHR(sb.st_mode)) {
        printf("character device\n");
    } else if (S_ISBLK(sb.st_mode)) {
        printf("block device\n");
    } else if (S_ISFIFO(sb.st_mode)) {
        printf("FIFO (named pipe)\n");
    } else if (S_ISSOCK(sb.st_mode)) {
        printf("socket\n");
    } else {
        printf("unknown\n");
    }

    //st_uid is the owner's user id
    printf("Owner (UID): %d\n", sb.st_uid);

    //st_size is an off_t, using ll because sizes can get large
    printf("Size: %lld bytes\n", (long long)sb.st_size);

    //Permissions are single bits in st_mode 
    //Building a 9 character string like "rwxr-xr--" 
    // For each, if the bit is set, print letter, else print '-'
    char perms[10];

    //owner permissions
    if (sb.st_mode & S_IRUSR) {
        perms[0] = 'r';
    } else {
        perms[0] = '-';
    }
    if (sb.st_mode & S_IWUSR) {
        perms[1] = 'w';
    } else {
        perms[1] = '-';
    }
    if (sb.st_mode & S_IXUSR) {
        perms[2] = 'x';
    } else {
        perms[2] = '-';
    }

    //group permissions
    if (sb.st_mode & S_IRGRP) {
        perms[3] = 'r';
    } else {
        perms[3] = '-';
    }
    if (sb.st_mode & S_IWGRP) {
        perms[4] = 'w';
    } else {
        perms[4] = '-';
    }
    if (sb.st_mode & S_IXGRP) {
        perms[5] = 'x';
    } else {
        perms[5] = '-';
    }

    //other (everyone else) permissions
    if (sb.st_mode & S_IROTH) {
        perms[6] = 'r';
    } else {
        perms[6] = '-';
    }
    if (sb.st_mode & S_IWOTH) {
        perms[7] = 'w';
    } else {
        perms[7] = '-';
    }
    if (sb.st_mode & S_IXOTH) {
        perms[8] = 'x';
    } else {
        perms[8] = '-';
    }

    perms[9] = '\0'; //terminating string
    printf("Permissions: %s\n", perms);

    //st_mtime is time of last modification as time_t (seconds since 1970)
    //localtime converts to a local time
    //strftime formats that as readable text.
    char timebuf[64];

    struct tm *tm_info = localtime(&sb.st_mtime);
    if (tm_info == NULL) {
        fprintf(stderr, "localtime failed\n");
        return 1;
    }
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Last modified: %s\n", timebuf);

    return 0;
}