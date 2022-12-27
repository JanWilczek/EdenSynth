#pragma once
///
///	\author Jan Wilczek
///	 \date 08.10.2018
///

namespace eden::synth {
/// <summary>
/// Interface of a mono sound synthesis module.
/// </summary>
class IMonoModule {
 public:
  virtual ~IMonoModule() = 0;

  /// <summary>
  /// Processes the mono audio channel according to the set parameters.
  /// </summary>
  /// <param name="audioBuffer"></param>
  /// <param name="startSample"></param>
  /// <param name="samplesToRender"></param>
  virtual void process(float* audioChannel,
                       int startSample,
                       int samplesToProcess) = 0;
};

inline IMonoModule::~IMonoModule() {}
}  // namespace eden::synth
