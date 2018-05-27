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
	TEST(TimerTest, ZeroTime)
	{
		Timer TimB(0.0);
		TimB.Start();
		EXPECT_EQ(TimA.IsExpired(), true);
		TimB.Reset();
		EXPECT_EQ(TimA.IsExpired(), true);
	}

	TEST(StableTest, SolidSignalTFT)
	{
		bool StateA = true;
		bool StateB = false;
		Stable StableDemo(StateA, 0.3, 0.3);
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateA);
		EXPECT_EQ(StableDemo.GetState(), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EXPECT_EQ(StableDemo.GetState(), StateB);
		EXPECT_EQ(StableDemo.Sample(StateB), StateB);
		EXPECT_EQ(StableDemo.GetState(), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateB);
		EXPECT_EQ(StableDemo.Sample(StateA), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateB);
		EXPECT_EQ(StableDemo.Sample(StateA), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateA), StateA);
		EXPECT_EQ(StableDemo.GetState(), StateA);
	}
	TEST(StableTest, SolidSignalFTF)
	{

		bool StateA = false;
		bool StateB = true;
		Stable StableDemo(StateA, 0.3, 0.3);
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateA);
		EXPECT_EQ(StableDemo.GetState(), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EXPECT_EQ(StableDemo.GetState(), StateB);
		EXPECT_EQ(StableDemo.Sample(StateB), StateB);
		EXPECT_EQ(StableDemo.GetState(), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateB);
		EXPECT_EQ(StableDemo.Sample(StateA), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateB);
		EXPECT_EQ(StableDemo.Sample(StateA), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateA), StateA);
		EXPECT_EQ(StableDemo.GetState(), StateA);
	}
	TEST(StableTest, SolidSignalFFT)
	{
		bool StateA = true;
		bool StateB = false;
		Stable StableDemo(StateA, 0.3, 0.3);
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateA), StateA);
		EXPECT_EQ(StableDemo.GetState(), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateA), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateA), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateA);
		EXPECT_EQ(StableDemo.GetState(), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EXPECT_EQ(StableDemo.GetState(), StateB);
		EXPECT_EQ(StableDemo.Sample(StateA), StateB);
		EXPECT_EQ(StableDemo.GetState(), StateB);
	}
	TEST(StableTest, SolidSignalTTF)
	{

		bool StateA = false;
		bool StateB = true;
		Stable StableDemo(StateA, 0.3, 0.3);
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateA), StateA);
		EXPECT_EQ(StableDemo.GetState(), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateA), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateA), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateA);
		EXPECT_EQ(StableDemo.GetState(), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EXPECT_EQ(StableDemo.GetState(), StateB);
		EXPECT_EQ(StableDemo.Sample(StateA), StateB);
		EXPECT_EQ(StableDemo.GetState(), StateB);
	}
	TEST(StableTest, SignalFastSwitchF)
	{

		bool StateA = true;
		bool StateB = false;
		Stable StableDemo(StateA, 0.0, 0.0);
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateA), StateA);
		EXPECT_EQ(StableDemo.GetState(), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateB);
		EXPECT_EQ(StableDemo.GetState(), StateB);
	}
	TEST(StableTest, SignalFastSwitchT)
	{

		bool StateA = false;
		bool StateB = true;
		Stable StableDemo(StateA, 0.0, 0.0);
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateA), StateA);
		EXPECT_EQ(StableDemo.GetState(), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateB);
		EXPECT_EQ(StableDemo.GetState(), StateB);
	}
	TEST(StableTest, LooseSignalTFT)
	{
		bool StateA = true;
		bool StateB = false;
		Stable StableDemo(StateA, 0.3, 0.3);
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateA);
		EXPECT_EQ(StableDemo.GetState(), StateA);
		
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateA), StateA);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateA), StateA);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateB), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EXPECT_EQ(StableDemo.GetState(), StateB);
		EXPECT_EQ(StableDemo.Sample(StateB), StateB);
		EXPECT_EQ(StableDemo.GetState(), StateB);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateB);
		EXPECT_EQ(StableDemo.Sample(StateA), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateB);
		EXPECT_EQ(StableDemo.Sample(StateB), StateB);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateB);
		EXPECT_EQ(StableDemo.Sample(StateA), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateB);
		EXPECT_EQ(StableDemo.Sample(StateB), StateB);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateB);
		EXPECT_EQ(StableDemo.Sample(StateA), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(StableDemo.GetState(), StateB);
		EXPECT_EQ(StableDemo.Sample(StateA), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EXPECT_EQ(StableDemo.GetState(), StateA);
		EXPECT_EQ(StableDemo.Sample(StateA), StateA);
		EXPECT_EQ(StableDemo.GetState(), StateA);
	}

	TEST(DebounceTest, SolidSignalTFT)
	{
		bool StateA = true;
		bool StateB = false;
		Debounce DebounceDemo(StateA, 0.3, 0.3);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateA);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		EXPECT_EQ(DebounceDemo.GetState(), StateB);	
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateB);
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateA);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
	}
	TEST(DebounceTest, SolidSignalFTF)
	{

		bool StateA = false;
		bool StateB = true;
		Debounce DebounceDemo(StateA, 0.3, 0.3);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateA);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateB);
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateA);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
	}
	TEST(DebounceTest, SolidSignalTTF)
	{
		bool StateA = true;
		bool StateB = false;
		Debounce DebounceDemo(StateA, 0.3, 0.3);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateA);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateB);
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateA);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
	}
	TEST(DebounceTest, SolidSignalFFT)
	{

		bool StateA = false;
		bool StateB = true;
		Debounce DebounceDemo(StateA, 0.3, 0.3);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateA);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateB);
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateA);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
	}
	TEST(DebounceTest, SignalFastSwitchF)
	{

		bool StateA = true;
		bool StateB = false;
		Debounce DebounceDemo(StateA, 0.0, 0.0);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateA);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateB);
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateA);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateB);
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
	}
	TEST(DebounceTest, SignalFastSwitchT)
	{

		bool StateA = false;
		bool StateB = true;
		Debounce DebounceDemo(StateA, 0.0, 0.0);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateA);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateB);
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateA);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateB);
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
	}
	TEST(DebounceTest, LooseSignalTFT)
	{
		bool StateA = true;
		bool StateB = false;
		Debounce DebounceDemo(StateA, 0.3, 0.3);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(400));
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateB);
		EXPECT_EQ(DebounceDemo.GetState(), StateB);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateB);
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateB);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateA);
		EXPECT_EQ(DebounceDemo.GetState(), StateA);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateA), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		EXPECT_EQ(DebounceDemo.GetState(), StateA);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateA);
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
		EXPECT_EQ(DebounceDemo.Sample(StateB), StateB);
		EXPECT_EQ(DebounceDemo.GetState(), StateB);
	}
