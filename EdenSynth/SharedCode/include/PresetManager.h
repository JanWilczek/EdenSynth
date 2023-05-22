#pragma once
#include <string>
#include <functional>

namespace eden_vst {
class PresetManager {
public:
  virtual ~PresetManager();
  virtual void saveCurrentPreset(
      std::function<void(const std::string&)> onPresetAdded) = 0;
  virtual void loadPreset(const std::string& presetName) = 0;
  virtual std::vector<std::string> presets() const = 0;
};

inline PresetManager::~PresetManager() {}
}  // namespace eden_vst
