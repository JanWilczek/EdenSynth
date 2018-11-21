#pragma once
/// 
/// \author Jan Wilczek
/// \date 26.08.2018
/// 
#include <memory>
#include <filesystem>
#include "eden_impl/EdenSynthesiserImpl.h"
#include "eden/OscillatorSource.h"
#include "eden/Oscillator.h"

namespace eden
{
	class AudioBuffer;
	class MidiBuffer;
	enum class WaveformGenerators;
	struct EnvelopeParameters;

	/// <summary>
	/// Main Eden synthesiser class.
	/// </summary>
	class EdenSynthesiser
	{
	public:
		EdenSynthesiser();

		/// <summary>
		/// Fills the given audio buffer with samples according to messages stored in the MIDI buffer
		/// and previous synthesiser state. The audio buffer does not need to be empty, but its
		/// contents will be completetly overwritten.
		/// </summary>
		/// <param name="bufferToFill"></param>
		/// <param name="midiBuffer"></param>
		void processInputBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer);

		/// <summary>
		/// Sets a new sample rate of synthesiser.
		/// </summary>
		/// <param name="sampleRate"></param>
		void setSampleRate(float sampleRate);

		/// <returns>currently used sample rate</returns>
		float sampleRate() const noexcept;

		/// <summary>
		/// Sets the expected length of processing block - use it to allocate memory before calling to <c>processInputBlock()</c>.
		/// </summary>
		/// <param name="samplesPerBlock"></param>
		void setBlockLength(int samplesPerBlock);

		/// <summary>
		/// Sets the overall volume of the synthesiser.
		/// </summary>
		/// <param name="volume">volume in range [0; 1]</param>
		void setVolume(float volume);

		/// <summary>
		/// Sets the frequency which should correspond to MIDI note 69.
		/// </summary>
		/// <param name="frequencyOfA4"></param>
		void setFrequencyOfA4(float frequencyOfA4);

		/// <summary>
		/// Sets the pitch bend range: how much in semitones can the pitch wheel transpose a sound.
		/// E.g. pair {-12, 2} transposes the pitch by a tone up, when pitch wheel's value is maximum and by an octave down, when pitch wheel's value is minimum.
		/// </summary>
		/// <param name="transposeDownTransposeUp">first element indicates the number of semitones corresponding to maximum down-bend, second the number of semitones corresponding to maximum up-bend</param>
		void setPitchBendRange(std::pair<int, int> transposeDownTransposeUp);

		/// <summary>
		/// Creates an oscillator source which generates samples in realtime, i.e. calculates actual function values.
		/// </summary>
		/// <param name="generatorName">name of the generator to base this source on</param>
		/// <returns>handle to the source</returns>
		std::unique_ptr<OscillatorSource> createRealtimeOscillatorSource(WaveformGenerators generatorName);

		/// <summary>
		/// Creates an oscillator source which generates samples by interpolating values given in the <paramref name="waveTable">.
		/// </summary>
		/// <param name="waveTable">one cycle of a waveform to be replayed</param>
		/// <returns>handle to the source</returns>
		std::unique_ptr<OscillatorSource> createWaveTableOscillatorSource(std::vector<float> waveTable);
		
		/// <summary>
		/// Creates an oscillator source which generates samples by interpolating values given in the wave file <paramref name="pathToWaveFile">.
		/// </summary>
		/// <param name="pathToWaveFile">path to the wave file containing one cycle of a waveform to replay</param>
		/// <returns>handl to the source</returns>
		std::unique_ptr<OscillatorSource> createWaveTableOscillatorSource(std::experimental::filesystem::path pathToWaveFile);
		
		/// <summary>
		/// Creates and oscillator based on the given <paramref name="oscillatorSource">. The source can be later altered.
		/// </summary>
		/// <param name="oscillatorSource">source of the waveform to be controlled by this oscillator</param>
		/// <returns>handle to the oscillator</returns>
		std::unique_ptr<Oscillator> createAndAddOscillator(std::unique_ptr<OscillatorSource> oscillatorSource);

		/// <summary>
		/// Sets new envelope of sound - the information about volume change in time in relation
		/// to keyboard events.
		/// </summary>
		/// <param name="envelopeParameters">parameters of the envelope to set - <c>ADBDRParameters</c> struct instance for example</param>
		void setEnvelopeParameters(std::shared_ptr<EnvelopeParameters> envelopeParameters);

		/// <summary>
		/// Set cutoff of the lowpass filter.
		/// </summary>
		/// <param name="cutoff">cutoff in range [0; 0,5]</param>
		void setCutoff(float cutoff);

		/// <summary>
		/// Set resonance or Q of the lowpass filter.
		/// </summary>
		/// <param name="resonance"></param>
		void setResonance(float resonance);

	private:
		/// <summary>
		/// Internal implementation.
		/// </summary>
		std::unique_ptr<EdenSynthesiserImpl> _impl;
	};
}
