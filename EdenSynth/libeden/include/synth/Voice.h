#pragma once
/// 
/// \author Jan Wilczek
/// \date 02.09.2018
/// 
#include <memory>
#include "eden/AudioBuffer.h"
#include "synth/subtractive/SubtractiveModule.h"
#include "synth/waveshaping/WaveshapingModule.h"
#include "synth/wavetable/SignalGenerator.h"
#include "settings/Tuner.h"
#include "envelope/EnvelopeGenerator.h"

namespace eden::settings
{
	class Settings;
}

namespace eden::synth
{
	/// <summary>
	/// Class representing a synthesiser voice which can be thought of as a single note on the keyboard.
	/// It contains all necessary elements of a typical synthesizer:
	/// 1. Source (i.e. waveform)
	/// 2. Modifier (i.e. subtractive and waveshaping modules)
	/// 3. Performance control
	/// </summary>
	class Voice
	{
	public:
		explicit Voice(settings::Settings& settings);
		Voice() = delete;

		/// <summary>
		/// Starts rendering a voice in effect of e.g. note on MIDI event.
		/// </summary>
		/// <param name="midiNoteNumber"></param>
		/// <param name="velocity"></param>
		/// <param name="currentPitchWheelPosition"></param>
		void startNote(int midiNoteNumber, float velocity);

		/// <summary>
		/// Stops playing note. It should only be called if <c>startNote()</c> method has been invoked earlier.
		/// </summary>
		/// <param name="velocity"></param>
		void stopNote(float velocity);

		/// <summary>
		/// Renders given number of samples of the played note. 
		/// </summary>
		/// <param name="samplesToRender">number of samples to render</param>
		/// <returns>a pointer to the block of rendered samples, nullptr if no note is being played</returns>
		const float* renderBlock(int samplesToRender);

		/// <param name="pitchBendValue">pitch bend value as specified in the MIDI standard. <see cref="settings::Tuner::getPitchBendInSemitones(int)"> for predefined pitch bend values</param>
		void setPitchBend(int pitchBendValue);

		/// <returns>true if the voice is active (renders a note in <c>renderBlock()</c>), false otherwise</returns>
		bool isPlaying() const noexcept;

		/// <returns>true if the voice is currently rendering a particular note, false otherwise</returns>
		bool isPlayingNote(const int midiNoteNumber) const noexcept;

		/// <summary>
		/// Sets the expected length of processing block. Must be called at least once before first call to <c>renderBlock()</c>.
		/// </summary>
		/// <param name="samplesPerBlock"></param>
		void setBlockLength(unsigned samplesPerBlock);

		/// <summary>
		/// Makes the voice available for a new note. Typically should be called when the envelope's release has ended.
		/// </summary>
		void finalizeVoice();

		/// <returns>current gain value by which every voice is scaled</returns>
		constexpr float gainValue() noexcept;

	private:
		/// <summary>
		/// Sets pitch of the voice.
		/// </summary>
		/// <param name="newPitch">frequency in Hz</param>
		void setPitch(float newPitch);

		/// <summary>
		/// Applies current velocity to the given <paramref name="channel"/>.
		/// </summary>
		/// <param name="channel"></param>
		/// <param name="startSample"></param>
		/// <param name="samplesToRender"></param>
		void applyVelocity(float* channel, int startSample, int samplesToRender);

		/// <summary>
		/// Applies gain that is equal and constant for all voices.
		/// </summary>
		/// <param name="channel"></param>
		/// <param name="startSample"></param>
		/// <param name="samplesToRender"></param>
		void applyGain(float* channel, int startSample, int samplesToRender);

		/// <summary>
		/// Performs all necessary actions before voice rendering.
		/// </summary>
		/// <param name="startSample"></param>
		/// <param name="samplesToRender"></param>
		void prepareToRender(int startSample, int samplesToRender);

		/// <summary>
		/// Registers all internal modules in <paramref name="settings"/>.
		/// </summary>
		/// <param name="settings"></param>
		void registerModules(settings::Settings& settings);

		/// <summary>
		/// Inner audio channel the voice is rendered to.
		/// </summary>
		AudioBuffer _innerBuffer;

		/// <summary>
		/// Responsible for rendering the initial signal.
		/// </summary>
		std::shared_ptr<wavetable::SignalGenerator> _signalGenerator;

		/// <summary>
		/// Responsible for filtering the signal.
		/// </summary>
		std::shared_ptr<subtractive::SubtractiveModule> _subtractiveModule;

		/// <summary>
		/// Responsible for waveshaping the signal.
		/// </summary>
		std::shared_ptr<waveshaping::WaveshapingModule> _waveshapingModule;

		/// <summary>
		/// Responsible for applying envelope to the signal. It holds the information about the current level of rendered samples.
		/// </summary>
		std::shared_ptr<envelope::EnvelopeGenerator> _envelopeGenerator;

		/// <summary>
		/// Tuner contains information about instrument pitch and pitch bend range.
		/// Use it to convert note number to frequency.
		/// </summary>
		std::shared_ptr<settings::Tuner> _tuner;

		/// <summary>
		/// This information if necessary if a new note is pressed and a new voice has to be launched whilst the pitch wheel is being moved.
		/// Then it starts to play with this value as pitch bend value.
		/// </summary>
		int _lastPitchBendValue;

		/// <summary>
		/// Currently played note by this voice. -1 means that no note is being played and voice is free to play a new one.
		/// </summary>
		int _currentNote = -1;

		/// <summary>
		/// Velocity with which the key was pressed.
		/// </summary>
		float _velocity = 0.f;
	};
}
