#pragma once
#include <iostream>

namespace Variables {
    // to be implemented: getting values from a file

    inline int width{400}; // Screen Width
    inline int height{40}; // Screen Height

    inline bool useDefaultVideoFPS{false}; // True = Caps limit to video default fps, takes priority over capFPS
    inline bool capFPS{false}; // True = Caps limit to maxFPS
    inline int maxFPS{50}; // Video FPS limit
    inline std::string videoFolder{"../video"}; // folder to scan for video files
    constexpr inline char gradient[11] = {" ,:;|/?$#@"} ; // gradient of the ascii symbols main ussage in videoConvert::processFrame
}     