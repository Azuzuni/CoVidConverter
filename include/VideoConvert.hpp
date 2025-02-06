#pragma once
#include <iostream>
#include "opencv2/opencv.hpp"
#include <memory>


class VideoConvert
{
public:
    VideoConvert(cv::VideoCapture& r_cap) : m_rcap(r_cap){
        // load first frame of r_cap video into buffor
        m_rcap >> m_processedFrame;
    };
    ~VideoConvert(){};

    // starts full video conversion process
    void run();
private:
    // processing logic for current frame in m_processedFrame
    void processFrame();
    
private:
    cv::VideoCapture& m_rcap;
    cv::Mat m_processedFrame;
    

};



