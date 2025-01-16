
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

void VideoConvert::processFrame() {
    namespace Vars = Variables;
    cv::resize(m_processedFrame,m_processedFrame, cv::Size(Vars::width,Vars::height));
    cv::cvtColor(m_processedFrame, m_processedFrame, cv::COLOR_BGR2GRAY);
    std::string temporaryString{"\r"};
    for(int x{0}; x<Vars::height; ++x) {
        for(int y{0}; y<static_cast<int>(Vars::width/3); ++y) {
            const cv::Vec3b& bgr = m_processedFrame.at<cv::Vec3b>(x,y);

            switch ((bgr[0]+bgr[1]+bgr[2])/3)
            {
                case 0 ... 25: temporaryString+=' '; break;
                case 26 ... 50: temporaryString+=','; break;
                case 51 ... 75: temporaryString+=':'; break;
                case 76 ... 100: temporaryString+=';'; break;
                case 101 ... 125: temporaryString+='|'; break;
                case 126 ... 150: temporaryString+='/'; break;
                case 151 ... 175: temporaryString+='?'; break;
                case 176 ... 200: temporaryString+='$'; break;
                case 201 ... 225: temporaryString+='#'; break;
                case 226 ... 255: temporaryString+='@'; break;
                default: temporaryString+='E'; break;
            }
        }
        temporaryString+='\n';
    }
        
    std::stringstream output;
    output.str(temporaryString);
    utilities::moveCursor(0,0);
    std::cout << output.str() << std::flush;
}




void VideoConvert::run() {
    namespace Vars = Variables;
    if(Vars::useDefaultVideoFPS) Vars::maxFPS = m_rcap.get(cv::CAP_PROP_FPS);
    else if(!Vars::capFPS) Vars::maxFPS = {9999};
    FPSLimiter fpsCap(Vars::maxFPS);
    std::cout << "\033[?25l"; // hide cursor

    processFrame();
    while(true) {
        fpsCap.startFrame(); // start point of the frame, needed for fps limitation
        
        m_rcap >> m_processedFrame; // load next frame into consoleNextFrame from m_rcap
        if (m_processedFrame.empty()) break;

        processFrame();
        utilities::moveCursor(0,40);
        std::cout << "FPS: " << fpsCap.getCurrentFPS() << "   ";
        
        fpsCap.endFrame();
    }


}