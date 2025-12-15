///
/// \author Jan Wilczek
/// \date 06.11.2018
///
#include "WaveshaperComponent.h"

#include "ParameterIds.h"

#include "eden/WaveshapingFunctionGenerator.h"

WaveshaperComponent::WaveshaperComponent(
    AudioProcessorValueTreeState& vts,
    std::shared_ptr<eden_vst::WaveshapingTransferFunctionContainer>
        transferFunction)
    : _transferFunction(std::move(transferFunction)),
      _curveParameterAttachment{
          *vts.getParameter(eden::plugin::parameter_id::waveshaperCurve),
          _curve},
      _chebyshevPolynomialOrderParameter{
          dynamic_cast<juce::AudioParameterInt*>(vts.getParameter(
              eden::plugin::parameter_id::waveshaperChebyshevPolynomialOrder))},
      _spreadValueParameter{
          dynamic_cast<juce::AudioParameterFloat*>(vts.getParameter(
              eden::plugin::parameter_id::waveshaperSpreadValue))} {
  _canvas.OnTransferFunctionChanged =
      [this](std::vector<float> newTransferFunction) {
        _transferFunction->setTransferFunction(std::move(newTransferFunction));
      };
  jassert(_chebyshevPolynomialOrderParameter);
  jassert(_spreadValueParameter != nullptr);

  addAndMakeVisible(_canvas);

  addAndMakeVisible(_curveLabel);

  const auto* curveParameter = dynamic_cast<AudioParameterChoice*>(
      vts.getParameter(eden::plugin::parameter_id::waveshaperCurve));
  jassert(curveParameter != nullptr);

  _curve.addItemList(curveParameter->choices, 1);
  _curve.addListener(this);
  _curveParameterAttachment.sendInitialUpdate();
  addAndMakeVisible(_curve);

  addAndMakeVisible(_chebyshevPolynomialOrderLabel);
  _chebyshevPolynomialOrder.setSliderStyle(
      juce::Slider::SliderStyle::IncDecButtons);
  const auto polynomialOrderRange =
      _chebyshevPolynomialOrderParameter->getNormalisableRange();
  _chebyshevPolynomialOrder.setRange(polynomialOrderRange.start,
                                     polynomialOrderRange.end,
                                     polynomialOrderRange.interval);
  _chebyshevPolynomialOrder.setValue(_chebyshevPolynomialOrderParameter->get());
  _chebyshevPolynomialOrder.addListener(this);
  addAndMakeVisible(_chebyshevPolynomialOrder);

  _spreadLabel.setJustificationType(Justification::centred);
  addAndMakeVisible(_spreadLabel);
  const auto spreadRange = _spreadValueParameter->getNormalisableRange();
  _spread.setRange(spreadRange.start, spreadRange.end, spreadRange.interval);
  _spread.setPopupDisplayEnabled(true, false, this);
  _spread.setValue(_spreadValueParameter->get());
  _spread.addListener(this);
  addAndMakeVisible(_spread);

  constexpr auto controlRateHz = 60;
  startTimerHz(controlRateHz);
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

void WaveshaperComponent::sliderValueChanged(Slider* s) {
  if (s == &_chebyshevPolynomialOrder) {
    *_chebyshevPolynomialOrderParameter =
        static_cast<int>(_chebyshevPolynomialOrder.getValue());
  } else if (s == &_spread) {
    *_spreadValueParameter = static_cast<float>(_spread.getValue());
  }
  setTransferFunction();
}

void WaveshaperComponent::setTransferFunction() {
  const auto spreadValue = _spreadValueParameter->get();
  const auto chebyshevPolynomialOrder =
      static_cast<unsigned long>(_chebyshevPolynomialOrderParameter->get());

  jassert(0 < _curve.getSelectedId());
  jassert(_curve.getSelectedId() < static_cast<int>(AvailableCurves::Count));

  const auto curveName = static_cast<AvailableCurves>(_curve.getSelectedId());

  constexpr auto horizontalResolution = 280u;
  auto curve = generateCurve(curveName, horizontalResolution, spreadValue,
                             chebyshevPolynomialOrder);

  _canvas.setTransferFunction(curve);
}

std::vector<float> WaveshaperComponent::generateCurve(
    AvailableCurves curveName,
    unsigned length,
    float spread,
    unsigned long chebyshevPolynomialOrder) {
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
          static_cast<unsigned>(chebyshevPolynomialOrder), length);
      break;
    case AvailableCurves::Count:
    default:
      jassertfalse;
      break;
  }

  eden::WaveshapingFunctionGenerator::spreadValuesRandomly(curve, spread);

  return curve;
}

void WaveshaperComponent::timerCallback() {
  auto performUpdate = false;

  if (_chebyshevPolynomialOrderParameter->get() !=
      static_cast<int>(_chebyshevPolynomialOrder.getValue())) {
    _chebyshevPolynomialOrder.setValue(
        _chebyshevPolynomialOrderParameter->get(), juce::dontSendNotification);
    performUpdate = true;
  }

  if (!juce::approximatelyEqual(_spreadValueParameter->get(),
                                static_cast<float>(_spread.getValue()))) {
    _spread.setValue(_spreadValueParameter->get(), juce::dontSendNotification);
    performUpdate = true;
  }

  if (performUpdate) {
    setTransferFunction();
  }
}
