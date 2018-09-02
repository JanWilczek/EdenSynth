#include "eden/AudioBuffer.h"

namespace eden
{
	AudioBuffer::AudioBuffer(unsigned numChannels, unsigned numSamples)
		: _numChannels(numChannels)
		, _numSamples(numSamples)
		, _channels(new SampleType*[_numChannels])
	{
		for (int channel = 0; channel < _numChannels; ++channel)
		{
			_channels[channel] = new SampleType[_numSamples];
		}
	}

	AudioBuffer::AudioBuffer(SampleType** dataToUse, unsigned numChannelsToUse, unsigned numSamplesToUse)
		: _numChannels(numChannelsToUse)
		, _numSamples(numSamplesToUse)
	{
		_channels = static_cast<SampleType**>(_preallocatedChannelSpace);
		for (int channel = 0; channel < _numChannels ; ++channel)
		{
			_channels[channel] = dataToUse[channel];
		}
	}

	AudioBuffer::~AudioBuffer()
	{
		for (int channel = 0; channel < _numChannels; ++channel)
		{
			delete _channels[channel];
		}
	}

	void AudioBuffer::clear()
	{
		for (int channel = 0; channel < _numChannels; ++channel)
		{
			for (int sample = 0; sample < _numSamples; ++sample)
			{
				_channels[channel][sample] = 0;
			}
		}
	}

	unsigned AudioBuffer::getNumChannels() const noexcept
	{
		return _numChannels;
	}

	unsigned AudioBuffer::getNumSamples() const noexcept
	{
		return _numSamples;
	}
}