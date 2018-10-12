/// 
/// \author Jan Wilczek
/// \date 08.10.2018
/// 
#include "synth/wavetable/SignalGenerator.h"
#include "utility/MathConstants.h"
#include <cmath>

namespace eden::synth::wavetable
{
	SignalGenerator::SignalGenerator(double sampleRate)
		: _sampleRate(sampleRate)
	{
	}

	void SignalGenerator::generateSignal(SampleType* audioChannel, int startSample, int samplesToGenerate)
	{
		for (int sample = startSample; sample < startSample + samplesToGenerate; ++sample)
		{
			generateSample(audioChannel, sample);
		}
	}

	void SignalGenerator::stop()
	{
		_currentPhase = 0.0;
	}

	void SignalGenerator::setPitch(double pitch)
	{
		const auto omega = 2.0 * math_constants::PI * pitch;
		_phaseDeltaPerSample = omega / _sampleRate;
	}

	void SignalGenerator::setSampleRate(double sampleRate)
	{
		_phaseDeltaPerSample = _phaseDeltaPerSample * _sampleRate / sampleRate;
		_sampleRate = sampleRate;
	}

	void SignalGenerator::generateSample(SampleType* audioChannel, int sampleIndex)
	{
		audioChannel[sampleIndex] += 0.15 * std::sin(_currentPhase);
		_currentPhase += _phaseDeltaPerSample;
	}
}
