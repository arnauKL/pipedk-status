#ifndef PIPED_H_
#define PIPED_H_

#include <string.h>
#include <stdio.h>

/********************************************
 * DEFINITIONS
 *********************************************/

#define TMP_BAT_WARNING_FILE    "/tmp/pipedk_bat_warning"

#define NUM_MODULES     (sizeof(modules)/sizeof(modules[0]))    // shortcut
#define MAX_LEN_MODULE  64                                      // Seems reasonable
#define MAX_LEN_OTUPUT  256                                     //   "       "

// Create "moudle_fn" function pointer for each module
typedef void (*module_fn)(char* buf, size_t size);  // now "module_fn fn_name" is equivalent to void (*fn_name)(char* buf, size_t size)

// Modules
void mod_time(char *buf, size_t size);
void mod_bat(char *buf, size_t size);

#endif // PIPED_H_
