#pragma once
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
		Synthesiser() = default;

		void processBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer, int numSamples);
		void processBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer, int startSample, int numSamples);

	private:
		void handleMidiMessage(MidiMessage& midiMessage);
		void renderVoices(AudioBuffer& outputBuffer, int startSample, int samplesToProcess);
		void noteOn(const int midiChannel, const int midiNoteNumber, const float velocity);
		void noteOff(const int midiChannel, const int midiNoteNumber, const float velocity);

		std::vector<std::shared_ptr<Voice>> _voices;
	};
}