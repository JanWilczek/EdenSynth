#pragma once
/// 
/// \author Jan Wilczek
/// \date 29.08.2018
/// 
#include <filesystem>
#include <vector>
#include <memory>
#include "synth/Synthesiser.h"
#include "settings/Settings.h"

namespace eden
{
	class AudioBuffer;
	class MidiBuffer;
	class OscillatorSource;
	class Oscillator;
	struct EnvelopeParameters;

	/// <summary>
	///	Internal EdenSynthesiser implementation.
	/// </summary>
	class EdenSynthesiserImpl
	{
	public:
		EdenSynthesiserImpl();

		void processInputBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer);

		void setSampleRate(float sampleRate);

		float sampleRate() const noexcept;

		void setBlockLength(int samplesPerBlock);

		void setVolume(float volume);

		void setFrequencyOfA4(float frequencyOfA4);

		void setPitchBendRange(std::pair<int, int> transposeDownTransposeUp);

		std::unique_ptr<OscillatorSource> createRealtimeOscillatorSource(WaveformGenerator generatorName);

		std::unique_ptr<OscillatorSource> createWaveTableOscillatorSource(std::vector<float> waveTable);

		std::unique_ptr<OscillatorSource> createWaveTableOscillatorSource(std::filesystem::path pathToWaveFile);
		
		std::unique_ptr<Oscillator> createAndAddOscillator(std::unique_ptr<OscillatorSource> oscillatorSource);

		void setCutoff(float cutoff);

		void setResonance(float resonance);

		void setContourAmount(float contourAmount);

		void setPassbandAttenuation(PassbandAttenuation passbandAttenuation);
		
		void setFilterEnvelopeParameters(std::shared_ptr<EnvelopeParameters> filterEnvelopeParameters);

		void setWaveshapingTransferFunction(std::vector<float> transferFunction);

		void setWaveshapingAutoMakeUpGain(bool makeUpGainEnabled);

		void setEnvelopeParameters(std::shared_ptr<EnvelopeParameters> envelopeParameters);

	private:
		/// <summary>
		/// Library's settings.
		/// </summary>
		settings::Settings _settings;

		/// <summary>
		/// The main synthesis class.
		/// </summary>
		synth::Synthesiser _synthesiser;
	};
}
