#pragma once
///
/// \author Jan Wilczek
/// \date 05.03.2019
///
#include <vector>

namespace eden {
class AudioBuffer;
}

namespace eden::synth {
/// <summary>
/// A mixing utility - stores pointers to outputs from all the voices
/// and mixes them the given AudioBuffer.
/// </summary>
class Mixer {
 public:
  explicit Mixer(size_t nb_voices);

  /// <summary>
  /// Use it to assign particular voice's output.
  /// </summary>
  /// <param name="voice"></param>
  /// <returns>a pointer to the output of the given voice, nullptr if the voice
  /// is inactive</returns>
  const float*& operator[](size_t voice);

  /// <summary>
  /// Mixes voices' output to the given AudioBuffer by means of simple addition.
  /// </summary>
  /// <param name="outputBuffer">buffer to mix to</param>
  /// <param name="startSample">offset in the buffer</param>
  /// <param name="samplesToMix">number of samples to mix</param>
  void mixTo(AudioBuffer& outputBuffer, int startSample, int samplesToMix);

 private:
  /// <summary>
  /// Pointers to blocks of audio output by each voice.
  /// </summary>
  std::vector<const float*> _renderedVoices;
};
}  // namespace eden::synth
