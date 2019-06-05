#include "ZorkLibUtility.hpp"

namespace ZorkLib {

	namespace Utility {

		// Returns input if input is in interval [min, max] - otherwise it returns the breached interval boundary.
		float SetMinMax(float input, float min, float max) {
			if (input < min) return min;
			if (input > max) return max;
			return input;
		}



		// Splits a wstring input with wstring splitter - the substrings are returned in a vector - if there is no occurence of splitter an empty vector is returned.
		std::vector<std::wstring> SplitString(std::wstring input, std::wstring splitter) {

			std::vector<std::wstring> res;
			std::wstring front;
			size_t pos;

			pos = input.find_first_of(splitter);

			if (pos == input.npos) return res;

			while (pos != input.npos) {
				front = input;
				front.erase(pos, front.size() - pos);
				res.push_back(front);
				input.erase(0, pos + splitter.size());
				pos = input.find_first_of(splitter);
			}

			res.push_back(input);

			return res;
		}



		std::vector<std::string> GetFoldersOfPath(std::string path) {

			std::vector<std::string> res;
			WIN32_FIND_DATA filedata;
			std::string searchPath = path + "\\*";
			HANDLE h = FindFirstFile(searchPath.c_str(), &filedata);

			while (h != INVALID_HANDLE_VALUE) {

				std::string file = filedata.cFileName;

				if (filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && file != ".." && file != ".") {
					res.push_back(path + "\\" + file); //TODO: Check if file is correct string.
				}

				if (FindNextFile(h, &filedata) == 0) break;

			}

			return res;

		}



		std::vector<std::string> GetFilesOfPath(std::string path) {

			std::vector<std::string> res;
			WIN32_FIND_DATA filedata;
			std::string searchPath = path + "\\*";
			HANDLE h = FindFirstFile(searchPath.c_str(), &filedata);

			while (h != INVALID_HANDLE_VALUE) {

				std::string file = filedata.cFileName;

				if ((filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
					res.push_back(path + "\\" + file); //TODO: Check if file is correct string.
				}

				if (FindNextFile(h, &filedata) == 0) break;

			}

			return res;

		}



		std::vector<std::string> GetFilesOfPathRecursive(std::string path) {

			std::vector<std::string> res = GetFilesOfPath(path);

			std::vector<std::string> folders = GetFoldersOfPath(path);

			for (size_t i = 0; i < folders.size(); ++i) {
				std::vector<std::string> temp = GetFoldersOfPath(folders[i]);
				folders.insert(folders.end(), temp.begin(), temp.end());
				temp = GetFilesOfPath(folders[i]);
				res.insert(res.end(), temp.begin(), temp.end());
			}

			return res;

		}



		std::vector<std::string> GetFoldersOfPathRecursive(std::string path) {

			std::vector<std::string> folders = GetFoldersOfPath(path);

			for (size_t i = 0; i < folders.size(); ++i) {
				std::vector<std::string> temp = GetFoldersOfPath(folders[i]);
				folders.insert(folders.end(), temp.begin(), temp.end());
			}

			return folders;

		}


		std::vector<std::string> GetFoldersOfPath(std::string path, std::function<bool(std::string)> filter) {

			std::vector<std::string> res;
			WIN32_FIND_DATA filedata;
			std::string searchPath = path + "\\*";
			HANDLE h = FindFirstFile(searchPath.c_str(), &filedata);

			while (h != INVALID_HANDLE_VALUE) {

				std::string file = filedata.cFileName;

				if (filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && file != ".." && file != ".") {
					if (filter(file)) {
						res.push_back(path + "\\" + file); //TODO: Check if file is correct string.
					}
				}

				if (FindNextFile(h, &filedata) == 0) break;

			}

			return res;

		}



		std::vector<std::string> GetFilesOfPath(std::string path, std::function<bool(std::string)> filter) {

			std::vector<std::string> res;
			WIN32_FIND_DATA filedata;
			std::string searchPath = path + "\\*";
			HANDLE h = FindFirstFile(searchPath.c_str(), &filedata);

			while (h != INVALID_HANDLE_VALUE) {

				std::string file = filedata.cFileName;

				if ((filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
					if (filter(file)) {
						res.push_back(path + "\\" + file); //TODO: Check if file is correct string.
					}
				}

				if (FindNextFile(h, &filedata) == 0) break;

			}

			return res;

		}



		std::vector<std::string> GetFilesOfPathRecursive(std::string path, std::function<bool(std::string)> filter) {

			std::vector<std::string> res = GetFilesOfPath(path, filter);

			std::vector<std::string> folders = GetFoldersOfPath(path);

			for (size_t i = 0; i < folders.size(); ++i) {
				std::vector<std::string> temp = GetFoldersOfPath(folders[i]);
				folders.insert(folders.end(), temp.begin(), temp.end());
				temp = GetFilesOfPath(folders[i], filter);
				res.insert(res.end(), temp.begin(), temp.end());
			}

			return res;

		}



		std::vector<std::string> GetFoldersOfPathRecursive(std::string path, std::function<bool(std::string)> filter) {

			std::vector<std::string> folders = GetFoldersOfPath(path, filter);

			for (size_t i = 0; i < folders.size(); ++i) {
				std::vector<std::string> temp = GetFoldersOfPath(folders[i], filter);
				folders.insert(folders.end(), temp.begin(), temp.end());
			}

			return folders;

		}

	}

}