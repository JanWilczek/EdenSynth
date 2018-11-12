#pragma once
/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 
#include <memory>
#include "eden/SampleType.h"

namespace eden::synth::wavetable
{
	class IOscillatorSource
	{
	public:
		virtual ~IOscillatorSource() = 0;
		virtual std::unique_ptr<IOscillatorSource> clone() = 0;

		virtual void setPitch(float pitch) = 0;
		virtual void setSampleRate(float sampleRate) = 0;
		virtual SampleType getSample() = 0;
	};

	inline IOscillatorSource::~IOscillatorSource()
	{
	}
}
