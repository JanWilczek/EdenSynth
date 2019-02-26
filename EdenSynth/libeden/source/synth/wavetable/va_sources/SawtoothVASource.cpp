/// 
/// \author Jan Wilczek
/// \date 11.02.2019
/// 
#include "synth/wavetable/va_sources/SawtoothVASource.h"
#include <cmath>

namespace eden::synth::wavetable
{
	SawtoothVASource::SawtoothVASource(float sampleRate)
		: _sampleRate(sampleRate)
		, _phase(0.f)
		, _z1(0.f)
		, _z2(0.f)
		, _phaseShift(0.f)
	{
	}

	SawtoothVASource::SawtoothVASource(const SawtoothVASource& other)
		: _sampleRate(other._sampleRate)
		, _phase(other._phase)
		, _delta(other._delta)
		, _z1(other._z1)
		, _z2(other._z2)
		, _c(other._c)
		, _pitch(other._pitch)
		, _phaseShift(other._phaseShift)
	{
	}

	void SawtoothVASource::setPhaseShift(float phaseShift)
	{
		_phaseShift = phaseShift;

		if (_delta != 0.f)
		{
			setPitch(_pitch);
		}
	}

	std::unique_ptr<IOscillatorSource> SawtoothVASource::clone()
	{
		return std::make_unique<SawtoothVASource>(*this);
	}

	void SawtoothVASource::reset()
	{
		_phase = 0.f;
		_delta = 0.f;
	}

	void SawtoothVASource::setPitch(float pitch)
	{
		_pitch = pitch;
		_delta = _pitch / _sampleRate;
		_c = _sampleRate / (4 * _pitch * (1 - _delta));

		_phase = _sampleRate / _pitch * _phaseShift;
	}

	float SawtoothVASource::getSample()
	{
		_phase = std::fmod(_phase + _delta, 1.f);
		const float bphase = 2 * _phase - 1.f;
		const float sq = std::pow(bphase, 2.f);
		
		//const float dsq = sq - _z1;
		//const float dsq = (sq - _z1) * (sq + _z1) / 2.f;
		const float dsq = (sq - _z2) / 2.f;

		_z2 = _z1;
		_z1 = sq;
		return _c * dsq;
	}

	void SawtoothVASource::setSampleRate(float sampleRate)
	{
		_sampleRate = sampleRate;
		setPitch(_pitch);
	}
}
