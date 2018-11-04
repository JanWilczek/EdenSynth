#pragma once
/// 
/// \author Jan Wilczek
/// \date 26.08.2018
/// 
#include <memory>
#include <eden_impl/EdenSynthesiserImpl.h>

namespace eden
{
	class AudioBuffer;
	class MidiBuffer;
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

		/// <summary>
		/// Sets the expected length of processing block - use it to allocate memory beforehand.
		/// </summary>
		/// <param name="samplesPerBlock"></param>
		void setBlockLength(int samplesPerBlock);

		void setWaveTable(std::vector<SampleType> waveTable);

		/// <summary>
		/// Sets new envelope of sound - the information about volume change in time in relation
		/// to keyboard events.
		/// </summary>
		/// <param name="envelopeParameters">parameters of the envelope to set - <c>ADBDRParameters</c> struct instance for example</param>
		void setEnvelope(std::shared_ptr<EnvelopeParameters> envelopeParameters);

	private:
		/// <summary>
		/// Internal implementation.
		/// </summary>
		std::unique_ptr<EdenSynthesiserImpl> _impl;
	};
}
