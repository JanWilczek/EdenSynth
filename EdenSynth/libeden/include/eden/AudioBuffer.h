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
		AudioBuffer(int numChannels, unsigned numSamples);

		/// <summary>
		/// Creates an audio buffer using preallocated data.
		/// </summary>
		/// <param name="dataToUse">preallocated data</param>
		/// <param name="numChannelsToUse">number of channels to use</param>
		/// <param name="numSamplesToUse">length of buffer to use</param>
		AudioBuffer(SampleType** dataToUse, int numChannelsToUse, unsigned numSamplesToUse);
		
		~AudioBuffer();

		int getNumChannels() const noexcept;
		unsigned getNumSamples() const noexcept;

		void addSample(int destChannel, int destSample, SampleType valueToAdd);

		void clear();

	private:
		int _numChannels = 2;
		unsigned _numSamples = 512;	// TODO: Reconsider - is it safe to have number of samples given as unsigned int?
		SampleType** _channels;
		SampleType* _preallocatedChannelSpace[32];
		bool _ownsChannels;
	};
}