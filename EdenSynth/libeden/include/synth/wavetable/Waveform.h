#pragma once
/// 
/// \author Jan Wilczek
/// \date 20.10.2018
/// 
#include <memory>
#include "synth/wavetable/WaveTable.h"
#include "interpolation/IInterpolator.h"

namespace eden::synth::wavetable
{
	/// <summary>
	/// Class representing a continuous periodic signal basing on given discrete values and interpolator.
	/// Its cycle's length is 2 * pi.
	/// </summary>
	class Waveform
	{
	public:
		/// <param name="waveTable">discrete values to interpolate from</param>
		/// <param name="interpolator">interpolation method to use</param>
		Waveform(WaveTable waveTable, std::shared_ptr<interpolation::IInterpolator> interpolator);
		Waveform(const Waveform&);

		/// <param name="phase"></param>
		/// <returns>interpolated function's value at specified <paramref name="phase"/></returns>
		float operator()(double phase) const;

		/// <param name="waveTable">discrete values to interpolate from upon the calls to <see cref="operator()"/></c></param>
		void setWaveTable(WaveTable waveTable);

		/// <param name="interpolator">interpolation method to use - linear, polynomial, spline etc.</param>
		void setInterpolator(std::shared_ptr<interpolation::IInterpolator> interpolator);

	private:
		/// <summary>
		/// Discrete values to interpolate from.
		/// </summary>
		WaveTable _waveTable;

		/// <summary>
		/// Interpolation method.
		/// </summary>
		std::shared_ptr<interpolation::IInterpolator> _interpolator;
	};
}
