#pragma once
#include <iostream>
#include <chrono>
#include <thread>

class FPSLimiter {
public:
    FPSLimiter(int targetFPS) : targetFPS(targetFPS) {
        frameDuration = std::chrono::milliseconds(1000 / targetFPS); // Time per frame
        frameCount = 0;
        lastTime = std::chrono::high_resolution_clock::now();
    }

    void startFrame() {
        startTime = std::chrono::high_resolution_clock::now();
    }

    void endFrame() {
        auto frameTime{std::chrono::high_resolution_clock::now() - startTime};
        if (frameTime < frameDuration) {
            // Sleep for the remaining time to cap FPS
            std::this_thread::sleep_for(frameDuration - frameTime);
        }

        // Update frame count and check FPS every second
        ++frameCount;
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;

        if (elapsed.count() >= 1.0f) {
            lastTime = currentTime;
            currentFPS = frameCount;
            frameCount = 0;  // Reset frame count for the next second
        }
    }

    int getCurrentFPS() const {
        return currentFPS;
    }

private:
    int targetFPS;
    int frameCount;
    int currentFPS = 0;
    std::chrono::milliseconds frameDuration;
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point lastTime;
};