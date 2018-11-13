/// 
/// \author Jan Wilczek
/// \date 02.09.2018
/// 
#include "eden/AudioBuffer.h"
#include "utility/EdenAssert.h"

namespace eden
{
	AudioBuffer::AudioBuffer(int numChannels, unsigned numSamples)
		: _numChannels(numChannels)
		, _numSamples(numSamples)
		, _channels(new SampleType*[_numChannels])
		, _ownsChannels(true)
	{
		for (int channel = 0; channel < _numChannels; ++channel)
		{
			_channels[channel] = new SampleType[_numSamples];
		}
	}

	AudioBuffer::AudioBuffer(SampleType** dataToUse, int numChannelsToUse, unsigned numSamplesToUse)
		: _numChannels(numChannelsToUse)
		, _numSamples(numSamplesToUse)
		, _channels(dataToUse)
		, _ownsChannels(false)
	{
	}

	AudioBuffer::AudioBuffer(AudioBuffer&& other) noexcept 
		: _numChannels(other._numChannels)
		, _numSamples(other._numSamples)
		, _channels(other._channels)
		, _ownsChannels(other._ownsChannels)
	{
		other._numChannels = 0;
		other._numSamples = 0u;
		other._ownsChannels = false;
		other._channels = nullptr;
	}

	AudioBuffer& AudioBuffer::operator=(AudioBuffer&& other) noexcept
	{
		if (this != &other)
		{
			freeAllChannels();

			_numChannels = other._numChannels;
			_numSamples = other._numSamples;
			_channels = other._channels;
			_ownsChannels = other._ownsChannels;

			other._numChannels = 0;
			other._numSamples = 0u; 
			other._ownsChannels = false;
			other._channels = nullptr;
		}

		return *this;
	}
	
	AudioBuffer::~AudioBuffer()
	{
		freeAllChannels();
	}

	SampleType* AudioBuffer::getWritePointer(int channel) const
	{
		EDEN_ASSERT(channel < _numChannels);

		return _channels[channel];
	}

	SampleType** AudioBuffer::getArrayOfWritePointers() const noexcept
	{
		return _channels;
	}

	const SampleType* AudioBuffer::getReadPointer(int channel) const
	{
		EDEN_ASSERT(channel < _numChannels);

		return const_cast<const SampleType*>(_channels[channel]);
	}

	const SampleType** AudioBuffer::getArrayOfReadPointers() const noexcept
	{
		return const_cast<const SampleType**>(_channels);
	}

	int AudioBuffer::getNumChannels() const noexcept
	{
		return _numChannels;
	}

	unsigned AudioBuffer::getNumSamples() const noexcept
	{
		return _numSamples;
	}

	void AudioBuffer::addSample(int destChannel, unsigned destSample, SampleType valueToAdd)
	{
		_channels[destChannel][destSample] += valueToAdd;
	}

	void AudioBuffer::fill(SampleType value)
	{
		fillFromTo(value, 0, _numSamples);
	}

	void AudioBuffer::fillFromTo(SampleType value, unsigned begin, unsigned end)
	{
		for (int channel = 0; channel < _numChannels; ++channel)
		{
			for (unsigned sample = begin; sample < end; ++sample)
			{
				_channels[channel][sample] = value;
			}
		}
	}

	void AudioBuffer::forEachChannel(std::function<void(SampleType*)> callback)
	{
		for (auto channel = 0; channel < _numChannels; ++channel)
		{
			callback(getArrayOfWritePointers()[channel]);
		}
	}

	void AudioBuffer::forEachSample(std::function<void(SampleType&)> callback)
	{
		for (int channel = 0; channel < _numChannels; ++channel)
		{
			for (unsigned sample = 0; sample < _numSamples; ++sample)
			{
				callback(_channels[channel][sample]);
			}
		}
	}

	void AudioBuffer::freeAllChannels()
	{
		if (_ownsChannels && _channels)
		{
			for (int channel = 0; channel < _numChannels; ++channel)
			{
				delete[] _channels[channel];
			}
			delete[] _channels;
		}
	}
}
