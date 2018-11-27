#pragma once
/// 
/// \author Jan Wilczek
/// \date 04.11.2018
/// 
#include <chrono>
#include "float.h"

namespace eden
{
	using namespace std::chrono_literals;

	enum class EnvelopeType
	{
		ADBDR,
		ADSR
	};

	/// <summary>
	/// Enum determining the shape of the envelope curve in particular segment.
	/// Names refer to curve names on a plot with linear scales.
	/// The exponential gain sounds linear to the human ear, since it is linear on the dB scale.
	/// </summary>
	enum class EnvelopeSegmentCurve
	{
		Linear = 0,
		Exponential = 1
	};
	
	/// <summary>
	/// Base struct to pass polymorphic envelope parameters in.
	/// </summary>
	struct EnvelopeParameters
	{
		EnvelopeParameters(EnvelopeType _type)
			: type(_type)
		{}

		virtual ~EnvelopeParameters() = 0;
		EnvelopeType getType() const { return type; }

	protected:
		EnvelopeType type;
	};
	inline EnvelopeParameters::~EnvelopeParameters(){}


	/// <summary>
	/// Parameters necessary to control an Attack Decay1 Break Decay2 Release envelope. Such an envelope is 
	/// appropriate for simulating piano-like envelope when Decay2 time is very long.
	/// The break level parameter specifies at what amplitude the envelope should pass from
	/// Decay1 to Decay2 segment.
	/// </summary>
	struct ADBDRParameters final : public EnvelopeParameters
	{
		ADBDRParameters()
			: EnvelopeParameters(EnvelopeType::ADBDR)
		{}
		
		ADBDRParameters(std::chrono::milliseconds _attackTime, EnvelopeSegmentCurve _attackCurve,
			std::chrono::milliseconds _decay1Time, EnvelopeSegmentCurve _decay1Curve,
			std::chrono::milliseconds _decay2Time, EnvelopeSegmentCurve _decay2Curve,
			std::chrono::milliseconds _releaseTime, EnvelopeSegmentCurve _releaseCurve,
			float _breakLevel) 
			: EnvelopeParameters(EnvelopeType::ADBDR)
			, attackTime(_attackTime), attackCurve(_attackCurve),
		decay1Time(_decay1Time), decay1Curve(_decay1Curve), decay2Time(_decay2Time), decay2Curve(_decay2Curve),
		releaseTime(_releaseTime), releaseCurve(_releaseCurve), breakLevel(_breakLevel)
		{}

		~ADBDRParameters(){}

		std::chrono::milliseconds attackTime = 50ms;
		EnvelopeSegmentCurve attackCurve = EnvelopeSegmentCurve::Exponential;

		std::chrono::milliseconds decay1Time = 100ms;
		EnvelopeSegmentCurve decay1Curve = EnvelopeSegmentCurve::Exponential;
		
		std::chrono::milliseconds decay2Time = 20000ms;
		EnvelopeSegmentCurve decay2Curve = EnvelopeSegmentCurve::Exponential;

		std::chrono::milliseconds releaseTime = 1000ms;
		EnvelopeSegmentCurve releaseCurve = EnvelopeSegmentCurve::Exponential;

		float breakLevel = float(0.8f);
	};

	struct ADSRParameters final : public EnvelopeParameters
	{
		ADSRParameters()
			: EnvelopeParameters(EnvelopeType::ADSR)
		{}

		std::chrono::milliseconds attackTime = 50ms;
		EnvelopeSegmentCurve attackCurve = EnvelopeSegmentCurve::Exponential;

		std::chrono::milliseconds decayTime = 100ms;
		EnvelopeSegmentCurve decayCurve = EnvelopeSegmentCurve::Exponential;

		float sustainLevel = 0.7f;

		std::chrono::milliseconds releaseTime = 1000ms;
		EnvelopeSegmentCurve releaseCurve = EnvelopeSegmentCurve::Exponential;
	};
}
