#pragma once

namespace eden::plugin::viewmodels {
class ViewModel {
public:
  virtual ~ViewModel() = 0;
};

inline ViewModel::~ViewModel() {}
}  // namespace eden::plugin::viewmodels
