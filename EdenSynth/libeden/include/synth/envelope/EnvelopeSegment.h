#pragma once
///
/// \author Jan Wilczek
/// \date 11.10.2018
///
#include <chrono>
#include <memory>
#include "synth/envelope/ISegmentGain.h"

using namespace std::chrono_literals;

namespace eden::synth::envelope {
/// <summary>
/// Abstract class representing envelope segment, i.e. time segment with gain
/// given with a function of time. Subclasses may represent attack or decay for
/// example. It's gain curve is based on passed ISegmentGain's subclass and the
/// duration of the gain change from initial to the final level.
/// </summary>
class EnvelopeSegment {
public:
  EnvelopeSegment(float sampleRate,
                  std::unique_ptr<ISegmentGain> envelopeGain,
                  std::chrono::milliseconds duration = 0ms,
                  float initialLevel = 0,
                  float finalLevel = 0);
  virtual ~EnvelopeSegment() = 0;

  /// <summary>
  /// Changes the current envelope level.
  /// </summary>
  /// <param name="currentLevel"></param>
  virtual void applyAndUpdateGain(float& currentLevel);

  /// <returns>true if current value of envelope means that current segment has
  /// ended, false otherwise</returns>
  virtual bool hasEnded(float currentLevel) = 0;

  virtual void setDuration(std::chrono::milliseconds duration);

  virtual void setGainCurve(std::unique_ptr<ISegmentGain> envelopeGain);

  virtual void setInitialLevel(float initialLevel);

  virtual void setFinalLevel(float finalLevel);

  virtual void setSampleRate(float sampleRate);

private:
  void calculateGain();

protected:
  float _sampleRate;
  std::unique_ptr<ISegmentGain> _envelopeGain;
  std::chrono::milliseconds _duration;
  float _initialLevel;
  float _finalLevel;
};
}  // namespace eden::synth::envelope
