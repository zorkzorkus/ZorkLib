#pragma once

#include <string>
#include <vector>
#include <queue>
#include <Windows.h>
#include <functional>

namespace ZorkLib {

	namespace Utility {

		float SetMinMax(float input, float min, float max);

		std::vector<std::wstring> SplitString(std::wstring input, std::wstring splitter);

		std::vector<std::string> GetFoldersOfPath(std::string path);
		std::vector<std::string> GetFilesOfPath(std::string path);
		std::vector<std::string> GetFilesOfPathRecursive(std::string path);
		std::vector<std::string> GetFoldersOfPathRecursive(std::string path);
		std::vector<std::string> GetFoldersOfPath(std::string path, std::function<bool(std::string)> filter);
		std::vector<std::string> GetFilesOfPath(std::string path, std::function<bool(std::string)> filter);
		std::vector<std::string> GetFilesOfPathRecursive(std::string path, std::function<bool(std::string)> filter);
		std::vector<std::string> GetFoldersOfPathRecursive(std::string path, std::function<bool(std::string)> filter);

	}

}