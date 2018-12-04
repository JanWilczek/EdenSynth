/// 
/// \author Jan Wilczek
/// \date 03.12.2018
/// 
#include "WaveshapingCanvas.h"
#include "eden/WaveshapingFunctionGenerator.h"

void WaveshapingCanvas::resized()
{
	_points = eden::WaveshapingFunctionGenerator::generateIdentity(getWidth());
}

void WaveshapingCanvas::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId).brighter(0.1f));

	drawPlotAxes(g);

	g.setColour(Colours::lime);
	g.drawRect(0,0,getWidth(),getHeight(), 1);

	drawTransferFunction(g);
}

void WaveshapingCanvas::setTransferFunction(std::vector<float> transferFunction)
{
	_points = std::move(transferFunction);
	OnTransferFunctionChanged(_points);
	repaint();
}

void WaveshapingCanvas::drawPlotAxes(Graphics& g)
{
	g.setColour(Colours::lightgrey);
	g.drawHorizontalLine(getHeight() / 2, 0.f, static_cast<float>(getWidth()));
	g.drawVerticalLine(getWidth() / 2, 0.f, static_cast<float>(getHeight()));

	Line<float> leftVerticalPlotLine(getWidth() / 4.f, 0.f, getWidth() / 4.f, static_cast<float>(getHeight()));
	constexpr float dashes[] = { 7.f, 5.f };
	g.drawDashedLine(leftVerticalPlotLine, dashes, 2, 0.5f);
	leftVerticalPlotLine.applyTransform(AffineTransform(1.f, 0.f, 0.f, 0.f, 1.f, 0.f).translated(getWidth() / 2.f, 0.f));
	g.drawDashedLine(leftVerticalPlotLine, dashes, 2, 0.5f);

	Line<float> upperHorizontalPlotLine(0.f, getHeight() / 4.f, static_cast<float>(getWidth()), getHeight() / 4.f);
	g.drawDashedLine(upperHorizontalPlotLine, dashes, 2, 0.5f);
	upperHorizontalPlotLine.applyTransform(AffineTransform(1.f, 0.f, 0.f, 0.f, 1.f, 0.f).translated(0.f, getHeight() / 2.f));
	g.drawDashedLine(upperHorizontalPlotLine, dashes, 2, 0.5f);
}

void WaveshapingCanvas::drawTransferFunction(Graphics& g)
{
	const auto scale = getHeight() / 2;

	Path path;
	path.startNewSubPath(0.f, getHeight() - (scale + scale * _points[0]));

	const auto nb_points = static_cast<int>(_points.size());
	for (auto i = 1; i < nb_points; ++i)
	{
		path.lineTo(static_cast<float>(i), getHeight() - (scale + scale * _points[i]));
	}

	g.setColour(Colours::aqua);
	g.strokePath(path, PathStrokeType(2.0f));
}
