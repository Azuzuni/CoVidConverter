  
#include "VidSearch.hpp"
#include "fmt/core.h"
void VidSearch::run()
{
    this->findVideos();
}

void VidSearch::findVideos()
{
    if(!fs::exists(m_dirPath) || !fs::is_directory(m_dirPath)) {
        std::cerr << "Could not find folder: " << m_dirPath << '\n';
        return;
    }
    int i{1};
    for(const auto& file : fs::directory_iterator(m_dirPath))
    {
        if(fs::is_regular_file(file))
        {
            const std::string& fPath{file.path().string()};
            this->fRelPaths.push_back(fPath);
            ++i;
        }
    }
}

void VidSearch::listVideoNames()
{
    int i{1};
    for(const auto& relPath : this->fRelPaths)
    {
        const std::string& fileName{relPath.substr((m_dirPath.string().length())+1)};
        fmt::print("{}. {}\n", i, fileName);
        ++i;
    }
}
std::string VidSearch::selectVid()
{
    
    this->listVideoNames();

    std::size_t select{};
    fmt::print("Select ID of the video you would like to convert into console video: ");
    std::cin >> select;
    if(select-1 > this->fRelPaths.size()) 
    {
        constexpr std::string_view errMsg{"Please select valid video ID."};
        throw errMsg;
    }
    return this->fRelPaths[select-1];
}