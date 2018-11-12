#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include <memory>
#include "settings/EnvelopeSettings.h"
#include "settings/GeneratorSettings.h"
#include "settings/SubtractiveModuleSettings.h"
#include "settings/WaveshapingModuleSettings.h"
#include "synth/wavetable/WaveTable.h"
#include "eden/OscillatorParameters.h"

namespace eden
{
	struct EnvelopeParameters;

	namespace synth 
	{
		namespace wavetable
		{
			class SignalGenerator;
			class IOscillatorSource;
		}

		namespace subtractive
		{
			class SubtractiveModule;
		}

		namespace waveshaping
		{
			class WaveshapingModule;
		}

		namespace envelope
		{
			class Envelope;
		}
	}
}

namespace eden::settings
{
	class Settings
	{
	public:
		Settings();

		void registerSignalGenerator(std::shared_ptr<synth::wavetable::SignalGenerator> signalGenerator);
		void registerSubtractiveModule(std::shared_ptr<synth::subtractive::SubtractiveModule> subtractiveModule);
		void registerWaveshapingModule(std::shared_ptr<synth::waveshaping::WaveshapingModule> waveshapingModule);
		void registerEnvelope(std::shared_ptr<synth::envelope::Envelope> envelope);

		OscillatorId getAvailableOscillatorId();

		//void setGenerator(OscillatorId oscillatorId, WaveformGenerators generatorName);
		//void setWaveTable(OscillatorId oscillatorId, synth::wavetable::WaveTable waveTable);
		void setOscillatorSource(OscillatorId, std::shared_ptr<synth::wavetable::IOscillatorSource> source);
		void setOctaveTransposition(OscillatorId oscillatorId, int octaveShift);
		void setSemitoneTransposition(OscillatorId oscillatorId, int semitoneShift);
		void setCentTransposition(OscillatorId oscillatorId, int centShift);

		void setCutoff(float cutoff);
		void setResonance(float resonance);

		void setEnvelopeParameters(std::shared_ptr<EnvelopeParameters> envelopeParameters);

	private:
		std::unique_ptr<GeneratorSettings> _generatorSettings;
		std::unique_ptr<SubtractiveModuleSettings> _subtractiveModuleSettings;
		std::unique_ptr<WaveshapingModuleSettings> _waveshapingModuleSettings;
		std::unique_ptr<EnvelopeSettings> _envelopeSettings;
	};
}
