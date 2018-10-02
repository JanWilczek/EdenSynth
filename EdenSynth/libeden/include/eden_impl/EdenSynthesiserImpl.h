#pragma once
/// 
/// \author Jan Wilczek
/// \date 29.08.2018
/// 
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

		/// <summary>
		/// Fills the given audio buffer with samples according to messages stored in the MIDI buffer
		/// and previous synthesiser state. The audio buffer does not need to be empty, but its
		/// contents may be completetly overwritten.
		/// </summary>
		/// <param name="bufferToFill"></param>
		/// <param name="midiBuffer"></param>
		void processInputBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer);

		/// <summary>
		/// Sets new sample rate of synthesiser.
		/// </summary>
		/// <param name="sampleRate"></param>
		void setSampleRate(double sampleRate);

		/// <returns>currently used sample rate</returns>
		double sampleRate() const noexcept;

	private:
		double _sampleRate = 48000;
		std::unique_ptr<synth::Synthesiser> _synthesiser;
	};
}
