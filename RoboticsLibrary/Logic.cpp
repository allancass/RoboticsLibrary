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
	

}
