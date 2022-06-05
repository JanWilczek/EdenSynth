/// 
/// \author Jan Wilczek
/// \date 08.10.2018
/// 
#include <stdexcept>
#include "synth/wavetable/SignalGenerator.h"
#include "synth/wavetable/SineWaveTable.h"

namespace eden::synth::wavetable
{
	void SignalGenerator::generateSignal(float* audioChannel, int startSample, int samplesToGenerate)
	{
		for (int sample = startSample; sample < startSample + samplesToGenerate; ++sample)
		{
			generateSample(audioChannel, sample);
		}
	}

	void SignalGenerator::stop()
	{
		for (auto& oscillatorPair : _oscillators)
		{
			oscillatorPair.second.reset();
		}
	}

	void SignalGenerator::setPitch(float pitch)
	{
		for (auto& oscillatorPair : _oscillators)
		{
			oscillatorPair.second.setPitch(pitch);
		}
	}

	void SignalGenerator::addOscillator(SynthOscillator oscillator)
	{
		_oscillators[oscillator.getId()] = std::move(oscillator);
	}

	void SignalGenerator::removeOscillator(OscillatorId oscillatorToRemove)
	{
		const auto erased = _oscillators.erase(oscillatorToRemove);

		if (erased == 0)
		{
			throw std::runtime_error("Tried to remove invalid oscillator ID.");
		}
	}

	void SignalGenerator::setOscillatorSource(OscillatorId oscillatorId, std::unique_ptr<IOscillatorSource> source)
	{
		_oscillators[oscillatorId].setSource(std::move(source));
	}

	void SignalGenerator::setOctaveTransposition(OscillatorId oscillatorId, int octaveShift)
	{
		_oscillators[oscillatorId].setOctaveTransposition(octaveShift);
	}

	void SignalGenerator::setSemitoneTransposition(OscillatorId oscillatorId, int semitoneShift)
	{
		_oscillators[oscillatorId].setSemitoneTransposition(semitoneShift);
	}

	void SignalGenerator::setCentTransposition(OscillatorId oscillatorId, int centShift)
	{
		_oscillators[oscillatorId].setCentTransposition(centShift);
	}

	void SignalGenerator::setOscillatorVolume(OscillatorId oscillatorId, float volume)
	{
		_oscillators[oscillatorId].setVolume(volume);
	}

	void SignalGenerator::setOscillatorOn(OscillatorId oscillatorId, bool isOn)
	{
		_oscillators[oscillatorId].setOn(isOn);
	}

	void SignalGenerator::setSampleRate(float sampleRate)
	{
		for (auto& oscillatorPair : _oscillators)
		{
			oscillatorPair.second.setSampleRate(sampleRate);
		}
	}

	void SignalGenerator::generateSample(float* audioChannel, int sampleIndex)
	{
		for (auto& oscillatorPair : _oscillators)
		{
			audioChannel[sampleIndex] += oscillatorPair.second.getSample();
		}

		if (!_oscillators.empty())
		{
			audioChannel[sampleIndex] /= static_cast<float>(_oscillators.size());
		}
	}
}
