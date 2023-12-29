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

PresetSavingResult PresetSaver::saveCurrentPreset(const std::string& name) {
  const auto presetOutputPath = presetPathFrom(name);

  if (presetOutputPath.empty() or not presetOutputPath.has_filename()) {
    return std::unexpected{PresetSavingError::InvalidPresetName};
  }

  if (std::filesystem::exists(presetOutputPath)) {
    return std::unexpected{PresetSavingError::PresetWithNameExists};
  }

  auto state = _pluginParameters.copyState();
  const auto presetXML(state.createXml());

  if (not presetXML) {
    return std::unexpected{PresetSavingError::FailedToCreatePresetFile};
  }

  const auto presetFile = File(presetOutputPath.c_str());
  if (not presetXML->writeTo(presetFile)) {
    return std::unexpected{PresetSavingError::FailedToWritePresetFile};
  }

  return PresetSavingSuccess::Ok;
}

PresetSavingResult PresetSaver::saveOrOverwriteCurrentPreset(
    const std::string& name) {
  const auto presetOutputPath = presetPathFrom(name);

  if (presetOutputPath.empty() or not presetOutputPath.has_filename()) {
    return std::unexpected{PresetSavingError::InvalidPresetName};
  }

  auto state = _pluginParameters.copyState();
  const auto presetXML(state.createXml());

  if (not presetXML) {
    return std::unexpected{PresetSavingError::FailedToCreatePresetFile};
  }

  const auto presetFile = File(presetOutputPath.c_str());
  if (not presetXML->writeTo(presetFile)) {
    return std::unexpected{PresetSavingError::FailedToWritePresetFile};
  }

  return PresetSavingSuccess::Ok;
}
}  // namespace eden_vst
