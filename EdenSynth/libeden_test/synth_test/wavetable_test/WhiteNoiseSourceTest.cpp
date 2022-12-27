///
/// \author Jan Wilczek
/// \date 12.10.2019
///
#include "pch.h"
#include "synth/wavetable/WhiteNoiseSource.h"

#include <functional>
#include "TestUtils.h"

namespace libeden_test {
using WhiteNoiseSource = eden::synth::wavetable::WhiteNoiseSource;

class WhiteNoiseSourceTest : public ::testing::Test {
 protected:
  void SetUp() override { _whiteNoiseSource = WhiteNoiseSource(); }

  WhiteNoiseSource _whiteNoiseSource;
};

TEST_F(WhiteNoiseSourceTest, Clipping) {
  // Given the noise generator

  // When it is playing

  // Then it should produce output in [-1,1] range
  constexpr auto bufferLength = 480;
  for (auto i = 0; i < bufferLength; ++i) {
    const auto sample = _whiteNoiseSource.getSample();

    ASSERT_GE(sample, -1.f);
    ASSERT_LE(sample, 1.f);
  }
}

TEST_F(WhiteNoiseSourceTest, ZeroMean) {
  // Given the noise generator

  // When it is playing
  constexpr auto bufferLength = 480000;
  auto sum = 0.f;
  for (auto i = 0; i < bufferLength; ++i) {
    sum += _whiteNoiseSource.getSample();
  }

  // Then the mean of the output should be around 0
  const auto mean = sum / static_cast<float>(bufferLength);
  ASSERT_NEAR(0.f, mean, 1e-2f);
}

TEST_F(WhiteNoiseSourceTest, ZeroCorrelationIsGreatest) {
  // Given the noise generator

  // When it is playing
  constexpr auto bufferLength = 480u;
  std::vector<float> samplesBuffer;
  std::generate_n(std::back_inserter(samplesBuffer), bufferLength,
                  std::bind(&WhiteNoiseSource::getSample, &_whiteNoiseSource));

  // Then the autocorrelation should be nonzero at zero shift and near to zero
  // elsewhere
  const auto autocorrelation =
      TestUtils::correlation(samplesBuffer, samplesBuffer);

  const auto zeroShiftIndex = samplesBuffer.size() - 1u;

  ASSERT_GT(autocorrelation[zeroShiftIndex], 0.f);

  for (auto i = 1u; i < autocorrelation.size(); ++i) {
    if (i != zeroShiftIndex)
      ASSERT_LT(autocorrelation[i], autocorrelation[zeroShiftIndex]);
  }
}

TEST_F(WhiteNoiseSourceTest, PitchInvariance) {
  // Given two noise generators initialized with the same seed
  constexpr auto seed = 10;
  WhiteNoiseSource whiteNoiseSource{seed};
  WhiteNoiseSource whiteNoiseSource2{seed};

  // When setting pitch of one of them
  whiteNoiseSource2.setPitch(440.f);

  // Then output should not change its character (i.e. it should still be white
  // noise) Therefore the outputs of two identically seeded generators should be
  // the same
  constexpr auto bufferLength = 480u;
  for (auto i = 0u; i < bufferLength; ++i) {
    ASSERT_FLOAT_EQ(whiteNoiseSource.getSample(),
                    whiteNoiseSource2.getSample());
  }
}
}  // namespace libeden_test
