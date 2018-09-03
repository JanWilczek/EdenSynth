#pragma once

namespace eden
{
	class AudioBuffer;
}

namespace eden::synth
{
	class Voice
	{
	public:
		void startNote(int midiNoteNumber, float velocity, int currentPitchWheelPosition);
		void stopNote(float velocity, bool allowTailOff);
		void renderBlock(AudioBuffer& outputBuffer, int startSample, int samplesToRender);

		bool isPlaying() const noexcept;
		bool isPlayingNote(const int midiNoteNumber) const noexcept;

		double getSampleRate() const noexcept;
		void setSampleRate(double newSampleRate);

	private:
		void generateSample(AudioBuffer& outputBuffer, int& startSample);

	private:
		double _sampleRate;
		double currentAngle = 0.0;
		double angleDelta = 0.0;
		double level = 0.0;
		double tailOff = 0.0;
		int _currentNote = -1;
	};
}