/// 
/// \author Jan Wilczek
/// \date 27.02.2019
/// 
#include "synth/wavetable/va_sources/TriangleVASource.h"

namespace eden::synth::wavetable
{
	TriangleVASource::TriangleVASource(float sampleRate)
		: _sampleRate(sampleRate)
		, _sine(_sampleRate)
		, _z1(0.f)
		, _c(0.f)
		, _isPlaying(false)
	{
	}

	TriangleVASource::TriangleVASource(const TriangleVASource& other)
		: _sampleRate(other._sampleRate)
		, _sine(other._sine)
		, _z1(other._z1)
		, _c(other._c)
		, _isPlaying(other._isPlaying)
	{
	}

	std::unique_ptr<IOscillatorSource> TriangleVASource::clone()
	{
		return std::make_unique<TriangleVASource>(*this);
	}

	void TriangleVASource::reset()
	{
		_sine.reset();
		_z1 = 0.f;
		_isPlaying = false;
	}

	void TriangleVASource::setPitch(float pitch)
	{
		_sine.setPitch(pitch);

		_c = _sampleRate / (4.f * pitch * (1.f - pitch / _sampleRate));

		_isPlaying = true;
	}

	float TriangleVASource::getSample()
	{
		const auto parabolicSine = _sine.getSample();
		const auto differentiated = parabolicSine - _z1;
		_z1 = parabolicSine;

		const auto scaled = _c * differentiated;

		return scaled;
	}

	void TriangleVASource::setSampleRate(float sampleRate)
	{
		_sampleRate = sampleRate;
		_sine.setSampleRate(sampleRate);

		if (_isPlaying)
		{
			setPitch(_sine.getPitch());
		}
	}
}
