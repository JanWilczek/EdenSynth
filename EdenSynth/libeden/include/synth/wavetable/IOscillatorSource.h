#pragma once
/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 
#include <memory>

namespace eden::synth::wavetable
{
	class IOscillatorSource
	{
	public:
		virtual ~IOscillatorSource() = 0;
		virtual std::unique_ptr<IOscillatorSource> clone() = 0;

		virtual void reset() = 0;
		virtual void setPitch(float pitch) = 0;
		virtual void setSampleRate(float sampleRate) = 0;
		virtual float getSample() = 0;
	};

	inline IOscillatorSource::~IOscillatorSource()
	{
	}
}
