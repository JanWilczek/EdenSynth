/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "settings/WaveshapingModuleSettings.h"
#include "synth/waveshaping/WaveshapingModule.h"

namespace eden::settings
{
	void WaveshapingModuleSettings::registerWaveshapingModule(std::shared_ptr<synth::waveshaping::WaveshapingModule> waveshapingModule)
	{
		_waveshapingModules.push_back(waveshapingModule);
	}
}