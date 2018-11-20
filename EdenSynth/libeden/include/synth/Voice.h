#pragma once
/// 
/// \author Jan Wilczek
/// \date 02.09.2018
/// 
#include "eden/AudioBuffer.h"
#include "synth/envelope/Envelope.h"
#include "synth/subtractive/SubtractiveModule.h"
#include "synth/waveshaping/WaveshapingModule.h"
#include "synth/wavetable/SignalGenerator.h"

namespace eden::settings
{
	class Settings;
}

namespace eden::synth
{
	/// <summary>
	/// Class representing a synthesiser voice which can be thought of as a single note on the keyboard.
	/// It contains all nevessary elements of a typical synthesizer:
	/// 1. Source (i.e. waveform)
	/// 2. Modifier (i.e. subtractive and waveshaping modules)
	/// 3. Performance control
	/// </summary>
	class Voice
	{
	public:
		Voice(settings::Settings& settings, double sampleRate);
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
		/// Renders played note to the given audio buffer. 
		/// If the voice plays no note it does not perform any action. 
		/// If the voice is active it adds its own segment (starting from <paramref name="startSample"/> to <paramref name="startSample"/> + <paramref name="samplesToRender"/> 
		/// on top of previously rendered voices.
		/// </summary>
		/// <param name="outputBuffer"></param>
		/// <param name="startSample">sample index in the buffer to start rendering from</param>
		/// <param name="samplesToRender">number of samples to render</param>
		void renderBlock(AudioBuffer& outputBuffer, int startSample, int samplesToRender);

		void setPitchBend(int pitchBendInSemitones);

		/// <returns>true if the voice is active (renders a note in <c>renderBlock()</c>), false otherwise</returns>
		bool isPlaying() const noexcept;

		/// <returns>true if the voice is currently rendering a particular note, false otherwise</returns>
		bool isPlayingNote(const int midiNoteNumber) const noexcept;

		/// <returns>currently used sample rate</returns>
		double getSampleRate() const noexcept;

		/// <summary>
		/// Sets new sample rate. May be costly because of internal filters recalculation.
		/// </summary>
		/// <param name="newSampleRate"></param>
		void setSampleRate(double newSampleRate);

		/// <summary>
		/// Sets the expected length of processing block. Must be called at least once before first call to <c>renderBlock()</c>.
		/// </summary>
		/// <param name="samplesPerBlock"></param>
		void setBlockLength(unsigned samplesPerBlock);

		/// <summary>
		/// Makes the voice available for a new note. Typically should be called when the envelope's release has ended.
		/// </summary>
		void finalizeVoice();

		/// <param name="midiNoteNumber"></param>
		/// <param name="pitchWheelPosition"></param>
		/// <returns>pitch based on given <paramref name="midiNoteNumber"> and <paramref name="pitchWheelPosition"></returns>
		double calculatePitch(int midiNoteNumber, int pitchWheelPosition);

		/// <returns>current gain value by which every voice is scaled</returns>
		SampleType gainValue() const noexcept;

	private:
		/// <summary>
		/// Sets pitch of the voice.
		/// </summary>
		/// <param name="newPitch">frequency in Hz</param>
		void setPitch(float newPitch);

		/// <summary>
		/// Applies current velocity to the given <paramref name="channel">.
		/// </summary>
		/// <param name="channel"></param>
		/// <param name="startSample"></param>
		/// <param name="samplesToRender"></param>
		void applyVelocity(SampleType* channel, int startSample, int samplesToRender);

		/// <summary>
		/// Mixes the contentt of the inner rendered block to the output buffer.
		/// </summary>
		/// <param name="outputBuffer"></param>
		/// <param name="startSample"></param>
		/// <param name="samplesToMix"></param>
		void mixTo(AudioBuffer& outputBuffer, int startSample, int samplesToMix);

		/// <summary>
		/// Performs all necessary actions before voice rendering.
		/// </summary>
		/// <param name="startSample"></param>
		/// <param name="samplesToRender"></param>
		void prepareToRender(int startSample, int samplesToRender);

		/// <summary>
		/// Registers all internal modules in <paramref name="settings">.
		/// </summary>
		/// <param name="settings"></param>
		void registerModules(settings::Settings& settings);

		double _sampleRate;

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
		std::shared_ptr<envelope::Envelope> _envelopeGenerator;

		/// <summary>
		/// Currently played note by this voice. -1 means that no note is being played and voice is free to play a new one.
		/// </summary>
		int _currentNote = -1;

		/// <summary>
		/// Velocity with which the key was pressed.
		/// </summary>
		SampleType _velocity = 0.f;
	};
}
