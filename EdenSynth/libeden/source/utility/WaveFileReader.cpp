///
/// \author Jan Wilczek
/// \date 20.10.2018
///
#include "utility/WaveFileReader.h"
#include "utility/EdenAssert.h"

namespace eden::utility {
WaveFileReader::WaveFileReader(const std::string& pathToWaveFile)
    : _fileStream(pathToWaveFile, std::ios::in | std::ios::binary) {
  if (!_fileStream.good()) {
    throw std::ios_base::failure("Wave file does not exist!");
  }

  readHeader();
}

WaveFileReader::~WaveFileReader() {
  _fileStream.close();
}

int WaveFileReader::sampleRate() const noexcept {
  return _header.SampleRate;
}

int WaveFileReader::getNumSamples() const {
  // Subchunk2Size = NumSamples * NumChannels * BitsPerSample/8
  return _header.Subchunk2Size * 8 /
         (_header.NumChannels * _header.BitsPerSample);
}

std::vector<float> WaveFileReader::readSamples() {
  if (_samples.size() > 0) {
    return _samples;
  }

  int readSample;
  const auto numSamples = getNumSamples();
  for (auto i = 0; i < numSamples; ++i) {
    _fileStream.read(reinterpret_cast<char*>(&readSample),
                     _header.BitsPerSample / 8);
    addSample(readSample);
  }

  return _samples;
}

void WaveFileReader::readHeader() {
  _fileStream.read(reinterpret_cast<char*>(&_header), sizeof(_header));

  if (_header.NumChannels != 1) {
    throw std::runtime_error("Unsupported number of channels - should be 1.");
  }

  if (_header.AudioFormat != 1) {
    throw std::runtime_error(
        "Unsupported audio format - currently only uncompressed PCM is "
        "supported.");
  }
}

void WaveFileReader::addSample(int sample) {
  switch (_header.BitsPerSample) {
    case 8:
      _samples.push_back(static_cast<float>(static_cast<double>(sample) /
                                            std::numeric_limits<char>::max()));
      break;
    case 16:
      _samples.push_back(static_cast<float>(static_cast<double>(sample) /
                                            std::numeric_limits<short>::max()));
      break;
    case 32:
      _samples.push_back(static_cast<float>(static_cast<double>(sample) /
                                            std::numeric_limits<int>::max()));
      break;
    case 64:
      _samples.push_back(static_cast<float>(static_cast<double>(sample) /
                                            std::numeric_limits<long>::max()));
      break;
    default:
      throw std::runtime_error(
          "Unsupported number of bits. Currently only 8-, 16- and 32-bit PCM "
          "samples are supported.");
  }
}
}  // namespace eden::utility
