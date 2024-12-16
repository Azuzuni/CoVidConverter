#include "myApp.h"
#include "fmt/core.h"
#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "vidSearch.h"
#include <memory>


#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <chrono>

class FPSCounter {
public:
    FPSCounter() : frameCount(0), startTime(std::chrono::high_resolution_clock::now()) {}

    // Call this function every frame to count the FPS
    void update() {
        frameCount++;
        
        // Check if one second has passed
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(now - startTime);
        
        if (elapsed.count() >= 1.0) {
            // Output FPS
            std::cout << "FPS: " << frameCount << std::endl;
            
            // Reset for the next second
            frameCount = 0;
            startTime = now;
        }
    }

private:
    int frameCount;  // Number of frames processed in one second
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;  // Time when FPS was last calculated
};



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
    FPSCounter fpsCounter;
    int width{210};
    int height{width/10};
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
            COORD position;
            position.X = col;
            position.Y = row;
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
    while(true) {

        moveCursor(25,0);
        fpsCounter.update();
        r_cap >> *consNextFrame;

        if (consCurrentFrame->empty() || consNextFrame->empty()) {
            break;
        }

        // Processing logic goes here
        cv::resize(*consNextFrame,*consNextFrame,cv::Size(width,height));
        cv::cvtColor(*consNextFrame, *consNextFrame, cv::COLOR_BGR2GRAY);



        for(int y{0}; y<width/3; ++y) {
            for(int x{0}; x<height; ++x) {

                const cv::Vec3b& bgr1 = consCurrentFrame->at<cv::Vec3b>(x,y);
                const cv::Vec3b& bgr2 = consNextFrame->at<cv::Vec3b>(x,y);

                if(bgr1 != bgr2) {
                    moveCursor(x,y);
                    switch ((bgr2[0]+bgr2[1]+bgr2[2])/3)
                    {
                        case 0 ... 25: printf("%c",'.'); break;
                        case 26 ... 50: printf("%c",','); break;
                        case 51 ... 75: printf("%c",':'); break;
                        case 76 ... 100: printf("%c",';'); break;
                        case 101 ... 125: printf("%c",'|'); break;
                        case 126 ... 150: printf("%c",'/'); break;
                        case 151 ... 175: printf("%c",'?'); break;
                        case 176 ... 200: printf("%c",'$'); break;
                        case 201 ... 225: printf("%c",'#'); break;
                        case 226 ... 255: printf("%c",'@'); break;
                        // case 0 ... 25: printf("%c",'.'); break;
                        // case 26 ... 50: fmt::print(","); break;
                        // case 51 ... 75: fmt::print(":"); break;
                        // case 76 ... 100: fmt::print(";"); break;
                        // case 101 ... 125: fmt::print("|"); break;
                        // case 126 ... 150: fmt::print("/"); break;
                        // case 151 ... 175: fmt::print("?"); break;
                        // case 176 ... 200: fmt::print("$"); break;
                        // case 201 ... 225: fmt::print("#"); break;
                        // case 226 ... 255: fmt::print("@"); break;
                        default: fmt::print("."); break;
                    }
                }

            }
        }

        // consCurrentFrame = consNextFrame;
        std::swap(consCurrentFrame,consNextFrame);
        // Display the processed mainFrame
        // cv::imshow("Processed mainFrame", mainFrame);
        // cv::imshow("Resized mainFrame", consCurrentFrame);

        // Check for user input to break out of the loop
        // if (cv::waitKey(100) == 27) { // Press 'Esc' to exit
        //     break;
        // }
    }
}

myApp::myApp(/* args */)
{
}

myApp::~myApp()
{
}


