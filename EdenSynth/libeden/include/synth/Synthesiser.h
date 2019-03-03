#pragma once
/// 
/// \author Jan Wilczek
/// \date 02.09.2018
/// 
#include <vector>
#include <memory>
#include "synth/Voice.h"
#include "utility/threading/ThreadPool.h"

namespace eden
{
	class AudioBuffer;
	class MidiBuffer;
	class MidiMessage;
	struct EnvelopeParameters;

	namespace settings
	{
		class Settings;
	}
}

namespace eden::synth
{
	/// <summary>
	/// Class responsible for generating sound based on provided input.
	/// </summary>
	class Synthesiser
	{
	public:
		static constexpr unsigned NB_VOICES = 64u;

		explicit Synthesiser(settings::Settings& settings);

		/// <summary>
		/// Fills the given audio buffer with samples in full based on internal state.
		/// </summary>
		/// <param name="bufferToFill"></param>
		/// <param name="midiBuffer"></param>
		/// <param name="numSamples"></param>
		void processBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer, int numSamples);

		/// <summary>
		/// Fills the given audio buffer with samples from <paramref name="startSample"/> (including) to <paramref name="startSample"/> + <paramref name="numSamples"/> (excluding).
		/// </summary>
		/// <param name="bufferToFill"></param>
		/// <param name="midiBuffer"></param>
		/// <param name="startSample"></param>
		/// <param name="numSamples"></param>
		void processBlock(AudioBuffer& bufferToFill, MidiBuffer& midiBuffer, int startSample, int numSamples);

		/// <summary>
		/// Sets the expected length of processing block - use it to allocate memory beforehand.
		/// </summary>
		/// <param name="samplesPerBlock"></param>
		void setBlockLength(unsigned samplesPerBlock);

		/// <summary>
		/// Sets the overall volume of the synthesiser.
		/// </summary>
		/// <param name="volume">volume in range [0; 1]</param>
		void setVolume(float volume);

	private:
		class IVoiceRenderer
		{
		public:
			virtual ~IVoiceRenderer() = 0;
			virtual void renderVoices(Synthesiser& synthesiser, AudioBuffer& outputBuffer, int startSample, int samplesToProcess) = 0;
		};

		class SynchronousVoiceRenderer : public IVoiceRenderer
		{
		public:
			~SynchronousVoiceRenderer() override = default;
			void renderVoices(Synthesiser& synthesiser, AudioBuffer& outputBuffer, int startSample, int samplesToProcess) override;
		};

		class AsynchronousVoiceRenderer : public IVoiceRenderer
		{
		public:
			~AsynchronousVoiceRenderer() override = default;
			void renderVoices(Synthesiser& synthesiser, AudioBuffer& outputBuffer, int startSample, int samplesToProcess) override;

		private:
			utility::threading::ThreadPool _threadPool;
		};

		/// <summary>
		/// Changes internal state based on received MIDI message. E.g. starts playing a note after note on message.
		/// </summary>
		/// <param name="midiMessage"></param>
		void handleMidiMessage(MidiMessage& midiMessage);

		/// <summary>
		/// Render all voices - both active and inactive.
		/// </summary>
		/// <param name="outputBuffer"></param>
		/// <param name="startSample"></param>
		/// <param name="samplesToProcess"></param>
		void renderVoices(AudioBuffer& outputBuffer, int startSample, int samplesToProcess);

		/// <summary>
		/// Handle note on event.
		/// </summary>
		/// <param name="midiChannel"></param>
		/// <param name="midiNoteNumber"></param>
		/// <param name="velocity"></param>
		void noteOn(const int midiChannel, const int midiNoteNumber, const float velocity);

		/// <summary>
		/// Handle note off event.
		/// </summary>
		/// <param name="midiChannel"></param>
		/// <param name="midiNoteNumber"></param>
		/// <param name="velocity"></param>
		void noteOff(const int midiChannel, const int midiNoteNumber, const float velocity);

		/// <summary>
		/// Handles changes of value of the pitch bend wheel.
		/// </summary>
		/// <param name="midiChannel"></param>
		/// <param name="pitchBendValue"></param>
		void pitchBendChange(const int midiChannel, const int pitchBendValue);

		/// <summary>
		/// Adds voices given number of voices to the synthesiser. More voices means, that more notes can be played at once.
		/// </summary>
		/// <param name="numVoicesToAdd"></param>
		void addVoices(settings::Settings& settings, unsigned numVoicesToAdd);

		/// <returns>first free voice</returns>
		Voice* getFreeVoice();

		/// <param name="midiNoteNumber"></param>
		/// <returns>voice that is playing given note, nullptr if there is no such voice</returns>
		Voice* getVoicePlayingNote(const int midiNoteNumber);

		/// <summary>
		/// Applies global volume to the buffer in range [<paramref name="startSample"/>; <paramref name="startSample"/> + <paramref name="numSamples"/>).
		/// </summary>
		/// <param name="bufferToFill"></param>
		/// <param name="startSample"></param>
		/// <param name="numSamples"></param>
		void applyVolume(AudioBuffer& bufferToFill, int startSample, int numSamples);

		/// <summary>
		/// Container with voices.
		/// </summary>
		std::vector<std::unique_ptr<Voice>> _voices;

		std::unique_ptr<IVoiceRenderer> _voiceRenderer;

		/// <summary>
		/// Size of the inner audio channel of each voice.
		/// </summary>
		unsigned _blockLength = 480u;

		/// <summary>
		/// Current synthesiser volume. Range: [0; 1].
		/// </summary>
		float _volume = 1.0f;
	};
}
