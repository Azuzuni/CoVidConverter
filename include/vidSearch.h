#pragma once
#include <filesystem>
#include <vector>
#include <iostream>

namespace fs = std::filesystem;

class vidSearch
{
// VARIABLES
private:
    fs::path dirPath{""};      // The input video directory relative path from vidSearch.h

public:
    std::vector<std::string> fRelPaths; // File Relative Paths

// METHODS
private:
    void findVideos(); // search video folder and assing all files inside to fRelPaths

public:
    void run();
    void listVideoNames();
    std::string selectVid();
    vidSearch(std::string_view dirPath) : dirPath(dirPath){};
    ~vidSearch(){};
};

