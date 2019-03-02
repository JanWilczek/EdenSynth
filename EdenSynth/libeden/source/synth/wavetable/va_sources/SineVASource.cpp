/// 
/// \author Jan Wilczek
/// \date 27.02.2019
/// 
#include "synth/wavetable/va_sources/SineVASource.h"

namespace eden::synth::wavetable
{
	SineVASource::SineVASource(float sampleRate)
		: _sampleRate(sampleRate)
		, _pitch(0.f)
		, _phase(0.f)
		, _delta(0.f)
		, _sign(1.f)
	{}

	SineVASource::SineVASource(const SineVASource& other)
		: _sampleRate(other._sampleRate)
		, _pitch(other._pitch)
		, _phase(other._phase)
		, _delta(other._delta)
		, _sign(other._sign)
	{}

	float SineVASource::getPitch() const noexcept
	{
		return _pitch;
	}

	std::unique_ptr<IOscillatorSource> SineVASource::clone()
	{
		return std::make_unique<SineVASource>(*this);
	}

	void SineVASource::reset()
	{
		_phase = 0.f;
		_delta = 0.f;
		_sign = 1.f;
	}

	void SineVASource::setPitch(float pitch)
	{
		_pitch = pitch;
		_delta = 2 * _pitch / _sampleRate;
	}

	float SineVASource::getSample()
	{
		_phase += _delta;

		if (_phase >= 1.0f)
		{
			_sign *= -1.f;
		}

		_phase = std::fmod(_phase, 1.f);
		const auto bphase = 2 * _phase - 1.f;
		const auto parabolic = std::powf(bphase, 2.f);
		const auto reverted = 1.f - parabolic;
		const auto parabolicSine = reverted * _sign;

		return parabolicSine;
	}

	void SineVASource::setSampleRate(float sampleRate)
	{
		_sampleRate = sampleRate;

		// if source is playing recalculate coefficients
		if (_delta != 0.f)
		{
			setPitch(_pitch);
		}
	}
}
