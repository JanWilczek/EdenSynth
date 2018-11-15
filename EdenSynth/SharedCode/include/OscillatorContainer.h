#pragma once
/// 
/// \author Jan Wilczek
/// \date 15.11.2018
/// 
#include <map>
#include "WaveTablePathProvider.h"
#include "eden/Oscillator.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "eden/EdenSynthesiser.h"

namespace eden_vst
{
	class OscillatorContainer
	{
	public:
		OscillatorContainer(eden::EdenSynthesiser& synthesiser, WaveTablePathProvider pathProvider, unsigned numOscillators);

		void addOscillatorParameters(AudioProcessorValueTreeState& pluginParameters);
		void updateOscillatorParameters(const AudioProcessorValueTreeState& pluginParameters);
		WaveTablePathProvider& getPathProvider();

	private:
		eden::EdenSynthesiser& _synthesiser;
		WaveTablePathProvider _pathProvider;
		std::map<std::string, std::unique_ptr<eden::Oscillator>> _oscillators;
		std::map<std::string, size_t> _waveTableIndices;
	};
}
