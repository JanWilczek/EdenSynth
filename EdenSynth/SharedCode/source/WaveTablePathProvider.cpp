///
/// \author Jan Wilczek
/// \date 14.11.2018
///
#include "WaveTablePathProvider.h"

namespace eden_vst {
WaveTablePathProvider::WaveTablePathProvider(std::filesystem::path assetsPath) {
  const auto path = assetsPath / "wavetables";

  if (exists(path)) {
    for (auto& file : std::filesystem::directory_iterator(path)) {
      auto name = file.path().filename().string();
      name = name.substr(0u, name.length() - 4u);  // remove ".wav" ending
      _waveTablePaths[name] = file.path();
    }
  }
}

size_t WaveTablePathProvider::size() const {
  return _waveTablePaths.size();
}

WaveTablePathProvider::path_container_iterator WaveTablePathProvider::begin() {
  return _waveTablePaths.begin();
}

WaveTablePathProvider::path_container_iterator WaveTablePathProvider::end() {
  return _waveTablePaths.end();
}

WaveTablePathProvider::path_container_const_iterator
WaveTablePathProvider::cbegin() const {
  return _waveTablePaths.cbegin();
}

WaveTablePathProvider::path_container_const_iterator
WaveTablePathProvider::cend() const {
  return _waveTablePaths.cend();
}

std::filesystem::path WaveTablePathProvider::getPath(
    std::string waveTableName) {
  if (_waveTablePaths.find(waveTableName) == _waveTablePaths.end()) {
    throw new std::runtime_error("Unknown wave table name.");
  }

  return _waveTablePaths[waveTableName];
}

std::filesystem::path WaveTablePathProvider::getPath(size_t index) {
  return getPath(indexToName(index));
}

std::string WaveTablePathProvider::indexToName(size_t waveTableIndex) {
  if (waveTableIndex >= _waveTablePaths.size()) {
    throw std::runtime_error("Invalid wave table index.");
  }

  auto it = begin();
  std::advance(it, waveTableIndex);

  return it->first;
}

size_t WaveTablePathProvider::nameToIndex(std::string waveTableName) {
  const auto it = _waveTablePaths.find(waveTableName);

  if (it == end()) {
    throw std::runtime_error("Invalid wave table name.");
  }

  return std::distance(begin(), it);
}
}  // namespace eden_vst
