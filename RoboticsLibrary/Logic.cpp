#include "Logic.h"

namespace RoboticsLibrary
{
	Pulse::Pulse(bool InitialState)
	{
		//Sets initial states
		PastSamples[0] = InitialState;
		PastSamples[1] = InitialState;
	}

	void Pulse::Sample(bool NewSample)
	{
		PastSamples[1] = PastSamples[0];
		PastSamples[0] = NewSample;
	}

	bool Pulse::IsRising()
	{
		return (PastSamples[0] && (!PastSamples[1]));
	}

	bool Pulse::IsFalling()
	{	
		return ((!PastSamples[0]) && PastSamples[1]);
	}

	Latch::Latch(bool InitialState, bool ToggleOn, bool InitialSample) : State (InitialState), ToggleEdge(ToggleOn), OneShot(InitialSample)
	{

	}

	void Latch::Sample(bool NewSample)
	{
		OneShot.Sample(NewSample);
		if (ToggleEdge)
		{
			if (OneShot.IsRising())
			{
				State = !State;
			}
		}
		else
		{
			if (OneShot.IsFalling())
			{
				State = !State;
			}
		}
	}

	bool Latch::GetState()
	{
		return State;
	}

	Schmidt::Schmidt(double HighLimit, double LowLimit, bool InitialState) : High(HighLimit), Low(LowLimit), State(InitialState)
	{

	}

	bool Schmidt::Sample(double NewSample)
	{
		if (State)
		{
			if (NewSample < Low) { State = false; }
		}
		else
		{
			if (NewSample > High) { State = true; }
		}
		return State;
	}

	bool Schmidt::GetState()
	{
		return State;
	}

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

	Timer::Timer(float Timeout) : Duration(Timeout)
	{
		//Duration = Timeout;
		Status = Idle;
		StartTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
		TimeNow = StartTime;
	}

	void Timer::Start(void)
	{
		Status = Active;
		StartTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
	}

	void Timer::Stop(void)
	{
		Status = Idle;
	}

	void Timer::Reset(void)
	{
		Status = Active;
		StartTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
	}

	bool Timer::IsExpired(void)
	{
		return((Duration <= TimeElapsed()) && (Status = Active));
	}

	bool Timer::IsActive(void)
	{
		return (Status == Active);
	}
	
	float Timer::TimeElapsed(void)
	{
		if (Status == Idle) { return -1.0; }
		TimeNow = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
		return TimeNow.count()-StartTime.count();
	}

	bool Stable::Sample(bool NewSample)
	{
		Edge.Sample(NewSample);
		if (State)
		{
			//If State is High and new sample is low, start the Low Timer, stop the High timer
			if (Edge.IsFalling())
			{
				LowTimer.Reset();
				HighTimer.Stop();
			}
			//If Low Timer is expired and the new sample is low, set new state to low
			if ((!NewSample)&&LowTimer.IsExpired())
			{
				State = false;
			}
		}
		else
		{
			//If State is Low and new sample is High, start the High Timer, stop the low timer
			if (Edge.IsRising())
			{
				HighTimer.Reset();
				LowTimer.Stop();
			}
			//If High Timer is expired and the new sample is High, set new state to High
			if (NewSample && HighTimer.IsExpired())
			{
				State = true;
			}
		}
		return(State);
	}

	bool Stable::GetState()
	{
		if (State)
		{
			//If Low Timer is expired set new state to low
			if (LowTimer.IsExpired())
			{
				State = false;
			}
		}
		else
		{
			//If High Timer is expired set new state to High
			if (HighTimer.IsExpired())
			{
				State = true;
			}
		}
		return(State);
	}

	bool Debounce::Sample(bool NewSample)
	{
		if (State)
		{
			if (HighTimer.IsExpired() && (!NewSample))
			{
				State = false;
				LowTimer.Reset();
			}
		}
		else
		{
			if (LowTimer.IsExpired() && NewSample)
			{
				State = true;
				HighTimer.Reset();
			}
		}
		LastSample = NewSample;
		return(State);
	}

	bool Debounce::GetState()
	{
		if (State)
		{
			if (HighTimer.IsExpired() && (!LastSample))
			{
				State = false;
				LowTimer.Reset();
			}
		}
		else
		{
			if (LowTimer.IsExpired() && (LastSample))
			{
				State = true;
				HighTimer.Reset();
			}
		}
		return(State);
	}

	BoolConfirmCheck::BoolConfirmCheck(float InitialTrueTime, float SecondaryTrueTime, float TimeOutTime) : 
		InitialTrueTimer(InitialTrueTime), 
		SecondaryTrueTimer(SecondaryTrueTime), 
		TimeOutTimer(TimeOutTime), 
		Edge(false), 
		ConfirmationState(PendingFirstPress)
	{};
	
