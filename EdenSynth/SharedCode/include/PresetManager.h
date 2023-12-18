#pragma once
#include <string>
#include <functional>
#include "PresetLoadingResult.h"

namespace eden_vst {
class PresetManager {
public:
  virtual ~PresetManager();
  virtual void saveCurrentPreset(
      std::function<void(const std::string&)> onPresetAdded) = 0;
  virtual [[nodiscard]] PresetLoadingResult loadPreset(
      const std::string& presetName) = 0;
  virtual std::vector<std::string> presets() const = 0;
};

inline PresetManager::~PresetManager() {}
}  // namespace eden_vst
