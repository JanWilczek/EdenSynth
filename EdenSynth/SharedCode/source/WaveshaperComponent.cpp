/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "WaveshaperComponent.h"
#include "eden/WaveshapingFunctionGenerator.h"

WaveshaperComponent::WaveshaperComponent(AudioProcessorValueTreeState&, std::shared_ptr<eden_vst::WaveshapingTransferFunctionContainer> transferFunction)
	: _transferFunction(std::move(transferFunction))
{
	_canvas.OnTransferFunctionChanged = [this](std::vector<float> newTransferFunction)
	{
		_transferFunction->setTransferFunction(newTransferFunction);
	};

	addAndMakeVisible(_canvas);

	addAndMakeVisible(_curveLabel);

	_curve.addItem("Identity", static_cast<int>(AvailableCurves::Identity));
	_curve.addItem("Hyperbolic tangent", static_cast<int>(AvailableCurves::HyperbolicTangent));
	_curve.addItem("Chebyshev polynomial", static_cast<int>(AvailableCurves::ChebyshevPolynomial));
	_curve.addItem("Custom", static_cast<int>(AvailableCurves::Custom));
	_curve.setSelectedId(static_cast<int>(AvailableCurves::Identity));
	_curve.addListener(this);
	addAndMakeVisible(_curve);

	addAndMakeVisible(_chebyshevPolynomialOrderLabel);
	addAndMakeVisible(_chebyshevPolynomialOrder);
}

void WaveshaperComponent::paint(Graphics& g)
{
	g.setColour(getLookAndFeel().findColour(Slider::ColourIds::textBoxOutlineColourId));
	g.drawRect(0,0,getWidth(),getHeight(), 5);
}

void WaveshaperComponent::resized()
{
	const auto canvasDim = getHeight() - 20;
	_canvas.setBounds(10, 10, canvasDim, canvasDim);

	const auto rightColumnWidth = getWidth() - canvasDim - 25;
	constexpr auto labelHeight = 20;
	_curveLabel.setBounds(_canvas.getX() + canvasDim + 5, _canvas.getY(), rightColumnWidth, labelHeight);
	_curve.setBounds(_curveLabel.getX(), _curveLabel.getY() + labelHeight, rightColumnWidth, labelHeight);

	constexpr int orderLabelWidth = 60;
	_chebyshevPolynomialOrderLabel.setBounds(_curve.getX(), _curve.getY() + labelHeight, orderLabelWidth, labelHeight);
	_chebyshevPolynomialOrder.setBounds(_chebyshevPolynomialOrderLabel.getX() + orderLabelWidth, _chebyshevPolynomialOrderLabel.getY(), rightColumnWidth - orderLabelWidth, labelHeight);

}

void WaveshaperComponent::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	const auto curve = static_cast<AvailableCurves>(comboBoxThatHasChanged->getSelectedId());

	switch(curve)
	{
	case AvailableCurves::Identity:
		_canvas.setTransferFunction(eden::WaveshapingFunctionGenerator::generateIdentity(_canvas.getWidth()));
		break;
	case AvailableCurves::HyperbolicTangent:
		_canvas.setTransferFunction(eden::WaveshapingFunctionGenerator::generateTransferFunction([](float x) { return std::tanh(2 * x); }, _canvas.getWidth()));
		break;
	case AvailableCurves::ChebyshevPolynomial:
		_canvas.setTransferFunction(eden::WaveshapingFunctionGenerator::generateChebyshevPolynomial(2u, _canvas.getWidth()));
		break;
	case AvailableCurves::Custom:
	default:
		break;
	}
}