	bool BoolConfirmCheck::Sample(bool NewSample)
	{
		Edge.Sample(NewSample);
		switch (ConfirmationState)
		{
		case RoboticsLibrary::BoolConfirmCheck::PendingFirstPress:
			if (Edge.IsRising()) { InitialTrueTimer.Reset(); }
			if (InitialTrueTimer.IsExpired() && NewSample) { 
				ConfirmationState = PendingRelease; 
				InitialTrueTimer.Stop();
			}
			break;
		case RoboticsLibrary::BoolConfirmCheck::PendingRelease:
			if (!NewSample)
			{
				ConfirmationState = PendingSecondPress;
				TimeOutTimer.Reset();
			}
			break;
		case RoboticsLibrary::BoolConfirmCheck::PendingSecondPress:
			if (!NewSample) { SecondaryTrueTimer.Stop(); }
			if (Edge.IsRising()) { SecondaryTrueTimer.Reset(); }
			if (SecondaryTrueTimer.IsExpired()) { 
				ConfirmationState = BoolConfirmed; 
				SecondaryTrueTimer.Stop();
			}
			if (TimeOutTimer.IsExpired()) { 
				ConfirmationState = PendingFirstPress; 
				SecondaryTrueTimer.Stop();
			}
			break;
		case RoboticsLibrary::BoolConfirmCheck::BoolConfirmed:
			if (!NewSample) { 
				ConfirmationState = PendingFirstPress; 
			}
			break;
		default:
			ConfirmationState = PendingFirstPress;
			TimeOutTimer.Stop();
			InitialTrueTimer.Stop();
			SecondaryTrueTimer.Stop();
			Edge.Sample(false);
			Edge.Sample(false);
			break;
		}
		return (ConfirmationState == BoolConfirmed);
	}

	BoolConfirmCheck::~BoolConfirmCheck() {};

	bool BoolConfirmCheck::IsBoolConfirmed()
	{
		return (ConfirmationState == BoolConfirmed);
	}

	bool BoolConfirmCheck::IsFirstTimeConfirmed()
	{
		return (!(ConfirmationState == PendingFirstPress));
	}

	int32_t BumpCounter::Sample(float NewSample)
	{
		PositiveThreshold.Sample(NewSample > Threshold);
		NegativeThreshold.Sample(NewSample < (-Threshold));
		if (PositiveThreshold.IsRising()) { BumpCount++; }
		if (NegativeThreshold.IsRising()) { BumpCount--; }
		return BumpCount;
	}

	int32_t BumpCounter::GetCount(void) { return BumpCount; }
	void BumpCounter::Reset(void) { BumpCount = 0; }

	int HLTDP(float HighTime, float LowTime, float &DutyCycle, float &Period)
	{
		int ErrorCode = 0;
		if (HighTime < 0.0) {
			HighTime = 0.0;
			ErrorCode = -1;
		}
		if (LowTime < 0.0) {
			LowTime = 0.0;
			ErrorCode = -1;
		}

		Period = HighTime + LowTime;
		
		if (Period <= 0) { 
			DutyCycle = 0.0; 
			ErrorCode = -1;
		}
		else { DutyCycle = HighTime / Period * 100.0; } //Percent Value
		
		return ErrorCode;
	}

	int32_t UpDownCounter::Sample(bool UpOne, bool DownOne)
	{
		Increment.Sample(UpOne);
		Decrement.Sample(DownOne);
		if (Increment.IsRising()) { BumpCount++; }
		if (Decrement.IsRising()) { BumpCount--; }
		return BumpCount;
	}

	void UpDownCounter::Reset(void) { BumpCount = InitialValue; }
	int32_t UpDownCounter::GetCount() { return BumpCount; }

	template <typename T>
	T GetMinMax(T ValueOne, T ValueTwo, bool Max)
	{
		if (Max)
		{
			if (ValueOne > ValueTwo) { return ValueOne; }
			else { return ValueTwo; }
		}
		else
		{
			if (ValueOne > ValueTwo) { return ValueTwo; }
			else { return ValueOne; }
		}
	}

	bool PulseStretchDelay::Sample(bool NewSample) {
		Edge.Sample(NewSample);
		switch (PulseState)
		{
		case RoboticsLibrary::PulseStretchDelay::WaitingForPulse:
			if (Edge.IsRising()) { 
				PulseState = Delaying; 
				DelayTimer.Reset();
				if (DelayTimer.IsExpired()) { PulseState = Stretching; }
			}
			break;
		case RoboticsLibrary::PulseStretchDelay::Delaying:
			if (DelayTimer.IsExpired()) { 
				PulseState = Stretching; 
				StretchTimer.Reset();
			}
			break;
		case RoboticsLibrary::PulseStretchDelay::Stretching:
			if (StretchTimer.IsExpired()) { PulseState = WaitingForPulse; }
			break;
		default:
			PulseState = WaitingForPulse;
			break;
		}
		return(PulseState == Stretching);
	}

	bool PulseStretchDelay::GetState()
	{
		return(PulseState == Stretching);
	}

	template <typename T>
	bool InTollerance(T Sample, T UpperLimit, T LowerLimit)
	{
		return ((Sample < UpperLimit) && (Sample > LowerLimit));
	}

	template <typename T>
	bool InTollerance(T Sample, T UpperLimit, T LowerLimit, T &UpperDistance, T &LowerDistance)
	{
		UpperDistance = abs(UpperLimit - Sample);
		LowerDistance = abs(LowerLimit - Sample);
		return ((Sample < UpperLimit) && (Sample > LowerLimit));
	}

	void SquareWaveGenerator::Disable() { WaveState = Disabled; }
	
	void SquareWaveGenerator::Enable() { WaveState = Enabled; }
	
	void SquareWaveGenerator::SetWave(float Period, float DutyCycle)
	{
		PeriodTime = Period;
		HighTime = Period * DutyCycle/100.0;
	}

	bool SquareWaveGenerator::GetWave()
	{
		Now = (std::chrono::duration<float>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()))).count();
		while ((StartOfPeriod + PeriodTime) < Now) { StartOfPeriod += PeriodTime; }
		if ((WaveState == Enabled) && (Now < (StartOfPeriod + HighTime))) { return true; }
		return false;
	}
}
