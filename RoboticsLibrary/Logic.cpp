#include "Logic.h"

namespace RoboticsLibrary
{
	Pulse::Pulse(bool InitialState)
	{
		//Sets initial states
		PastSamples[0] = InitialState;
		PastSamples[1] = InitialState;
	}

	Pulse::~Pulse()
	{
		//No deconstruction required
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

	Latch::~Latch(void)
	{
		//No deconstruction required
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

	Schmidt::~Schmidt()
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

	Timer::Timer(float Timeout)
	{
		Duration = Timeout;
		Status = Idle;
		StartTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch());
		TimeNow = StartTime;
	}

	Timer::~Timer(void)
	{

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

	Stable::~Stable()
	{
		//No Deconstruction Required
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

	Debounce::~Debounce()
	{
		//No Deconstruction Required
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


}
