#pragma once

namespace eden
{
	/// <summary>
	/// Main Eden synthesiser class.
	/// </summary>
	class EdenSynthesiser
	{
		class AudioBuffer;
		class MidiBuffer;

	public:
		void processInputBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer);
		void setSampleRate(unsigned int sampleRate);
		unsigned int sampleRate() const noexcept;

	};
}