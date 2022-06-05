/// 
/// \author Jan Wilczek
/// \date 26.02.2019
/// 
#include "synth/wavetable/va_sources/PulseVASource.h"
#include <stdexcept>

namespace eden::synth::wavetable
{
	PulseVASource::PulseVASource(float sampleRate)
		: _saw(sampleRate)
		, _sawShifted(sampleRate)
	{
		// shift the second saw in phase - default is 50 %, a square
		setDutyCycle(0.5f);
	}

	PulseVASource::PulseVASource(const PulseVASource& other)
		: _saw(other._saw)
		, _sawShifted(other._sawShifted)
	{
	}

	void PulseVASource::setDutyCycle(float dutyCycle)
	{
		if (dutyCycle < 0.f || dutyCycle > 1.f)
		{
			throw std::invalid_argument("Duty cycle must be in the [0, 1] range.");
		}

		_sawShifted.setPhaseShift(dutyCycle);
	}

	std::unique_ptr<IOscillatorSource> PulseVASource::clone()
	{
		return std::make_unique<PulseVASource>(*this);
	}

	void PulseVASource::reset()
	{
		_saw.reset();
		_sawShifted.reset();
	}

	void PulseVASource::setPitch(float pitch)
	{
		_saw.setPitch(pitch);
		_sawShifted.setPitch(pitch);
	}

	float PulseVASource::getSample()
	{
		return _saw.getSample() - _sawShifted.getSample();
	}

	void PulseVASource::setSampleRate(float sampleRate)
	{
		_saw.setSampleRate(sampleRate);
		_sawShifted.setSampleRate(sampleRate);
	}
}
