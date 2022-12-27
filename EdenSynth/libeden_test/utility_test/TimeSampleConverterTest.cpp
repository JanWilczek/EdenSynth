///
/// \author Jan Wilczek
/// \date 01.11.2018
///
#include <utility/TimeSampleConverter.h>
#include "pch.h"

namespace libeden_test {
TEST(TimeSampleConverterTest, SecondsToSamples) {
  constexpr auto sampleRate = 48000.0;

  using namespace std::chrono_literals;

  constexpr std::chrono::seconds durationsInS[] = {1s, 2s, 10s};
  constexpr unsigned resultsOfS[] = {48000u, 96000u, 480000u};

  for (auto i = 0; i < 3; ++i) {
    const auto result = eden::utility::TimeSampleConverter::timeToSamples(
        durationsInS[i], sampleRate);
    EXPECT_EQ(result, resultsOfS[i]);
  }
}

TEST(TimeSampleConverterTest, MillisecondsToSamples) {
  constexpr auto sampleRate = 48000.0;

  using namespace std::chrono_literals;

  constexpr std::chrono::milliseconds durationsInS[] = {500ms, 2000ms, 10ms,
                                                        20ms};
  constexpr unsigned resultsOfS[] = {24000, 96000u, 480u, 960u};

  for (auto i = 0; i < 4; ++i) {
    const auto result = eden::utility::TimeSampleConverter::timeToSamples(
        durationsInS[i], sampleRate);
    EXPECT_EQ(result, resultsOfS[i]);
  }
}

TEST(TimeSampleConverterTest, SamplesToTime) {
  constexpr auto sampleRate = 48000.0;

  using namespace std::chrono_literals;

  constexpr unsigned samples[] = {480u, 48000u, 240u, 96000u};
  constexpr std::chrono::seconds secondsResults[] = {0s, 1s, 0s, 2s};
  constexpr std::chrono::milliseconds millisecondsResults[] = {10ms, 1000ms,
                                                               5ms, 2000ms};

  for (auto i = 0; i < 4; ++i) {
    const auto seconds = eden::utility::TimeSampleConverter::samplesToSeconds(
        samples[i], sampleRate);
    EXPECT_EQ(seconds, secondsResults[i]);

    const auto milliseconds =
        eden::utility::TimeSampleConverter::samplesToMilliseconds(samples[i],
                                                                  sampleRate);
    EXPECT_EQ(milliseconds, millisecondsResults[i]);
  }
}
}  // namespace libeden_test
