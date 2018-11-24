#pragma once
/// 
/// \author Jan Wilczek
/// \date 14.11.2018
/// 
#include "../JuceLibraryCode/JuceHeader.h"
#include "OscillatorComponent.h"

namespace eden_vst
{
	class WaveTablePathProvider;
}

class OscillatorsComponent : public Component
{
public:
	OscillatorsComponent(AudioProcessorValueTreeState& valueTreeState, const eden_vst::WaveTablePathProvider& pathProvider);

	void paint(Graphics& g) override;
	void resized() override;

private:
	OscillatorComponent _oscillator1;
	OscillatorComponent _oscillator2;
	OscillatorComponent _oscillator3;
};
