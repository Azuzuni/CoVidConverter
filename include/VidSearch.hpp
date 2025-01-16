#pragma once
#include <filesystem>
#include <vector>
#include <iostream>

namespace fs = std::filesystem;

class VidSearch
{
public:
    void run();
    void listVideoNames();
    std::string selectVid();
    VidSearch(std::string_view dirPath) : m_dirPath(dirPath){};
    ~VidSearch(){};
public:
    std::vector<std::string> fRelPaths;

private:
    void findVideos(); // search video folder and assing all files inside to fRelPaths

private:
    fs::path m_dirPath{""};      // The input video directory relative path from VidSearch.h

};

