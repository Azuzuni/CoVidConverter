#include "vidSearch.h"

void vidSearch::run()
{
    this->findVideos();
}

void vidSearch::findVideos()
{
    if(!fs::exists(this->dirPath) || !fs::is_directory(this->dirPath)) throw this->dirPath;
    int i{1};
    for(const auto& file : fs::directory_iterator(this->dirPath))
    {
        if(fs::is_regular_file(file))
        {
            const std::string& fPath{file.path().string()};
            this->fRelPaths.push_back(fPath);
            ++i;
        }
    }
}

void vidSearch::listVideoNames()
{
    int i{1};
    for(const auto& relPath : this->fRelPaths)
    {
        const std::string_view fileName{relPath.substr((this->dirPath.string().length())+1)};
        fmt::print("{}. {}\n", i, fileName);
        ++i;
    }
}
std::string vidSearch::selectVid()
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

vidSearch::vidSearch(std::string_view dirPath)
{
    this->dirPath = dirPath;
}

vidSearch::~vidSearch()
{
}