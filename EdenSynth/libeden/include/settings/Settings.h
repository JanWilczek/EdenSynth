#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include <memory>
#include "settings/EnvelopeSettings.h"
#include "settings/GeneratorSettings.h"
#include "settings/SubtractiveModuleSettings.h"
#include "settings/WaveshapingModuleSettings.h"
#include "eden/OscillatorParameters.h"
#include "eden/FilterParameters.h"
#include "settings/Tuner.h"

namespace eden
{
	struct EnvelopeParameters;

	namespace synth 
	{
		namespace wavetable
		{
			class SignalGenerator;
			class IOscillatorSource;
		}

		namespace subtractive
		{
			class SubtractiveModule;
		}

		namespace waveshaping
		{
			class WaveshapingModule;
		}

		namespace envelope
		{
			class EnvelopeGenerator;
		}
	}
}

namespace eden::settings
{
	/// <summary>
	/// A facade class providing an interface for all settings modules: generator, subtractive, waveshaping and envelope.
	/// </summary>
	class Settings
	{
	public:
		Settings();

		/// <summary>
		/// Stores the sample rate. It does not change it in any of the modules.
		/// </summary>
		/// <param name="sampleRate"></param>
		void setSampleRate(float sampleRate) noexcept;

		/// <returns>currently stored sample rate</returns>
		float sampleRate() const noexcept;

		std::shared_ptr<Tuner> tuner() const noexcept;

		void setFrequencyOfA4(float frequencyOfA4);

		void setPitchBendRange(std::pair<int, int> transposeDownTransposeUp);

		/// <summary>
		/// Registers a SignalGenerator module. Each voice should register its generator in settings.
		/// </summary>
		/// <param name="signalGenerator"></param>
		void registerSignalGenerator(std::shared_ptr<synth::wavetable::SignalGenerator> signalGenerator);

		/// <summary>
		/// Registers a SubtractiveModule. Each voice should register its subtractive module in settings.
		/// </summary>
		/// <param name="subtractiveModule"></param>
		void registerSubtractiveModule(std::shared_ptr<synth::subtractive::SubtractiveModule> subtractiveModule);

		/// <summary>
		/// Registers a WaveshapingModule. Each voice should register its waveshaping module in settings.
		/// </summary>
		/// <param name="waveshapingModule"></param>
		void registerWaveshapingModule(std::shared_ptr<synth::waveshaping::WaveshapingModule> waveshapingModule);

		/// <summary>
		/// Registers an envelope. Each voice should register its envelope in settings.
		/// </summary>
		/// <param name="envelope"></param>
		void registerEnvelope(std::shared_ptr<synth::envelope::EnvelopeGenerator> envelope);

		/// <summary>
		/// Creates and registers a realtime oscillator source.
		/// </summary>
		/// <param name="generatorName">name of the generator to create</param>
		/// <returns>unique id representing the createed oscillator source</returns>
		OscillatorSourceId createGeneratorSource(WaveformGenerators generatorName);

		/// <summary>
		/// Creates and registers a wavetable oscillator source.
		/// </summary>
		/// <param name="waveTable">one cycle of a waveform to replay</param>
		/// <returns>unique id representing the createed oscillator source</returns>
		OscillatorSourceId createWaveTableSource(std::vector<float> waveTable);

		/// <summary>
		/// Removes oscillator source with the given id.
		/// </summary>
		/// <param name="sourceId"></param>
		void removeOscillatorSource(OscillatorSourceId sourceId);

		/// <summary>
		/// Adds an oscillator to each voice using the source as pointed by <paramref name="sourceId"/>.
		/// </summary>
		/// <param name="sourceId">id of the oscillator sound to create the oscillator with (can be later changed)</param>
		/// <returns>unique id representing the created oscillator</returns>
		OscillatorId addOscillator(OscillatorSourceId sourceId);

