# Lab9-SystemCalls
Gabi Heuser | CPE2600 Section 121

## To Build
- `make` then use specific run calls listed below.
- `make clean` to remove extra build output from directory

## Information Maintenance System Calls - info.c

Run: `./info`
#### Prints:
  - current time of day (nanoseconds since local midnight)
  - network
  - name
  - OS name, release, and version
  - hardware type
  - number of CPUs
  - total and free physical memory in bytes

### System Calls
* `clock_gettime` - gets current time from, `CLOCK_REALTIME` provides wall clock time, as seconds and nanoseconds in a `struct timespec`.
* `uname` - fills `struct utsname` with the OS name, network node name, release, version, and hardware type.
* `gethostname` - returns system network name
* `get_nprocs` - returns # of processors currently online
* `sysconf` - looks up system configuration values at runtime
  * `_SC_PHYS_PAGES` and `_SC_AVPHYS_PAGES` give # of total and free memory pages (not bytes)
* `getpagesize` - returns size of a memory page in bytes

## Process Control System Calls - pinfo.c/pmod.c

### System Calls
* `getpid` - returns process ID of calling process
* `getpriority` - returns "nice value" (-20 highest priority to 19 lowest) of a process
* `sched_getscheduler` - returns scheduling policy of a process as an integer (`SCHED_OTHER`, `SCHED_FIFO`, `SCHED_RR`, `SCHED_BATCH`, `SCHED_IDLE`)
* `getcpu` - reports which CPU core the calling thread is running on
* `getrusage` - reports resource usage for a process
  - user and system CPU time, peak memory, page faults, context switches.
* `getrlimit` - reports process's resource limits (stack size, open files, CPU time, etc.) as a soft and hard limits
* `nice` - adds a value to the calling process's nice value. Higher nice value means lower priority.
* `nanosleep` - pauses process for a given time

### man nice vs man 2 nice
`man nice` opens section 1, the `nice` shell command, which launches a
program with an adjusted priority. `man 2 nice` opens section 2, the `nice()`
system call, which a program uses to change its own priority.

### pinfo.c -> pinfo

Run: `./pinfo [pid]`

Prints the priority (nice value) and scheduling method of the given process. 
No argument reports on the current process. 
A nonexistent or invalid process ID prints an error and exits.

### pmod.c -> pmod

Run: `./pmod`

Lowers its own priority by 10 - `nice(10)`, sleeps for 1,837,272,638 nanoseconds, then prints a goodbye message.

Observations in htop: While `pmod` was running, `htop` showed a NI (nice value) of 10 and a PRI (priority) of 30. 
Normal processes showed NI = 0 and PRI = 20, so `nice(10)` raised the nice value by 10, which lowered the priority.

## File Management System Calls - finfo.c

Run: `./finfo <filename>`

Prints the file type, permissions legibly, owner user ID, size in bytes, and last modification time.

### System Calls
* `stat` - fills a `struct stat` for a file name; follows symbolic links.
* `fstat` - same information, but for an already-open file descriptor.
* `lstat` - like `stat` but does not follow symbolic links, so a link is reported as a link. Used in `finfo.c` for that reason.