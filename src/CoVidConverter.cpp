#include "CoVidConverter.hpp"
#include "fmt/core.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "VidSearch.hpp"
#include <thread>
#include "VideoConvert.hpp"
#include "Variables.hpp"






void runCoVidConverter()
{
    namespace Vars = Variables;

    // search video folder and select video option.
    VidSearch vidSrch(Vars::videoFolder);
    vidSrch.run(); 
    cv::VideoCapture cap(vidSrch.selectVid());
    if(!cap.isOpened()) 
    {
        std::cerr << "Failed to open video file.\n"; 
        return;
    }

    // start processing frames from selected video
    VideoConvert vidConvert(cap);
    vidConvert.run();
    cap.release();
}

