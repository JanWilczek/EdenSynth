///
/// \autor Jan Wilczek
/// \date 02.12.2018
///
#include "FilterControlsComponent.h"

FilterControlsComponent::FilterControlsComponent(
    AudioProcessorValueTreeState& valueTreeState)
    : _cutoff(Slider::SliderStyle::Rotary,
              Slider::TextEntryBoxPosition::NoTextBox),
      _resonance(Slider::SliderStyle::Rotary,
                 Slider::TextEntryBoxPosition::NoTextBox),
      _contourAmount(Slider::SliderStyle::Rotary,
                     Slider::TextEntryBoxPosition::NoTextBox) {
  _cutoffLabel.setJustificationType(Justification::centred);
  addAndMakeVisible(_cutoffLabel);
  _cutoff.setPopupDisplayEnabled(true, false, this);
  addAndMakeVisible(_cutoff);
  _cutoffAttachment = std::make_unique<SliderAttachment>(
      valueTreeState, "filter.cutoff", _cutoff);

  _resonanceLabel.setJustificationType(Justification::centred);
  addAndMakeVisible(_resonanceLabel);
  _resonance.setPopupDisplayEnabled(true, false, this);
  addAndMakeVisible(_resonance);
  _resonanceAttachment = std::make_unique<SliderAttachment>(
      valueTreeState, "filter.resonance", _resonance);

  _contourAmountLabel.setJustificationType(Justification::centred);
  addAndMakeVisible(_contourAmountLabel);
  _contourAmount.setPopupDisplayEnabled(true, false, this);
  addAndMakeVisible(_contourAmount);
  _contourAmountAttachment = std::make_unique<SliderAttachment>(
      valueTreeState, "filter.contourAmount", _contourAmount);

  _passbandAttenuationLabel.setJustificationType(Justification::centred);
  addAndMakeVisible(_passbandAttenuationLabel);
  _passbandAttenuation.addItem("12 dB", 1);
  _passbandAttenuation.addItem("24 dB", 2);
  _passbandAttenuation.setSelectedId(1);
  addAndMakeVisible(_passbandAttenuation);
  _passbandAttenuationAttachment = std::make_unique<ComboBoxAttachment>(
      valueTreeState, "filter.passbandAttenuation", _passbandAttenuation);
}

void FilterControlsComponent::resized() {
  constexpr int labelHeight = 30;
  const int cellSize = getWidth() / 4;
  const int knobDim = getHeight() - labelHeight;
  const int knobOffset = cellSize / 2 - knobDim / 2;

  _cutoffLabel.setBounds(0, 0, cellSize, labelHeight);
  _cutoff.setBounds(knobOffset, labelHeight, knobDim, knobDim);

  _resonanceLabel.setBounds(_cutoffLabel.getX() + cellSize, _cutoffLabel.getY(),
                            cellSize, labelHeight);
  _resonance.setBounds(_cutoff.getX() + cellSize, _cutoff.getY(), knobDim,
                       knobDim);

  _contourAmountLabel.setBounds(_resonanceLabel.getX() + cellSize,
                                _resonanceLabel.getY(), cellSize, labelHeight);
  _contourAmount.setBounds(_resonance.getX() + cellSize, _resonance.getY(),
                           knobDim, knobDim);

  _passbandAttenuationLabel.setBounds(_contourAmountLabel.getX() + cellSize,
                                      _contourAmountLabel.getY(), cellSize,
                                      labelHeight);
  _passbandAttenuation.setBounds(_passbandAttenuationLabel.getX(), cellSize / 2,
                                 cellSize, labelHeight);
}
