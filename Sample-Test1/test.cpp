#include "gtest/gtest.h"
#include "../RoboticsLibrary/Logic.h"

using namespace RoboticsLibrary;

	TEST(DeadZone, Upper)
	{
		EXPECT_EQ(0.8, DeadZone<double>(0.8, -0.7, 1.0));		
	}
	TEST(DeadZone, Lower)
	{
		EXPECT_EQ(-0.7, DeadZone<double>(0.8, -0.7, -1.0));
	}
	TEST(DeadZone, Mid)
	{
		EXPECT_EQ(0.0, DeadZone<double>(0.8, -0.7, 0.0));
	}
	TEST(DeadZoneTF, Upper)
	{
		bool TFVar;
		EXPECT_EQ(0.8, DeadZone<double>(0.8, -0.7, 1.0, TFVar));
		EXPECT_FALSE(TFVar);
	}
	TEST(DeadZoneTF, Lower)
	{
		bool TFVar;
		EXPECT_EQ(-0.7, DeadZone<double>(0.8, -0.7, -1.0, TFVar));
		EXPECT_FALSE(TFVar);
	}
	TEST(DeadZoneTF, Mid)
	{
		bool TFVar;
		EXPECT_EQ(0.0, DeadZone<double>(0.8, -0.7, 0.0, TFVar));
		EXPECT_TRUE(TFVar);
	}
	TEST(SampleHold, EmptyConstructor) {
		SampleAndHold<double> SNH(0.0);
		EXPECT_EQ(SNH.Get(), 0.0);
	}