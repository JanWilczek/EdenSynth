#pragma once

namespace eden::plugin::parameter_id {
#define EDEN_MACRO_MAKE_PARAMETER(variableName, parameterName) \
  constexpr auto variableName = parameterName;                 \
  static const juce::ParameterID variableName##Parameter{parameterName, 1};

EDEN_MACRO_MAKE_PARAMETER(waveshaperSpreadValue, "waveshaper.spread.value")
EDEN_MACRO_MAKE_PARAMETER(waveshaperSpreadSeed, "waveshaper.spread.seed")
EDEN_MACRO_MAKE_PARAMETER(waveshaperCurve, "waveshaper.curve")
EDEN_MACRO_MAKE_PARAMETER(waveshaperChebyshevPolynomialOrder,
                          "waveshaper.cheb.order")

#undef EDEN_MAKE_PARAMETER
}  // namespace eden::plugin::parameter_id
