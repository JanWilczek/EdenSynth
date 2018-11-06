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
}
