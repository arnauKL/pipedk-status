#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // C sleep function
#include "config.h"

#define NUM_MODULES     (sizeof(modules)/sizeof(modules[0]))    // shortcut
#define MAX_LEN_MODULE  64                                      // Seems reasonable
#define MAX_LEN_OTUPUT  256                                     //   "       "

/*** MODULE DEFINITIONS ***/ // maybe move to its own file in the future

// Create "moudle_fn" function pointer for each module
typedef void (*module_fn)(char* buf, size_t size);  // now "module_fn fn_name" is equivalent to void (*fn_name)(char* buf, size_t size)

void mod_time(char *buf, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, size, "%d/%m %H:%M", t);
}

// void mod_cpu(char *buf, size_t size) { /* ... */ }
// void mod_mem(char *buf, size_t size) { /* ... */ }

// Define the array of mdules to be used
// module_fn modules[] = { mod_time, mod_cpu, mod_mem };
module_fn modules[] = { mod_time };

void build_status(char *out, size_t size) {
// Function to build the status string from all the different modules selected in the "modules" array

    char tmp[MAX_LEN_MODULE];
    out[0] = '\0';  // Output string

    for (int i = 0; i < NUM_MODULES; ++i) {
        // Call the function and pass the buffer and its size_t
        modules[i](tmp, sizeof(tmp));

        // Add the module's output to the output status string
        strncat(out, tmp, size - strlen(out) - 1);

        if (i < NUM_MODULES - 1) {
            // Add sepparators btween the modules
            strncat(out, " | ", size - strlen(out) - 1);
        }
        else 
            strncat(out, " ", size - strlen(out) - 1);
    }
}

void setStatus(Status stat) {
    printf("%s\n", stat.s);
    fflush(stdout);
}

void printCorrectUsage() {
    printf("Wrong input arguments\n");
    printf("usage: pipedk [OPTIONS]\n");
    printf("\toptions: \"-v\": print version and exit\n");
}

int main(int argc, char *argv[]) {

    /* Input args */
    if (argc == 2 && !strcmp(argv[1], "-v")) {  // strcmp returns 0 if identical
        printf("alpha 0.-1 lol\n");
        return 0;
    }
    else if (argc > 1) {
        printCorrectUsage();
        return 1;
    }

    Status st = { 
        .s = "",
        .len = 0, // 0 chars
    };

    /* Main loop */
    while (1) {
        build_status(st.s, sizeof(st.s));
        setStatus(st);
        sleep(update_interval_secs);
    }

    return 0;
}
