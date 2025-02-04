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

    inline const void startFrame() {
        startTime = std::chrono::high_resolution_clock::now();
    }
    void endFrame();

    inline const int getAvgFPS() const {
    return sumFPS/callCount;
    }
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