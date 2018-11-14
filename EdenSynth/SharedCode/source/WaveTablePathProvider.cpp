/// 
/// \author Jan Wilczek
/// \date 14.11.2018
/// 
#include "WaveTablePathProvider.h"

namespace eden_vst
{
	WaveTablePathProvider::WaveTablePathProvider(std::experimental::filesystem::path assetsPath)
	{
		const auto path = assetsPath / "wavetables";
		
		for (auto& file : std::experimental::filesystem::directory_iterator(path))
		{
			auto name = file.path().filename().string();
			name = name.substr(0u, name.length() - 4u);	// remove ".wav" ending
			_waveTablePaths[name] = file.path();
		}
	}

	WaveTablePathProvider::path_container_iterator WaveTablePathProvider::begin()
	{
		return _waveTablePaths.begin();
	}

	WaveTablePathProvider::path_container_iterator WaveTablePathProvider::end()
	{
		return _waveTablePaths.end();
	}

	std::experimental::filesystem::path WaveTablePathProvider::getPath(std::string waveTableName)
	{
		if (_waveTablePaths.find(waveTableName) == _waveTablePaths.end())
		{
			throw new std::runtime_error("Unknown wave table name.");
		}

		return _waveTablePaths[waveTableName];
	}
}
