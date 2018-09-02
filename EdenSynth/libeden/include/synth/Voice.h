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
		void renderBlock(AudioBuffer& outputBuffer, int startSample, int samplesToRender);
	};
}