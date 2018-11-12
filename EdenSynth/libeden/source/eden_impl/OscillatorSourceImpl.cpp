/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 
#include "eden_impl/OscillatorSourceImpl.h"

namespace eden
{
	std::unique_ptr<OscillatorSourceImpl> OscillatorSourceImpl::createRealtimeOscillatorSource(WaveformGenerators generatorName)
	{
		
	}

	std::unique_ptr<OscillatorSourceImpl> OscillatorSourceImpl::createWaveTableOscillatorSource(std::vector<SampleType> waveTable)
	{
		
	}

	OscillatorSourceImpl::OscillatorSourceImpl(std::unique_ptr<synth::wavetable::IOscillatorSource> synthOscillatorSource)
	{
		
	}
}
