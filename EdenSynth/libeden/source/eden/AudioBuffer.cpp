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
		, _ownsChannels(false)
	{
		_channels = static_cast<SampleType**>(_preallocatedChannelSpace);
		for (int channel = 0; channel < _numChannels ; ++channel)
		{
			_channels[channel] = dataToUse[channel];
		}
	}

	AudioBuffer::~AudioBuffer()
	{
		if (_ownsChannels)
		{
			for (int channel = 0; channel < _numChannels; ++channel)
			{
				delete _channels[channel];
			}
		}
	}

	void AudioBuffer::clear()
	{
		for (int channel = 0; channel < _numChannels; ++channel)
		{
			for (unsigned sample = 0; sample < _numSamples; ++sample)
			{
				_channels[channel][sample] = 0;
			}
		}
	}

	int AudioBuffer::getNumChannels() const noexcept
	{
		return _numChannels;
	}

	unsigned AudioBuffer::getNumSamples() const noexcept
	{
		return _numSamples;
	}

	void AudioBuffer::addSample(int destChannel, int destSample, SampleType valueToAdd)
	{
		_channels[destChannel][destSample] += valueToAdd;
	}
}