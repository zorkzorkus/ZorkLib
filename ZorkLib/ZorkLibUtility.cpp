#include "ZorkLibUtility.hpp"

namespace ZorkLib {

	namespace Utility {

		std::vector<std::wstring> GetFoldersOfPathW(std::wstring path) {

			std::vector<std::wstring> res;
			WIN32_FIND_DATAW filedata;
			std::wstring searchPath = path + L"\\*";
			HANDLE h = FindFirstFileW(searchPath.c_str(), &filedata);

			while (h != INVALID_HANDLE_VALUE) {

				std::wstring file = filedata.cFileName;

				if (filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && file != L".." && file != L".") {
					res.push_back(path + L"\\" + file); //TODO: Check if file is correct string.
				}

				if (FindNextFileW(h, &filedata) == 0) break;

			}

			return res;

		}

		std::vector<std::wstring> GetFilesOfPathW(std::wstring path) {

			std::vector<std::wstring> res;
			WIN32_FIND_DATAW filedata;
			std::wstring searchPath = path + L"\\*";
			HANDLE h = FindFirstFileW(searchPath.c_str(), &filedata);

			while (h != INVALID_HANDLE_VALUE) {

				std::wstring file = filedata.cFileName;

				if ((filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
					res.push_back(path + L"\\" + file); //TODO: Check if file is correct string.
				}

				if (FindNextFileW(h, &filedata) == 0) break;

			}

			return res;

		}

		std::vector<std::wstring> GetFilesOfPathRecursiveW(std::wstring path) {

			std::vector<std::wstring> res = GetFilesOfPathW(path);

			std::vector<std::wstring> folders = GetFoldersOfPathW(path);

			for (size_t i = 0; i < folders.size(); ++i) {
				std::vector<std::wstring> temp = GetFoldersOfPathW(folders[i]);
				folders.insert(folders.end(), temp.begin(), temp.end());
				temp = GetFilesOfPathW(folders[i]);
				res.insert(res.end(), temp.begin(), temp.end());
			}

			return res;

		}

		std::vector<std::wstring> GetFoldersOfPathRecursiveW(std::wstring path) {

			std::vector<std::wstring> folders = GetFoldersOfPathW(path);

			for (size_t i = 0; i < folders.size(); ++i) {
				std::vector<std::wstring> temp = GetFoldersOfPathW(folders[i]);
				folders.insert(folders.end(), temp.begin(), temp.end());
			}

			return folders;

		}

		std::vector<std::wstring> GetFoldersOfPathW(std::wstring path, std::function<bool(std::wstring)> filter) {

			std::vector<std::wstring> res;
			WIN32_FIND_DATAW filedata;
			std::wstring searchPath = path + L"\\*";
			HANDLE h = FindFirstFileW(searchPath.c_str(), &filedata);

			while (h != INVALID_HANDLE_VALUE) {

				std::wstring file = filedata.cFileName;

				if (filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && file != L".." && file != L".") {
					file = path + L"\\" + file; // local file does not contain full path
					if (filter(file)) {
						res.push_back(file);
					}
				}

				if (FindNextFileW(h, &filedata) == 0) break;

			}

			return res;

		}

		std::vector<std::wstring> GetFilesOfPathW(std::wstring path, std::function<bool(std::wstring)> filter) {

			std::vector<std::wstring> res;
			WIN32_FIND_DATAW filedata;
			std::wstring searchPath = path + L"\\*";
			HANDLE h = FindFirstFileW(searchPath.c_str(), &filedata);

			while (h != INVALID_HANDLE_VALUE) {

				std::wstring file = filedata.cFileName;

				if ((filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
					file = path + L"\\" + file; // local file does not contain full path
					if (filter(file)) {
						res.push_back(file);
					}
				}

				if (FindNextFileW(h, &filedata) == 0) break;

			}

			return res;

		}



		std::vector<std::wstring> GetFilesOfPathRecursiveW(std::wstring path, std::function<bool(std::wstring)> filter) {

			std::vector<std::wstring> res = GetFilesOfPathW(path, filter);

			std::vector<std::wstring> folders = GetFoldersOfPathW(path);

			for (size_t i = 0; i < folders.size(); ++i) {
				std::vector<std::wstring> temp = GetFoldersOfPathW(folders[i]);
				folders.insert(folders.end(), temp.begin(), temp.end());
				temp = GetFilesOfPathW(folders[i], filter);
				res.insert(res.end(), temp.begin(), temp.end());
			}

			return res;

		}



		std::vector<std::wstring> GetFoldersOfPathRecursiveW(std::wstring path, std::function<bool(std::wstring)> filter) {

			std::vector<std::wstring> folders = GetFoldersOfPathW(path, filter);

			for (size_t i = 0; i < folders.size(); ++i) {
				std::vector<std::wstring> temp = GetFoldersOfPathW(folders[i], filter);
				folders.insert(folders.end(), temp.begin(), temp.end());
			}

			return folders;

		}

		std::vector<std::wstring> LoadTextfileW(std::wstring path) {
			std::vector<std::wstring> res;
			std::wifstream ifs(path);
			if (ifs) {
				std::wstring in;
				while (std::getline(ifs, in)) {
					res.push_back(in);
				}
			}
			return res;
		}

		// ANSI-C Strings
		//
		//
		//
		// ANSI-C Strings

		std::vector<std::string> GetFilesOfPathA(std::string path) {

			std::vector<std::string> res;
			WIN32_FIND_DATAA filedata;
			std::string searchPath = path + "\\*";
			HANDLE h = FindFirstFileA(searchPath.c_str(), &filedata);

			while (h != INVALID_HANDLE_VALUE) {

				std::string file = filedata.cFileName;

				if ((filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
					res.push_back(path + "\\" + file); //TODO: Check if file is correct string.
				}

				if (FindNextFileA(h, &filedata) == 0) break;

			}

			return res;

		}

		std::vector<std::string> GetFoldersOfPathA(std::string path) {

			std::vector<std::string> res;
			WIN32_FIND_DATAA filedata;
			std::string searchPath = path + "\\*";
			HANDLE h = FindFirstFileA(searchPath.c_str(), &filedata);

			while (h != INVALID_HANDLE_VALUE) {

				std::string file = filedata.cFileName;

				if (filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && file != ".." && file != ".") {
					res.push_back(path + "\\" + file); //TODO: Check if file is correct string.
				}

				if (FindNextFileA(h, &filedata) == 0) break;

			}

			return res;

		}

		std::vector<std::string> GetFilesOfPathRecursiveA(std::string path) {

			std::vector<std::string> res = GetFilesOfPathA(path);

			std::vector<std::string> folders = GetFoldersOfPathA(path);

			for (size_t i = 0; i < folders.size(); ++i) {
				std::vector<std::string> temp = GetFoldersOfPathA(folders[i]);
				folders.insert(folders.end(), temp.begin(), temp.end());
				temp = GetFilesOfPathA(folders[i]);
				res.insert(res.end(), temp.begin(), temp.end());
			}

			return res;

		}

		std::vector<std::string> GetFoldersOfPathRecursiveA(std::string path) {

			std::vector<std::string> folders = GetFoldersOfPathA(path);

			for (size_t i = 0; i < folders.size(); ++i) {
				std::vector<std::string> temp = GetFoldersOfPathA(folders[i]);
				folders.insert(folders.end(), temp.begin(), temp.end());
			}

			return folders;

		}

		std::vector<std::string> GetFoldersOfPathA(std::string path, std::function<bool(std::string)> filter) {

			std::vector<std::string> res;
			WIN32_FIND_DATAA filedata;
			std::string searchPath = path + "\\*";
			HANDLE h = FindFirstFileA(searchPath.c_str(), &filedata);

			while (h != INVALID_HANDLE_VALUE) {

				std::string file = filedata.cFileName;

				if (filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && file != ".." && file != ".") {
					file = path + "\\" + file; // local file does not contain full path
					if (filter(file)) {
						res.push_back(file);
					}
				}

				if (FindNextFileA(h, &filedata) == 0) break;

			}

			return res;

		}

		std::vector<std::string> GetFilesOfPathA(std::string path, std::function<bool(std::string)> filter) {

			std::vector<std::string> res;
			WIN32_FIND_DATAA filedata;
			std::string searchPath = path + "\\*";
			HANDLE h = FindFirstFileA(searchPath.c_str(), &filedata);

			while (h != INVALID_HANDLE_VALUE) {

				std::string file = filedata.cFileName;

				if ((filedata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
					file = path + "\\" + file; // local file does not contain full path
					if (filter(file)) {
						res.push_back(file);
					}
				}

				if (FindNextFileA(h, &filedata) == 0) break;

			}

			return res;

		}

		std::vector<std::string> GetFilesOfPathRecursiveA(std::string path, std::function<bool(std::string)> filter) {

			std::vector<std::string> res = GetFilesOfPathA(path, filter);

			std::vector<std::string> folders = GetFoldersOfPathA(path);

			for (size_t i = 0; i < folders.size(); ++i) {
				std::vector<std::string> temp = GetFoldersOfPathA(folders[i]);
				folders.insert(folders.end(), temp.begin(), temp.end());
				temp = GetFilesOfPathA(folders[i], filter);
				res.insert(res.end(), temp.begin(), temp.end());
			}

			return res;

		}

		std::vector<std::string> GetFoldersOfPathRecursiveA(std::string path, std::function<bool(std::string)> filter) {

			std::vector<std::string> folders = GetFoldersOfPathA(path, filter);

			for (size_t i = 0; i < folders.size(); ++i) {
				std::vector<std::string> temp = GetFoldersOfPathA(folders[i], filter);
				folders.insert(folders.end(), temp.begin(), temp.end());
			}

			return folders;

		}

		std::vector<std::string> LoadTextfileA(std::string path) {
			std::vector<std::string> res;
			std::ifstream ifs(path);
			if (ifs) {
				std::string in;
				while (std::getline(ifs, in)) {
					res.push_back(in);
				}
			}
			return res;
		}

		std::string WidestringToString(std::wstring wstr) {
			if (wstr.empty()) {
				return std::string();
			}
#if defined WIN32
			int size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &wstr[0], wstr.size(), NULL, 0, NULL, NULL);
			std::string ret = std::string(size, 0);
			WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &wstr[0], wstr.size(), &ret[0], size, NULL, NULL);
#else
			size_t size = 0;
			_locale_t lc = _create_locale(LC_ALL, "en_US.UTF-8");
			errno_t err = _wcstombs_s_l(&size, NULL, 0, &wstr[0], _TRUNCATE, lc);
			std::string ret = std::string(size, 0);
			err = _wcstombs_s_l(&size, &ret[0], size, &wstr[0], _TRUNCATE, lc);
			_free_locale(lc);
			ret.resize(size - 1);
#endif
			return ret;
		}

		std::wstring StringToWideString(std::string str) {
			if (str.empty()) {
				return std::wstring();
			}
			size_t len = str.length() + 1;
			std::wstring ret = std::wstring(len, 0);
#if defined WIN32
			int size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, &str[0], str.size(), &ret[0], len);
			ret.resize(size);
#else
			size_t size = 0;
			_locale_t lc = _create_locale(LC_ALL, "en_US.UTF-8");
			errno_t retval = _mbstowcs_s_l(&size, &ret[0], len, &str[0], _TRUNCATE, lc);
			_free_locale(lc);
			ret.resize(size - 1);
#endif
			return ret;
		}

	}

}