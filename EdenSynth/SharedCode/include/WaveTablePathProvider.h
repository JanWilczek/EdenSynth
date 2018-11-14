#pragma once
/// 
/// \author Jan Wilczek
/// \date 14.11.2018
/// 
#include <filesystem>
#include <map>

namespace eden_vst
{
	class WaveTablePathProvider
	{
	public:
		typedef std::map<std::string, std::experimental::filesystem::path> path_container;
		typedef std::map<std::string, std::experimental::filesystem::path>::iterator path_container_iterator;

		explicit WaveTablePathProvider(std::experimental::filesystem::path assetsPath);
		path_container_iterator begin();
		path_container_iterator end();
		std::experimental::filesystem::path getPath(std::string waveTableName);

	private:
		path_container _waveTablePaths;
	};
}
