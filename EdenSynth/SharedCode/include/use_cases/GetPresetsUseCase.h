#pragma once
#include <memory>
#include <vector>
#include "presets/Preset.h"
#include "presets/PresetRepository.h"

namespace eden::plugin {
class GetPresetsUseCase {
public:
  explicit GetPresetsUseCase(std::shared_ptr<PresetRepository>);

  std::vector<PresetV2> operator()();

private:
  std::shared_ptr<PresetRepository> _presetsRepository;
};
}  // namespace eden::plugin