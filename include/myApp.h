#pragma once
#include "opencv2/opencv.hpp"

class myApp
{
private:

public:
    void run();

    myApp();
    ~myApp();
    int m_width{210};
    int m_height{m_width/10};
private:
    void m_window(cv::VideoCapture& r_Cap);
};
