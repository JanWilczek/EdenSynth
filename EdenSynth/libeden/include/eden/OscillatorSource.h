#pragma once
/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 
#include <memory>
#include <vector>
#include "eden/SampleType.h"

namespace eden
{
	enum class WaveformGenerators;

	class OscillatorSource
	{
	public:
		static std::unique_ptr<OscillatorSource> createRealtimeOscillatorSource(WaveformGenerators generatorName);

		static std::unique_ptr<OscillatorSource> createWaveTableOscillatorSource(std::vector<SampleType> waveTable);
	};
}
