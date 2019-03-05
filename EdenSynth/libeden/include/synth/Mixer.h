#pragma once
/// 
/// \author Jan Wilczek
/// \date 05.03.2019
/// 
#include <vector>

namespace eden
{
	class AudioBuffer;
}

namespace eden::synth
{
	class Mixer
	{
	public:
		explicit Mixer(size_t nb_voices);

		const float*& operator[](size_t channel);
		void mixTo(AudioBuffer& outputBuffer, int startSample, int samplesToMix);

	private:
		std::vector<const float*> _renderedVoices;
	};
}
