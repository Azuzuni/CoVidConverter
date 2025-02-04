#include "FPSLimiter.hpp"


void FPSLimiter::endFrame() {
    using namespace std::chrono_literals;
    auto frameTime{std::chrono::high_resolution_clock::now() - startTime};
    if (frameTime < frameDuration)
        std::this_thread::sleep_for(frameDuration - frameTime);

    ++frameCount;
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = currentTime - lastTime;

    if (elapsed >= (1s)) {
        lastTime = currentTime;
        currentFPS = frameCount;
        frameCount = 0;
    }
    ++callCount;
    sumFPS+=currentFPS;
}


