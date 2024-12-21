#pragma once
#include <iostream>
#include "opencv2/opencv.hpp"
#include <memory>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif


class videoConvert
{
public:   
    int width{210}; // output video horizontal length / amount of columns
    int height{width/10}; // output video vertical lenth / amount of rows

private:
    HANDLE hConsole;
    cv::VideoCapture& r_cap;
    cv::Mat buffor1;
    cv::Mat buffor2;
    std::unique_ptr<cv::Mat> consCurrentFrame{};
    std::unique_ptr<cv::Mat> consNextFrame{};
    
public:
    videoConvert(cv::VideoCapture& r_cap) : r_cap(r_cap){
        // handle required for SetConsoleCursorPosition
        #ifdef _WIN32
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        #endif
        if (hConsole == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Unable to get console handle." << std::endl;
        return;
        }   
        // load first frame of r_cap video into buffor
        r_cap >> buffor1;
        buffor2 = buffor1;
        consCurrentFrame =  std::make_unique<cv::Mat>(buffor1) ;
        consNextFrame =  std::make_unique<cv::Mat>(buffor2) ;
    };
    ~videoConvert(){ std::cout << "destructed";};
    void run();

private:
    // y for rows (vertical change), x for columns (horizontal change)
    void moveCursor(int y, int x);
    void processFrame(bool first);
};



