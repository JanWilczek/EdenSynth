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
	using SliderAttachment = AudioProcessorValueTreeState::SliderAttachment;
	using ComboBoxAttachment = AudioProcessorValueTreeState::ComboBoxAttachment;

	EnvelopeSegmentComponent(AudioProcessorValueTreeState& valueTreeState, String timeParameterID, String curveParameterID);

	void resized() override;

	//std::chrono::milliseconds getTime() const;
	//eden::EnvelopeSegmentCurve getCurve() const;

private:
	Slider _time;
	std::unique_ptr<SliderAttachment> _timeAttachment;

	ComboBox _curve;
	std::unique_ptr<ComboBoxAttachment> _curveAttachment;
};
