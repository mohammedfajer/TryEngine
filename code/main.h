#if !defined(MAIN_H)

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SDL.h"

#include <sstream>
#include <fstream>
#include <string>
#include <cmath> // Include the <cmath> header for trigonometric functions and constants like PI

#include <stdint.h>

// TODO(mo): Implement Sinf for ourselves
#include <math.h>

#define local_persist static
#define global_variable static
#define internal static

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int32 bool32;

typedef float real32;
typedef double real64;

#define Pi32 3.14159265359



#define MAIN_H
#endif