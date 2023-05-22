#pragma once

namespace eden_vst::viewmodels {
class ViewModel {
public:
  virtual ~ViewModel() = 0;
};

inline ViewModel::~ViewModel() {}
}  // namespace eden_vst::viewmodels
