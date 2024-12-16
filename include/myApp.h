#pragma once
#include "opencv2/opencv.hpp"

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
