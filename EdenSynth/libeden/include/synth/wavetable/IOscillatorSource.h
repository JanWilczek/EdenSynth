#pragma once
/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 
#include "eden/SampleType.h"

namespace eden::synth::wavetable
{
	class IOscillatorSource
	{
	public:
		virtual ~IOscillatorSource() = 0;

		virtual SampleType operator()(float phase) = 0;
	};

	inline IOscillatorSource::~IOscillatorSource()
	{
	}
}
