/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 
#include "eden/OscillatorSource.h"

namespace eden
{
	std::unique_ptr<OscillatorSource> OscillatorSource::createRealtimeOscillatorSource(WaveformGenerators generatorName)
	{
		return std::make_unique<OscillatorSource>(OscillatorSourceImpl::createRealtimeOscillatorSource(generatorName));
	}

	std::unique_ptr<OscillatorSource> OscillatorSource::createWaveTableOscillatorSource(std::vector<SampleType> waveTable)
	{
		return std::make_unique<OscillatorSource>(OscillatorSourceImpl::createWaveTableOscillatorSource(waveTable));
	}

	OscillatorSource::OscillatorSource(std::unique_ptr<OscillatorSourceImpl> impl)
		: _impl(std::move(impl))
	{
	}
}
