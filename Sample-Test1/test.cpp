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
		EXPECT_TRUE(TFVar);
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

	TEST(BoolConfirmCheck, CheckPendingFirstPress)
	{
		BoolConfirmCheck BCC(0.1, 0.1, 0.3);
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), false);
		BCC.Sample(false);
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), false);
		BCC.Sample(true);
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), false);
		std::this_thread::sleep_for(std::chrono::milliseconds(80));
		BCC.Sample(true);
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), false);
		BCC.Sample(false);
		BCC.Sample(true);
		std::this_thread::sleep_for(std::chrono::milliseconds(80));
		BCC.Sample(true);
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), false);
		BCC.Sample(true);
		std::this_thread::sleep_for(std::chrono::milliseconds(80));
		BCC.Sample(true);
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), true);
	}	
	TEST(BoolConfirmCheck, CheckPendingSecondPress)
	{
		BoolConfirmCheck BCC(0.1, 0.1, 0.3);
		//Waiting for first press
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), false);
		BCC.Sample(false);
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), false);
		BCC.Sample(true);
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), false);

		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		BCC.Sample(true);
		//Now in waitin for release
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), true);
		BCC.Sample(false);
		//Now in waiting for second press
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), true);
		BCC.Sample(true);
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		BCC.Sample(true);
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), true);
		BCC.Sample(false);
		std::this_thread::sleep_for(std::chrono::milliseconds(350));
		//Time out, back to waiting for first press
		BCC.Sample(false);
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), false);
		BCC.Sample(true);
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		BCC.Sample(true);
		//Waitin for Release
		BCC.Sample(false);
		//Now in waiting for second press
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), true);
		BCC.Sample(true);
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), true);
		BCC.Sample(false);
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), true);
		BCC.Sample(true);
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), true);
		BCC.Sample(false);
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), true);
		std::this_thread::sleep_for(std::chrono::milliseconds(80));
		BCC.Sample(true);
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), true);
		BCC.Sample(false);
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), true);
		BCC.Sample(true);
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), true);
		std::this_thread::sleep_for(std::chrono::milliseconds(80));
		BCC.Sample(true);
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), true);
		std::this_thread::sleep_for(std::chrono::milliseconds(80));
		BCC.Sample(true);
		//Now Confirmed
		EXPECT_EQ(BCC.IsBoolConfirmed(), true);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), true);
		std::this_thread::sleep_for(std::chrono::milliseconds(350));
		BCC.Sample(true);
		//Still Confirmed
		EXPECT_EQ(BCC.IsBoolConfirmed(), true);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), true);
		BCC.Sample(false);
		//Back to waiting for first press
		EXPECT_EQ(BCC.IsBoolConfirmed(), false);
		EXPECT_EQ(BCC.IsFirstTimeConfirmed(), false);
	}

	TEST(BumpCount, CountTest)
	{
		BumpCounter BPC(0.5);
		EXPECT_EQ(BPC.GetCount(), 0);
		EXPECT_EQ(BPC.Sample(0.0), 0);
		EXPECT_EQ(BPC.Sample(0.49), 0);
		EXPECT_EQ(BPC.Sample(-0.49), 0);
		EXPECT_EQ(BPC.Sample(-0.51), -1);
		EXPECT_EQ(BPC.Sample(-0.49), -1);
		EXPECT_EQ(BPC.Sample(-0.51), -2);
		EXPECT_EQ(BPC.Sample(0.51), -1);
		EXPECT_EQ(BPC.Sample(0.49), -1);
		EXPECT_EQ(BPC.Sample(1.49), 0);
		EXPECT_EQ(BPC.Sample(-0.51), -1);
		EXPECT_EQ(BPC.Sample(1.49), 0);
		EXPECT_EQ(BPC.Sample(-0.49), 0);
		EXPECT_EQ(BPC.Sample(1.49), 1);
		EXPECT_EQ(BPC.Sample(-0.49), 1);
		BPC.Reset();
		EXPECT_EQ(BPC.GetCount(), 0);
	}

	float DutyCycle;
	float Period;
	TEST(HLTDP, HighPositiveLowPositive)
	{
		EXPECT_EQ(HLTDP(1.0, 3.0, DutyCycle, Period), 0);
		EXPECT_FLOAT_EQ(DutyCycle, 25.0);
		EXPECT_FLOAT_EQ(Period, 4.0);

		EXPECT_EQ(HLTDP(3.0, 1.0, DutyCycle, Period), 0);
		EXPECT_FLOAT_EQ(DutyCycle, 75.0);
		EXPECT_FLOAT_EQ(Period, 4.0);
	}
	TEST(HLTDP, HighZeroLowPositive)
	{
		EXPECT_EQ(HLTDP(0.0, 3.5, DutyCycle, Period), 0);
		EXPECT_FLOAT_EQ(DutyCycle, 0.0);
		EXPECT_FLOAT_EQ(Period, 3.5);
	}
	TEST(HLTDP, HighNegativeLowPositive)
	{
		EXPECT_NE(HLTDP(-1.0, 3.0, DutyCycle, Period), 0);
		EXPECT_FLOAT_EQ(DutyCycle, 0.0);
		EXPECT_FLOAT_EQ(Period, 3.0);
	}
	TEST(HLTDP, HighPositiveLowZero)
	{
		EXPECT_EQ(HLTDP(3.5, 0.0, DutyCycle, Period), 0);
		EXPECT_FLOAT_EQ(DutyCycle, 100.0);
		EXPECT_FLOAT_EQ(Period, 3.5);
	}
	TEST(HLTDP, HighZeroLowZero)
	{
		EXPECT_NE(HLTDP(0.0, 0.0, DutyCycle, Period), 0);
		EXPECT_FLOAT_EQ(DutyCycle, 0.0);
		EXPECT_FLOAT_EQ(Period, 0.0);
	}
	TEST(HLTDP, HighNegativeLowZero)
	{	
		EXPECT_NE(HLTDP(-3.0, 0.0, DutyCycle, Period), 0);
		EXPECT_FLOAT_EQ(DutyCycle, 0.0);
		EXPECT_FLOAT_EQ(Period, 0.0);
	}
	TEST(HLTDP, HighPositiveLowNegative)
	{	
		EXPECT_NE(HLTDP(3.5, -1.0, DutyCycle, Period), 0);
		EXPECT_FLOAT_EQ(DutyCycle, 100.0);
		EXPECT_FLOAT_EQ(Period, 3.5);
	}
	TEST(HLTDP, HighZeroLowNegative)
	{	
		EXPECT_NE(HLTDP(0.0, -1.0, DutyCycle, Period), 0);
		EXPECT_FLOAT_EQ(DutyCycle, 0.0);
		EXPECT_FLOAT_EQ(Period, 0.0);
	}
	TEST(HLTDP, HighNegativeLowNegative)
	{	
		EXPECT_NE(HLTDP(-3.0, -1.0, DutyCycle, Period), 0);
		EXPECT_FLOAT_EQ(DutyCycle, 0.0);
		EXPECT_FLOAT_EQ(Period, 0.0);
	}

	TEST(UpDownCounter)
	{
		UpDownCounter UDC(5);
		EXPECT_EQ(UDC.GetCount(), 5);
		EXPECT_EQ(UDC.Sample(false, false), 5);
		EXPECT_EQ(UDC.Sample(true, false), 6);
		EXPECT_EQ(UDC.Sample(true, true), 5);
		EXPECT_EQ(UDC.Sample(false, true), 5);
		EXPECT_EQ(UDC.Sample(false, false), 5);
		EXPECT_EQ(UDC.Sample(false, true), 4);
		EXPECT_EQ(UDC.Sample(true, true), 5);
		EXPECT_EQ(UDC.Sample(false, true), 5);
		EXPECT_EQ(UDC.Sample(true, true), 6);
		EXPECT_EQ(UDC.Sample(true, false), 6);
		EXPECT_EQ(UDC.Sample(true, true), 5);
		EXPECT_EQ(UDC.Sample(true, false), 5);
		EXPECT_EQ(UDC.Sample(true, true), 4);
		UDC.Reset();
		EXPECT_EQ(UDC.GetCount(), 5);
		EXPECT_EQ(UDC.Sample(false, false), 5);
		EXPECT_EQ(UDC.Sample(true, true), 5);
		EXPECT_EQ(UDC.Sample(false, false), 5);
		EXPECT_EQ(UDC.Sample(true, true), 5);
	}

	TEST(MinMax)
	{
		EXPECT_EQ(GetMinMax<int>(5.0, 4.0, true), 5.0);
		EXPECT_EQ(GetMinMax<int>(5.0, 4.0, false), 4.0);
		EXPECT_EQ(GetMinMax<int>(4.0, 5.0, true), 5.0);
		EXPECT_EQ(GetMinMax<int>(4.0, 5.0, false), 4.0);
		EXPECT_EQ(GetMinMax<int>(4.0, 4.0, false), 4.0);
		EXPECT_EQ(GetMinMax<int>(4.0, 4.0, true), 4.0);
	}

	TEST(PulseStretchDelay)
	{
		PulseStretchDelay PSD(0.1, 0.2);
		EXPECT_EQ(PSD.GetState(), false);
		EXPECT_EQ(PSD.Sample(false), false);
		std::this_thread::sleep_for(std::chrono::milliseconds(350));
		EXPECT_EQ(PSD.GetState(), false);
		EXPECT_EQ(PSD.Sample(false), false);
		EXPECT_EQ(PSD.Sample(true), false);
		EXPECT_EQ(PSD.GetState(), false);
		std::this_thread::sleep_for(std::chrono::milliseconds(50)); //50
		EXPECT_EQ(PSD.Sample(false), false);
		EXPECT_EQ(PSD.GetState(), false);
		std::this_thread::sleep_for(std::chrono::milliseconds(60)); //110
		EXPECT_EQ(PSD.Sample(true), true);
		EXPECT_EQ(PSD.GetState(), true);
		std::this_thread::sleep_for(std::chrono::milliseconds(50)); //160
		EXPECT_EQ(PSD.Sample(false), true);
		EXPECT_EQ(PSD.GetState(), true);
		std::this_thread::sleep_for(std::chrono::milliseconds(50)); //210
		EXPECT_EQ(PSD.Sample(true), true);
		EXPECT_EQ(PSD.GetState(), true);
		std::this_thread::sleep_for(std::chrono::milliseconds(50)); //260
		EXPECT_EQ(PSD.Sample(false), true);
		EXPECT_EQ(PSD.GetState(), true);
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); //360
		EXPECT_EQ(PSD.Sample(false), false);
		EXPECT_EQ(PSD.Sample(true), false);
		EXPECT_EQ(PSD.GetState(), false);
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		EXPECT_EQ(PSD.Sample(true), true);
		EXPECT_EQ(PSD.GetState(), true);
	}

	TEST(InTollerance)
	{
		EXPECT_EQ(InTollerance<float>(-5.0, 1.0, -1.0), false);
		EXPECT_EQ(InTollerance<float>(5.0, 1.0, -1.0), false);
		EXPECT_EQ(InTollerance<float>(0.0, 1.0, -1.0), true);
	}

	TEST(InTolleranceTF)
	{
		float UpperDist;
		float LowerDist;
		EXPECT_EQ(InTollerance<float>(-5.0, 1.0, -1.0, UpperDist, LowerDist), false);
		EXPECT_FLOAT_EQ(UpperDist, 6.0);
		EXPECT_FLOAT_EQ(LowerDist, 4.0);
		EXPECT_EQ(InTollerance<float>(5.0, 1.0, -1.0, UpperDist, LowerDist), false);
		EXPECT_FLOAT_EQ(UpperDist, 4.0);
		EXPECT_FLOAT_EQ(LowerDist, 6.0);
		EXPECT_EQ(InTollerance<float>(0.0, 1.0, -1.0, UpperDist, LowerDist), true);
		EXPECT_FLOAT_EQ(UpperDist, 1.0);
		EXPECT_FLOAT_EQ(LowerDist, 1.0);
	}