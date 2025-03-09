#ifndef Configs_h
#define Configs_h

// IMPORTANT !!!!!
// Comment to use SSD1306 display instead
//#define USE_I2C

// USEFULL
#define ARRAYSIZE(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0])))

// BOARD
#define SQUARE_SIZE                         100

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

#endif