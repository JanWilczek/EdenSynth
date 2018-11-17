/// 
/// \author Jan Wilczek
/// \date 08.10.2018
/// 
#include "synth/wavetable/WaveTableSource.h"
#include "synth/wavetable/SineWaveTable.h"
#include "utility/MathConstants.h"
#include "interpolation/LinearInterpolator.h"

namespace eden::synth::wavetable
{
	WaveTableSource::WaveTableSource(float sampleRate)
		: _sampleRate(sampleRate)
		, _waveform(SineWaveTable, std::make_shared<interpolation::LinearInterpolator>())
		, _omega(0.f)
		, _phaseDeltaPerSample(0.f)
		, _currentPhase(0.f)
	{
	}

	WaveTableSource::WaveTableSource(const WaveTableSource& other)
		: _sampleRate(other._sampleRate)
		, _waveform(other._waveform)
		, _omega(other._omega)
		, _phaseDeltaPerSample(other._phaseDeltaPerSample)
		, _currentPhase(other._currentPhase)
	{
	}

	std::unique_ptr<IOscillatorSource> WaveTableSource::clone()
	{
		return std::make_unique<WaveTableSource>(*this);
	}

	void WaveTableSource::reset()
	{
		_currentPhase = 0.f;
		_phaseDeltaPerSample = 0.f;
	}

	SampleType WaveTableSource::getSample()
	{
		const auto sample = _waveform(_currentPhase);

		_currentPhase += _phaseDeltaPerSample;
		while (_currentPhase > static_cast<float>(2.0 * math_constants::PI))
		{
			_currentPhase -= static_cast<float>(2.0 * math_constants::PI);
		}

		return sample;
	}

	void WaveTableSource::setWaveTable(WaveTable waveTable)
	{
		_waveform.setWaveTable(waveTable);
	}

	void WaveTableSource::setPitch(float pitch)
	{
		_omega = 2.f * static_cast<float>(math_constants::PI) * pitch;
		_phaseDeltaPerSample = _omega / _sampleRate;
	}

	void WaveTableSource::setSampleRate(float sampleRate)
	{
		_phaseDeltaPerSample = _phaseDeltaPerSample * _sampleRate / sampleRate;
		_sampleRate = sampleRate;
	}
}
