#pragma once
/// 
/// \author Jan Wilczek
/// \date 03.12.2018
/// 
#include "../JuceLibraryCode/JuceHeader.h"

class WaveshapingCanvas : public Component
{
public:
	void paint(Graphics& g) override;
	void resized() override;

private:
	void drawPlotAxes(Graphics& g);
	void drawTransferFunction(Graphics& g);

	std::vector<float> _points;
};
