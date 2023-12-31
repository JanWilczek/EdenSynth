#include "PresetSaver.h"
#include "FileHelper.h"
#include "JuceHeader.h"

namespace eden_vst {
PresetSaver::PresetSaver(juce::AudioProcessorValueTreeState& vts)
    : _pluginParameters{vts} {}

PresetSavingResult PresetSaver::saveOrOverwriteCurrentPreset(
    const std::filesystem::path& outputPath) {
  auto state = _pluginParameters.copyState();
  const auto presetXML(state.createXml());

  if (not presetXML) {
    return std::unexpected{PresetSavingError::FailedToCreatePresetFile};
  }

  const auto presetFile = File(outputPath.c_str());
  if (not presetXML->writeTo(presetFile)) {
    return std::unexpected{PresetSavingError::FailedToWritePresetFile};
  }

  return PresetSavingSuccess::Ok;
}
}  // namespace eden_vst
