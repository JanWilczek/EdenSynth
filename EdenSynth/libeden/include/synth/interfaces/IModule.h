#pragma once
///
///	\author Jan Wilczek 
///	 \date 08.10.2018
///

namespace eden
{
	class AudioBuffer;
}

namespace eden::synth
{
	/// <summary>
	/// Interface of a sound synthesis module.
	/// </summary>
	class IModule
	{
	public:
		virtual ~IModule() = 0;

		/// <summary>
		/// Processes the audio buffer according to the set parameters.
		/// </summary>
		/// <param name="audioBuffer"></param>
		/// <param name="startSample"></param>
		/// <param name="samplesToRender"></param>
		virtual void process(eden::AudioBuffer& audioBuffer, int startSample, int samplesToProcess) = 0;
	};

	inline IModule::~IModule()
	{	
	}
}
