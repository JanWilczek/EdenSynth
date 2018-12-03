#pragma once
/// 
/// \author Jan Wilczek
/// \date 25.11.2018
/// 
#include "synth/subtractive/OnePoleLowPassFilter.h"
#include "eden/FilterParameters.h"

namespace eden::synth::subtractive
{
	/// <summary>
	/// Virtual analog implementation of Moog ladder filter.
	/// Based on an article "Oscillator and Filter Algorithms for Virtual Analog Synthesis" by Vesa Välimäki and Antti Huovilainen.
	/// </summary>
	class MoogFilter
	{
	public:
		MoogFilter(float sampleRate);

		/// <summary>
		/// Filter one sample of audio.
		/// </summary>
		/// <param name="sample"></param>
		/// <returns>filtered sample</returns>
		float processSample(float sample);

		/// <summary>
		/// Sets the cutoff frequency of the filter (frequency at which attenuation is 3 dB).
		/// </summary>
		/// <param name="cutoffFrequency">frequency in Hz1</param>
		void setCutoffFrequency(float cutoffFrequency);

		/// <summary>
		/// Sets the gain boost at the cutoff frequency.
		/// </summary>
		/// <param name="resonance">boost in range [0, 1]</param>
		void setResonance(float resonance);

		/// <summary>
		/// Sets the attenuation of the filter in the pass-band. May be 12 dB per octave or 24 dB per octave.
		/// </summary>
		/// <param name="passbandAttenuation"></param>
		void setPassbandAttenuation(PassbandAttenuation passbandAttenuation);

		void setSampleRate(float sampleRate);

	private:
		/// <summary>
		/// Calculates <see cref="_gRes"/> based on  <see cref="_cutoffFrequency"/>, <see cref="_sampleRate"/> and <see cref="_resonance"/>.
		/// </summary>
		void calculateGRes();

		float _sampleRate;

		/// <summary>
		/// Subsequent one pole filters. Signal taken from between them constitutes the output signal.
		/// </summary>
		OnePoleLowPassFilter _afterA;
		OnePoleLowPassFilter _afterB;
		OnePoleLowPassFilter _afterC;
		OnePoleLowPassFilter _afterD;

		/// <summary>
		/// Factors with which each signal output contributes to the final output. parameters <c>_a</c>, <c>_b</c> and <c>_d</c> may be used to create
		/// filters other than low-pass, but this possibility is not currently implemented.
		/// </summary>
		const float _a = 0.f;
		const float _b = 0.f;
		float _c;
		const float _d = 0.f;
		float _e;

		/// <summary>
		/// Frequency in Hz at which gain falls to -3 dB.
		/// </summary>
		float _cutoffFrequency;

		/// <summary>
		/// Size of the resonant peak in frequency spectrum at the cutoff frequency.
		/// </summary>
		float _resonance;

		float _gRes;
		const float _gComp = 0.5f;

		float _feedbackSample;
	};
}
