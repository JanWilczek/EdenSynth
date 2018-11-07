/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include "settings/SubtractiveModuleSettings.h"
#include "synth/subtractive/SubtractiveModule.h"

namespace eden::settings
{
	void SubtractiveModuleSettings::registerSubtractiveModule(std::shared_ptr<synth::subtractive::SubtractiveModule> subtractiveModule)
	{
		_subtractiveModules.push_back(subtractiveModule);
	}

	void SubtractiveModuleSettings::setCutoff(float cutoff)
	{
		if (cutoff != _cutoff)
		{
			// TODO: Set cutoff in all subtractive modules.
		}
	}

	void SubtractiveModuleSettings::setResonance(float resonance)
	{
		if (resonance != _resonance)
		{
			// TODO: Set resonance in all subtractive modules;
		}
	}
}
