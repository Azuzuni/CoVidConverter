#include "myApp.h"


void myApp::run()
{
    try
    {
        vidSearch vidSrch("..\\video");
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

         while (true) {
        cv::Mat frame;
        cap >> frame;

        if (frame.empty()) {
            break;
        }

        // Your processing logic goes here
        // For example, you can apply some image processing operations on 'frame'
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);

        // Display the processed frame
        cv::imshow("Processed Frame", frame);

        // Check for user input to break out of the loop
        if (cv::waitKey(30) == 27) { // Press 'Esc' to exit
            break;
        }
    }

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

myApp::myApp(/* args */)
{
}

myApp::~myApp()
{
}

