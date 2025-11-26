#pragma once
#include "FPSLimiter.hpp"
#include <gmock/gmock.h>

class MockFPSLimiter : public FPSLimiter {
  public:
    MockFPSLimiter(int targetFPS) : FPSLimiter{targetFPS} {}
    virtual ~MockFPSLimiter() = default;
    MOCK_METHOD(void, startFrame, (), (const));
    MOCK_METHOD(void, endFrame, ());
    MOCK_METHOD(int,  getCurrentFPS, (), (const));
};