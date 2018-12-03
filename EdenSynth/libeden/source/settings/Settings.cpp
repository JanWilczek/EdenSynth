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
		: _tuner(std::make_shared<Tuner>())
		, _generatorSettings(std::make_unique<GeneratorSettings>())
		, _subtractiveModuleSettings(std::make_unique<SubtractiveModuleSettings>(_sampleRate))
		, _waveshapingModuleSettings(std::make_unique<WaveshapingModuleSettings>())
		, _envelopeSettings(std::make_unique<EnvelopeSettings>(_sampleRate))
	{
	}

	void Settings::setSampleRate(float sampleRate) noexcept
	{
		if (sampleRate != _sampleRate)
		{
			_sampleRate = sampleRate;

			_generatorSettings->setSampleRate(_sampleRate);
			_subtractiveModuleSettings->setSampleRate(_sampleRate);
			_envelopeSettings->setSampleRate(_sampleRate);
		}
	}

	float Settings::sampleRate() const noexcept
	{
		return _sampleRate;
	}

	std::shared_ptr<Tuner>	Settings::tuner() const noexcept
	{
		return _tuner;
	}

	void Settings::setFrequencyOfA4(float frequencyOfA4)
	{
		_tuner->setFrequencyOfA4(frequencyOfA4);
	}

	void Settings::setPitchBendRange(std::pair<int, int>  transposeDownTransposeUp)
	{
		_tuner->setPitchBendRange(transposeDownTransposeUp);
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

	void Settings::registerEnvelope(std::shared_ptr<synth::envelope::IEnvelopeHolder> envelope)
	{
		_envelopeSettings->registerEnvelope(envelope);
	}

	OscillatorSourceId Settings::createGeneratorSource(WaveformGenerators generatorName)
	{
		return _generatorSettings->createGeneratorSource(sampleRate(), generatorName);
	}

	OscillatorSourceId Settings::createWaveTableSource(std::vector<float> waveTable)
	{
		return _generatorSettings->createWaveTableSource(sampleRate(), waveTable);
	}

	void Settings::removeOscillatorSource(OscillatorSourceId sourceId)
	{
		_generatorSettings->removeOscillatorSource(sourceId);
	}

	OscillatorId Settings::addOscillator(OscillatorSourceId sourceId)
	{
		return _generatorSettings->addOscillator(sourceId);
	}

	void Settings::removeOscillator(OscillatorId oscillatorToRemove)
	{
		_generatorSettings->removeOscillator(oscillatorToRemove);
	}

	void Settings::setOscillatorSource(OscillatorId oscillatorId, OscillatorSourceId sourceId)
	{
		_generatorSettings->setOscillatorSource(oscillatorId, sourceId);
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

	void Settings::setOscillatorVolume(OscillatorId oscillatorId, float volume)
	{
		_generatorSettings->setOscillatorVolume(oscillatorId, volume);
	}

	void Settings::setOscillatorOn(OscillatorId oscillatorId, bool isOn)
	{
		_generatorSettings->setOscillatorOn(oscillatorId, isOn);
	}

	void Settings::setCutoff(float cutoff)
	{
		_subtractiveModuleSettings->setCutoff(cutoff);
	}

	void Settings::setResonance(float resonance)
	{
		_subtractiveModuleSettings->setResonance(resonance);
	}

	void Settings::setContourAmount(float contourAmount)
	{
		_subtractiveModuleSettings->setContourAmount(contourAmount);
	}

	void Settings::setPassbandAttenuation(PassbandAttenuation passbandAttenuation)
	{
		_subtractiveModuleSettings->setPassbandAttenuation(passbandAttenuation);
	}

	void Settings::setFilterEnvelopeParameters(std::shared_ptr<EnvelopeParameters> filterEnvelopeParameters)
	{
		_subtractiveModuleSettings->setEnvelopeParameters(filterEnvelopeParameters);
	}

	void Settings::setWaveshapingTransferFunction(std::vector<float> transferFunction)
	{
		_waveshapingModuleSettings->setTransferFunction(std::move(transferFunction));
	}

	void Settings::setWaveshapingAutoMakeUpGain(bool makeUpGainEnabled)
	{
		_waveshapingModuleSettings->setAutoMakeUpGain(makeUpGainEnabled);
	}

	void Settings::setEnvelopeParameters(std::shared_ptr<EnvelopeParameters> envelopeParameters)
	{
		_envelopeSettings->setEnvelopeParameters(envelopeParameters);
	}
}
