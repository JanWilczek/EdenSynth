#pragma once
#include <string>
#include <vector>
#include "PresetLoadingResult.h"
#include "PresetSavingResult.h"

namespace eden_vst {
class PresetManager {
public:
  virtual ~PresetManager();
  virtual [[nodiscard]] PresetSavingResult saveCurrentPreset(
      const std::string& name) = 0;
  virtual [[nodiscard]] PresetSavingResult saveOrOverwriteCurrentPreset(
      const std::string& name) = 0;
  virtual [[nodiscard]] PresetLoadingResult loadPreset(
      const std::string& presetName) = 0;
  virtual [[nodiscard]] std::vector<std::string> presets() const = 0;
};

inline PresetManager::~PresetManager() {}
}  // namespace eden_vst
