#pragma once
#include <iostream>

#if defined(_WIN32) || defined(WIN32)
#define NOMINMAX
#include <Windows.h>
void PrintColorful(std::string str, size_t color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    size_t col = color % 6 + 1;

    SetConsoleTextAttribute(hConsole, col);
    std::cout << str;
    SetConsoleTextAttribute(hConsole, 7);
}

#else
void PrintColorful(std::string str, size_t color)
{
    std::string col = "\033[0m";

    std::array<std::string, 6> colors = {
    "\033[0;34m",
    "\033[0;32m",
    "\033[0;36m",
    "\033[0;31m",
    "\033[0;35m",
    "\033[0;33m"
    };

    col = colors[color];

    std::cout << col << str << "\033[0m";
}
#endif // _WIN32 || WIN32
