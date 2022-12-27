///
/// \author Jan Wilczek
/// \date 06.11.2018
///
#include "WaveshaperComponent.h"
#include <string>
#include "eden/WaveshapingFunctionGenerator.h"

WaveshaperComponent::WaveshaperComponent(
    AudioProcessorValueTreeState&,
    std::shared_ptr<eden_vst::WaveshapingTransferFunctionContainer>
        transferFunction)
    : _transferFunction(std::move(transferFunction)) {
  _canvas.OnTransferFunctionChanged =
      [this](std::vector<float> newTransferFunction) {
        _transferFunction->setTransferFunction(newTransferFunction);
      };

  addAndMakeVisible(_canvas);

  addAndMakeVisible(_curveLabel);

  _curve.addItem("Identity", static_cast<int>(AvailableCurves::Identity));
  _curve.addItem("Hyperbolic tangent",
                 static_cast<int>(AvailableCurves::HyperbolicTangent));
  _curve.addItem("Chebyshev polynomial",
                 static_cast<int>(AvailableCurves::ChebyshevPolynomial));
  _curve.setSelectedId(static_cast<int>(AvailableCurves::Identity));
  _curve.addListener(this);
  addAndMakeVisible(_curve);

  addAndMakeVisible(_chebyshevPolynomialOrderLabel);
  _chebyshevPolynomialOrder.addListener(this);
  _chebyshevPolynomialOrder.setJustificationType(Justification::right);
  _chebyshevPolynomialOrder.setEditable(true);
  addAndMakeVisible(_chebyshevPolynomialOrder);
  _chebyshevPolynomialOrder.setEnabled(false);

  _spreadLabel.setJustificationType(Justification::centred);
  addAndMakeVisible(_spreadLabel);
  _spread.setRange(0.0, 1.0, 0.0001);
  _spread.setPopupDisplayEnabled(true, false, this);
  _spread.addListener(this);
  addAndMakeVisible(_spread);
}

void WaveshaperComponent::paint(Graphics& g) {
  g.setColour(
      getLookAndFeel().findColour(Slider::ColourIds::textBoxOutlineColourId));
  g.drawRect(0, 0, getWidth(), getHeight(), 5);
}

void WaveshaperComponent::resized() {
  const auto canvasDim = getHeight() - 20;
  _canvas.setBounds(10, 10, canvasDim, canvasDim);

  const auto rightColumnWidth = getWidth() - canvasDim - 25;
  constexpr auto labelHeight = 20;
  _curveLabel.setBounds(_canvas.getX() + canvasDim + 5, _canvas.getY(),
                        rightColumnWidth, labelHeight);
  _curve.setBounds(_curveLabel.getX(), _curveLabel.getY() + labelHeight,
                   rightColumnWidth, labelHeight);

  constexpr int orderLabelWidth = 60;
  _chebyshevPolynomialOrderLabel.setBounds(
      _curve.getX(), _curve.getY() + labelHeight, orderLabelWidth, labelHeight);
  _chebyshevPolynomialOrder.setBounds(
      _chebyshevPolynomialOrderLabel.getX() + orderLabelWidth,
      _chebyshevPolynomialOrderLabel.getY(), rightColumnWidth - orderLabelWidth,
      labelHeight);

  _spreadLabel.setBounds(_chebyshevPolynomialOrder.getX(),
                         _chebyshevPolynomialOrder.getY() + labelHeight,
                         _chebyshevPolynomialOrder.getWidth(), labelHeight);
  _spread.setBounds(_spreadLabel.getX(), _spreadLabel.getY() + labelHeight,
                    _spreadLabel.getWidth(),
                    getHeight() - _spreadLabel.getY() - labelHeight - 10);
}

void WaveshaperComponent::comboBoxChanged(ComboBox* comboBoxThatHasChanged) {
  const auto curveName =
      static_cast<AvailableCurves>(comboBoxThatHasChanged->getSelectedId());

  if (curveName == AvailableCurves::ChebyshevPolynomial) {
    _chebyshevPolynomialOrderLabel.setEnabled(true);
    _chebyshevPolynomialOrder.setEnabled(true);
  } else {
    _chebyshevPolynomialOrderLabel.setEnabled(false);
    _chebyshevPolynomialOrder.setEnabled(false);
  }

  setTransferFunction();
}

void WaveshaperComponent::labelTextChanged(Label* labelThatHasChanged) {
  if (const auto curve = static_cast<AvailableCurves>(_curve.getSelectedId());
      curve == AvailableCurves::ChebyshevPolynomial) {
    unsigned order = 2u;
    try {
      order = std::stoul(labelThatHasChanged->getText().toStdString());
    } catch (...) {
      labelThatHasChanged->setText(std::to_string(order), dontSendNotification);
    }

    setTransferFunction();
  }
}

void WaveshaperComponent::sliderValueChanged(Slider*) {
  setTransferFunction();
}

void WaveshaperComponent::setTransferFunction() {
  const auto spreadValue = static_cast<float>(_spread.getValue());
  const auto chebyshevPolynomialOrder =
      std::stoul(_chebyshevPolynomialOrder.getText().toStdString());
  const auto curveName = static_cast<AvailableCurves>(_curve.getSelectedId());

  auto curve = generateCurve(curveName, _canvas.getWidth(), spreadValue,
                             chebyshevPolynomialOrder);

  _canvas.setTransferFunction(curve);
}

std::vector<float> WaveshaperComponent::generateCurve(
    AvailableCurves curveName,
    unsigned length,
    float spread,
    unsigned chebyshevPolynomialOrder) {
  std::vector<float> curve;

  switch (curveName) {
    case AvailableCurves::Identity:
      curve = eden::WaveshapingFunctionGenerator::generateIdentity(length);
      break;
    case AvailableCurves::HyperbolicTangent:
      curve = eden::WaveshapingFunctionGenerator::generateTransferFunction(
          [](float x) { return std::tanh(2 * x); }, length);
      break;
    case AvailableCurves::ChebyshevPolynomial:
      curve = eden::WaveshapingFunctionGenerator::generateChebyshevPolynomial(
          chebyshevPolynomialOrder, length);
      break;
    default:
      break;
  }

  eden::WaveshapingFunctionGenerator::spreadValuesRandomly(curve, spread);

  return curve;
}
