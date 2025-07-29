/* Config file for pipedk */

#define UPDATE_INTERVAL_SECS    2       // Update interval in seconds
#define TIME_FORMAT_STR "%d/%m %H:%M"   // Datetime format string

// BAT module
#define BAT0_STATE_PATH "/sys/class/power_supply/BAT0/status"  // Filepath to BAT0 status info (Charging/Discharging)
#define BAT0_CAPAC_PATH "/sys/class/power_supply/BAT0/capacity"  // Filepath to BAT0 capacity (charge percentage)
