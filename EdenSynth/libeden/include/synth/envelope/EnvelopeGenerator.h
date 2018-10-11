#pragma once
/// 
/// \author Jan Wilczek
/// \date 08.10.2018
/// 
#include <chrono>
#include "eden/AudioBuffer.h"
#include <map>

namespace eden
{
	class AudioBuffer;

	namespace synth
	{
		class Voice;
	}
}

namespace eden::synth::envelope
{
	class EnvelopeGenerator
	{
	public:
		enum class CurveShape
		{
			Linear,
			Exponential
		};

		enum class EnvelopePhase
		{
			Attack,
			Decay1,
			Decay2,
			Release,
			Silent
		};

		explicit EnvelopeGenerator(Voice& voice);

		void applyEnvelope(AudioBuffer::SampleType* audioChannel, int startSample, int samplesToProcess);
		void attack();
		void release();

		void setSampleRate(double sampleRate);
		void setPhaseTime(EnvelopePhase phase, std::chrono::milliseconds time);
		double calculateLinearPerSampleGain(std::chrono::milliseconds timeInterval, double initialLevel, double finalLevel);
		double calculateExponentialPerSampleGain(std::chrono::milliseconds timeInterval, double initialLevel, double finalLevel);

	private:
		struct PhaseData
		{
			std::chrono::milliseconds phaseDuration;
			double initialValue;
			double finalValue;
			double linearPerSampleGain;
			double exponentialPerSampleGain;
		};

		void increaseGain();
		void changePhase(EnvelopePhase phase);
		void checkAndChangePhase();
		void initializePhase(EnvelopePhase phase, PhaseData data);
		
		const double BASE_VALUE = 0.0;
		const double PEAK_VALUE = 1.0;

		Voice* _voice;
		double _currentEnvelopeValue;
		double _breakValue = 0.7;
		double _gainPerSample;
		EnvelopePhase _currentPhase;
		CurveShape _curveShape = CurveShape::Linear;
		double _sampleRate;

		std::map<EnvelopePhase, PhaseData> _phasesData;
	};
}
