

#pragma once
namespace RoboticsLibrary
{
	class Pulse{
	public:
		Pulse(bool InitialState);
		~Pulse(void);
		void Sample(bool NewSample);
		bool IsRising();
		bool IsFalling();
	private:
		//Samples are stored with most recent sample at index 0
		bool PastSamples[2];
	};

	class Latch {
	public:
		Latch(bool InitialState, bool ToggleOn, bool InitialSample);
		~Latch(void);
		void Sample(bool NewSample);
		bool GetState();
	private:
		Pulse OneShot;
		bool State;
		bool ToggleEdge;
	};

	class Schmidt {
	public:
		Schmidt(double HighLimit, double LowLimit, bool InitialState);
		~Schmidt();
		bool Sample(double NewSample);
		bool GetState();

	private:
		double High;
		double Low;
		bool State;
	};
}