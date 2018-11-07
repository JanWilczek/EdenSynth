/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "settings/Settings.h"

namespace eden::settings
{
	Settings::Settings()
		: _generatorSettings(std::make_unique<GeneratorSettings>())
		, _subtractiveModuleSettings(std::make_unique<SubtractiveModuleSettings>())
		, _waveshapingModuleSettings(std::make_unique<WaveshapingModuleSettings>())
		, _envelopeSettings(std::make_unique<EnvelopeSettings>())
	{
	}

	void Settings::registerSignalGenerator(std::shared_ptr<synth::wavetable::SignalGenerator> signalGenerator)
	{
		_generatorSettings->registerSignalGenerator(signalGenerator);
	}

	void Settings::registerSubtractiveModule(std::shared_ptr<synth::subtractive::SubtractiveModule> subtractiveModule)
	{
		_subtractiveModuleSettings->registerSubtractiveModule(subtractiveModule);
	}

	void Settings::registerWaveshapingModule(std::shared_ptr<synth::waveshaping::WaveshapingModule> waveshapingModule)
	{
		_waveshapingModuleSettings->registerWaveshapingModule(waveshapingModule);
	}

	void Settings::registerEnvelope(std::shared_ptr<synth::envelope::Envelope> envelope)
	{
		_envelopeSettings->registerEnvelope(envelope);
	}

	void Settings::setCutoff(float cutoff)
	{
		_subtractiveModuleSettings->setCutoff(cutoff);
	}

	void Settings::setResonance(float resonance)
	{
		_subtractiveModuleSettings->setResonance(resonance);
	}
}
