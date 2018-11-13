/// 
/// \author Jan Wilczek
/// \date 08.10.2018
/// 
#include "synth/wavetable/SignalGenerator.h"
#include "synth/wavetable/SineWaveTable.h"

namespace eden::synth::wavetable
{
	SignalGenerator::SignalGenerator()
	{
	}

	void SignalGenerator::generateSignal(SampleType* audioChannel, int startSample, int samplesToGenerate)
	{
		for (int sample = startSample; sample < startSample + samplesToGenerate; ++sample)
		{
			generateSample(audioChannel, sample);
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

	/*void SignalGenerator::stop()
	{
		_currentPhase = 0.0;
		_phaseDeltaPerSample = 0.0;
	}*/

	//void SignalGenerator::setWaveTable(WaveTable waveTable)
	//{
	//	_signalSource.setWaveTable(waveTable);
	//}

	void SignalGenerator::setPitch(float pitch)
	{
		/*const auto omega = 2.0 * math_constants::PI * pitch;
		_phaseDeltaPerSample = omega / _sampleRate;*/
		for (auto oscillatorPair : _oscillators)
		{
			oscillatorPair.second.setPitch(pitch);
		}
	}

	void SignalGenerator::setSampleRate(double sampleRate)
	{
		//_phaseDeltaPerSample = _phaseDeltaPerSample * _sampleRate / sampleRate;
		//_sampleRate = sampleRate;
		for (auto oscillatorPair : _oscillators)
		{
			oscillatorPair.second.setSampleRate(sampleRate);
		}
	}

	void SignalGenerator::generateSample(SampleType* audioChannel, int sampleIndex)
	{
		audioChannel[sampleIndex] = SampleType(0);

		for (auto oscillatorPair : _oscillators)
		{
			audioChannel[sampleIndex] += oscillatorPair.second.getSample();
		}
		//_currentPhase += _phaseDeltaPerSample;
	}
}
