///
/// \author Jan Wilczek
/// \date 05.10.2018
///

#include "eden/AudioBuffer.h"

namespace libeden_test {
TEST(AudioBufferTest, PassMemoryBlock) {
  constexpr int numChannels = 2;
  float** data = new float*[numChannels];

  constexpr unsigned numSamples = 1024;
  for (int i = 0; i < numChannels; ++i) {
    data[i] = new float[numSamples];
  }

  for (int i = 0; i < numChannels; ++i) {
    for (int j = 0; j < numSamples; ++j) {
      data[i][j] = 0;
    }
  }

  eden::AudioBuffer testBuffer(data, numChannels, numSamples);

  ASSERT_EQ(testBuffer.getNumChannels(), numChannels);
  ASSERT_EQ(testBuffer.getNumSamples(), numSamples);

  for (int i = 0; i < testBuffer.getNumChannels(); ++i) {
    for (unsigned j = 0; j < testBuffer.getNumSamples(); ++j) {
      EXPECT_FLOAT_EQ(testBuffer.getArrayOfReadPointers()[i][j], 0);
    }
  }

  for (int i = 0; i < numChannels; ++i) {
    delete[] data[i];
  }
  delete[] data;
}

TEST(AudioBufferTest, AllocationDeallocation) {
  constexpr int numCases = 6;
  constexpr std::array<int, numCases> numChannels{2, 5, 0, 1, 10, 3};
  constexpr std::array<unsigned, numCases> numSamples{2048, 512,   234,
                                                      340,  48000, 96000};
  for (int i = 0; i < numCases; ++i) {
    eden::AudioBuffer testBuffer(numChannels[i], numSamples[i]);
    EXPECT_EQ(testBuffer.getNumChannels(), numChannels[i]);
    EXPECT_EQ(testBuffer.getNumSamples(), numSamples[i]);
  }

  ASSERT_NO_THROW(eden::AudioBuffer testBuffer = eden::AudioBuffer());
}

TEST(AudioBufferTest, Fill) {
  eden::AudioBuffer testBuffer(2, 1024);

  const std::vector<double> testValues{0, 0.5, 0.6, 0.33, 0.1};

  for (size_t k = 0; k < testValues.size(); k++) {
    testBuffer.fill(static_cast<float>(testValues[k]));

    for (auto i = 0; i < testBuffer.getNumChannels(); ++i) {
      for (auto j = 0u; j < testBuffer.getNumSamples(); ++j) {
        EXPECT_FLOAT_EQ(testBuffer.getArrayOfReadPointers()[i][j],
                        static_cast<float>(testValues[k]));
      }
    }
  }
}

TEST(AudioBufferTest, AddSample) {
  eden::AudioBuffer testBuffer(1, 1024);
  testBuffer.fill(0);

  constexpr auto FREQUENCY1 = 1000;
  constexpr auto FREQUENCY2 = 440;
  constexpr auto SAMPLE_RATE = 48000;
  constexpr auto ACCURACY_THRESHOLD = 0.000001;

  for (auto i = 0u; i < testBuffer.getNumSamples(); ++i) {
    testBuffer.addSample(
        0, i,
        static_cast<float>(std::sin(2 * FREQUENCY1 * eden::math_constants::PI *
                                    i / SAMPLE_RATE)));
    testBuffer.addSample(
        0, i,
        static_cast<float>(std::sin(2 * FREQUENCY2 * eden::math_constants::PI *
                                    i / SAMPLE_RATE)));
  }
  for (auto j = 0u; j < testBuffer.getNumSamples(); ++j) {
    const auto expectedValue = static_cast<float>(
        std::sin(2 * FREQUENCY1 * eden::math_constants::PI * j / SAMPLE_RATE) +
        std::sin(2 * FREQUENCY2 * eden::math_constants::PI * j / SAMPLE_RATE));
    EXPECT_NEAR(testBuffer.getArrayOfReadPointers()[0][j], expectedValue,
                ACCURACY_THRESHOLD);
  }
}

TEST(AudioBufferTest, ForEachChannel) {
  eden::AudioBuffer testBuffer(3, 300);

  ASSERT_EQ(testBuffer.getNumChannels(), 3);
  ASSERT_EQ(testBuffer.getNumSamples(), 300);

  testBuffer.forEachChannel([&](float* channel) {
    for (auto i = 0u; i < testBuffer.getNumSamples(); ++i) {
      if (i % 2 == 0) {
        channel[i] = 1;
      } else {
        channel[i] = -1;
      }
    }
  });

  for (auto channel = 0; channel < testBuffer.getNumChannels(); ++channel) {
    for (auto sample = 0u; sample < testBuffer.getNumSamples(); ++sample) {
      const auto expectedValue = static_cast<float>(sample % 2 == 0 ? 1 : -1);
      EXPECT_FLOAT_EQ(testBuffer.getArrayOfReadPointers()[channel][sample],
                      expectedValue);
    }
  }
}
}  // namespace libeden_test