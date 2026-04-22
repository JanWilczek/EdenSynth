#pragma once
///
/// \author Jan Wilczek
/// \date 06.11.2018
///
#include <JuceHeader.h>
#include "WaveshapingCanvas.h"
#include "WaveshapingTransferFunctionContainer.h"
#include "parameters/ParameterRefs.h"

class WaveshaperComponent : public Component,
                            public ComboBox::Listener,
                            public Slider::Listener,
                            private juce::Timer {
public:
  WaveshaperComponent(
      const eden::plugin::ParameterRefs& parameters,
      std::shared_ptr<eden::plugin::WaveshapingTransferFunctionContainer>
          transferFunction);

  void paint(Graphics& g) override;
  void resized() override;

  void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
  void sliderValueChanged(Slider* slider) override;

private:
  enum class AvailableCurves {
    Identity = 1,
    HyperbolicTangent,
    ChebyshevPolynomial,
    Count,
  };

  void setTransferFunction();
  static std::vector<float> generateCurve(
      AvailableCurves curveName,
      unsigned length,
      float spread,
      unsigned long chebyshevPolynomialOrder);

  void timerCallback() override;

  std::shared_ptr<eden::plugin::WaveshapingTransferFunctionContainer>
      _transferFunction;

  WaveshapingCanvas _canvas;

  Label _curveLabel{"transferFunctionLabel", "Transfer function"};
  ComboBox _curve{"transferFunctionCurveComboBox"};
  ComboBoxParameterAttachment _curveParameterAttachment;

  Label _chebyshevPolynomialOrderLabel{"orderLabel", "Degree: "};
  juce::Slider _chebyshevPolynomialOrder;
  juce::AudioParameterInt* _chebyshevPolynomialOrderParameter;

  Label _spreadLabel{"spreadlabel", "Spread"};
  Slider _spread{Slider::SliderStyle::LinearVertical,
                 Slider::TextEntryBoxPosition::NoTextBox};
  juce::AudioParameterFloat* _spreadValueParameter;
};
