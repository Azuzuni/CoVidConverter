#include "myApp.h"
#include "fmt/core.h"
#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "vidSearch.h"
#include <memory>
#include "FPSLimiter.h"
#include <thread>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif






void myApp::run()
{

    try
    {
        vidSearch vidSrch("../video");
        vidSrch.run();

        cv::VideoCapture cap(vidSrch.selectVid());
        if(!cap.isOpened()) 
        {
            constexpr std::string_view errMsg{"Failed to open video file."};
            throw errMsg;
        }

        double fps = cap.get(cv::CAP_PROP_FPS);
        int totalFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
        fmt::print("FPS: {}, Total Frames: {}",fps, totalFrames);
        std::this_thread::sleep_for(std::chrono::seconds(3));

        m_window(cap);
        cap.release();
    }
    
    catch(const fs::path& path)
    {
        std::cerr << "Could not find folder at \"" 
                  << path.string()
                  << "\"\n";
        return;
    }
    catch(const std::string_view errMsg)
    {
        std::cerr << errMsg << '\n';
        return;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void myApp::m_window(cv::VideoCapture& r_cap) {

    FPSLimiter fpsCap(r_cap.get((cv::CAP_PROP_FPS)));
    std::cout << "\033[?25l";

    
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    #endif
    if (hConsole == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Unable to get console handle." << std::endl;
        return;
    }

    auto moveCursor = [&hConsole](int row, int col){
        #ifdef _WIN32
            // Windows-specific implementation
            COORD position{position.X=col,position.Y=row};
            SetConsoleCursorPosition(hConsole, position);
        #else
            // Unix-like (ANSI escape codes) implementation
            std::cout << "\033[" << row << ";" << col << "H";
        #endif
    };

 

    cv::Mat buffor1;
    cv::Mat buffor2;
    
    r_cap >> buffor1;
    r_cap >> buffor2;

    std::unique_ptr<cv::Mat> consCurrentFrame { std::make_unique<cv::Mat>(buffor1) };
    std::unique_ptr<cv::Mat> consNextFrame { std::make_unique<cv::Mat>(buffor2) };


    moveCursor(0,0);
    for(int i{0}; i<m_height; ++i) {
        for(int n{0}; n<m_width/3; ++n) {
            const cv::Vec3b& bgr = consCurrentFrame->at<cv::Vec3b>(n,i);
            switch ((bgr[0]+bgr[1]+bgr[2])/3)
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
                        default: fmt::print("."); break;
                    }
        }
        std::cout << "\n";
    }


    while(true) {
        fpsCap.startFrame();
        r_cap >> *consNextFrame;
        

        if (consCurrentFrame->empty() || consNextFrame->empty()) {
            break;
        }

        // Processing logic goes here
        cv::resize(*consNextFrame,*consNextFrame,cv::Size(m_width,m_height));
        cv::cvtColor(*consNextFrame, *consNextFrame, cv::COLOR_BGR2GRAY);



        for(int y{0}; y<m_width/3; ++y) {
            for(int x{0}; x<m_height; ++x) {

                const cv::Vec3b& bgr1 = consCurrentFrame->at<cv::Vec3b>(x,y);
                const cv::Vec3b& bgr2 = consNextFrame->at<cv::Vec3b>(x,y);

                if(bgr1 != bgr2) {
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
                        default: fmt::print("."); break;
                    }
                }

            }
        }

        // consCurrentFrame = consNextFrame;
        std::swap(consCurrentFrame,consNextFrame);
        moveCursor(m_height+5,0);
        std::cout << "FPS: " << fpsCap.getCurrentFPS();
        fpsCap.endFrame();
    }
}

myApp::myApp(/* args */)
{
}

myApp::~myApp()
{
}


