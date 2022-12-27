#pragma once
///
/// \author Jan Wilczek
/// \date 11.10.2018
///
#include <functional>
#include <vector>

namespace eden::synth::envelope {
class EnvelopeSegment;

/// <summary>
/// Abstract class representing time envelope.
/// Subclasses can consist of multiple envelope segments.
/// "Level" in envelope classes refers to linear value (in range [-1, 1]).
/// </summary>
class Envelope {
 public:
  /// <summary>
  /// Type of function invoked when the envelope has ended (i.e. gain is 0
  /// again). The sample index of envelope's end should be passed to it.
  /// </summary>
  typedef std::function<void()> OnEnvelopeEnded;

  virtual ~Envelope() = 0;

  /// <summary>
  /// Applies envelope to the given sample.
  /// </summary>
  /// <param name="sample"></param>
  virtual void apply(float& sample);

  virtual void applyInRange(float* samples,
                            int startSample,
                            int samplesToApply);

  /// <summary>
  /// Handles note on event - typically starts attack segment.
  /// </summary>
  virtual void keyOn() = 0;

  /// <summary>
  /// Handles note off event - typically starts release segment.
  /// </summary>
  virtual void keyOff() = 0;

  /// <summary>
  /// Checks if the envelope has ended and applies appropriate actions if it
  /// did.
  /// </summary>
  virtual bool hasEnded() = 0;

  /// <summary>
  /// Sets sample rate.
  /// </summary>
  /// <param name="sampleRate"></param>
  virtual void setSampleRate(float sampleRate);

  /// <summary>
  /// Sets the function to be called when the envelope has ended.
  /// Sample index at which the envelope has ended is passed to it.
  /// </summary>
  /// <param name="callback"></param>
  void setOnEnvelopeEndedCallback(OnEnvelopeEnded callback);

 protected:
  /// <summary>
  /// Switches to given envelope segment.
  /// </summary>
  /// <param name="segment"></param>
  void switchToSegment(size_t segment);

  /// <summary>
  /// Updates current gain value.
  /// </summary>
  void updateGain();

  /// <summary>
  /// Current envelope value (linear).
  /// </summary>
  float _currentLevel = 0.f;

  /// <summary>
  /// Envelope's segments.
  /// </summary>
  std::vector<EnvelopeSegment*> _segments;

  /// <summary>
  /// Current envelope segment.
  /// </summary>
  size_t _currentSegment = 0u;

  /// <summary>
  /// Callback to call after envelope has ended.
  /// </summary>
  OnEnvelopeEnded _onEnvelopeEndedCallback = []() {};
};
}  // namespace eden::synth::envelope
