
#include "utility/dsp/Correlation.h"

namespace libeden_test {
using eden::utility::dsp::correlation;
using eden::utility::dsp::CorrelationMode;

TEST(CorrelationTest, DiracImpulses) {
  const std::vector<float> dirac1{0.f, 1.f, 0.f, 0.f};
  const std::vector<float> dirac2{0.f, 0.f, 1.f, 0.f};

  const auto result = correlation<CorrelationMode::Full>(dirac1, dirac2);

  ASSERT_EQ(result.size(), 7u);
  ASSERT_FLOAT_EQ(result[2], 1.f);
  for (auto i = 0u; i < result.size(); ++i) {
    if (i != 2u) {
      ASSERT_EQ(result[i], 0.f);
    }
  }
}

TEST(CorrelationTest, AveragePowerAtZeroShiftOfAutocorrelation) {
  const std::vector<float> signal{1.f, 2.f, 6.f, 0.f, 2.f};

  const auto autocorrelation = correlation(signal, signal);

  ASSERT_EQ(autocorrelation.size(), 9);
  ASSERT_FLOAT_EQ(autocorrelation[4], 45.f);
}

TEST(CorrelationTest, SymmetryOfAutocorrelation) {
  const std::vector<float> signal{10.f, 20.f, 4.f, 2.f, 5.f, 0.f, -4.f};

  const auto autocorrelation = correlation(signal, signal);

  ASSERT_EQ(autocorrelation.size(), 13u);
  for (auto i = 0u; i < 7u; ++i) {
    ASSERT_FLOAT_EQ(autocorrelation[i], autocorrelation[12 - i]);
  }
}

TEST(CorrelationTest, MaximumAtZeroOfAutocorrelation) {
  const std::vector<float> signal{-0.3f, 0.5f, 1.f, -0.66f, -0.99f,
                                  0.3f,  0.2f, 0.f, 0.42f};

  const auto autocorrelation = correlation(signal, signal);

  ASSERT_EQ(autocorrelation.size(), 17u);

  const auto expectedMaximumValue = autocorrelation[8];
  for (auto i = 0u; i < 9u; ++i) {
    ASSERT_GE(expectedMaximumValue, autocorrelation[i]);
    ASSERT_GE(expectedMaximumValue, autocorrelation[16 - i]);
  }
}
}  // namespace libeden_test
