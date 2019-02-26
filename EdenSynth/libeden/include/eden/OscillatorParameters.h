#pragma once
/// 
/// \author Jan Wilczek
/// \date 12.11.2018
/// 

namespace eden
{
	/// <summary>
	/// Currently available realtime generators.
	/// </summary>
	enum class WaveformGenerator
	{
		SawtoothRampUp,
		Square
	};

	/// <summary>
	/// Global identifier of an oscillator.
	/// </summary>
	typedef unsigned long long OscillatorId;

	/// <summary>
	/// Global identifier of an oscillator source.
	/// </summary>
	typedef unsigned long long OscillatorSourceId;
}
