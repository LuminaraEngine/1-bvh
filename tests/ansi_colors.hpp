#pragma once

// Text formatting options
#define BOLD    "\033[1m"
#define FAINT   "\033[2m"
#define ITALIC  "\033[3m"
#define UNDERLINE "\033[4m"
#define BLINK   "\033[5m"
#define INVERT  "\033[7m"
#define STRIKE  "\033[9m"

// Regular colors
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// Background colors
#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"
#define BG_WHITE   "\033[47m"

// High intensity colors
#define HI_RED     "\033[91m"
#define HI_GREEN   "\033[92m"
#define HI_YELLOW  "\033[93m"
#define HI_BLUE    "\033[94m"
#define HI_MAGENTA "\033[95m"
#define HI_CYAN    "\033[96m"
#define HI_WHITE   "\033[97m"

// High intensity background colors
#define BG_HI_RED     "\033[101m"
#define BG_HI_GREEN   "\033[102m"
#define BG_HI_YELLOW  "\033[103m"
#define BG_HI_BLUE    "\033[104m"
#define BG_HI_MAGENTA "\033[105m"
#define BG_HI_CYAN    "\033[106m"
#define BG_HI_WHITE   "\033[107m"

// Reset to default
#define RESET_COLOR "\033[39m"
#define RESET_BG    "\033[49m"
#define RESET_ALL   "\033[0m"
