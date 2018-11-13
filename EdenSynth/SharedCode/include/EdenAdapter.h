#pragma once
/// 
/// \author Jan Wilczek
/// \date 02.09.2018
/// 
#include "../JuceLibraryCode/JuceHeader.h"

namespace eden 
{
	class EdenSynthesiser;
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
		EdenAdapter(eden::EdenSynthesiser& synthesiser);

		/// <summary>
		/// Converts JUCE MIDI buffer to Eden MIDI buffer.
		/// </summary>
		/// <param name="juceMidiBuffer">buffer to get messages from</param>
		/// <param name="edenMidiBuffer">buffer to pass messages to</param>
		static eden::MidiBuffer convertToEdenMidi(const juce::MidiBuffer& juceMidiBuffer);

		void addEdenParameters(AudioProcessorValueTreeState& pluginParameters);
		void updateEdenParameters(const AudioProcessorValueTreeState& pluginParameters);

	private:
		eden::EdenSynthesiser& _synthesiser;
	};
}
