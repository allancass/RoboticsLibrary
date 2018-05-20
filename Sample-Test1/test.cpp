#include "gtest/gtest.h"
#include "../RoboticsLibrary/Logic.h"
#include "../RoboticsLibrary/Logic.cpp"
#include <thread>
#include <chrono>

using namespace RoboticsLibrary;

	TEST(DeadZone, Upper)
	{
		EXPECT_EQ(1.0, DeadZone<double>(0.8, -0.7, 1.0));		
	}
	TEST(DeadZone, Lower)
	{
		EXPECT_EQ(-1.0, DeadZone<double>(0.8, -0.7, -1.0));
	}
	TEST(DeadZone, Mid)
	{
		EXPECT_EQ(0.0, DeadZone<double>(0.8, -0.7, 0.0));
	}
	TEST(DeadZoneTF, Upper)
	{
		bool TFVar;
		EXPECT_EQ(1.0, DeadZone<double>(0.8, -0.7, 1.0, TFVar));
		EXPECT_FALSE(TFVar);
	}
	TEST(DeadZoneTF, Lower)
	{
		bool TFVar;
		EXPECT_EQ(-1.0, DeadZone<double>(0.8, -0.7, -1.0, TFVar));
		EXPECT_FALSE(TFVar);
	}
	TEST(DeadZoneTF, Mid)
	{
		bool TFVar;
		EXPECT_EQ(0.0, DeadZone<double>(0.8, -0.7, 0.1, TFVar));
		EXPECT_EQ(0.0, DeadZone<double>(0.8, -0.7, -0.1, TFVar));
		EXPECT_TRUE(TFVar);
	}
	TEST(SampleHold) {
		SampleAndHold<double> SNH(0.0);
		EXPECT_EQ(SNH.Get(), 0.0);
		SNH.Set(1.0, false);
		EXPECT_EQ(SNH.Get(), 0.0);
		SNH.Set(1.0, true);
		EXPECT_EQ(SNH.Get(), 1.0);
		SNH.Set(0.0, false);
		EXPECT_EQ(SNH.Get(), 1.0);
	}
	
	Timer TimA(1.0);
	TEST(TimerTest, TimerInit)
	{
		EXPECT_EQ(TimA.IsActive(), false);
		EXPECT_EQ(TimA.IsExpired(), false);
		EXPECT_LT(TimA.TimeElapsed(), 0.0);
	}
	TEST(TimerTest, TimerStart)
	{
		TimA.Start();
		EXPECT_EQ(TimA.IsExpired(), false);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		EXPECT_GT(TimA.TimeElapsed(), 0.0);
		EXPECT_EQ(TimA.IsExpired(), false);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		EXPECT_GT(TimA.TimeElapsed(), 0.0);
		EXPECT_EQ(TimA.IsExpired(), false);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		EXPECT_GT(TimA.TimeElapsed(), 0.0);
		EXPECT_EQ(TimA.IsExpired(), false);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		EXPECT_GT(TimA.TimeElapsed(), 0.0);
		EXPECT_EQ(TimA.IsExpired(), false);
		EXPECT_GT(TimA.TimeElapsed(), 0.0);
		std::this_thread::sleep_for(std::chrono::milliseconds(400));
		EXPECT_EQ(TimA.IsExpired(), true);
	}
	TEST(TimerTest, TimerStop)
	{
		TimA.Stop();
		EXPECT_EQ(TimA.IsActive(), false);
		EXPECT_EQ(TimA.IsExpired(), false);
		EXPECT_LT(TimA.TimeElapsed(), 0.0);
	}
	TEST(TimerTest, TimerReset)
	{
		TimA.Reset();
		EXPECT_EQ(TimA.IsExpired(), false);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		EXPECT_GT(TimA.TimeElapsed(), 0.0);
		EXPECT_EQ(TimA.IsExpired(), false);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		EXPECT_GT(TimA.TimeElapsed(), 0.0);
		EXPECT_EQ(TimA.IsExpired(), false);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		EXPECT_GT(TimA.TimeElapsed(), 0.0);
		EXPECT_EQ(TimA.IsExpired(), false);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		EXPECT_GT(TimA.TimeElapsed(), 0.0);
		EXPECT_EQ(TimA.IsExpired(), false);
		TimA.Reset();
		EXPECT_EQ(TimA.IsExpired(), false);
		std::this_thread::sleep_for(std::chrono::milliseconds(400));
		EXPECT_GT(TimA.TimeElapsed(), 0.0);
		EXPECT_EQ(TimA.IsExpired(), false);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		EXPECT_GT(TimA.TimeElapsed(), 0.0);
		EXPECT_EQ(TimA.IsExpired(), false);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		EXPECT_GT(TimA.TimeElapsed(), 0.0);
		EXPECT_EQ(TimA.IsExpired(), false);
		EXPECT_GT(TimA.TimeElapsed(), 0.0);
		std::this_thread::sleep_for(std::chrono::milliseconds(400));
		EXPECT_EQ(TimA.IsExpired(), true);
	}
