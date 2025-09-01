#include "piped.h"
#include "config.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h> // C sleep function

struct module_ptr {
    char *start;                              // Pointer to first mutable char
    int   len;                                // Length of mutable region (len > 0)
    void (*update)(char *ptr, const int len); // Function to update this region
};

static char status_bar[MAX_LEN_OTUPUT] = "00.00W | BAT0: 00% | 00/00 - 00:00 |  ";
//                                        ^^^^^          ^^%   ^^^^^   ^^^^^
//                                        power       battery%  date   time

// Yup, manually set, maybe some C macro could do this, idk
#define POWER_OFFSET     0
#define BAT_LEVEL_OFFSET 15
#define DATE_OFFSET      21
#define TIME_OFFSET      29

static struct module_ptr modules[] = {
    // { status_bar + padding,  n_chars, fn_pointer },
    {status_bar + DATE_OFFSET, 5, update_date},           // "00/00"
    {status_bar + TIME_OFFSET, 5, update_time},           // "00:00"
    {status_bar + BAT_LEVEL_OFFSET, 2, update_bat_level}, // "00%"
    {status_bar + POWER_OFFSET, 5, update_power_now},     // "00.00W"
};

// -------------------------------

// Main
int main() {

    // Date will be onlly set at startup
    modules[0].update(modules[0].start, modules[0].len);

    /* Main loop */
    while (1) {

        for (int i = 1; i < NUM_MODULES; i++) {
            modules[i].update(modules[i].start, modules[i].len);
        }

        printf("%s\n", status_bar);

        fflush(stdout);
        sleep(UPDATE_INTERVAL_SECS);
    }

    return 0;
}

// -------------------------------

// Module-updating function definitions (declared in piped.h)
void update_time(char *ptr, const int len) {
    // Set chars for time (hour and minute)
    time_t     now = time(NULL);
    struct tm *t   = localtime(&now);
    strftime(ptr, len + 1, TIME_FORMAT_STR, t);

    *(ptr + len) = ' '; // strftime adds null terminator, reomve it
}

void update_date(char *ptr, int const len) {
    // Set chars for date (day and month)
    time_t     now = time(NULL);
    struct tm *t   = localtime(&now);
    strftime(ptr, len + 1, DATE_FORMAT_STR, t);

    *(ptr + len) = ' '; // strftime adds null terminator, reomve it
}

void update_power_now(char *ptr, const int len) {
    // Read 'power_now' file
    FILE *fPow = fopen(WATTS_PATH, "r");

    if (!fPow) {
        perror("power_now");
        return;
    }

    unsigned long uwatts = 0;
    fscanf(fPow, "%lu", &uwatts);
    fclose(fPow);

    double watts = uwatts / 1000000.0; // uW to W
    snprintf(ptr, len + 1, "%5.2fW", watts);
    *(ptr + len) = 'W';
}

void update_bat_level(char *ptr, const int len) {
    // Read battery percentage
    FILE *fCap = fopen(BAT0_CAPAC_PATH, "r");

    if (!fCap) {
        perror("power_now");
        return;
    }

    int percent = 0;

    fscanf(fCap, "%d", &percent);

    snprintf(ptr, len + 1, "%2d", percent);
    *(ptr + len) = '%';

    fclose(fCap);
}
