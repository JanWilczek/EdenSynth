#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include <memory>
#include <vector>
#include <map>
#include "eden/OscillatorParameters.h"
#include "eden/SampleType.h"

namespace eden::synth::wavetable
{
	class SynthOscillator;
	class SignalGenerator;
	class IOscillatorSource;
}

namespace eden::settings
{
	class GeneratorSettings
	{
	public:
		void registerSignalGenerator(std::shared_ptr<synth::wavetable::SignalGenerator> signalGenerator);
		
		OscillatorSourceId createGeneratorSource(float sampleRate, WaveformGenerators generatorName);
		OscillatorSourceId createWaveTableSource(float sampleRate, std::vector<SampleType> waveTable);
		void removeOscillatorSource(OscillatorSourceId sourceId);
		OscillatorId addOscillator(OscillatorSourceId sourceId);
		void removeOscillator(OscillatorId oscillatorToRemove);
		void setOscillatorSource(OscillatorId oscillatorId, OscillatorSourceId sourceId);
		void setOctaveTransposition(OscillatorId oscillatorId, int octaveShift);
		void setSemitoneTransposition(OscillatorId oscillatorId, int semitoneShift);
		void setCentTransposition(OscillatorId oscillatorId, int centShift);
		void setOscillatorVolume(OscillatorId oscillatorId, float volume);
		void setOscillatorOn(OscillatorId oscillatorId, bool isOn);

	private:
		std::vector<std::shared_ptr<synth::wavetable::SignalGenerator>> _signalGenerators;
		OscillatorSourceId _firstAvailableSourceId = 0u;
		std::map<OscillatorSourceId, std::unique_ptr<synth::wavetable::IOscillatorSource>> _oscillatorSources;
		OscillatorId _firstAvailableOscillatorId = 0u;
	};
}
