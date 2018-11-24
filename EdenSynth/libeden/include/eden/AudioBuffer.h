#pragma once
/// 
/// \author Jan Wilczek
/// \date 27.08.2018
/// 
#include <functional>

namespace eden
{
	/// <summary>
	/// A container of samples used to output the synthesized sound.
	/// </summary>
	class AudioBuffer
	{
	public:
		/// <summary>
		/// Creates a new audio buffer allocating the given number of channels of given length.
		/// </summary>
		/// <param name="numChannels"></param>
		/// <param name="numSamples"></param>
		AudioBuffer(int numChannels = 2, unsigned numSamples = 512u);

		/// <summary>
		/// Creates an audio buffer using preallocated data.
		/// </summary>
		/// <param name="dataToUse">preallocated data</param>
		/// <param name="numChannelsToUse">number of channels to use</param>
		/// <param name="numSamplesToUse">length of buffer to use</param>
		AudioBuffer(float** dataToUse, int numChannelsToUse, unsigned numSamplesToUse);
		
		AudioBuffer(AudioBuffer&&) noexcept;
		AudioBuffer& operator=(AudioBuffer&&) noexcept;

		~AudioBuffer();

		/// <param name="channel">channel of samples to get a write pointer to</param>
		/// <returns>write pointer to an array of float samples</returns>
		float* getWritePointer(int channel) const;

		/// <returns>array of pointers to inner mutable data block, each element in array being a pointer to a channel</returns>
		float** getArrayOfWritePointers() const noexcept;

		/// <param name="channel">channel of samples to get a read pointer to</param>
		/// <returns>a pointer to a read-only array of float samples</returns>
		const float* getReadPointer(int channel) const;

		/// <returns>array of pointers to inner immutable data block, each element in array being a pointer to a channel</returns>
		const float** getArrayOfReadPointers() const noexcept;

		/// <summary>
		/// Sets the number of used channels in the buffer. May need to allocate data.
		/// </summary>
		//void setNumChannels(int numChannels);

		/// <returns>currently used number of channels</returns>
		int getNumChannels() const noexcept;

		/// <summary>
		/// Sets the length of the buffer. May need to allocate data.
		/// </summary>
		void setNumSamples(unsigned length);

		/// <returns>current length of the buffer</returns>
		unsigned getNumSamples() const noexcept;

		/// <summary>
		/// Adds <paramref name="valueToAdd"/> at a specified position.
		/// </summary>
		/// <param name="destChannel">channel of the sample</param>
		/// <param name="destSample">index of the sample</param>
		/// <param name="valueToAdd"></param>
		void addSample(int destChannel, unsigned destSample, float valueToAdd);

		/// <summary>
		/// Fills whole currently used buffer with the given value.
		/// </summary>
		/// <param name="value"></param>
		void fill(float value);

		/// <summary>
		/// Fills the buffer on all channels with given <paramref name="value"/> in range [<paramref name="begin"/>, <paramref name="end"/>).
		/// </summary>
		/// <param name="value"></param>
		/// <param name="begin"></param>
		/// <param name="end"></param>
		void fillFromTo(float value, unsigned begin, unsigned end);

		/// <summary>
		/// Performs given operation on each channel.
		/// </summary>
		/// <param name="callback">function to call on each channel (array of samples)</param>
		void forEachChannel(std::function<void(float*)> callback);

		/// <summary>
		/// Performs given operation on each sample stored in the buffer.
		/// </summary>
		/// <param name="callback">function to call on each sample</param>
		void forEachSample(std::function<void(float&)> callback);

	private:
		/// <summary>
		/// Frees any allocated memory if it belongs to the buffer.
		/// </summary>
		void freeAllChannels();

		/// <summary>
		/// Allocate a memory block of size _numChannels x _allocatedNumSamples.
		/// </summary>
		void allocateChannels();

		int _numChannels;
		unsigned _numSamples;
		unsigned _allocatedNumSamples;
		float** _channels;
		bool _ownsChannels;
	};
}
