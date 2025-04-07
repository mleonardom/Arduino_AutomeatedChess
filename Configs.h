#ifndef Configs_h
#define Configs_h

// USEFULL
#define ARRAYSIZE(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0])))

// BOARD
#define SQUARE_SIZE                         44

// MOTORS
#define SPEED_SLOW                          3000
#define SPEED_FAST                          1000

// GLOBALS
#define MD_DEVICE_NAME                      "ChessBoard"

// WiFiManager
#define CONFIGURE_PORTAL_TIMEOUT            300

// BUTTONS
#define LONG_PRESS_TIME                     2000

// GAME
#define AI_USE_CLOCKS                       false

// AI SERVER DATA
#define AI_TIMOUT                           30000

#endif