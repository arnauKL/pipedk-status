#include "piped.h"
#include "config.h"
#include <poll.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h> // C sleep function

struct module_ptr {
    char *start;                        // Pointer to first mutable char
    int   len;                          // Length of mutable region
    void (*update)(char *ptr, int len); // Function to update this region
};

static char status_bar[MAX_LEN_OTUPUT] = " BAT0: 00% | 00/00 - 00:00 ";
//       ^^^ +   ^^^^^   ^^^^^
// battery%sign   date   time

static struct module_ptr modules[] = {
    // { status_bar + padding,  n_chars, function_pointer },  // "000"
    // { status_bar + 10, 1, update_battery_status },   // "+"
    {status_bar + 13, 6, update_date},           // "00/00"
    {status_bar + 21, 6, update_time},           // "00:00"
    {status_bar + 7, 3, update_battery_percent}, // "000"
};

// -------------------------------

// Main
int main(int argc, char *argv[]) {

    // Date will be onlly set at startup
    modules[0].update(modules[0].start, modules[0].len);

    /* Main loop */
    while (1) {

        // Time module
        modules[1].update(modules[1].start, modules[1].len);
        // Battery module
        modules[2].update(modules[2].start, modules[2].len);

        printf("%s\n", status_bar);
        fflush(stdout);
        sleep(UPDATE_INTERVAL_SECS);
    }

    return 0;
}

// -------------------------------

// Module-updating function definitions (declared in piped.h)
void update_time(char *ptr, int len) {
    // Set chars for time (hour and minute)
    time_t     now = time(NULL);
    struct tm *t   = localtime(&now);
    strftime(ptr, len, TIME_FORMAT_STR, t);

    *(ptr + len - 1) = ' '; // strftime adds null terminator, reomve it
}

void update_date(char *ptr, int len) {
    // Set chars for date (day and month)
    time_t     now = time(NULL);
    struct tm *t   = localtime(&now);
    strftime(ptr, len, DATE_FORMAT_STR, t);

    *(ptr + len - 1) = ' '; // strftime adds null terminator, reomve it
}

void update_battery_percent(char *ptr, int len) {
    // Set chars for battery percent (000-padded (0 : 100 range)

    // First Capacity
    FILE *fBAT = fopen(BAT0_CAPAC_PATH, "r");

    if (fBAT == NULL) {
        perror("battery capacity file");
        return;
    }

    fgets(ptr, len, fBAT); // LMAO i think so?
    *(ptr + len - 1) = '%';

    fclose(fBAT);
}
