/* Config file for pipedk */
#ifndef CONFIG_H_
#define CONFIG_H_

#include <string.h>
#include <stdio.h>

/********************************************
 * DEFINITIONS
 *********************************************/

#define TMP_BAT_WARNING_FILE    "/tmp/pipedk_bat_warning"

#define NUM_MODULES     (sizeof(modules)/sizeof(modules[0]))    // shortcut
#define MAX_LEN_OTUPUT  256                                     // Seems reasonable

#define UPDATE_INTERVAL_SECS    5       // Update interval in seconds
#define DATE_FORMAT_STR "%d/%m"         // Date format string
#define TIME_FORMAT_STR "%H:%M"         // Date format string

// BAT module
#define BAT0_STATE_PATH "/sys/class/power_supply/BAT0/status"  // Filepath to BAT0 status info (Charging/Discharging)
#define BAT0_CAPAC_PATH "/sys/class/power_supply/BAT0/capacity"  // Filepath to BAT0 capacity (charge percentage)

#endif // !CONFIG_H_
