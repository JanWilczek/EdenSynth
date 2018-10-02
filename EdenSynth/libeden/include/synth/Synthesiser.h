#pragma once
/// 
/// \author Jan Wilczek
/// \date 02.09.2018
/// 
#include <vector>
#include <memory>

namespace eden
{
	class AudioBuffer;
	class MidiBuffer;
	class MidiMessage;
}

namespace eden::synth
{
	class Voice;

	/// <summary>
	/// Class responsible for generating sound based on provided input.
	/// </summary>
	class Synthesiser
	{
	public:
		explicit Synthesiser(double sampleRate);

		/// <summary>
		/// Fills the given audio buffer with samples in full based on internal state.
		/// </summary>
		/// <param name="bufferToFill"></param>
		/// <param name="midiBuffer"></param>
		/// <param name="numSamples"></param>
		void processBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer, int numSamples);

		/// <summary>
		/// Fills the given audio buffer with samples from <paramref="startSample"> (including) to <paramref="startSample"> + <paramref="numSamples"> (excluding).
		/// </summary>
		/// <param name="bufferToFill"></param>
		/// <param name="midiBuffer"></param>
		/// <param name="startSample"></param>
		/// <param name="numSamples"></param>
		void processBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer, int startSample, int numSamples);

		double getSampleRate() const noexcept;
		void setSampleRate(double newSampleRate);

	private:
		void handleMidiMessage(MidiMessage& midiMessage);
		void renderVoices(AudioBuffer& outputBuffer, int startSample, int samplesToProcess);
		void noteOn(const int midiChannel, const int midiNoteNumber, const float velocity);
		void noteOff(const int midiChannel, const int midiNoteNumber, const float velocity);
		void addVoices(unsigned numVoicesToAdd);
		std::shared_ptr<Voice> getFreeVoice();
		std::shared_ptr<Voice> getVoicePlayingNote(const int midiNoteNumber);

		std::vector<std::shared_ptr<Voice>> _voices;
		double _sampleRate;
	};
}