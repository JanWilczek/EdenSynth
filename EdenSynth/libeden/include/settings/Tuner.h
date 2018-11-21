#pragma once
/// 
/// \author Jan Wilczek
/// \date 20.11.2018
///
#include <utility>

namespace eden::settings
{
	class Tuner
	{
	public:
		static constexpr int PITCH_BEND_NEUTRAL_VALUE = 0x2000;
		static constexpr int MAX_PITCH_BEND_VALUE = 0x3FFF;
		static constexpr int MIN_PITCH_BEND_VALUE = 0x0000;

		void setPitchBendRange(std::pair<int, int> pitchBendRange);
		void setFrequencyOfA4(float frequencyOfA4);

		float calculatePitch(int midiNoteNumber, int pitchBendValue);
		int getPitchBendInSemitones(int pitchBendValue);

	private:
		float _frequencyOfA4 = 440.f;
		std::pair<int, int> _pitchBendRange = {-2, 2};
	};
}