#include "EdenAdapter.h"

namespace eden_vst
{
	void EdenAdapter::convertToEdenBuffer(juce::AudioBuffer<eden::AudioBuffer::SampleType>& juceAudioBuffer, eden::AudioBuffer& edenBuffer)
	{
		/*
		1. Check for number of channels
		2. If they are equal copy the content and set length
		3. If juce's bigger 
		 */
	}

}