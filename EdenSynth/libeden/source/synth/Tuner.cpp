/// 
/// \author Jan Wilczek
/// \date 20.11.2018
///
#include <cmath>
#include "synth/Tuner.h"
#include "eden/MidiMessage.h"

namespace eden::synth
{
	void Tuner::pitchBendChange(int pitchWheelValue)
	{
		_lastPitchBendValue = pitchWheelValue;
	}

	void Tuner::setPitchBendRange(std::pair<int, int> pitchBendRange)
	{
		_pitchBendRange = pitchBendRange;
	}

	void Tuner::setFrequencyOfA4(float frequencyOfA4)
	{
		_frequencyOfA4 = frequencyOfA4;
	}

	float Tuner::calculatePitch(int midiNoteNumber)
	{
		return MidiMessage::getMidiNoteInHertz(midiNoteNumber, _frequencyOfA4) * std::pow(2.f, getPitchBendInSemitones() / 12.f);
	}

	int Tuner::getPitchBendInSemitones()
	{
		if (_lastPitchBendValue > PITCH_BEND_NEUTRAL_VALUE)
		{
			return static_cast<float>(_lastPitchBendValue - PITCH_BEND_NEUTRAL_VALUE) / (MAX_PITCH_BEND_VALUE - PITCH_BEND_NEUTRAL_VALUE) * _pitchBendRange.second;
		}
		return static_cast<float>(_lastPitchBendValue - PITCH_BEND_NEUTRAL_VALUE) / (PITCH_BEND_NEUTRAL_VALUE - MIN_PITCH_BEND_VALUE) * _pitchBendRange.first;
	}
}
