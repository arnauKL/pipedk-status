/* Config file for pipedk */
#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdio.h>
#include <string.h>

/********************************************
 * DEFINITIONS
 *********************************************/

#define NUM_MODULES (sizeof (modules) / sizeof (modules[0])) // shortcut

#define UPDATE_INTERVAL_SECS 6       // Update interval in seconds
#define DATE_FORMAT_STR      "%d/%m" // Date format string
#define TIME_FORMAT_STR      "%H:%M" // Date format string

// BAT module
#define BAT0_STATE_PATH                                                       \
    "/sys/class/power_supply/BAT0/status" // Filepath to BAT0 status info
                                          // (Charging/Discharging)
#define BAT0_CAPAC_PATH                                                       \
    "/sys/class/power_supply/BAT0/capacity" // Filepath to BAT0 capacity
                                            // (charge percentage)

#define WATTS_PATH "/sys/class/power_supply/BAT0/power_now"

#endif // !CONFIG_H_
