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
		, _octaveShift(0)
		, _semitoneShift(0)
		, _centShift(0)
		, _volume(1.f)
		, _isOn(true)
		, _originalPitch(0.f)
	{
	}

	SynthOscillator::SynthOscillator(const SynthOscillator& other)
		: _oscillatorSource(other._oscillatorSource->clone())
		, _id(other._id)
		, _octaveShift(other._octaveShift)
		, _semitoneShift(other._semitoneShift)
		, _centShift(other._centShift)
		, _volume(other._volume)
		, _isOn(other._isOn)
		, _originalPitch(other._originalPitch)
	{
	}

	SynthOscillator::SynthOscillator(SynthOscillator&& other) noexcept
		: _oscillatorSource(std::move(other._oscillatorSource))
		, _id(other._id)
		, _octaveShift(other._octaveShift)
		, _semitoneShift(other._semitoneShift)
		, _centShift(other._centShift)
		, _volume(other._volume)
		, _isOn(other._isOn)
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
		_volume = other._volume;
		_isOn = other._isOn;
		_originalPitch = other._originalPitch;

		return *this;
	}

	SynthOscillator& SynthOscillator::operator=(SynthOscillator&& other) noexcept
	{
		_oscillatorSource = std::move(other._oscillatorSource);
		_id = other._id;
		_octaveShift = other._octaveShift;
		_semitoneShift = other._semitoneShift;
		_centShift = other._centShift;
		_volume = other._volume;
		_isOn = other._isOn;
		_originalPitch = other._originalPitch;

		return *this;
	}

	OscillatorId SynthOscillator::getId() const noexcept
	{
		return _id;
	}

	SampleType SynthOscillator::getSample()
	{
		if (_isOn)
		{
			return static_cast<SampleType>(_volume) * _oscillatorSource->getSample();
		}
		return SampleType(0);
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

	void SynthOscillator::setVolume(float volume)
	{
		_volume = volume;
	}

	void SynthOscillator::setOn(bool isOn)
	{
		_isOn = isOn;
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
