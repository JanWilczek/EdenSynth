#pragma once

namespace eden
{
	/// <summary>
	/// A container of samples used to output the synthesized sound.
	/// </summary>
	class AudioBuffer
	{
	public:
		typedef float SampleType;

		AudioBuffer() = default;

		/// <summary>
		/// Creates a new audio buffer allocating the given number of channels of given length.
		/// </summary>
		/// <param name="numChannels"></param>
		/// <param name="length"></param>
		AudioBuffer(unsigned numChannels = 2, unsigned numSamples = 512);

		/// <summary>
		/// Creates an audio buffer using preallocated data.
		/// </summary>
		/// <param name="dataToUse">preallocated data</param>
		/// <param name="numChannelsToUse">number of channels to use</param>
		/// <param name="numSamplesToUse">length of buffer to use</param>
		AudioBuffer(SampleType** dataToUse, unsigned numChannelsToUse, unsigned numSamplesToUse);
		
		~AudioBuffer();

		unsigned getNumChannels() const noexcept;
		unsigned getNumSamples() const noexcept;

		void clear();

	private:
		unsigned _numChannels;
		unsigned _numSamples;
		SampleType** _channels;
		SampleType* _preallocatedChannelSpace[32];
	};
}