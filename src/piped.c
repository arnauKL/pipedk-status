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

static char status_bar[MAX_LEN_OTUPUT] = "00.00W | BAT0: 00% | 00/00 - 00:00";
//       ^^^ +   ^^^^^   ^^^^^
// battery%sign   date   time

static struct module_ptr modules[] = {
    // { status_bar + padding,  n_chars, function_pointer },  // "000"
    // { status_bar + 10, 1, update_battery_status },   // "+"
    {status_bar + 21, 6, update_date},      // "00/00"
    {status_bar + 29, 6, update_time},      // "00:00"
    {status_bar + 15, 3, update_bat_level}, // "000"
    {status_bar, 6, update_power_now},      // "000"
};

// -------------------------------

// Main
int main() {

    // Date will be onlly set at startup
    modules[0].update(modules[0].start, modules[0].len);

    /* Main loop */
    while (1) {

        // Time module
        modules[1].update(modules[1].start, modules[1].len);
        // Battery module
        modules[2].update(modules[2].start, modules[2].len);
        modules[3].update(modules[3].start, modules[3].len);

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

void update_power_now(char *ptr, int len) {
    FILE         *f      = fopen(WATTS_PATH, "r");
    unsigned long uwatts = 0;
    if (f) {
        fscanf(f, "%lu", &uwatts);
        fclose(f);
    }

    double watts = uwatts / 1000000.0; // uW to W
    //  %2.2fW
    printf("\n%f\n", watts);
    snprintf(ptr, len, "%.2fW", watts);
    *(ptr + len - 1) = 'W';
}

void update_bat_level(char *ptr, int len) {
    FILE *f;
    int   percent = 0;

    // Read battery percentage
    f = fopen(BAT0_CAPAC_PATH, "r");
    if (f) {
        fscanf(f, "%d", &percent);
        fclose(f);
    }

    snprintf(ptr, len, "%2d", percent);
    *(ptr + len - 1) = '%';
}
