#include "fmt/core.h"
#include <iostream>
#include "opencv2/opencv.hpp"
int main()
{
    std::cout << cv::max(10,15) << '\n';
    fmt::print("Hello, World!\n");
}