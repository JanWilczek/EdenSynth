#pragma once
/// 
/// \author Jan Wilczek
/// \date 03.12.2018
/// 
#include "../JuceLibraryCode/JuceHeader.h"

class WaveshapingCanvas : public Component
{
public:
	std::function<void(std::vector<float>)> OnTransferFunctionChanged = [](std::vector<float>) {};

	void paint(Graphics& g) override;
	void resized() override;

	void setTransferFunction(std::vector<float> transferFunction);

private:
	void drawPlotAxes(Graphics& g);
	void drawTransferFunction(Graphics& g);

	std::vector<float> _points;
};
