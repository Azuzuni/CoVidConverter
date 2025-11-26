#include "FPSLimiter.hpp"
#include "include/Test_FPSLimiter.hpp"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <chrono>



TEST(FPSLimiterTest, TimeAccuracy) {
  using namespace std::chrono_literals;
  auto startTime = std::chrono::steady_clock::now();
  MockFPSLimiter cap30{30};

  EXPECT_CALL(cap30, startFrame()).Times(1);
  cap30.startFrame();

  EXPECT_CALL(cap30, endFrame()).Times(1);
  cap30.endFrame();
  
  auto duration = std::chrono::steady_clock::now() - startTime;
  bool lessThan{false};
  if( std::chrono::duration_cast<std::chrono::milliseconds>(duration) <  33.33ms ) lessThan = true;
  EXPECT_TRUE(lessThan);
 
}