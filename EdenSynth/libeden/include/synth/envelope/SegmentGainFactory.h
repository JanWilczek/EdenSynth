#pragma once
/// 
/// \author Jan Wilczek
/// \date 04.11.2018
/// 
#include <memory>

namespace eden
{
	enum class EnvelopeSegmentCurve;
}

namespace eden::synth::envelope
{
	class ISegmentGain;

	class SegmentGainFactory
	{
	public:
		static std::unique_ptr<ISegmentGain> createSegmentGain(EnvelopeSegmentCurve segmentCurve);
	};
}
