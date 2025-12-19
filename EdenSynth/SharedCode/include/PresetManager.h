#pragma once
#include <string>
#include <vector>
#include "PresetLoadingResult.h"
#include "PresetSavingResult.h"
#include "../source/presets/Presets.h"

namespace eden_vst {
class PresetManager {
public:
  virtual ~PresetManager();
  [[nodiscard]] virtual PresetSavingResult saveCurrentPreset(
      const std::string& name) = 0;
  [[nodiscard]] virtual PresetSavingResult saveOrOverwriteCurrentPreset(
      const std::string& name) = 0;
  [[nodiscard]] virtual PresetLoadingResult loadPreset(
      const eden::plugin::Preset& preset) = 0;
  [[nodiscard]] virtual const eden::plugin::Presets::Container& presets() = 0;
};

inline PresetManager::~PresetManager() {}
}  // namespace eden_vst
