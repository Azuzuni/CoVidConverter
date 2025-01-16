#pragma once
#include <iostream>
#include "opencv2/opencv.hpp"
#include <memory>


class VideoConvert
{
public:
    void run();
    VideoConvert(cv::VideoCapture& r_cap) : m_rcap(r_cap){
        // load first frame of r_cap video into buffor
        m_rcap >> m_processedFrame;
    };
    ~VideoConvert(){};

private:
    void processFrame();
    
private:
    cv::VideoCapture& m_rcap;
    cv::Mat m_processedFrame;
    

};



