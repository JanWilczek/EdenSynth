/// 
/// \author Jan Wilczek
/// \date 08.10.2018
/// 
#include <cmath>
#include "synth/envelope/EnvelopeGenerator.h"
#include "synth/Voice.h"
#include "utility/EdenAssert.h"

namespace eden::synth::envelope
{
	EnvelopeGenerator::EnvelopeGenerator(Voice& voice)
		: _voice(&voice)
		, _sampleRate(_voice->getSampleRate())
	{
		using namespace std::chrono_literals;
		initializePhase(EnvelopePhase::Attack, { 500ms, BASE_VALUE, PEAK_VALUE, 0.0, 0.0 });
		initializePhase(EnvelopePhase::Decay1, { 100ms, PEAK_VALUE, _breakValue, 0.0, 0.0 });
		initializePhase(EnvelopePhase::Decay2, { 100000ms, _breakValue, BASE_VALUE, 0.0, 0.0 });
		initializePhase(EnvelopePhase::Release, { 500ms, _breakValue, BASE_VALUE, 0.0, 0.0 });
		initializePhase(EnvelopePhase::Silent, { 0ms, BASE_VALUE, BASE_VALUE, 0.0, 0.0 });
	}

	void EnvelopeGenerator::applyEnvelope(SampleType* audioChannel, int startSample, int samplesToProcess)
	{
		for (auto sample = startSample; sample < startSample + samplesToProcess; ++sample)
		{
			audioChannel[sample] *= static_cast<SampleType>(_currentEnvelopeValue);
			increaseGain();
			checkAndChangePhase();
		}
	}
	void EnvelopeGenerator::attack()
	{
		changePhase(EnvelopePhase::Attack);
	}

	void EnvelopeGenerator::release()
	{
		changePhase(EnvelopePhase::Release);
	}

	void EnvelopeGenerator::setPhaseTime(EnvelopePhase phase, std::chrono::milliseconds time)
	{
		_phasesData[phase].linearPerSampleGain = calculateLinearPerSampleGain(time, _phasesData[phase].initialValue, _phasesData[phase].finalValue);
		_phasesData[phase].exponentialPerSampleGain = calculateExponentialPerSampleGain(time, _phasesData[phase].initialValue, _phasesData[phase].finalValue);
	}

	double EnvelopeGenerator::calculateLinearPerSampleGain(std::chrono::milliseconds timeInterval, double initialLevel, double finalLevel)
	{
		const auto samplesInterval = static_cast<unsigned>(std::chrono::duration_cast<std::chrono::seconds>(timeInterval).count() * _sampleRate);

		/*
		 * newLevel = oldLevel + gainPerSample;
		 * finalLevel = initialLevel + samplesInterval * gainPerSample;
		 * gainPerSample = (finalLevel - initialLevel) / samplesInterval;
		 */
		const auto linearGainPerSample = (finalLevel - initialLevel) / samplesInterval;

		return linearGainPerSample;
	}

	double EnvelopeGenerator::calculateExponentialPerSampleGain(std::chrono::milliseconds timeInterval, double initialLevel, double finalLevel)
	{
		const auto samplesInterval = static_cast<unsigned>(std::chrono::duration_cast<std::chrono::seconds>(timeInterval).count() * _sampleRate);

		/*
		* newLevel = oldLevel * gainPerSample;
		* finalLevel = initialLevel * (gainPerSample ^ samplesInterval);
		* gainPerSample = std::pow(finalLevel / initialLevel, 1/samplesInterval);
		*/
		const auto exponentialGainPerSample = std::pow(finalLevel / initialLevel, 1 / samplesInterval);

		return exponentialGainPerSample;
	}

	void EnvelopeGenerator::increaseGain()
	{
		switch (_curveShape)
		{
		case CurveShape::Linear:
			_currentEnvelopeValue += _gainPerSample;
			break;
		case CurveShape::Exponential:
			_currentEnvelopeValue *= _gainPerSample;
			break;
		default:
			EDEN_ASSERT(false);
		}
	}

	void EnvelopeGenerator::changePhase(EnvelopePhase phase)
	{
		_currentPhase = phase;

		switch (_curveShape)
		{
		case CurveShape::Linear:
			_gainPerSample = _phasesData[_currentPhase].linearPerSampleGain;
			break;
		case CurveShape::Exponential:
			_gainPerSample = _phasesData[_currentPhase].exponentialPerSampleGain;
			break;
		default:
			EDEN_ASSERT(false);
		}
	}

	void EnvelopeGenerator::checkAndChangePhase()
	{
		switch (_currentPhase)
		{
		case EnvelopePhase::Attack:
			if (_currentEnvelopeValue >= PEAK_VALUE)
			{
				changePhase(EnvelopePhase::Decay1);
			}
			break;
		case EnvelopePhase::Decay1:
			if (_currentEnvelopeValue <= _breakValue)
			{
				changePhase(EnvelopePhase::Decay2);
			}
			break;
		case EnvelopePhase::Release:
			if (_currentEnvelopeValue <= BASE_VALUE)
			{
				changePhase(EnvelopePhase::Silent);
				_voice->finalizeVoice();
			}
			break;
		case EnvelopePhase::Decay2:
			if (_currentEnvelopeValue <= BASE_VALUE)
			{
				changePhase(EnvelopePhase::Silent);
				_voice->finalizeVoice();
			}
		case EnvelopePhase::Silent:
		default:
			break;
		}
	}

	void EnvelopeGenerator::initializePhase(EnvelopePhase phase, PhaseData data)
	{
		_phasesData[phase] = data;
	}

}
