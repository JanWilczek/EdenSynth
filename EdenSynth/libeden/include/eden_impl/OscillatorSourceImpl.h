#pragma once
/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 
#include <memory>
#include <vector>
#include "eden/OscillatorParameters.h"
#include "eden/SampleType.h"

namespace eden
{
	namespace settings 
	{
		class Settings;
	}

	class OscillatorSourceImpl
	{
	public:
		OscillatorSourceImpl(settings::Settings& settings, WaveformGenerators generatorName);
		OscillatorSourceImpl(settings::Settings& settings, std::vector<SampleType> waveTable);
		~OscillatorSourceImpl();

		OscillatorSourceId getId();

	private:

		settings::Settings& _ext_settings;
		OscillatorSourceId _id;
	};
}
