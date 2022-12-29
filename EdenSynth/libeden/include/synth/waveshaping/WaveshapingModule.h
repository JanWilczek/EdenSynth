#pragma once
///
/// \auhtor Jan Wilczek
/// \date 08.10.2018
///
#include "synth/IMonoModule.h"
#include "synth/waveshaping/Waveshaper.h"

namespace eden::synth::waveshaping {
class WaveshapingModule : public IMonoModule {
public:
  ~WaveshapingModule() override = default;

  void process(float* audioChannel,
               int startSample,
               int samplesToProcess) override;

  void setTransferFunction(std::vector<float> transferFunction);

  void setAutoMakeUpGain(bool makeUpGainEnabled);

private:
  Waveshaper _waveshaper;
};
}  // namespace eden::synth::waveshaping
