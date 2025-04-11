#pragma once
#include <string>
#include <vector>
#include "PresetLoadingResult.h"
#include "PresetSavingResult.h"

namespace eden_vst {
class PresetManager {
public:
  virtual ~PresetManager();
  [[nodiscard]] virtual PresetSavingResult saveCurrentPreset(
      const std::string& name) = 0;
  [[nodiscard]] virtual PresetSavingResult saveOrOverwriteCurrentPreset(
      const std::string& name) = 0;
  [[nodiscard]] virtual PresetLoadingResult loadPreset(
      const std::string& presetName) = 0;
  [[nodiscard]] virtual std::vector<std::string> presets() const = 0;
};

inline PresetManager::~PresetManager() {}
}  // namespace eden_vst
