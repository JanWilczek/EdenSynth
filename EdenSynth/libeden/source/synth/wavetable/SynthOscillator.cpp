/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 
#include "synth/wavetable/SynthOscillator.h"
#include "synth/wavetable/IOscillatorSource.h"

namespace eden::synth::wavetable
{
	SynthOscillator::SynthOscillator(OscillatorId id, std::unique_ptr<IOscillatorSource> oscillatorSource)
		: _oscillatorSource(std::move(oscillatorSource))
		, _id(id)
	{
	}

	SynthOscillator::SynthOscillator(const SynthOscillator& other)
		: _oscillatorSource(other._oscillatorSource->clone())
		, _id(other._id)
		, _octaveShift(other._octaveShift)
		, _semitoneShift(other._semitoneShift)
		, _centShift(other._centShift)
		, _originalPitch(other._originalPitch)
	{
	}

	SynthOscillator& SynthOscillator::operator=(const SynthOscillator& other)
	{
		_oscillatorSource = other._oscillatorSource->clone();
		_id = other._id;
		_octaveShift = other._octaveShift;
		_semitoneShift = other._semitoneShift;
		_centShift = other._centShift;
		_originalPitch = other._originalPitch;

		return *this;
	}

	OscillatorId SynthOscillator::getId() const noexcept
	{
		return _id;
	}

	SampleType SynthOscillator::getSample()
	{
		return _oscillatorSource->getSample();
	}

	void SynthOscillator::reset()
	{
		_oscillatorSource->reset();
	}

	void SynthOscillator::setSource(std::unique_ptr<IOscillatorSource> oscillatorSource)
	{
		_oscillatorSource = std::move(oscillatorSource);
		setOscillatorPitch();
	}

	void SynthOscillator::setOctaveTransposition(int octaveShift)
	{
		_octaveShift = octaveShift;
		setOscillatorPitch();
	}

	void SynthOscillator::setSemitoneTransposition(int semitoneShift)
	{
		_semitoneShift = semitoneShift;
		setOscillatorPitch();
	}

	void SynthOscillator::setCentTransposition(int centShift)
	{
		_centShift = centShift;
		setOscillatorPitch();
	}

	void SynthOscillator::setPitch(float pitch)
	{
		_originalPitch = pitch;
		setOscillatorPitch();
	}

	void SynthOscillator::setSampleRate(float sampleRate)
	{
		_oscillatorSource->setSampleRate(sampleRate);
	}

	float SynthOscillator::pitch()
	{
		const auto transpositionInCents = static_cast<float>(_octaveShift) * 1200.f + static_cast<float>(_semitoneShift) * 100.f + static_cast<float>(_centShift);
		return _originalPitch * std::powf(2, transpositionInCents / 1200.f);
	}

	void SynthOscillator::setOscillatorPitch()
	{
		const auto pitchToSet = pitch();
		_oscillatorSource->setPitch(pitchToSet);
	}
}
