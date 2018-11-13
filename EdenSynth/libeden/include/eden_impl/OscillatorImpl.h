#pragma once
/// 
/// \author Jan Wilczek
/// \date 11.11.2018
/// 
#include <memory>
#include "eden/OscillatorParameters.h"
#include "eden/OscillatorSource.h"

namespace eden
{
	namespace settings
	{
		class Settings;
	}

	class OscillatorImpl
	{
	public:
		OscillatorImpl(settings::Settings& settings, std::unique_ptr<OscillatorSource> source);
		~OscillatorImpl();

		OscillatorId getId() const noexcept;

		void setSource(std::unique_ptr<OscillatorSource> source);

		void setOctaveTransposition(int octaveShift);
		void setSemitoneTransposition(int semitoneShift);
		void setCentTransposition(int centShift);

	private:
		settings::Settings& _ext_settings;
		OscillatorId _id;
		std::unique_ptr<OscillatorSource> _oscillatorSource;
	};
}