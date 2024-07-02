#pragma once
///
/// \author Jan Wilczek
/// \date 02.11.2018
///

#include <numeric>

namespace libeden_test {
class TestUtils {
public:
  /// <summary>
  /// Fills audio <paramref name="channel"/> with given <paramref name="value"/>
  /// from <paramref name="begin"/> (including) to <paramref name="end"/>
  /// (excluding).
  /// </summary>
  /// <param name="channel"></param>
  /// <param name="value"></param>
  /// <param name="begin"></param>
  /// <param name="end"></param>
  static void fillChannel(float* channel, float value, int begin, int end) {
    for (auto i = begin; i < end; ++i) {
      channel[i] = value;
    }
  }

  /// <summary>
  /// Calculates frequency of a signal in <paramref name="channel"/> using zero
  /// crossings method.
  /// </summary>
  /// <param name="channel"></param>
  /// <param name="bufferSize"></param>
  /// <param name="sampleRate"></param>
  /// <returns>detected frequency in Hz, 0 if no period has been found</returns>
  static double detectFrequency(const float* channel,
                                int bufferSize,
                                double sampleRate) {
    unsigned zeroCrossingsCount = 0u;
    int startIndex, stopIndex;

    int numberOfPeriods = 0, periodsLength = 0;

    for (auto i = 1; i < bufferSize; ++i) {
      if (channel[i] * channel[i - 1] < 0.f) {
        ++zeroCrossingsCount;

        if (zeroCrossingsCount == 1) {
          startIndex = i;
        } else if (zeroCrossingsCount == 3) {
          zeroCrossingsCount = 1;
          stopIndex = i;

          ++numberOfPeriods;
          periodsLength += stopIndex - startIndex;

          startIndex = stopIndex;
        }
      }
    }

    if (numberOfPeriods < 1) {
      return 0.0;
    }

    const auto meanPeriod =
        (static_cast<double>(periodsLength) / numberOfPeriods) / sampleRate;
    return 1 / meanPeriod;
  }

  static std::vector<float> correlation(const std::vector<float>& signal1,
                                        const std::vector<float>& signal2) {
    const std::vector<float>& shorterSignal =
        signal1.size() < signal2.size() ? signal1 : signal2;
    const std::vector<float>& longerSignal =
        &shorterSignal == &signal1 ? signal2 : signal1;

    std::vector<float> nonnegativeShiftCorrelation(shorterSignal.size());

    // TODO: Fix correlation calculation algorithm
    for (auto kappa = 0u; kappa < shorterSignal.size(); ++kappa) {
      float sum = 0.f;
      for (auto k = 0u;
           k + kappa < longerSignal.size() && k < shorterSignal.size(); ++k) {
        sum += longerSignal[k + kappa] * shorterSignal[k];
      }

      nonnegativeShiftCorrelation[kappa] = sum / shorterSignal.size();
    }

    // Negative sample shifts are just a mirror reflection of the positive
    // shifts.
    std::vector<float> negativeShiftCorrelation(
        nonnegativeShiftCorrelation.size() -
        1);  // Zero-shift correlation is not mirrored.
    std::copy(nonnegativeShiftCorrelation.begin() + 1,
              nonnegativeShiftCorrelation.end(),
              negativeShiftCorrelation.begin());
    std::reverse(negativeShiftCorrelation.begin(),
                 negativeShiftCorrelation.end());

    std::vector<float> correlationResult(negativeShiftCorrelation.size() +
                                         nonnegativeShiftCorrelation.size());
    std::copy(negativeShiftCorrelation.begin(), negativeShiftCorrelation.end(),
              correlationResult.begin());
    std::copy(nonnegativeShiftCorrelation.begin(),
              nonnegativeShiftCorrelation.end(),
              correlationResult.begin() + negativeShiftCorrelation.size());

    return correlationResult;
  }
};
}  // namespace libeden_test
