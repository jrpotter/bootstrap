/**
@file
@brief Console output.
*/
#ifndef _BOOTSTRAP_CONSOLE_H
#define _BOOTSTRAP_CONSOLE_H

// clang-format off

#define ANSI_BLACK  "\e[0;30m"
#define ANSI_RED    "\e[0;31m"
#define ANSI_GREEN  "\e[0;32m"
#define ANSI_YELLOW "\e[0;33m"
#define ANSI_BLUE   "\e[0;34m"
#define ANSI_PURPLE "\e[0;35m"
#define ANSI_CYAN   "\e[0;36m"
#define ANSI_WHITE  "\e[0;37m"
#define ANSI_RESET  "\e[0m"

#define ANSI_BLACK_F(...)  ANSI_BLACK , __VA_ARGS__, ANSI_RESET
#define ANSI_RED_F(...)    ANSI_RED   , __VA_ARGS__, ANSI_RESET
#define ANSI_GREEN_F(...)  ANSI_GREEN , __VA_ARGS__, ANSI_RESET
#define ANSI_YELLOW_F(...) ANSI_YELLOW, __VA_ARGS__, ANSI_RESET
#define ANSI_BLUE_F(...)   ANSI_BLUE  , __VA_ARGS__, ANSI_RESET
#define ANSI_PURPLE_F(...) ANSI_PURPLE, __VA_ARGS__, ANSI_RESET
#define ANSI_CYAN_F(...)   ANSI_CYAN  , __VA_ARGS__, ANSI_RESET
#define ANSI_WHITE_F(...)  ANSI_WHITE , __VA_ARGS__, ANSI_RESET

// clang-format on

#endif /* _BOOTSTRAP_CONSOLE_H */
