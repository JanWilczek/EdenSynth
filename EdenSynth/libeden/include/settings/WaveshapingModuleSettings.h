#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include <memory>
#include <vector>

namespace eden::synth::waveshaping
{
	class WaveshapingModule;
}

namespace eden::settings
{
	class WaveshapingModuleSettings
	{
	public:
		void registerWaveshapingModule(std::shared_ptr<synth::waveshaping::WaveshapingModule> waveshapingModule);

	private:
		std::vector<std::shared_ptr<synth::waveshaping::WaveshapingModule>> _waveshapingModules;
	};
}
