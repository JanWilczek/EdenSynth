#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveshapingCanvas.h"
#include "WaveshapingTransferFunctionContainer.h"

class WaveshaperComponent : public Component, public ComboBox::Listener, public Label::Listener
{
public:
	enum class AvailableCurves
	{
		Identity = 1,
		HyperbolicTangent = 2,
		ChebyshevPolynomial = 3,
		Custom = 4
	};

	WaveshaperComponent(AudioProcessorValueTreeState& valueTreeState, std::shared_ptr<eden_vst::WaveshapingTransferFunctionContainer> transferFunction);

	void paint(Graphics& g) override;
	void resized() override;

	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
	void labelTextChanged(Label* labelThatHasChanged) override;

private:
	std::shared_ptr<eden_vst::WaveshapingTransferFunctionContainer> _transferFunction;

	WaveshapingCanvas _canvas;

	Label _curveLabel {"transferFunctionLabel", "Transfer function"};
	ComboBox _curve{"transferFunctionCurveComboBox"};

	Label _chebyshevPolynomialOrderLabel{"orderLabel", "Order: "};
	Label _chebyshevPolynomialOrder{"order","2"};
};
