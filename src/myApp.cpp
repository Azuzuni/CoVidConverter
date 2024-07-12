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
        fmt::print("FPS: {}, Total frame1s: {}",fps, totalFrames);

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
    int width{480};
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
            fmt::print(" ");
        }
    }


    cv::Mat frame1;
    cv::Mat consoleCurrentFrame;        
    cv::Mat consoleNextFrame;        
    r_cap >> frame1;
    consoleCurrentFrame = frame1;
    consoleNextFrame = frame1;

    while(true) {
        if(cv::waitKey(50) == 107) {
            continue;
        }
        r_cap >> frame1;

        if (frame1.empty() || consoleNextFrame.empty()) {
            break;
        }
        // Processing logic goes here
        cv::resize(frame1,consoleNextFrame,cv::Size(width,height));
        // cv::cvtColor(frame1, frame1, cv::COLOR_BGR2GRAY);
        cv::cvtColor(consoleNextFrame, consoleNextFrame, cv::COLOR_BGR2GRAY);

        // int x{0}; x<width; ++x
        // int y{0}; y<width; ++y
        // int x{0}; x<height; ++x
        
        for(int y{0}; y<width/3; ++y) {
            for(int x{0}; x<height; ++x) {

                int blue1 = consoleCurrentFrame.at<cv::Vec3b>(x,y)[0];
                int green1 = consoleCurrentFrame.at<cv::Vec3b>(x,y)[1];
                int red1 = consoleCurrentFrame.at<cv::Vec3b>(x,y)[2];
                
                int blue2 = consoleNextFrame.at<cv::Vec3b>(x,y)[0];
                int green2 = consoleNextFrame.at<cv::Vec3b>(x,y)[1];
                int red2 = consoleNextFrame.at<cv::Vec3b>(x,y)[2];

                int bgr1 = (blue1+green1+red1)/3;
                int bgr2 = (blue2+green2+red2)/3;

                if(bgr1 != bgr2) {
                    moveCursor(x,y);
                    switch (bgr2)
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
                        // case 0 ... 25: fmt::print("."); break;
                        // case 26 ... 50: fmt::print(","); break;
                        // case 51 ... 75: fmt::print(";"); break;
                        // case 76 ... 100: fmt::print("v"); break;
                        // case 101 ... 125: fmt::print("l"); break;
                        // case 126 ... 150: fmt::print("L"); break;
                        // case 151 ... 175: fmt::print("F"); break;
                        // case 176 ... 200: fmt::print("E"); break;
                        // case 201 ... 225: fmt::print("#"); break;
                        // case 226 ... 255: fmt::print("$"); break;
                        default: fmt::print("."); break;
                        
                    }
                }

            }
        }

        consoleCurrentFrame = consoleNextFrame;


        // Display the processed frame1
        cv::imshow("Processed frame1", frame1);
        // cv::imshow("Resized frame1", consoleCurrentFrame);

        // Check for user input to break out of the loop
        if (cv::waitKey(30) == 27) { // Press 'Esc' to exit
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

