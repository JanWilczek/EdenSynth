/// 
/// \author Jan Wilczek
/// \date 02.09.2018
/// 
#include "eden/AudioBuffer.h"

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

	AudioBuffer::~AudioBuffer()
	{
		if (_ownsChannels)
		{
			for (int channel = 0; channel < _numChannels; ++channel)
			{
				delete[] _channels[channel];
			}
			delete[] _channels;
		}
	}

	AudioBuffer::SampleType** AudioBuffer::getArrayOfWritePointers() const noexcept
	{
		return _channels;
	}

	const AudioBuffer::SampleType** AudioBuffer::getArrayOfReadPointers() const noexcept
	{
		return const_cast<const AudioBuffer::SampleType**>(_channels);
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
		for (int channel = 0; channel < _numChannels; ++channel)
		{
			for (unsigned sample = 0; sample < _numSamples; ++sample)
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

}