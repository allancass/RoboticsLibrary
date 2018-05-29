#include <chrono>

#ifndef _Logic_H_INCLUDED
#define _Logic_H_INCLUDED


namespace RoboticsLibrary
{
	class Pulse{
	public:
		Pulse(bool InitialState);
		~Pulse(void) {};
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
		~Latch(void) {};
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
		~Schmidt() {};
		bool Sample(double NewSample);
		bool GetState();

	private:
		double High;
		double Low;
		bool State;
	};

	template <typename T>
	T DeadZone(T Upper, T Lower, T Sample);

	template <typename T>
	T DeadZone(T Upper, T Lower, T Sample, bool &InDeadZone);

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
		~Timer(void) {};
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
		~Stable() {};
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
		~Debounce() {};
		bool Sample(bool NewSample);
		bool GetState();
	private:
		Timer HighTimer;
		Timer LowTimer;
		bool State;
		bool LastSample;
	};


	/*Requires True for X time followed by a single false followed by True for Y Time.  If no 2nd true for Z time, times out.  Once confirmed,
	Output will be True until a false is sampled.  Only Sample() updates internal states.  IsFirstTimeConfirmed will be true when first True timer expires.*/
	class BoolConfirmCheck {
	public:
		BoolConfirmCheck(float InitialTrueTime, float SecondaryTrueTime, float TimeOutTime);
		~BoolConfirmCheck();
		bool Sample(bool NewSample);
		bool IsBoolConfirmed();
		bool IsFirstTimeConfirmed();
	private:
		enum State {
			PendingFirstPress,
			PendingRelease,
			PendingSecondPress,
			BoolConfirmed,
		};
		State ConfirmationState;
		Timer InitialTrueTimer;
		Timer TimeOutTimer;
		Timer SecondaryTrueTimer;
		Pulse Edge;
	};

	/*Bump Counter counts the number of instances (not samples) the input value execeeds the given threshold.  A positive exceedence will
	increment the counter by one.  A negative exceedance will decrement it by one.  Reset will reset the count to zero.*/
	/*For Future Consideration, add in minimum time between bumps or minimum time above threshold to mitigate noise.*/
	class BumpCounter {
	public:
		BumpCounter(float Threshold) : Threshold(Threshold), PositiveThreshold(false), NegativeThreshold(false) {};
		~BumpCounter() {};
		int32_t Sample(float NewSample);
		int32_t GetCount(void);
		void Reset(void);
	private:
		int32_t BumpCount = 0;
		float Threshold;
		Pulse PositiveThreshold;
		Pulse NegativeThreshold;
	};

	/*Converts desired High Time & Low Time to a Duty Cycle & Period
	Duty Cycle is in Percent (0-100).  Returns -1 if High Time or Low Time is <0*/
	int HLTDP(float HighTime, float LowTime, float &DutyCycle, float &Period);

	/*Bump Counter Increments/Decrements a counter on rising edge of the Up/Down inputs.  Reset will reset the count to the Starting Value.*/
	/*For Future Consideration, add in minimum time between bumps or minimum time above threshold to mitigate noise.*/
	class UpDownCounter {
	public:
		UpDownCounter(int StartingValue) : InitialValue(StartingValue), BumpCount(StartingValue), Increment(false), Decrement(false) {};
		~UpDownCounter() {};
		int32_t Sample(bool UpOne, bool DownOne);
		int32_t GetCount(void);
		void Reset(void);
	private:
		int32_t BumpCount = 0;
		int32_t InitialValue;
		Pulse Increment;
		Pulse Decrement;
	};


	/*If Max is true, returns the max value, otherwise returns the min value.*/
	template <typename T>
	T GetMinMax(T ValueOne, T ValueTwo, bool Max);

	/*On a Rising Edge, waits X Seconds then outputs a High for Y Seconds.*/
	class PulseStretchDelay
	{
	public:
		PulseStretchDelay(float DelayTime, float StretchTime) : DelayTimer(DelayTime), StretchTimer(StretchTime), PulseState(WaitingForPulse), Edge(false) {};
		~PulseStretchDelay() {};
		bool Sample(bool NewSample);
		bool GetState();
	private:
		enum State {
			WaitingForPulse,
			Delaying,
			Stretching
		} PulseState;
		Timer DelayTimer;
		Timer StretchTimer;
		Pulse Edge;
	};

	template <typename T>
	bool InTollerance(T Sample, T UpperLimit, T LowerLimit);

	template <typename T>
	bool InTollerance(T Sample, T UpperLimit, T LowerLimit, T &UpperDistance, T &LowerDistance);

	class SquareWaveGenerator {
	public:
		SquareWaveGenerator() : WaveState(Disabled), HighTime(1.0), PeriodTime(2.0) {};
		~SquareWaveGenerator() {};
		bool GetWave(void);
		void Enable(void);
		void Disable(void);
		void SetWave(float Period, float DutyCycle);
	private:
		enum State {
			Enabled,
			Disabled
		}WaveState;
		float HighTime;
		float PeriodTime;
		float StartOfPeriod = (std::chrono::duration<float>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()))).count();
		float Now;
	};
}

#endif // !