#pragma once
/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 
#include <memory>
#include "eden_impl/OscillatorSourceImpl.h"
#include "eden/OscillatorParameters.h"

namespace eden
{
	enum class WaveformGenerators;

	class OscillatorSource
	{
	public:
		OscillatorSource(std::unique_ptr<OscillatorSourceImpl> impl);
		
		OscillatorSourceId getId();

	private:

		std::unique_ptr<OscillatorSourceImpl> _impl;
	};
}
