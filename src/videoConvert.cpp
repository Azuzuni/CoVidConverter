
#include "videoConvert.h"
#include "FPSLimiter.h"
#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <memory>

// y for vertical movement, x for horizontal
void videoConvert::moveCursor(int y, int x) {
        #ifdef _WIN32
            // Windows-specific implementation
            COORD position{position.X=x,position.Y=y};
            SetConsoleCursorPosition(hConsole, position);
        #else
            // Unix-like (ANSI escape codes) implementation
            std::cout << "\033[" << row << ";" << col << "H";
        #endif
}

void videoConvert::processFrame(bool first = false) {
    cv::resize(*consNextFrame,*consNextFrame,cv::Size(width,height));
    cv::cvtColor(*consNextFrame, *consNextFrame, cv::COLOR_BGR2GRAY);
    for(int y{0}; y<width/3; ++y) {
            for(int x{0}; x<height; ++x) {

                const cv::Vec3b& bgr1 = consCurrentFrame->at<cv::Vec3b>(x,y);
                const cv::Vec3b& bgr2 = consNextFrame->at<cv::Vec3b>(x,y);

                if(bgr1 != bgr2 || first) {
                    moveCursor(x,y);
                    switch ((bgr2[0]+bgr2[1]+bgr2[2])/3)
                    {
                        case 0 ... 25: printf("%c",' '); break;
                        case 26 ... 50: printf("%c",','); break;
                        case 51 ... 75: printf("%c",':'); break;
                        case 76 ... 100: printf("%c",';'); break;
                        case 101 ... 125: printf("%c",'|'); break;
                        case 126 ... 150: printf("%c",'/'); break;
                        case 151 ... 175: printf("%c",'?'); break;
                        case 176 ... 200: printf("%c",'$'); break;
                        case 201 ... 225: printf("%c",'#'); break;
                        case 226 ... 255: printf("%c",'@'); break;
                        default: printf("%c",' '); break;
                    }
                }

            }
        }
}




void videoConvert::run() {

    // set video FPS limit based on r_cap video fps rate
    FPSLimiter fpsCap(r_cap.get((cv::CAP_PROP_FPS)));
    std::cout << "\033[?25l"; // hide cursor

    moveCursor(0,0);
    processFrame(true);

    while(true) {
        fpsCap.startFrame(); // start point of the frame, needed for fps limitation
        r_cap >> *consNextFrame; // load next frame into consoleNextFrame from r_cap

        // if there are no more frames break the loop
        if (consCurrentFrame->empty() || consNextFrame->empty()) {
            break;
        }
        processFrame();
        std::swap(consCurrentFrame,consNextFrame);
        fpsCap.endFrame();
    }


}