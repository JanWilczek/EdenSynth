#pragma once

namespace eden::plugin::parameter_id {
constexpr auto WAVESHAPER_SPREAD_VALUE = "waveshaper.spread.value";
constexpr auto WAVESHAPER_SPREAD_SEED = "waveshaper.spread.seed";
constexpr auto WAVESHAPER_CURVE = "waveshaper.curve";
constexpr auto WAVESHAPER_CHEBYSHEV_POLYNOMIAL_ORDER = "waveshaper.cheb.order";

ParameterID make(auto parameterId) {
  static const ParameterID id{parameterId, 1};
  return id;
}
}  // namespace eden::plugin::parameter_id
