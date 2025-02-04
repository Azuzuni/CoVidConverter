
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
    namespace Vars = Variables;
    cv::resize(m_processedFrame,m_processedFrame, cv::Size(Vars::width,Vars::height));
    cv::cvtColor(m_processedFrame, m_processedFrame, cv::COLOR_BGR2GRAY);
    constexpr char gradient[11] = {" ,:;|/?$#@"} ;
#if 0
    std::string temporaryString{"\r"};
    for(int x{0}; x<Vars::height; ++x) {
        for(int y{0}; y<static_cast<int>(Vars::width/3); ++y) {
            const cv::Vec3b& bgr = m_processedFrame.at<cv::Vec3b>(x,y);

            switch ((bgr[0]+bgr[1]+bgr[2])/3)
            {
                case 0 ... 25:      temporaryString+=gradient[0]; break;
                case 26 ... 50:     temporaryString+=gradient[1]; break;
                case 51 ... 75:     temporaryString+=gradient[2]; break;
                case 76 ... 100:    temporaryString+=gradient[3]; break;
                case 101 ... 125:   temporaryString+=gradient[4]; break;
                case 126 ... 150:   temporaryString+=gradient[5]; break;
                case 151 ... 175:   temporaryString+=gradient[6]; break;
                case 176 ... 200:   temporaryString+=gradient[7]; break;
                case 201 ... 225:   temporaryString+=gradient[8]; break;
                case 226 ... 255:   temporaryString+=gradient[9]; break;
                default: temporaryString+='E'; break;
            }
        }
        temporaryString+='\n';
    }
#else
    std::stringstream output;
    output.str().reserve(Vars::height*Vars::width);
     for(int x{0}; x<Vars::height; ++x) {
        for(int y{0}; y<static_cast<int>(Vars::width/3); ++y) {
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
#endif
#if 1
    // output.str(temporaryString);
    utilities::moveCursor(0,0);
    std::cout << output.str() << std::flush;
    // fmt::print("{}",output.str());
    // std::cout << std::flush;
#else
    // fmt::
#endif


}

static bool isRunning{true};


void VideoConvert::run() {
    namespace Vars = Variables;

    if(Vars::useDefaultVideoFPS) Vars::maxFPS = m_rcap.get(cv::CAP_PROP_FPS);
    else if(!Vars::capFPS) Vars::maxFPS = {9999};
    FPSLimiter fpsCap(Vars::maxFPS);
    std::cout << "\033[?25l"; // hide cursor
    auto stopLoop = []() {
        int x{0};
        std::cin >> x;
        if(x!=0) isRunning=false;
    };
    std::thread waitForEnter(stopLoop);
    processFrame();
    while(isRunning) {
        fpsCap.startFrame(); // start point of the frame, needed for fps limitation 
        
        m_rcap >> m_processedFrame;
        if (m_processedFrame.empty()) break;

        processFrame();
#if 1
    
#endif

        utilities::moveCursor(0,40);
        std::cout << "FPS: " << fpsCap.getCurrentFPS() << "   ";
        
        fpsCap.endFrame();
    }
    waitForEnter.join();
    utilities::moveCursor(0,Vars::height+1);
    std::cout << "\n\nAverage FPS: "<< fpsCap.getAvgFPS();

}