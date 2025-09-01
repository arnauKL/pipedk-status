#ifndef PIPED_H_
#define PIPED_H_

#include <stdio.h>
#include <string.h>

/********************************************
 * DEFINITIONS
 *********************************************/

#define NUM_MODULES    (sizeof(modules) / sizeof(modules[0])) // shortcut
#define MAX_LEN_OTUPUT 256                                    //   "       "

// Module-updating function declarations
void update_date(char *ptr, const int len);
void update_time(char *ptr, const int len);
void update_bat_level(char *ptr, const int len);
void update_power_now(char *ptr, const int len);
void update_volume(char *ptr, const int len);

#endif // PIPED_H_
