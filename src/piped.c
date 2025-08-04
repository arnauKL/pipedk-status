#include <stdio.h>
#include <unistd.h>         // C sleep function
#include <sys/inotify.h>
#include <time.h>
#include "piped.h"
#include "config.h"


/* TEMPORARY */
void printFull(const char* string, int len) {
    // Debugging 
    for (int i = 0; i < len; i++) {
        if (string[i] == '\0')
            printf("%s", "\\0");
        else
            printf("%c", string[i]);
    }
    printf("\n");
    return;
}

struct module_ptr {
    char *start;        // Pointer to first mutable char
    int len;            // Length of mutable region
    void (*update)(char *ptr, int len);  // Function to update this region
};


/* Bar layout */
// static char status_bar[MAX_LEN_OTUPUT] = " CPU: 00% | BAT0: 000%+ | 00/00 - 00:00 ";
//                                         //      ^^          ^^^ +   ^^^^^   ^^^^^
//                                         //     cpu%   battery%sign   date   time 

static char status_bar[MAX_LEN_OTUPUT] = " BAT0: 000%+ | 00/00 - 00:00 ";
                                        //       ^^^ +   ^^^^^   ^^^^^
                                        // battery%sign   date   time 

static struct module_ptr modules[] = {
    // { status_bar + 7,  3, update_battery_percent },  // "000"
    // { status_bar + 10, 1, update_battery_status },   // "+"
    { status_bar + 15, 6, update_date },             // "00/00"  
    { status_bar + 23, 6, update_time },             // "00:00"
};

static int inotify_fd;
static int capacity_wd, status_wd;  // watch descriptors

// Main
int main(int argc, char *argv[]) {

    if (init_inotify() < 0) {
        perror("inotify error");
        return -1;
    }

    // Date will be onlly set at startup
    modules[0].update(modules[0].start, modules[0].len);
    
    /* Main loop */
    while (1) {
        modules[1].update(modules[1].start, modules[1].len);    // Time module
        printf("%s\n", status_bar);
        fflush(stdout);
        sleep(UPDATE_INTERVAL_SECS);
    }

    close(inotify_fd);
    return 0;
}



// -----------------------------

// Module-updating function definitions (declared in piped.h)
void update_time (char *ptr, int len){
    // Set chars for time (hour and minute)
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(ptr, len, TIME_FORMAT_STR, t);
    
    // strftime adds null terminator, reomve it:
    *(ptr + len-1) = ' ';
    
    return;
}

void update_date(char *ptr, int len){
    // Set chars for date (day and month)
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(ptr, len, DATE_FORMAT_STR, t);
    
    // strftime adds null terminator, reomve it:
    *(ptr + len-1) = ' ';
    
    return;
} 

// Utility functions
// init inotify and handle errors
int init_inotify(void) {
    inotify_fd = inotify_init1(IN_NONBLOCK);  // Non-blocking

    if (inotify_fd == -1) {
        perror("inotify_init1");
        return -1;
    }
    
    /* Watch battery files for modifications */
    capacity_wd = inotify_add_watch(inotify_fd, BAT0_CAPAC_PATH, IN_MODIFY);
    status_wd = inotify_add_watch(inotify_fd, BAT0_STATE_PATH, IN_MODIFY);
    
    if (capacity_wd < 0 || status_wd < 0) {
        perror("inotify_add_watch");
        return -1;
    }
    
    return 0;
}
