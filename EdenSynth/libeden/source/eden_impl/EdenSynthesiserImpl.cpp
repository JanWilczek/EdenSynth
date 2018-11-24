/// 
/// \author Jan Wilczek
/// \date 29.08.2018
/// 
#include "eden_impl/EdenSynthesiserImpl.h"
#include "eden/AudioBuffer.h"
#include "eden/EnvelopeParameters.h"
#include "eden_impl/OscillatorImpl.h"
#include "eden/Oscillator.h"
#include "utility/WaveFileReader.h"

namespace eden
{
	EdenSynthesiserImpl::EdenSynthesiserImpl()
		: _settings()
		, _synthesiser(_settings)
	{
	}

	void EdenSynthesiserImpl::processInputBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer)
	{
		_synthesiser.processBlock(bufferToFill, midiBuffer, bufferToFill.getNumSamples());
	}

	void EdenSynthesiserImpl::setSampleRate(float sampleRate)
	{
		_settings.setSampleRate(sampleRate);
	}

	float EdenSynthesiserImpl::sampleRate() const noexcept
	{
		return _settings.sampleRate();
	}

	void EdenSynthesiserImpl::setBlockLength(int samplesPerBlock)
	{
		_synthesiser.setBlockLength(samplesPerBlock);
	}

	void EdenSynthesiserImpl::setVolume(float volume)
	{
		_synthesiser.setVolume(volume);
	}

	void EdenSynthesiserImpl::setFrequencyOfA4(float frequencyOfA4)
	{
		_settings.setFrequencyOfA4(frequencyOfA4);
	}

	void EdenSynthesiserImpl::setPitchBendRange(std::pair<int, int> transposeDownTransposeUp)
	{
		_settings.setPitchBendRange(transposeDownTransposeUp);
	}

	std::unique_ptr<OscillatorSource> EdenSynthesiserImpl::createRealtimeOscillatorSource(WaveformGenerators generatorName)
	{
		return std::make_unique<OscillatorSource>(std::make_unique<OscillatorSourceImpl>(_settings, generatorName));
	}

	std::unique_ptr<OscillatorSource> EdenSynthesiserImpl::createWaveTableOscillatorSource(std::vector<float> waveTable)
	{
		return std::make_unique<OscillatorSource>(std::make_unique<OscillatorSourceImpl>(_settings, waveTable));
	}

	std::unique_ptr<OscillatorSource> EdenSynthesiserImpl::createWaveTableOscillatorSource(std::experimental::filesystem::path pathToWaveFile)
	{
		utility::WaveFileReader reader(pathToWaveFile.string());
		const auto wave = reader.readSamples();
		return std::make_unique<OscillatorSource>(std::make_unique<OscillatorSourceImpl>(_settings, wave));
	}

	std::unique_ptr<Oscillator> EdenSynthesiserImpl::createAndAddOscillator(std::unique_ptr<OscillatorSource> oscillatorSource)
	{
		return std::make_unique<Oscillator>(std::make_unique<OscillatorImpl>(_settings, std::move(oscillatorSource)));
	}

	void EdenSynthesiserImpl::setEnvelopeParameters(std::shared_ptr<EnvelopeParameters> envelopeParameters)
	{
		_settings.setEnvelopeParameters(envelopeParameters);
	}

	void EdenSynthesiserImpl::setCutoff(float cutoff)
	{
		_settings.setCutoff(cutoff);
	}

	void EdenSynthesiserImpl::setResonance(float resonance)
	{
		_settings.setResonance(resonance);
	}
}
