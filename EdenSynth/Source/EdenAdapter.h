#pragma once

#include "eden/AudioBuffer.h"
#include "../JuceLibraryCode/JuceHeader.h"

namespace eden_vst
{
	class EdenAdapter
	{
		static void convertToEdenBuffer(juce::AudioBuffer<eden::AudioBuffer::SampleType>& juceAudioBuffer,
			eden::AudioBuffer& edenBuffer);
	};
}
