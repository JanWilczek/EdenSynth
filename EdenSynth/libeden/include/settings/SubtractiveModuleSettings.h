#pragma once
/// 
/// \author Jan Wilczek
/// \date 06.11.2018
/// 
#include <memory>
#include <vector>

namespace eden::synth::subtractive
{
	class SubtractiveModule;
}

namespace eden::settings
{
	class SubtractiveModuleSettings
	{
	public:
		void registerSubtractiveModule(std::shared_ptr<synth::subtractive::SubtractiveModule> subtractiveModule);

	private:
		std::vector<std::shared_ptr<synth::subtractive::SubtractiveModule>> _subtractiveModules;
	};
}