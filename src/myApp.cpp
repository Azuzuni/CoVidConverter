#include "myApp.h"
#include "fmt/core.h"
#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "vidSearch.h"
#include <thread>
#include "videoConvert.h"






void myApp::run()
{

    try
    {
        // search video folder and select video option.
        vidSearch vidSrch("../video");
        vidSrch.run(); 
        cv::VideoCapture cap(vidSrch.selectVid());
        if(!cap.isOpened()) 
        {
            constexpr std::string_view errMsg{"Failed to open video file."};
            throw errMsg;
        }

        // print selected video fps rate and total frame amount
        double fps = cap.get(cv::CAP_PROP_FPS);
        int totalFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
        fmt::print("FPS: {}, Total Frames: {}",fps, totalFrames);
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // start processing frames from selected video
        videoConvert vidConvert(cap);
        vidConvert.run();
        cap.release();
    }
    
    // error handling in case path to the video folder
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



myApp::myApp(/* args */)
{
}

myApp::~myApp()
{
}


