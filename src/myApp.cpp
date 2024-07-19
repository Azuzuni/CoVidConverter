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
    int width{380};
    int height{width/10};

    auto moveCursor = [](int row, int col){
        #ifdef _WIN32
            // Windows-specific implementation
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hConsole == INVALID_HANDLE_VALUE) {
                std::cerr << "Error: Unable to get console handle." << std::endl;
                return;
            }

            COORD position;
            position.X = col;
            position.Y = row;
            SetConsoleCursorPosition(hConsole, position);
        #else
            // Unix-like (ANSI escape codes) implementation
            std::cout << "\033[" << row << ";" << col << "H";
        #endif
    };


    moveCursor(0,0);
    for(int i{0}; i<100; ++i) {
        std::cout << '\n';
    }

    for(int row{0}; row<height; ++row) {
        fmt::print("\n");
        for(int col{0}; col<width/3; ++col) {
            moveCursor(row,col);
            fmt::print("$");
        }
    }


    cv::Mat mainFrame;
    cv::Mat buffor1;
    cv::Mat buffor2;
    // std::shared_ptr<cv::Mat> consCurrentFrame = std::make_unique<cv::Mat>();
    std::unique_ptr<cv::Mat> consCurrentFrame = static_cast<std::unique_ptr<cv::Mat>>(&buffor1) ;
    std::unique_ptr<cv::Mat> consNextFrame = static_cast<std::unique_ptr<cv::Mat>>(&buffor2);       
    r_cap >> mainFrame;
    buffor1 = mainFrame;
    buffor2 = mainFrame;

    while(true) {
        r_cap >> mainFrame;

        if (mainFrame.empty() || consCurrentFrame->empty()) {
            break;
        }

        // Processing logic goes here
        cv::resize(mainFrame,*consNextFrame,cv::Size(width,height));
        cv::cvtColor(*consNextFrame, *consNextFrame, cv::COLOR_BGR2GRAY);

        for(int y{0}; y<width/3; ++y) {
            for(int x{0}; x<height; ++x) {

                cv::Vec3b bgr1 = consCurrentFrame->at<cv::Vec3b>(x,y);
                cv::Vec3b bgr2 = consNextFrame->at<cv::Vec3b>(x,y);


                if(bgr1 != bgr2) {
                    moveCursor(x,y);
                    switch ((bgr2[0]+bgr2[1]+bgr2[2])/3)
                    {
                        case 0 ... 25: fmt::print(" "); break;
                        case 26 ... 50: fmt::print(","); break;
                        case 51 ... 75: fmt::print(":"); break;
                        case 76 ... 100: fmt::print(";"); break;
                        case 101 ... 125: fmt::print("|"); break;
                        case 126 ... 150: fmt::print("/"); break;
                        case 151 ... 175: fmt::print("?"); break;
                        case 176 ... 200: fmt::print("$"); break;
                        case 201 ... 225: fmt::print("#"); break;
                        case 226 ... 255: fmt::print("@"); break;
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
        if (cv::waitKey(100) == 27) { // Press 'Esc' to exit
            break;
        }
    }
}

myApp::myApp(/* args */)
{
}

myApp::~myApp()
{
}


