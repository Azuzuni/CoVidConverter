#pragma once
#include "fmt/core.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "vidSearch.h"
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

class myApp
{
private:

public:
    void run();

    myApp();
    ~myApp();
private:
    void m_window(cv::VideoCapture& r_Cap);

};
