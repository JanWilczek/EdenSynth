#include "PresetLoader.h"
#include "JuceHeader.h"

namespace eden_vst {
PresetLoader::PresetLoader(juce::AudioProcessorValueTreeState& vts, Presets p)
    : _pluginParameters{vts}, _presets{std::move(p)} {}

void PresetLoader::operator()(const std::string& presetName) {
  loadPreset(presetName);
}

void PresetLoader::loadPreset(const std::string& presetName) {
  // check if the preset exists
  if (_presets.notContains(presetName)) {
    return;
  }

  // if yes, load from file
  const auto presetPath = _presets.absolutePathTo(presetName);
  const auto presetFile = File(presetPath.c_str());

  auto presetXML = std::make_unique<XmlDocument>(presetFile);
  auto presetXMLState = presetXML->getDocumentElementIfTagMatches(
      _pluginParameters.state.getType());

  // set the value tree state
  if (presetXMLState) {
    _pluginParameters.replaceState(ValueTree::fromXml(*presetXMLState));
  } else {
    AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon, "Error",
                                     "Failed to load the preset", "");
  }
}
}  // namespace eden_vst
