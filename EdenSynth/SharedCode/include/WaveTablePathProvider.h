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
		typedef std::map<std::string, std::experimental::filesystem::path>::const_iterator path_container_const_iterator;

		explicit WaveTablePathProvider(std::experimental::filesystem::path assetsPath);
		size_t size() const;
		path_container_iterator begin();
		path_container_iterator end();
		path_container_const_iterator cbegin() const;
		path_container_const_iterator cend() const;
		std::experimental::filesystem::path getPath(std::string waveTableName);

		std::string indexToName(size_t waveTableIndex);
		size_t nameToIndex(std::string waveTableName);

	private:
		path_container _waveTablePaths;
	};
}
