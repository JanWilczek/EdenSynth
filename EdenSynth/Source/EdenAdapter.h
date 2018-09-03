#pragma once

#include "eden/AudioBuffer.h"
#include "../JuceLibraryCode/JuceHeader.h"

namespace eden 
{
	class MidiBuffer;
}

namespace eden_vst
{
	class EdenAdapter
	{
	public:
		static void convertToEdenBuffer(juce::AudioBuffer<eden::AudioBuffer::SampleType>& juceAudioBuffer,
			eden::AudioBuffer& edenBuffer);

		static void convertToEdenMidi(const juce::MidiBuffer& juceMidiBuffer, eden::MidiBuffer& edenMidiBuffer);
	};
}
