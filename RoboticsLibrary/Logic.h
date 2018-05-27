#include <chrono>

#ifndef _Logic_H_INCLUDED
#define _Logic_H_INCLUDED


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
		if (Sample > Upper) { return Sample; }
		else if (Sample < Lower) { return Sample; }
		else {
			return 0;
		}
	}

	template <typename T>
	T DeadZone(T Upper, T Lower, T Sample, bool &InDeadZone)
	{
		InDeadZone = false;
		if (Sample > Upper) { return Sample; }
		else if (Sample < Lower) { return Sample; }
		else {
			InDeadZone = true;
			return 0;
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

	template <typename T>
	T GetTime(void)
	{
		std::chrono::duration<T> fus = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
		return fus.count();
		//return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}
	
	class Timer {
	public:
		Timer(float Timeout);
		~Timer(void);
		void Start(void);
		void Stop(void);
		void Reset(void);
		bool IsExpired(void);
		bool IsActive(void);
		float TimeElapsed(void);
	private:
		double Duration;
		enum State {
			Idle,
			Active
		} Status;
		std::chrono::duration<float> StartTime;
		std::chrono::duration<float> TimeNow;
	};

	/*The intent of Stable is to ensure you have a solid/stable signal prior to changing states.  The output will not change until it has been T/F consistently  while
	the corresponding time has elapsed.  Any intermittent change to the other state will reset the timer.
	*/
	class Stable {
	public:
		Stable(bool StartingValue, float SecondsHigh, float SecondsLow) : State(StartingValue), Edge(StartingValue), HighTimer(SecondsHigh), LowTimer(SecondsLow) {};
		~Stable();
		bool Sample(bool NewSample);
		bool GetState();
	private:
		Timer HighTimer;
		Timer LowTimer;
		bool State;
		Pulse Edge;
	};

	/*The intent of Debounce is to ensure that when a signal switches, any temporary chatter will be eliminated.  This is similar to Stable but
	instead of waiting until the signal is solid before switching, it will switch immediately.  It will not switch back until the corresponding timer
	has elapsed AND a sample (after the timer expires) is the opposite.  Timers only reset when the output changes.*/
	class Debounce {
	public:
		Debounce(bool StartingValue, float SecondsHigh, float SecondsLow) : State(StartingValue), LastSample(StartingValue), HighTimer(SecondsHigh), LowTimer(SecondsLow)
		{
			if (StartingValue)
			{
				HighTimer.Reset();
			}
			else
			{
				LowTimer.Reset();
			}
		};
		~Debounce();
		bool Sample(bool NewSample);
		bool GetState();
	private:
		Timer HighTimer;
		Timer LowTimer;
		bool State;
		bool LastSample;
	};
}

#endif // !