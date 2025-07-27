#include <stdio.h>
#include <time.h>
#include <unistd.h> // C sleep function
#include "config.h"

void printStatus(const char* s); /* print the status string (s) to stdout and flush so piping the output works*/

/* 
 * slstatus clone.
 * Main goal: have more efficient slstatus.
 * - File watchers instead of polling
 * - Instantly update volume levels without polling (no clue how yet)
 * - Some way to show custom modules: music, weather, etc
 *
 *
 * Ideas: 
 * - Fixed max-character length string: say 50 chars, something it will never extend and check bounds to avoid it going over
 * 
 * */

int main() {
    char *output_string = "Hello world!";  // This won't do, look into dynamic strings or sth

    while (1) {
        printStatus(output_string);
        sleep(10);
    }

    return 0;
}


void printStatus(const char* s) {
    printf("%s\n", s);
    fflush(stdout);
}
