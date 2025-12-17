#pragma once
#include <string>
#include <functional>
#include "Presets.h"
#include "PresetManager.h"
#include <JuceHeader.h>

namespace juce {
class AudioProcessorValueTreeState;
}

namespace eden_vst {
class ProductionPresetManager : public PresetManager {
public:
  using GetSerializedState = std::function<juce::MemoryBlock()>;
  using SetSerializedState = std::function<void(const juce::MemoryBlock&)>;

  struct Args {
    /// <summary>
    /// Path to the folder with system-wide presets. This path is treated
    /// as read-only.
    /// </summary>
    std::filesystem::path systemPresetsPath;

    /// <summary>
    /// Path to the folder with user presets. This is the path where new
    /// presets will be saved.
    /// </summary>
    std::filesystem::path userPresetsPath;

    /// <summary>
    /// Retrieves the current state of the plugin as a binary block.
    /// The block may contain XML, JSON, or something else.
    /// </summary>
    GetSerializedState getSerializedState;

    /// <summary>
    /// Sets the passed-in state on the plugin. The passed-in block is
    /// one of the blocks that were earlier retrieved with getSerializedState.
    /// </summary>
    SetSerializedState setSerializedState;
  };

  explicit ProductionPresetManager(Args&&);

  [[nodiscard]] PresetSavingResult saveCurrentPreset(
      const std::string& name) override;
  [[nodiscard]] PresetSavingResult saveOrOverwriteCurrentPreset(
      const std::string& name) override;
  [[nodiscard]] PresetLoadingResult loadPreset(
      const std::string& presetName) override;
  [[nodiscard]] std::vector<std::string> presets() const override;

private:
  Presets _presets;
  GetSerializedState _getSerializedState;
  SetSerializedState _setSerializedState;
};
}  // namespace eden_vst
