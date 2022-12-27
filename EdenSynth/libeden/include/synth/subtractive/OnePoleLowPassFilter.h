#pragma once
///
/// \author Jan Wilczek
/// \date 25.11.2018
///

namespace eden::synth::subtractive {
/// <summary>
/// Simple one pole low-pass filter base on article
/// "Oscillator and Filter Algorithms for Virtual Analog Synthesis" by Vesa
/// Välimäki and Antti Huovilainen. Original concept by Huovilainen.
/// </summary>
class OnePoleLowPassFilter {
 public:
  OnePoleLowPassFilter(float sampleRate);

  /// <summary>
  /// Filters one sample of the signal.
  /// </summary>
  /// <param name="sample"></param>
  /// <returns>the filtered sample</returns>
  float processSample(float sample);

  /// <summary>
  /// Sets the cutoff frequency of the filter - point at which gain falls to -3
  /// dB.
  /// </summary>
  /// <param name="cutoffFrequency">frequency in Hz</param>
  void setCutoffFrequency(float cutoffFrequency);

  void setSampleRate(float sampleRate);

 private:
  /// <summary>
  /// Calculates the <see cref="_g"/> coefficient based on <see
  /// cref="_cutoffFrequency"/> and <see cref="_sampleRate"/>.
  /// </summary>
  void calculateG();

  float _sampleRate;

  float _g;
  float _cutoffFrequency;

  float _feedforwardSample;
  float _feedbackSample;
};
}  // namespace eden::synth::subtractive
