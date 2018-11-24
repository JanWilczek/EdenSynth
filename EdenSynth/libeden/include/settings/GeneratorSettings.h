#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include <memory>
#include <vector>
#include <map>
#include "eden/OscillatorParameters.h"

namespace eden::synth::wavetable
{
	class SynthOscillator;
	class SignalGenerator;
	class IOscillatorSource;
}

namespace eden::settings
{
	/// <summary>
	/// Class responsible for changing parameters of signal generators belonging to all voices.
	/// </summary>
	class GeneratorSettings
	{
	public:
		/// <summary>
		/// Registers given <paramref name="signalGenerator"/> for control.
		/// </summary>
		/// <param name="signalGenerator"></param>
		void registerSignalGenerator(std::shared_ptr<synth::wavetable::SignalGenerator> signalGenerator);

		/// <summary>
		/// Sets sample rate of all registered generators.
		/// </summary>
		/// <param name="sampleRate"></param>
		void setSampleRate(float sampleRate);

		/// <summary>
		/// Creates a new realtime oscillator source with a unique id. Each sample will be calculated exactly from phase information.
		/// </summary>
		/// <param name="sampleRate"></param>
		/// <param name="generatorName"></param>
		/// <returns>unique id of the created source</returns>
		OscillatorSourceId createGeneratorSource(float sampleRate, WaveformGenerators generatorName);

		/// <summary>
		/// Creates a new wavetable oscillator source with a unique id. 
		/// Each sample will be approximated from the given wavetable based on phase information and interpolation algorithm.
		/// </summary>
		/// <param name="sampleRate"></param>
		/// <param name="waveTable"></param>
		/// <returns>unique id of the created source</returns>
		OscillatorSourceId createWaveTableSource(float sampleRate, std::vector<float> waveTable);

		/// <summary>
		/// Removes oscillator source with the given <paramref name="sourceId"/>.
		/// One source belongs to one oscillator, so it can affect only one oscillator (all oscillators with a particular id to be exact).
		/// </summary>
		/// <param name="sourceId"></param>
		void removeOscillatorSource(OscillatorSourceId sourceId);

		/// <summary>
		/// Creates a new oscillator with the given source and adds it to the sound generation process.
		/// </summary>
		/// <param name="sourceId"></param>
		/// <returns>unique id of the created oscillator</returns>
		OscillatorId addOscillator(OscillatorSourceId sourceId);

		/// <summary>
		/// Removes oscillator with the given id.
		/// </summary>
		/// <param name="oscillatorToRemove"></param>
		void removeOscillator(OscillatorId oscillatorToRemove);

		/// <summary>
		/// Sets source of the given oscillator.
		/// </summary>
		/// <param name="oscillatorId">id of the oscillator to set the source of</param>
		/// <param name="sourceId">id of the source to set</param>
		void setOscillatorSource(OscillatorId oscillatorId, OscillatorSourceId sourceId);

		/// <summary>
		/// Sets octave transposition of the oscillator with the given id.
		/// </summary>
		/// <param name="oscillatorId"></param>
		/// <param name="octaveShift"></param>
		void setOctaveTransposition(OscillatorId oscillatorId, int octaveShift);

		/// <summary>
		/// Sets semitone transposition of the oscillator with the given id.
		/// </summary>
		/// <param name="oscillatorId"></param>
		/// <param name="semitoneShift"></param>
		void setSemitoneTransposition(OscillatorId oscillatorId, int semitoneShift);

		/// <summary>
		/// Sets cent transposition of the oscillator with the given id.
		/// </summary>
		/// <param name="oscillatorId"></param>
		/// <param name="centShift"></param>
		void setCentTransposition(OscillatorId oscillatorId, int centShift);

		/// <summary>
		/// Sets volume of the oscillator with the given id.
		/// </summary>
		/// <param name="oscillatorId"></param>
		/// <param name="volume"></param>
		void setOscillatorVolume(OscillatorId oscillatorId, float volume);

		/// <summary>
		/// Sets on or off the oscillator with the given id.
		/// </summary>
		/// <param name="oscillatorId"></param>
		/// <param name="isOn">true to turn the oscillator on, false to turn it off</param>
		void setOscillatorOn(OscillatorId oscillatorId, bool isOn);

	private:
		/// <summary>
		/// Registered signal generators.
		/// </summary>
		std::vector<std::shared_ptr<synth::wavetable::SignalGenerator>> _signalGenerators;

		/// <summary>
		/// Each new oscillator source creation will assign this value as the source's unique id and then increment it.
		/// </summary>
		OscillatorSourceId _firstAvailableSourceId = 0u;

		/// <summary>
		/// All created oscillator sources.
		/// </summary>
		std::map<OscillatorSourceId, std::unique_ptr<synth::wavetable::IOscillatorSource>> _oscillatorSources;

		/// <summary>
		/// Each new oscillator creation will assign this value as the oscillator's unique id and then increment it.
		/// </summary>
		OscillatorId _firstAvailableOscillatorId = 0u;
	};
}
