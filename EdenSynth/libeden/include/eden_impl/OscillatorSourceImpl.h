#pragma once
/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 
#include <vector>
#include "eden/OscillatorParameters.h"

namespace eden
{
	namespace settings 
	{
		class Settings;
	}

	class OscillatorSourceImpl
	{
	public:
		OscillatorSourceImpl(settings::Settings& settings, WaveformGenerator generatorName);
		OscillatorSourceImpl(settings::Settings& settings, std::vector<float> waveTable);
		~OscillatorSourceImpl();

		OscillatorSourceId getId();

	private:

		settings::Settings& _ext_settings;
		OscillatorSourceId _id;
	};
}
