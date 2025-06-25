#pragma once

namespace eden::plugin::parameter_id {
#define EDEN_MACRO_MAKE_PARAMETER(variableName, parameterName) \
  constexpr auto variableName = parameterName;                 \
  static const juce::ParameterID variableName##_PARAMETER{parameterName, 1};

EDEN_MACRO_MAKE_PARAMETER(WAVESHAPER_SPREAD_VALUE, "waveshaper.spread.value")
EDEN_MACRO_MAKE_PARAMETER(WAVESHAPER_SPREAD_SEED, "waveshaper.spread.seed")
EDEN_MACRO_MAKE_PARAMETER(WAVESHAPER_CURVE, "waveshaper.curve")
EDEN_MACRO_MAKE_PARAMETER(WAVESHAPER_CHEBYSHEV_POLYNOMIAL_ORDER,
                          "waveshaper.cheb.order")

#undef EDEN_MAKE_PARAMETER
}  // namespace eden::plugin::parameter_id