		/// <summary>
		/// Removes oscillator with the given id.
		/// </summary>
		/// <param name="oscillatorToRemove"></param>
		void removeOscillator(OscillatorId oscillatorToRemove);

		/// <summary>
		/// Sets the source represented by <paramref name="sourceId"/> of the oscillator represented by <paramref name="oscillatorId"/>.
		/// </summary>
		/// <param name="oscillatorId"></param>
		/// <param name="sourceId"></param>
		void setOscillatorSource(OscillatorId oscillatorId, OscillatorSourceId sourceId);

		/// <summary>
		/// Sets octave transposition of the given oscillator. E.g. -1 means that the oscillator will play a pitch octave lower than with <paramref name="octaveShift"/> equal to 0.
		/// </summary>
		/// <param name="oscillatorId"></param>
		/// <param name="octaveShift"></param>
		void setOctaveTransposition(OscillatorId oscillatorId, int octaveShift);

		/// <summary>
		/// Sets semitone transposition of the given oscillator. E.g. +2 means that the oscillator will play a tone higher than with <paramref name="semitoneShift"/> equal to 0.
		/// </summary>
		/// <param name="oscillatorId"></param>
		/// <param name="semitoneShift"></param>
		void setSemitoneTransposition(OscillatorId oscillatorId, int semitoneShift);

		/// <summary>
		/// Sets cent transposition of the given oscillator. E.g. +50 means that the oscillator will play a quarter tone higher than with <paramref name="centShift"/> equal to 0.
		/// </summary>
		/// <param name="oscillatorId"></param>
		/// <param name="centShift"></param>
		void setCentTransposition(OscillatorId oscillatorId, int centShift);

		/// <summary>
		/// Sets the volume of the given oscillator.
		/// </summary>
		/// <param name="oscillatorId"></param>
		/// <param name="volume">volume in range [0; 1]</param>
		void setOscillatorVolume(OscillatorId oscillatorId, float volume);

		/// <summary>
		/// Sets the given oscillator on or off.
		/// </summary>
		/// <param name="oscillatorId"></param>
		/// <param name="isOn">true to turn the oscillator on, false to turn it off</param>
		void setOscillatorOn(OscillatorId oscillatorId, bool isOn);

		/// <summary>
		/// Sets the cutoff frequency of the low-pass filter.
		/// </summary>
		/// <param name="cutoff">number of harmonic to set the cutoff frequency at (1 is the fundamental), may be non-integer</param>
		void setCutoff(float cutoff);

		/// <summary>
		/// Sets the resonance or Q of the low-pass filter.
		/// </summary>
		/// <param name="resonance"></param>
		void setResonance(float resonance);

		/// <summary>
		/// Sets the attenuation of the filter in the pass-band. May be 12 dB per octave or 24 dB per octave.
		/// </summary>
		/// <param name="passbandAttenuation"></param>
		void setPassbandAttenuation(PassbandAttenuation passbandAttenuation);

		/// <summary>
		/// Sets the envelope parameters of the envelope generator.
		/// The envelope type is based on which instance of parameters the passed argument represents.
		/// </summary>
		/// <param name="envelopeParameters"></param>
		void setEnvelopeParameters(std::shared_ptr<EnvelopeParameters> envelopeParameters);

	private:
		/// <summary>
		/// The only default sample rate value in the whole project.
		/// </summary>
		float _sampleRate = 48000.f;

		/// <summary>
		/// Tuner contains information about global pitch settings, i.e. frequency of A4 note and pitch bend.
		/// </summary>
		std::shared_ptr<Tuner> _tuner;

		/// <summary>
		/// Particular synthesis modules which actually handle all the settings.
		/// </summary>
		std::unique_ptr<GeneratorSettings> _generatorSettings;
		std::unique_ptr<SubtractiveModuleSettings> _subtractiveModuleSettings;
		std::unique_ptr<WaveshapingModuleSettings> _waveshapingModuleSettings;
		std::unique_ptr<EnvelopeSettings> _envelopeSettings;
	};
}
