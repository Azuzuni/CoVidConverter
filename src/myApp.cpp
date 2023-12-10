#include "myApp.h"


void myApp::run()
{
    const std::string vidName{"bad_apple.mp4"};
    cv::VideoCapture vid("../"+vidName);
    fmt::print("{}\n{}",vid.isOpened(),vidName);
}

myApp::myApp(/* args */)
{
}

myApp::~myApp()
{
}

