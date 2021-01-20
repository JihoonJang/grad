/**
 * @file debug.h
 * @author jhjang
 * @brief debug macros
 * @version 0.1
 * @date 2021-01-14
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <cassert>
#ifndef NDEBUG

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define debug_print(fmt, ...)          \
  {                                    \
    fprintf(stderr, ANSI_COLOR_CYAN);  \
    fprintf(stderr, fmt, __VA_ARGS__); \
    fprintf(stderr, ANSI_COLOR_RESET); \
    fprintf(stderr, "\n");             \
  }

constexpr bool _DEBUG = true;

#else

constexpr bool _DEBUG = false;

#define debug_print(fmt, ...)

#endif  // NDEBUG
#endif  // DEBUG_H_
