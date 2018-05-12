

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

	template <typename T>
	T DeadZone(T Upper, T Lower, T Sample)
	{
		if (Sample > Upper) { return Upper; }
		else if (Sample < Lower) { return Lower; }
		else {
			return Sample;
		}
	}

	template <typename T>
	T DeadZone(T Upper, T Lower, T Sample, bool &InRange)
	{
		InRange = false;
		if (Sample > Upper) { return Upper; }
		else if (Sample < Lower) { return Lower; }
		else {
			InRange = true;
			return Sample;
		}
	}

	template <typename T>
	class SampleAndHold
	{
	public:
		SampleAndHold(T DefaultValue) : Sample(DefaultValue) {};
		~SampleAndHold() {};
		
		T SampleAndHold::Set(T Value, bool Hold)
		{
			if (Hold) { Sample = Value; }
			return Sample;
		}

		T SampleAndHold::Get()
		{
			return Sample;
		}
	private:
		T Sample;
	};
}

