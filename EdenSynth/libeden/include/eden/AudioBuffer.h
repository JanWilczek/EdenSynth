#pragma once
/// 
/// \author Jan Wilczek
/// \date 27.08.2018
/// 

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
		/// <param name="numSamples"></param>
		AudioBuffer(int numChannels, unsigned numSamples);

		/// <summary>
		/// Creates an audio buffer using preallocated data.
		/// </summary>
		/// <param name="dataToUse">preallocated data</param>
		/// <param name="numChannelsToUse">number of channels to use</param>
		/// <param name="numSamplesToUse">length of buffer to use</param>
		AudioBuffer(SampleType** dataToUse, int numChannelsToUse, unsigned numSamplesToUse);
		
		~AudioBuffer();

		/// <summary>
		/// Sets the number of used channels in the buffer. May need to allocate data.
		/// </summary>
		//void setNumChannels(int numChannels);

		/// <returns>currently used number of channels</returns>
		int getNumChannels() const noexcept;

		/// <summary>
		/// Sets the length of the buffer. May need to allocate data.
		/// </summary>
		//void setNumSamples(unsigned length);

		/// <returns>current length of the buffer</returns>
		unsigned getNumSamples() const noexcept;

		/// <summary>
		/// Adds <paramref="valueToAdd"> at a specified position.
		/// </summary>
		/// <param name="destChannel">channel of the sample</param>
		/// <param name="destSample">index of the sample</param>
		/// <param name="valueToAdd"></param>
		void addSample(int destChannel, int destSample, SampleType valueToAdd);

		/// <summary>
		/// Fills all currently used buffer with the given value.
		/// </summary>
		/// <param name="value"></param>
		void fill(SampleType value);

	private:
		int _numChannels = 2;
		unsigned _numSamples = 512;	// TODO: Reconsider - is it safe to have number of samples given as unsigned int?
		SampleType** _channels;
		SampleType* _preallocatedChannelSpace[32];
		bool _ownsChannels;
	};
}