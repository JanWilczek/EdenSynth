#include "PresetLoader.h"
#include "JuceHeader.h"

namespace eden_vst {
PresetLoader::PresetLoader(juce::AudioProcessorValueTreeState& vts,
                           const Presets& p)
    : _pluginParameters{vts}, _presets{p} {}

auto PresetLoader::operator()(const std::string& presetName)
    -> PresetLoadingResult {
  return loadPreset(presetName);
}

auto PresetLoader::loadPreset(const std::string& presetName)
    -> PresetLoadingResult {
  // check if the preset exists
  if (_presets.notContains(presetName)) {
    return std::unexpected{PresetLoadingError::DoesNotExist};
  }

  // if yes, load from file
  const auto presetPath = _presets.absolutePathTo(presetName);
  const auto presetFile = File(presetPath.c_str());

  auto presetXML = std::make_unique<XmlDocument>(presetFile);
  auto presetXMLState = presetXML->getDocumentElementIfTagMatches(
      _pluginParameters.state.getType());

  // set the value tree state
  if (not presetXMLState) {
    return std::unexpected{PresetLoadingError::WrongTag};
  }

  _pluginParameters.replaceState(ValueTree::fromXml(*presetXMLState));
  return PresetLoadingSuccess::Ok;
}
}  // namespace eden_vst
