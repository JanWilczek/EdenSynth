/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 
#include "eden_impl/OscillatorSourceImpl.h"
#include "settings/Settings.h"

namespace eden
{
	OscillatorSourceImpl::OscillatorSourceImpl(settings::Settings& settings, WaveformGenerators generatorName)
		: _ext_settings(settings)
		, _id(settings.createGeneratorSource(generatorName))
	{
	}

	OscillatorSourceImpl::OscillatorSourceImpl(settings::Settings& settings, std::vector<float> waveTable)
		: _ext_settings(settings)
		, _id(settings.createWaveTableSource(waveTable))
	{
	}

	OscillatorSourceImpl::~OscillatorSourceImpl()
	{
		_ext_settings.removeOscillatorSource(getId());
	}

	OscillatorSourceId OscillatorSourceImpl::getId()
	{
		return _id;
	}
}
