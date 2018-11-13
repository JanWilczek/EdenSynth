#pragma once
/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 

namespace eden
{
	enum class WaveformGenerators
	{
		Sine,
		Triangle,
		SawtoothRampUp,
		Square,
		Pulse10Cycle,
		Pulse20Cycle,
		Pulse30Cycle,
		Pulse40Cycle,
		WhiteNoise
	};

	typedef unsigned long long OscillatorId;
	typedef unsigned long long OscillatorSourceId;
}
