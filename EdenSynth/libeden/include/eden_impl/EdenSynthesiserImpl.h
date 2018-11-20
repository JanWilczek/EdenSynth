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

		void setSampleRate(double sampleRate);

		double sampleRate() const noexcept;

		void setBlockLength(int samplesPerBlock);

		std::unique_ptr<OscillatorSource> createRealtimeOscillatorSource(WaveformGenerators generatorName);

		std::unique_ptr<OscillatorSource> createWaveTableOscillatorSource(std::vector<SampleType> waveTable);

		std::unique_ptr<OscillatorSource> createWaveTableOscillatorSource(std::experimental::filesystem::path pathToWaveFile);
		
		std::unique_ptr<Oscillator> createAndAddOscillator(std::unique_ptr<OscillatorSource> oscillatorSource);

		void setEnvelopeParameters(std::shared_ptr<EnvelopeParameters> envelopeParameters);

		void setCutoff(float cutoff);

		void setResonance(float resonance);

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
