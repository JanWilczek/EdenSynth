#pragma once
/// 
/// \author Jan Wilczek
/// \date 08.10.2018
/// 
#include <map>
#include "eden/OscillatorParameters.h"
#include "synth/wavetable/SynthOscillator.h"

namespace eden::synth::wavetable
{
	/// <summary>
	/// Signal generator module. Responsible for creating the initial signal, which
	/// can be further modified. It is the "source" part of the "source and modifier"
	/// synthesis idiom.
	/// </summary>
	class SignalGenerator
	{
	public:
		/// <summary>
		/// Creates the generator with the default sine timbre.
		/// </summary>
		SignalGenerator() = default;

		/// <summary>
		/// Fills the <paramref name="audioChannel"> from <paramref name="startSample">
		/// to <paramref name="startSample"> + <paramref name="samplesToGenerate">(excluding)
		/// with currently set timbre at currently set pitch.
		/// Whatever has been on the channel before will be overwritten.
		/// </summary>
		/// <param name="audioChannel">channel to fill in specified range</param>
		/// <param name="startSample">the first index to fill</param>
		/// <param name="samplesToGenerate">number of samples to generate</param>
		void generateSignal(SampleType* audioChannel, int startSample, int samplesToGenerate);

		/// <summary>
		/// Stops generating the signal - all subsequent calls to <c>generateSignal</c> will fill the channel
		/// with zeros until new pitch is set.
		/// </summary>
		void stop();

		/// <summary>
		/// Sets the pitch of the generator. From now on all calls to generateSignal will fill the
		/// given channel with samples at specified pitch.
		/// </summary>
		/// <param name="pitch"></param>
		void setPitch(float pitch);

		void setPitchBend(int pitchBendInSemitones);

		void addOscillator(SynthOscillator oscillator);
		void removeOscillator(OscillatorId oscillatorToRemove);

		void setOscillatorSource(OscillatorId oscillatorId, std::unique_ptr<IOscillatorSource> source);
		void setOctaveTransposition(OscillatorId oscillatorId, int octaveShift);
		void setSemitoneTransposition(OscillatorId oscillatorId, int semitoneShift);
		void setCentTransposition(OscillatorId oscillatorId, int centShift);
		void setOscillatorVolume(OscillatorId oscillatorId, float volume);
		void setOscillatorOn(OscillatorId oscillatorId, bool isOn);

		void setSampleRate(double sampleRate);

	private:
		/// <summary>
		/// Generates one sample at specified sample index and overwrites the pre-existing value with new one
		/// based on current pitch. Advances in phase.
		/// </summary>
		/// <param name="audioChannel"></param>
		/// <param name="sampleIndex"></param>
		void generateSample(SampleType* audioChannel, int sampleIndex);

		std::map<OscillatorId, SynthOscillator> _oscillators;
		std::pair<float, float> _pitchBendRange;
	};
}
