#include "PresetLoader.h"
#include "JuceHeader.h"

namespace eden_vst {
PresetLoader::PresetLoader(juce::AudioProcessorValueTreeState& vts,
                           const Presets& p)
    : _pluginParameters{vts}, _presets{p} {}

auto PresetLoader::operator()(const std::string& presetName)
    -> std::expected<LoadingResult, LoadingError> {
  return loadPreset(presetName);
}

auto PresetLoader::loadPreset(const std::string& presetName)
    -> std::expected<LoadingResult, LoadingError> {
  // check if the preset exists
  if (_presets.notContains(presetName)) {
    return std::unexpected{LoadingError::DoesNotExist};
  }

  // if yes, load from file
  const auto presetPath = _presets.absolutePathTo(presetName);
  const auto presetFile = File(presetPath.c_str());

  auto presetXML = std::make_unique<XmlDocument>(presetFile);
  auto presetXMLState = presetXML->getDocumentElementIfTagMatches(
      _pluginParameters.state.getType());

  // set the value tree state
  if (not presetXMLState) {
    AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon, "Error",
                                     "Failed to load the preset", "");
    return std::unexpected{LoadingError::WrongTag};
  }

  _pluginParameters.replaceState(ValueTree::fromXml(*presetXMLState));
  return LoadingResult::Ok;
}
}  // namespace eden_vst
