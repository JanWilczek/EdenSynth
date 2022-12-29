#pragma once
///
/// \author Jan Wilczek
/// \date 20.10.2018
///
#include <fstream>
#include <vector>

namespace eden::utility {
/// <summary>
/// Wave file header according to http://soundfile.sapp.org/doc/WaveFormat/
/// </summary>
struct WaveHeader {
  char ChunkID[4];
  int ChunkSize;
  char Format[4];
  char Subchunk1ID[4];
  int Subchunk1Size;
  short AudioFormat;
  short NumChannels;
  int SampleRate;
  int ByteRate;
  short BlockAlign;
  short BitsPerSample;
  char Subchunk2ID[4];
  int Subchunk2Size;
};

/// <summary>
/// Class reading an uncompressed, standard mono WAVE file.
/// WAVE format specification: http://soundfile.sapp.org/doc/WaveFormat/
/// Currently 8-, 16- and 32-bit PCM samples are supported.
/// </summary>
class WaveFileReader {
public:
  explicit WaveFileReader(const std::string& pathToWaveFile);
  ~WaveFileReader();

  /// <returns>sample rate of the wave file</returns>
  int sampleRate() const noexcept;

  /// <returns>number of samples stored in wave file</returns>
  int getNumSamples() const;

  /// <returns>samples from decoded wave file in float format</returns>
  std::vector<float> readSamples();

private:
  /// <summary>
  /// Reads the header part of the wave file.
  /// </summary>
  void readHeader();

  /// <summary>
  /// Converts the given <paramref name="sample"/> to float and adds it at the
  /// end of samples vector.
  /// </summary>
  /// <param name="sample"></param>
  void addSample(int sample);

  /// <summary>
  /// File stream associated with the wave file.
  /// </summary>
  std::ifstream _fileStream;

  /// <summary>
  /// Read wave file's header.
  /// </summary>
  WaveHeader _header;

  /// <summary>
  /// Samples read from wave file.
  /// </summary>
  std::vector<float> _samples;
};
}  // namespace eden::utility
