#pragma once

#include <stdio.h>

// This is to get malloc
#include <stdlib.h>

// This is to get memset
#include <string.h>

// Used to get the edit timestamp of files
#include <sys/stat.h>

// Obvious right?
#include <math.h>

#ifdef _WIN32
#define DEBUG_BREAK() __debugbreak()
#define EXPORT_FN __declspec(dllexport)
#elif __linux__
#define DEBUG_BREAK() __builtin_debugtrap()
#define EXPORT_FN
#elif __APPLE__
#define DEBUG_BREAK() __builtin_trap()
#define EXPORT_FN
#endif

#define ArraySize(x) (sizeof((x)) / sizeof((x)[0]))

#define b8 char
#define BIT(x) 1 << (x)
#define KB(x) ((unsigned long long)1024 * x)
#define MB(x) ((unsigned long long)1024 * KB(x))
#define GB(x) ((unsigned long long)1024 * MB(x))

// #############################################################################
//                           Logging
// #############################################################################
enum TextColor
{  
    TEXT_COLOR_BLACK,
    TEXT_COLOR_RED,
    TEXT_COLOR_GREEN,
    TEXT_COLOR_YELLOW,
    TEXT_COLOR_BLUE,
    TEXT_COLOR_MAGENTA,
    TEXT_COLOR_CYAN,
    TEXT_COLOR_WHITE,
    TEXT_COLOR_BRIGHT_BLACK,
    TEXT_COLOR_BRIGHT_RED,
    TEXT_COLOR_BRIGHT_GREEN,
    TEXT_COLOR_BRIGHT_YELLOW,
    TEXT_COLOR_BRIGHT_BLUE,
    TEXT_COLOR_BRIGHT_MAGENTA,
    TEXT_COLOR_BRIGHT_CYAN,
    TEXT_COLOR_BRIGHT_WHITE,
    TEXT_COLOR_COUNT
};

template <typename ...Args>
void _log(const char* prefix, const char* msg, TextColor textColor, Args... args)
{
    static const char* TextColorTable[TEXT_COLOR_COUNT] = 
    {    
        "\x1b[30m", // TEXT_COLOR_BLACK
        "\x1b[31m", // TEXT_COLOR_RED
        "\x1b[32m", // TEXT_COLOR_GREEN
        "\x1b[33m", // TEXT_COLOR_YELLOW
        "\x1b[34m", // TEXT_COLOR_BLUE
        "\x1b[35m", // TEXT_COLOR_MAGENTA
        "\x1b[36m", // TEXT_COLOR_CYAN
        "\x1b[37m", // TEXT_COLOR_WHITE
        "\x1b[90m", // TEXT_COLOR_BRIGHT_BLACK
        "\x1b[91m", // TEXT_COLOR_BRIGHT_RED
        "\x1b[92m", // TEXT_COLOR_BRIGHT_GREEN
        "\x1b[93m", // TEXT_COLOR_BRIGHT_YELLOW
        "\x1b[94m", // TEXT_COLOR_BRIGHT_BLUE
        "\x1b[95m", // TEXT_COLOR_BRIGHT_MAGENTA
        "\x1b[96m", // TEXT_COLOR_BRIGHT_CYAN
        "\x1b[97m", // TEXT_COLOR_BRIGHT_WHITE
    };
    
    char formatBuffer[8192] = {};
    sprintf(formatBuffer, "%s %s %s \033[0m", TextColorTable[textColor], prefix, msg);
    
    char textBuffer[8912] = {};
    sprintf(textBuffer, formatBuffer, args...);
    
    puts(textBuffer);
}

#define SM_TRACE(msg, ...) _log("TRACE: ", msg, TEXT_COLOR_GREEN, ##__VA_ARGS__);
#define SM_WARN(msg, ...) _log("WARN: ", msg, TEXT_COLOR_YELLOW, ##__VA_ARGS__);
#define SM_ERROR(msg, ...) _log("ERROR: ", msg, TEXT_COLOR_RED, ##__VA_ARGS__);

#define SM_ASSERT(x, msg, ...)              \
{                                       \
if(!(x))                            \
{                                   \
SM_ERROR(msg, ##__VA_ARGS__);   \
DEBUG_BREAK();                  \
SM_ERROR("Assertion HIT!")      \
}                                   \
}

// Dynamic Array (Vector Replacement)

/*
struct Numbers {
     int *items;
    size_t count;
    size_t capacity;
};
*/

#define da_append(arr, item)\
do{\
if(arr.count >= arr.capacity) {\
if(arr.capacity == 0) arr.capacity = 256;\
else arr.capacity *= 2;\
arr.items = static_cast<decltype(arr.items)> (realloc(arr.items, arr.capacity *sizeof(arr.items)));\
}\
arr.items[arr.count++] = item;\
} while(0)


// General

// Settings
global_variable uint32 SCR_WIDTH = 1920;
global_variable uint32 SCR_HEIGHT = 1200;

enum Engine_Mode {
	Editor,
	Game
};