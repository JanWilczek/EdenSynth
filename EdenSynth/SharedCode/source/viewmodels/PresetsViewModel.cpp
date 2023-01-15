#include "viewmodels/PresetsViewModel.h"
#include "PresetManager.h"

namespace eden_vst::viewmodels {
PresetsViewModel::PresetsViewModel(std::unique_ptr<PresetManager> presetManager)
    : _presetManager{std::move(presetManager)} {}
}  // namespace eden_vst::viewmodels
