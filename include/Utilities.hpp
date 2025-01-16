#pragma once
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace utilities {

    static HANDLE hConsole;
    #ifdef _WIN32
        // x for horizontal, y for vertical movement,
        void moveCursor(const int& x,const int& y) {
                if(!hConsole) {
                    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    if (hConsole == INVALID_HANDLE_VALUE) {
                        std::cerr << "Error: Unable to get console handle." << std::endl;
                        return;
                    }   
                } 
                COORD position{position.X=x,position.Y=y};
                SetConsoleCursorPosition(hConsole, position);
        }
    #else
        // x for horizontal, y for vertical movement,
        void moveCursor(const int& x,const int& y) {
            std::cout << "\033[" << x << ";" << y << "H";
        }
    #endif
}