#include "PresetSaver.h"
#include "FileHelper.h"
#include "JuceHeader.h"

namespace eden_vst {
namespace {
std::filesystem::path presetPathFrom(const std::string& presetName) {
  return (FileHelper::presetsPath() / presetName).replace_extension("xml");
}
}  // namespace

PresetSaver::PresetSaver(juce::AudioProcessorValueTreeState& vts)
    : _pluginParameters{vts} {}

PresetSavingResult PresetSaver::saveCurrentPreset(
    std::function<void(const std::string&)> onPresetAdded,
    const std::string& name) {
  auto state = _pluginParameters.copyState();
  const auto presetXML(state.createXml());

  if (not presetXML) {
    return std::unexpected{PresetSavingError::FailedToCreatePresetFile};
  }

  const auto presetOutputPath = presetPathFrom(name);

  if (std::filesystem::exists(presetOutputPath)) {
    return std::unexpected{PresetSavingError::PresetWithNameExists};
  }

  if (presetOutputPath.empty() or not presetOutputPath.has_filename()) {
    return std::unexpected{PresetSavingError::InvalidPresetName};
  }

  const auto presetFile = File(presetOutputPath.c_str());
  if (not presetXML->writeTo(presetFile)) {
    // AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon,
    // "Error", "Failed to save the preset file.");
    return std::unexpected{PresetSavingError::FailedToWritePresetFile};
  }

  onPresetAdded(presetOutputPath.stem().string());
  return PresetSavingSuccess::Ok;
}
}  // namespace eden_vst
