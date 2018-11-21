#pragma once
///
///	\author Jan Wilczek 
///	 \date 08.10.2018
///
#include "synth/IMonoModule.h"
#include "eden/SampleType.h"

namespace eden::synth::subtractive
{
	class SubtractiveModule : public IMonoModule
	{
	public:
		~SubtractiveModule() override;

		void process(SampleType* audioChannel, int startSample, int samplesToProcess) override;
		
		void setSampleRate(float sampleRate);
		void setPitch(double pitch);
		void setPitchBend(int pitchBendInSemitones);
	};
}
