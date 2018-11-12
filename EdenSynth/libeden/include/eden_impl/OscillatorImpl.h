#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.11.2018
/// 
#include <memory>
#include "eden/OscillatorParameters.h"

namespace eden
{
	class OscillatorSource;

	namespace settings
	{
		class Settings;
	}

	class OscillatorImpl
	{
	public:
		OscillatorImpl(settings::Settings& settings, std::unique_ptr<OscillatorSource> source);

		void setOctaveTransposition(int octaveShift);
		void setSemitoneTransposition(int semitoneShift);
		void setCentTransposition(int centShift);

	private:
		settings::Settings& _ext_settings;
		OscillatorId _id;
	};
}