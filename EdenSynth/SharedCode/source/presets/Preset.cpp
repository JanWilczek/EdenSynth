#include "Preset.h"

namespace eden::plugin {
Preset::Preset(Args&& args)
    : _name{std::move(args.name)},
      _isFactory{args.isFactory},
      _absolutePath{std::move(args.absolutePath)} {}
}  // namespace eden::plugin
