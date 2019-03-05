/// 
/// \author Jan Wilczek
/// \date 05.03.2019
/// 
#include "synth/Mixer.h"
#include "eden/AudioBuffer.h"

namespace eden::synth
{
	Mixer::Mixer(size_t nb_voices)
		: _renderedVoices(nb_voices)
	{}

	const float*& Mixer::operator[](size_t voice)
	{
		return _renderedVoices[voice];
	}

	void Mixer::mixTo(AudioBuffer& outputBuffer, int startSample, int samplesToMix)
	{
		outputBuffer.forEachChannel([this, &startSample, &samplesToMix](float* channel)
		{
			for (auto renderedVoice : _renderedVoices)
			{
				if (renderedVoice)	// the voice may have been inactive
				{
					for (auto sample = 0; sample < samplesToMix; ++sample)
					{
						channel[sample + startSample] += renderedVoice[sample];
					}
				}
			}
		});
	}
}
