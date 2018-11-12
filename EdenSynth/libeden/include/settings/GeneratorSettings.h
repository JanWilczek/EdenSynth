#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include <memory>
#include <vector>
#include "eden/OscillatorParameters.h"

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
		
		OscillatorId getAvailableOscillatorId();
		void addOscillator(synth::wavetable::SynthOscillator oscillator);
		void removeOscillator(OscillatorId oscillatorToRemove);
		void setOscillatorSource(OscillatorId oscillatorId, std::unique_ptr<synth::wavetable::IOscillatorSource> source);
		void setOctaveTransposition(OscillatorId oscillatorId, int octaveShift);
		void setSemitoneTransposition(OscillatorId oscillatorId, int semitoneShift);
		void setCentTransposition(OscillatorId oscillatorId, int centShift);

	private:
		std::vector<std::shared_ptr<synth::wavetable::SignalGenerator>> _signalGenerators;
		OscillatorId _firstAvailableOscillatorId = 0u;
	};
}
