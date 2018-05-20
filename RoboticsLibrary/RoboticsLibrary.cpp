// RoboticsLibrary.cpp : Defines the entry point for the console application.
//

#include "Logic.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

using std::cout;
using std::endl;
//using namespace std::chrono;

void TestPulse(bool StartingSample);
void BatchTestLatch(void);
void TestLatch(bool InitialState, bool ToggleOn, bool InitialSample);
void TestSchmidt(void);


int main()
{
	//TestPulse(true);
	//BatchTestLatch();
	//TestSchmidt();
	
	//cout << std::setprecision(9) << fms.count() << " " << std::setprecision(9) << ums.count() << endl;
		
	cout.setf(std::ios::fixed);

	std::cin.ignore();
    return 0;
}

void TestPulse(bool StartingSample)
{
	RoboticsLibrary::Pulse B37(StartingSample);
	bool SampleSamples[16] = { StartingSample, false, false, false, false, true, true, false, false, true, true,  false, true,  false, true, true };

	cout << "Testing Pulse" << endl;
	cout << "Run " << "C " << "P " << "R " << "F" << endl;

	for (int iteration = 1; iteration < 16; iteration++)
	{
		B37.Sample(SampleSamples[iteration]);

		cout << std::setw(3) << iteration << " " << SampleSamples[iteration] << " " << SampleSamples[iteration - 1] << " " << B37.IsRising() << " " << B37.IsFalling() << endl;
	}


	std::cin.ignore();
}

void BatchTestLatch(void)
{
	for (int i = 0; i < 8; i++)
	{
		TestLatch(i & 4, i & 2, i & 1);
	}
}

void TestLatch(bool InitialState, bool ToggleOn, bool InitialSample)
{
	RoboticsLibrary::Latch S49(InitialState, ToggleOn, InitialSample);

	bool SampleSamples[16] = { InitialSample, InitialSample, !InitialSample, !InitialSample, InitialSample, InitialSample, !InitialSample, !InitialSample, InitialSample, InitialSample, !InitialSample, !InitialSample, InitialSample, InitialSample, !InitialSample, !InitialSample };

	cout << "Initial State: " << InitialState << endl;
	cout << "Toggle Edge: " << ToggleOn << endl;
	cout << "Initial Sample: " << InitialSample << endl;
	cout << "Run " << "C " << "S " << endl;
	for (int iteration = 0; iteration < 16; iteration++)
	{
		S49.Sample(SampleSamples[iteration]);

		cout << std::setw(3) << iteration << " " << SampleSamples[iteration] << " " << S49.GetState() << endl;
	}
	cout << endl;
	
	std::cin.ignore();
}

void TestSchmidt(void)
{
	RoboticsLibrary::Schmidt A42(0.8, -0.75, false);
	double TestPattern[6] = { 0.0, 0.5, 1.0, -0.7, 1.0, 0.5 };
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			cout << std::setw(5) << TestPattern[j] << " " << A42.Sample(TestPattern[j]) << endl;
		}
		for (int j = 0; j < 6; j++)
		{
			cout << std::setw(5) << -TestPattern[j] << " " << A42.Sample(-TestPattern[j]) << endl;
		}
	}
	std::cin.ignore();
}

