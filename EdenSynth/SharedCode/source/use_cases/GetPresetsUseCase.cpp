#include "utility/EdenAssert.h"
#include "use_cases/GetPresetsUseCase.h"

namespace eden::plugin {
GetPresetsUseCase::GetPresetsUseCase(
    std::shared_ptr<PresetRepository> presetsRepository)
    : _presetsRepository{std::move(presetsRepository)} {
  //  EDEN_ASSERT(_presetsRepository);
}

std::vector<PresetV2> GetPresetsUseCase::operator()() {
  return _presetsRepository->presets();
}
}  // namespace eden::plugin