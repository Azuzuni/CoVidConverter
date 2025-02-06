#pragma once
#include <iostream>
#include <chrono>
#include <thread>

class FPSLimiter {
public:
    FPSLimiter(int targetFPS) : targetFPS(targetFPS) {
        frameDuration = std::chrono::milliseconds(1000 / targetFPS);
        lastTime = std::chrono::high_resolution_clock::now();
    }

    // start point of the frame, needed for fps limitation 
    inline const void startFrame() {
        startTime = std::chrono::high_resolution_clock::now();
    }

    // ends frame and applies fps cap if its set
    void endFrame();

    // returns currentFPS
    inline const int getCurrentFPS() const {
        return currentFPS;
    }


private:
    int targetFPS;
    int frameCount{0};
    int sumFPS{0};
    int callCount{0};
    int currentFPS = 0;
    std::chrono::milliseconds frameDuration;
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point lastTime;
};