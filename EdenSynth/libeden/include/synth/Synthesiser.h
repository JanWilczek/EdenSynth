#pragma once
/// 
/// \author Jan Wilczek
/// \date 02.09.2018
/// 
#include <vector>
#include <memory>
#include "synth/Voice.h"

namespace eden
{
	class AudioBuffer;
	class MidiBuffer;
	class MidiMessage;
}

namespace eden::synth
{
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
		/// Fills the given audio buffer with samples from <paramref name="startSample"> (including) to <paramref name="startSample"> + <paramref name="numSamples"> (excluding).
		/// </summary>
		/// <param name="bufferToFill"></param>
		/// <param name="midiBuffer"></param>
		/// <param name="startSample"></param>
		/// <param name="numSamples"></param>
		void processBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer, int startSample, int numSamples);

		/// <returns>currently used sample rate</returns>
		double getSampleRate() const noexcept;

		/// <summary>
		/// Sets internal sample rate. May be costly due to filters' parameters recalculation.
		/// </summary>
		/// <param name="newSampleRate"></param>
		void setSampleRate(double newSampleRate);

	private:
		/// <summary>
		/// Changes internal state based on received MIDI message. E.g. starts playing a note after note on message.
		/// </summary>
		/// <param name="midiMessage"></param>
		void handleMidiMessage(MidiMessage& midiMessage);

		/// <summary>
		/// Render all voices - both active and inactive.
		/// </summary>
		/// <param name="outputBuffer"></param>
		/// <param name="startSample"></param>
		/// <param name="samplesToProcess"></param>
		void renderVoices(AudioBuffer& outputBuffer, int startSample, int samplesToProcess);

		/// <summary>
		/// Handle note on event.
		/// </summary>
		/// <param name="midiChannel"></param>
		/// <param name="midiNoteNumber"></param>
		/// <param name="velocity"></param>
		void noteOn(const int midiChannel, const int midiNoteNumber, const float velocity);

		/// <summary>
		/// Handle note off event.
		/// </summary>
		/// <param name="midiChannel"></param>
		/// <param name="midiNoteNumber"></param>
		/// <param name="velocity"></param>
		void noteOff(const int midiChannel, const int midiNoteNumber, const float velocity);

		/// <summary>
		/// Adds voices given number of voices to the synthesiser. More voices means, that more notes can be played at once.
		/// </summary>
		/// <param name="numVoicesToAdd"></param>
		void addVoices(unsigned numVoicesToAdd);

		/// <returns>first free voice</returns>
		Voice* getFreeVoice() const;

		/// <param name="midiNoteNumber"></param>
		/// <returns>voice that is playing given note, nullptr if there is no such voice</returns>
		Voice* getVoicePlayingNote(const int midiNoteNumber) const;

		/// <summary>
		/// Container with voices.
		/// </summary>
		std::vector<std::unique_ptr<Voice>> _voices;
		double _sampleRate;
	};
}