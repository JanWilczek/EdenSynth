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

		void setCutoff(float cutoff);
		void setResonance(float resonance);

	private:
		std::vector<std::shared_ptr<synth::subtractive::SubtractiveModule>> _subtractiveModules;
		float _cutoff;
		float _resonance;
	};
}
