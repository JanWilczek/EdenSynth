#pragma once
/// 
/// \author Jan Wilczek
/// \date 02.09.2018
/// 
#include "eden/AudioBuffer.h"
#include "synth/envelope/EnvelopeGenerator.h"
#include "synth/subtractive/SubtractiveModule.h"
#include "synth/waveshaping/WaveshapingModule.h"
#include "synth/wavetable/SignalGenerator.h"

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
		explicit Voice(double sampleRate);

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

		void pitchWheelMoved(int newPitchWheelValue);

		/// <returns>true if the voice is active (renders a note in <c>renderBlock()</c>, false otherwise</returns>
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

		void finalizeVoice();

	private:
		/// <summary>
		/// Generates a single sample.
		/// </summary>
		/// <param name="outputBuffer"></param>
		/// <param name="startSample">the index of sample to render, will be incremented by 1</param>
		//void generateSample(AudioBuffer& outputBuffer, int& startSample);

		double calculatePitch(int midiNoteNumber, int pitchWheelPosition);
		void setPitch(double newPitch);
		void applyVelocity(AudioBuffer& outputBuffer, int startSample, int samplesToRender);
		void duplicateMonoChannel(AudioBuffer& outputBuffer, int channelToDuplicate, int startSample, int samplesToCopy);

		double _sampleRate;
		bool _isActive = false;
		//AudioBuffer::SampleType currentAngle = 0.0;
		//AudioBuffer::SampleType angleDelta = 0.0;
		//AudioBuffer::SampleType level = 0.0;
		//AudioBuffer::SampleType tailOff = 0.0;
		std::unique_ptr<wavetable::SignalGenerator> _signalGenerator;
		std::unique_ptr<subtractive::SubtractiveModule> _subtractiveModule;
		std::unique_ptr<waveshaping::WaveshapingModule> _waveshapingModule;
		std::unique_ptr<envelope::EnvelopeGenerator> _envelopeGenerator;
		int _currentNote = -1;
		AudioBuffer::SampleType _velocity = 0.f;
	};
}
