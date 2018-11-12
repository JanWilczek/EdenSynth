/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 
#include "synth/wavetable/SynthOscillator.h"
#include "synth/wavetable/IOscillatorSource.h"

namespace eden::synth::wavetable
{
	SynthOscillator::SynthOscillator(std::shared_ptr<IOscillatorSource> oscillatorSource)
		: _oscillatorSource(oscillatorSource)
	{
	}

	OscillatorId SynthOscillator::getId()
	{
		return _id;
	}

	SampleType SynthOscillator::getSample()
	{
		return _oscillatorSource->getSample();
	}

	void SynthOscillator::setSource(std::shared_ptr<IOscillatorSource> oscillatorSource)
	{
		_oscillatorSource = _oscillatorSource;
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
