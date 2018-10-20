/// 
/// \author Jan Wilczek
/// \date 08.10.2018
/// 
#include <cmath>
#include "synth/wavetable/SignalGenerator.h"
#include "synth/wavetable/SineWaveTable.h"
#include "utility/MathConstants.h"
#include "interpolation/LinearInterpolator.h"

namespace eden::synth::wavetable
{
	SignalGenerator::SignalGenerator(double sampleRate)
		: _sampleRate(sampleRate)
		, _signalSource(SineWaveTable, std::make_shared<interpolation::LinearInterpolator>())
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

	void SignalGenerator::setWaveTable(WaveTable waveTable)
	{
		_signalSource.setWaveTable(waveTable);
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
		audioChannel[sampleIndex] = _signalSource(_currentPhase);
		_currentPhase += _phaseDeltaPerSample;
	}
}
