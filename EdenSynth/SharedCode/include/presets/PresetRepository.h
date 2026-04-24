#pragma once
#include <optional>
#include <vector>
#include "presets/Preset.h"

namespace eden::plugin {
class PresetRepository {
public:
  PresetRepository() = default;
  virtual ~PresetRepository() = default;
  PresetRepository(const PresetRepository&) = delete;
  PresetRepository& operator=(const PresetRepository&) = delete;
  PresetRepository(PresetRepository&&) = delete;
  PresetRepository& operator=(PresetRepository&&) = delete;

  virtual std::optional<PresetV2> findPreset(const PresetId& presetId) = 0;
  virtual void savePreset(PresetV2) = 0;
  virtual std::vector<PresetV2> presets() = 0;
};
}  // namespace eden::plugin