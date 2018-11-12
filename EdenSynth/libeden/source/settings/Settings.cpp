/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "settings/Settings.h"
#include "eden/EnvelopeParameters.h"
#include "synth/wavetable/SynthOscillator.h"

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

	OscillatorId Settings::getAvailableOscillatorId()
	{
		return _generatorSettings->getAvailableOscillatorId();
	}

	void Settings::addOscillator(synth::wavetable::SynthOscillator oscillator)
	{
		_generatorSettings->addOscillator(oscillator);
	}

	void Settings::removeOscillator(OscillatorId oscillatorToRemove)
	{
		_generatorSettings->removeOscillator(oscillatorToRemove);
	}

	void Settings::setOscillatorSource(OscillatorId oscillatorId, std::unique_ptr<synth::wavetable::IOscillatorSource> source)
	{
		_generatorSettings->setOscillatorSource(oscillatorId, std::move(source));
	}

	void Settings::setOctaveTransposition(OscillatorId oscillatorId, int octaveShift)
	{
		_generatorSettings->setOctaveTransposition(oscillatorId, octaveShift);
	}

	void Settings::setSemitoneTransposition(OscillatorId oscillatorId, int semitoneShift)
	{
		_generatorSettings->setSemitoneTransposition(oscillatorId, semitoneShift);
	}

	void Settings::setCentTransposition(OscillatorId oscillatorId, int centShift)
	{
		_generatorSettings->setCentTransposition(oscillatorId, centShift);
	}

	void Settings::setCutoff(float cutoff)
	{
		_subtractiveModuleSettings->setCutoff(cutoff);
	}

	void Settings::setResonance(float resonance)
	{
		_subtractiveModuleSettings->setResonance(resonance);
	}

	void Settings::setEnvelopeParameters(std::shared_ptr<EnvelopeParameters> envelopeParameters)
	{
		_envelopeSettings->setEnvelopeParameters(envelopeParameters);
	}
}
