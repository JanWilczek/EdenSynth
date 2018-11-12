#pragma once
/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 
#include <memory>
#include <vector>
#include "eden/SampleType.h"
#include "eden_impl/OscillatorSourceImpl.h"

namespace eden
{
	enum class WaveformGenerators;

	class OscillatorSource
	{
	public:
		static std::unique_ptr<OscillatorSource> createRealtimeOscillatorSource(WaveformGenerators generatorName);

		static std::unique_ptr<OscillatorSource> createWaveTableOscillatorSource(std::vector<SampleType> waveTable);

	private:
		explicit OscillatorSource(std::unique_ptr<OscillatorSourceImpl> impl);

		std::unique_ptr<OscillatorSourceImpl> _impl;
	};
}
