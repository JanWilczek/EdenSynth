#pragma once
/// 
/// \author Jan Wilczek
/// \date 20.10.2018
/// 
#include <vector>
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

	class WaveFileReader
	{
	public:
		std::vector<SampleType> readWaveFile(const std::string& pathToWaveFile);

	private:
		WaveHeader _header;
	};
}
