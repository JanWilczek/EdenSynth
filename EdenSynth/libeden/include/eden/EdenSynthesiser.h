#pragma once
#include <memory>
#include <eden_impl/EdenSynthesiserImpl.h>

namespace eden
{
	class AudioBuffer;
	class MidiBuffer;

	/// <summary>
	/// Main Eden synthesiser class.
	/// </summary>
	class EdenSynthesiser
	{

	public:
		EdenSynthesiser();

		void processInputBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer);
		void setSampleRate(unsigned int sampleRate);
		unsigned int sampleRate() const noexcept;

	private:
		std::unique_ptr<EdenSynthesiserImpl> _impl;
	};
}
