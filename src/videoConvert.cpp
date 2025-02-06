
#include "VideoConvert.hpp"
#include "FPSLimiter.hpp"
#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <memory>
#include <sstream>
#include "fmt/core.h"
#include "Utilities.hpp"
#include "Variables.hpp"
#include <thread>
#include <chrono>

void VideoConvert::processFrame() {

    // Namespace for Width, Height, Gradient variables
    using namespace Variables;

    // resize and apply grayscale filter on the frame
    cv::resize(m_processedFrame,m_processedFrame, cv::Size(width,height));
    cv::cvtColor(m_processedFrame, m_processedFrame, cv::COLOR_BGR2GRAY);

    // buffor to store character info for the frame
    std::stringstream output;
    output.str().reserve(height*width);

    // go pixel by pixel then calculate average color and assign character from gradient
    for(int x{0}; x<height; ++x) {
        for(int y{0}; y<static_cast<int>(width/3); ++y) {
            const cv::Vec3b& bgr{ m_processedFrame.at<cv::Vec3b>(x,y) };

            switch ((bgr[0]+bgr[1]+bgr[2])/3)
            {
                case 0   ... 25:    output << fmt::format("{}",gradient[0]); break;
                case 26  ... 50:    output << fmt::format("{}",gradient[1]); break;
                case 51  ... 75:    output << fmt::format("{}",gradient[2]); break;
                case 76  ... 100:   output << fmt::format("{}",gradient[3]); break;
                case 101 ... 125:   output << fmt::format("{}",gradient[4]); break;
                case 126 ... 150:   output << fmt::format("{}",gradient[5]); break;
                case 151 ... 175:   output << fmt::format("{}",gradient[6]); break;
                case 176 ... 200:   output << fmt::format("{}",gradient[7]); break;
                case 201 ... 225:   output << fmt::format("{}",gradient[8]); break;
                case 226 ... 255:   output << fmt::format("{}",gradient[9]); break;
            }
        }
        output << fmt::format("\n");
    }

    // start from 0,0 then display entire frame at once
    utilities::moveCursor(0,0);
    std::cout << output.str() << std::flush;
}

static bool isRunning{true};


void VideoConvert::run() {
    namespace Vars = Variables;

    // apply FPS cap / remove it depending on Variables.hpp settings
    if(Vars::useDefaultVideoFPS) Vars::maxFPS = m_rcap.get(cv::CAP_PROP_FPS);
    else if(!Vars::capFPS) Vars::maxFPS = {9999};
    FPSLimiter fpsCap(Vars::maxFPS);

    // hide cursor
    std::cout << "\033[?25l"; 

    while(isRunning) {
        // start point of the frame, needed for fps limitation 
        fpsCap.startFrame(); 
        
        // load next frame into buffer and check if its empty
        m_rcap >> m_processedFrame;
        if (m_processedFrame.empty()) break;

        processFrame();

        // display FPS (might make it into a function in future)
        utilities::moveCursor(0,Vars::height+2);
        std::cout << "FPS: " << fpsCap.getCurrentFPS() << "   ";
        
        // applies fps cap if its set
        fpsCap.endFrame();
    }
}