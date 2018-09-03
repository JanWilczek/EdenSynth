#pragma once

#include "synth/Synthesiser.h"

namespace eden
{
	class AudioBuffer;
	class MidiBuffer;

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

	private:
		double _sampleRate = 48000;
		std::unique_ptr<synth::Synthesiser> _synthesiser;
	};
}
