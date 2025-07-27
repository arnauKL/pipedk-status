/* Config file for pipedk */

/* 
 * slstatus clone.
 * Main goal: have more efficient slstatus.
 * - File watchers instead of polling
 * - Instantly update volume levels without polling (no clue how yet)
 * - Some way to show custom modules: music, weather, etc
 *
 * Ideas: 
 * - Fixed max-character length string: say 50 chars, something it will never extend and check bounds to avoid it going over
 */

/* Definitions (should maybe belong somewhere else) */
#define MAXLEN 100

typedef struct {
    const char s[MAXLEN];
    int len;
} Status;

typedef struct {
    const char m;
    const unsigned int update_int;
} Module;


/* Real user config */

// const Module[] modules {
//
// };

const unsigned int update_interval_secs = 2;
