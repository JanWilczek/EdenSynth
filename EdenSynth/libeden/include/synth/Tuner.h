#pragma once
/// 
/// \author Jan Wilczek
/// \date 20.11.2018
///
#include <utility>

namespace eden::synth
{
	class Tuner
	{
	public:
		void pitchBendChange(int pitchBendValue);
		void setPitchBendRange(std::pair<int, int> pitchBendRange);
		void setFrequencyOfA4(float frequencyOfA4);

		/// <param name="midiNoteNumber"></param>
		/// <param name="pitchWheelPosition"></param>
		/// <returns>pitch based on given <paramref name="midiNoteNumber"><returns>
		float calculatePitch(int midiNoteNumber);
		int getPitchBendInSemitones();

	private:
		static constexpr int PITCH_BEND_NEUTRAL_VALUE = 0x2000;
		static constexpr int MAX_PITCH_BEND_VALUE = 0x3FFF;
		static constexpr int MIN_PITCH_BEND_VALUE = 0x0000;
		float _frequencyOfA4 = 440.f;
		int _lastPitchBendValue = 0;
		std::pair<int, int> _pitchBendRange = {-2, 2};
	};
}