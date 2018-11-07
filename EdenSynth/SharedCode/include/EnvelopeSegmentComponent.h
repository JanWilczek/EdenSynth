#pragma once
/// 
/// \author Jan Wilczek
/// \date 07.11.2018
/// 
#include <chrono>
#include <../JuceLibraryCode/JuceHeader.h>
#include <eden/EnvelopeParameters.h>

class EnvelopeComponent;

class EnvelopeSegmentComponent : public Component
{
public:
	EnvelopeSegmentComponent(EnvelopeComponent& parent, NormalisableRange<double> timeRangeMs);

	void resized() override;

	std::chrono::milliseconds getTime() const;
	eden::EnvelopeSegmentCurve getCurve() const;

private:
	Slider _time;
	ComboBox _curve;
	int _comboBoxOffset = 1;
};
