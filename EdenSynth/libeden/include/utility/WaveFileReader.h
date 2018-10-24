#pragma once
/// 
/// \author Jan Wilczek
/// \date 20.10.2018
/// 
#include <vector>
#include <fstream>
#include "eden/SampleType.h"

namespace eden::utility
{
	struct WaveHeader
	{
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
	class WaveFileReader
	{
	public:
		explicit WaveFileReader(const std::string& pathToWaveFile);
		~WaveFileReader();

		int sampleRate() const noexcept;
		int getNumSamples() const;
		std::vector<SampleType> readSamples();

	private:
		void readHeader();
		void addSample(int sample);

		std::ifstream _fileStream;
		WaveHeader _header;
		std::vector<SampleType> _samples;
	};
}
