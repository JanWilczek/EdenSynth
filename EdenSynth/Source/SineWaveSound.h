#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

struct SineWaveSound : public SynthesiserSound
{
	SineWaveSound(){}

	bool appliesToNote(int) override
	{
		return true;
	}

	bool appliesToChannel(int) override
	{
		return true;
	}
};
