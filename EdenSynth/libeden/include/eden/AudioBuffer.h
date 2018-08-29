#pragma once

namespace eden
{
	/// <summary>
	/// A container of samples used to output the synthesized sound.
	/// </summary>
	class AudioBuffer
	{
	public:
		typedef float SampleType;

		AudioBuffer() = default;

		unsigned int getNumChannels() const noexcept;
		unsigned int getNumSamples() const noexcept;

		void clear();
	};
}