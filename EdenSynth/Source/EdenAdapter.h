#pragma once
/// 
/// \author Jan Wilczek
/// \date 02.09.2018
/// 
#include "eden/AudioBuffer.h"
#include "../JuceLibraryCode/JuceHeader.h"

namespace eden 
{
	class MidiBuffer;
}

namespace eden_vst
{
	/// <summary>
	/// Static class containing utilities connecting JUCE with Eden.
	/// </summary>
	class EdenAdapter
	{
	public:
		/// <summary>
		/// Converts JUCE audio buffer to Eden audio buffer.
		/// </summary>
		/// <param name="juceAudioBuffer">buffer to convert from</param>
		/// <param name="edenBuffer">buffer to convert to</param>
		static void convertToEdenBuffer(juce::AudioBuffer<eden::AudioBuffer::SampleType>& juceAudioBuffer,
			eden::AudioBuffer& edenBuffer);

		/// <summary>
		/// Converts JUCE MIDI buffer to Eden MIDI buffer.
		/// </summary>
		/// <param name="juceMidiBuffer">buffer to get messages from</param>
		/// <param name="edenMidiBuffer">buffer to pass messages to</param>
		static void convertToEdenMidi(const juce::MidiBuffer& juceMidiBuffer, eden::MidiBuffer& edenMidiBuffer);
	};
}
