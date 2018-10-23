/// 
/// \author Jan Wilczek
/// \date 20.10.2018
/// 
#include <fstream>
#include "utility/WaveFileReader.h"
#include "utility/EdenAssert.h"

namespace eden::utility
{
	std::vector<SampleType> WaveFileReader::readWaveFile(const std::string& pathToWaveFile)
	{
		std::ifstream fileStream(pathToWaveFile, std::ios::in | std::ios::binary);
		
		EDEN_ASSERT(fileStream.good());

		fileStream >> _header.ChunkID;
		fileStream >> _header.ChunkSize;
		fileStream >> _header.Format;
		fileStream >> _header.Subchunk1ID;
		fileStream >> _header.Subchunk1Size;
		fileStream >> _header.AudioFormat;
		fileStream >> _header.NumChannels;
		fileStream >> _header.SampleRate;
		fileStream >> _header.ByteRate;
		fileStream >> _header.BlockAlign;
		fileStream >> _header.BitsPerSample;
		fileStream >> _header.Subchunk2ID;
		fileStream >> _header.Subchunk2Size;

		//fileStream.read((char*)&_header, sizeof(_header));


		//fileStream.read(_header.ChunkID, 4);

		EDEN_ASSERT(_header.NumChannels == 1);

		std::vector<SampleType> readSamples;
		int readSample;

		const auto numSamples = _header.Subchunk2Size * 8 / (_header.NumChannels * _header.BitsPerSample);
		for (int i = 0; i < numSamples; ++i)
		{
			fileStream >> readSample;
			switch (_header.BitsPerSample)
			{
			case 8:
				readSamples.push_back(static_cast<double>(readSample) / (1 << 8 - 1));
				break;
			case 16:
				readSamples.push_back(static_cast<double>(readSample) / (1 << 16 - 1));
				break;
			case 32:
				readSamples.push_back(static_cast<double>(readSample) / (1 << 32 - 1));
				break;
			default:
				EDEN_ASSERT(false);
				break;
			}
		}

		return readSamples;
	}
}
