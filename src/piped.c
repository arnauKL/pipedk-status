#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>         // C sleep function
#include <sys/inotify.h>    // File watchers (inode notify, linux only)
#include "piped.h"
#include "config.h"

// TODO: File watchers instead of polling
// TODO: Instantly update volume levels without polling (no clue how yet)
// TODO: Some way to show custom modules: music, weather, etc

/*******************************************
 * UTILS
 ********************************************/

void printCorrectUsage() {
    printf("Wrong input arguments\n");
    printf("usage: pipedk [OPTIONS]\n");
    printf("\toptions: \"-v\": print version and exit\n");
}

void checkBatteryWarning(char *tmp) {
    long batteryLevel = strtol(tmp, NULL, 10);
    FILE *fTmp = fopen(TMP_BAT_WARNING_FILE, "r");

    if (fTmp == NULL && BAT_WARNING_LEVEL >= batteryLevel) {
        system("notify-send 'LOW BATTERY' 'Battery below threshold, please charge'");
        fTmp = fopen(TMP_BAT_WARNING_FILE, "w");  // reassign to write
    } else if (fTmp != NULL && BAT_WARNING_LEVEL < batteryLevel) {
        remove(TMP_BAT_WARNING_FILE);
    }

    if (fTmp != NULL) {
        fclose(fTmp);
    }
}


/*********************************************
 * MODULE FUNCTIONS
 **********************************************/

void mod_time(char *buf, size_t size) {
    // Format current time
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, size, TIME_FORMAT_STR, t);
}

void  mod_bat(char *buf, size_t size) {
    // Format battery level and charging status
    // First Capacity
    FILE *fBAT = fopen(BAT0_CAPAC_PATH, "r");

    if (fBAT == NULL) {
        strcpy(buf, "BAT0 couldn't open");  // Print the error instead of the info
    }
    else {
        strncpy(buf, "BAT0: ", MAX_LEN_MODULE);

        char tmp[MAX_LEN_MODULE];
        fgets(tmp, MAX_LEN_MODULE, fBAT);
        const int len = strlen(tmp);
        if (len > 0) {
            tmp[len - 1] = '\0';    // Remove "\n"
        }
        strncat(buf, tmp, MAX_LEN_MODULE);
        strncat(buf, "%", MAX_LEN_MODULE);

        checkBatteryWarning(tmp);

        fclose(fBAT);
    }

    // Add status info (Charging,  Discharging)
    fBAT = fopen(BAT0_STATE_PATH, "r");
    if (fBAT == NULL) {
        strncpy(buf, "BAT0 state file error", MAX_LEN_MODULE);
    }
    else {
        // Append character
        switch (fgetc(fBAT)) {
            case 'C':
                strncat(buf, "+", MAX_LEN_MODULE);
                break;
            case 'D':
                strncat(buf, "-", MAX_LEN_MODULE);
                break;
            default:
                strncat(buf, "o", MAX_LEN_MODULE);  // F for "Full"
                break;
        }

        fclose(fBAT);
    }
}

// void mod_audio(char *buf, size_t size) {

// }
// void mod_cpu(char *buf, size_t size) { /* ... */ }
// void mod_mem(char *buf, size_t size) { /* ... */ }


/*******************************************
 * CORE LOGIC 
 ********************************************/
void build_status(char *out, size_t size) {
    // Function to build the status string from all the different modules selected in the "modules" array

    char tmp[MAX_LEN_MODULE];
    out[0] = '\0';  // Output string

    strncat(out, " ", size - strlen(out) - 1);
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

void setStatus(const char* stat) {
    printf("%s\n", stat);
    fflush(stdout);
}


/*******************************************
 * MAIN
 ********************************************/
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

    /* Initialisation */
    const char status_string[MAX_LEN_OTUPUT] = "";

    // File watchers
    int fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        return -1;
    }


    /* Main loop */
    while (1) {
        build_status(status_string, sizeof(status_string));
        setStatus(status_string);
        sleep(UPDATE_INTERVAL_SECS);
    }

    return 0;
}
