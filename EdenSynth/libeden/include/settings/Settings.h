#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include <memory>

namespace eden::synth
{
	namespace wavetable
	{
		class SignalGenerator;
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


namespace eden::settings
{
	class Settings
	{
	public:
		void registerSignalGenerator(std::shared_ptr<synth::wavetable::SignalGenerator> signalGenerator);
		void registerSubtractiveModule(std::shared_ptr<synth::subtractive::SubtractiveModule> subtractiveModule);
		void registerWaveshapingModule(std::shared_ptr<synth::waveshaping::WaveshapingModule> waveshapingModule);
		void registerEnvelope(std::shared_ptr<synth::envelope::Envelope> envelope);

	private:
		std::unique_ptr<GeneratorSettings> _generatorSettings;
		std::unique_ptr<SubtractiveModuleSettings> _subtractiveModuleSettings;
		std::unique_ptr<WaveshapingModuleSettings> _waveshapingModuleSettings;
		std::unique_ptr<EnvelopeSettings> _envelopeSettings;
	};
}
