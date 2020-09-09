#pragma once

#include <string>
#include <vector>
#include <queue>
#include <Windows.h>
#include <functional>
#include <fstream>

#ifdef ZORKLIB_USE_UNICODE_MACROS
#ifdef UNICODE
#define GetFilesOfPath GetFilesOfPathW
#define GetFoldersOfPath GetFoldersOfPathW
#define GetFilesOfPathRecursive GetFilesOfPathRecursiveW
#define GetFoldersOfPathRecursive GetFoldersOfPathRecursiveW
#define LoadTextfile LoadTextfileW
#else // UNICODE
#define GetFilesOfPath GetFilesOfPathA
#define GetFoldersOfPath GetFoldersOfPathA
#define GetFilesOfPathRecursive GetFilesOfPathRecursiveA
#define GetFoldersOfPathRecursive GetFoldersOfPathRecursiveA
#define LoadTextfile LoadTextfileA
#endif // UNICODE
#endif // ZORKLIB_USE_UNICODE_MACROS

namespace ZorkLib {

	namespace Utility {

		// Returns input if input is in interval [min, max] - otherwise it returns the overstepped interval boundary.
		template <typename T>
		T MinMax(T input, T min, T max) {
			if (input < min) return min;
			if (input > max) return max;
			return input;
		}

		template <typename T>
		T Min(T a, T b) {
			return (a < b) ? a : b;
		}

		template <typename T>
		T Max(T a, T b) {
			return (a > b) ? a : b;
		}

		template <typename T>
		T Wrap(T in, T min, T max) {
			while (in < min) in += max - min;
			while (in >= max) in -= max - min;
			return in;
		}

		inline float Round(float in, float roundTo) {
			float f = Wrap(in, 0.f, roundTo);
			if (2 * f >= roundTo) {
				return in - f + roundTo;
			} else {
				return in - f;
			}
		}

		inline UINT32 Round(UINT32 in, UINT32 roundTo) {
			UINT32 m = in % roundTo;
			if (2 * m >= roundTo) {
				return in - m + roundTo;
			} else {
				return in - m;
			}
		}

		// Splits a string input with string splitter - the substrings are returned in a vector - if there is no occurence of splitter an empty vector is returned.
		template <typename basic_string>
		std::vector<basic_string> SplitString(basic_string input, basic_string splitter) {

			std::vector<basic_string> res;
			basic_string front;
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

		std::vector<std::wstring> GetFilesOfPathW(std::wstring path);
		std::vector<std::wstring> GetFoldersOfPathW(std::wstring path);
		std::vector<std::wstring> GetFilesOfPathRecursiveW(std::wstring path);
		std::vector<std::wstring> GetFoldersOfPathRecursiveW(std::wstring path);
		std::vector<std::wstring> GetFilesOfPathW(std::wstring path, std::function<bool(std::wstring)> filter);
		std::vector<std::wstring> GetFoldersOfPathW(std::wstring path, std::function<bool(std::wstring)> filter);
		std::vector<std::wstring> GetFilesOfPathRecursiveW(std::wstring path, std::function<bool(std::wstring)> filter);
		std::vector<std::wstring> GetFoldersOfPathRecursiveW(std::wstring path, std::function<bool(std::wstring)> filter);
		std::vector<std::wstring> LoadTextfileW(std::wstring path);
		void SaveTextfileW(std::wstring path, std::vector<std::wstring> text);
		std::vector<UINT8> LoadBinaryFileW(std::wstring file);
		void SaveBinaryFileW(std::wstring file, std::vector<UINT8> data);

		std::vector<std::string> GetFilesOfPathA(std::string path);
		std::vector<std::string> GetFoldersOfPathA(std::string path);
		std::vector<std::string> GetFilesOfPathRecursiveA(std::string path);
		std::vector<std::string> GetFoldersOfPathRecursiveA(std::string path);
		std::vector<std::string> GetFilesOfPathA(std::string path, std::function<bool(std::string)> filter);
		std::vector<std::string> GetFoldersOfPathA(std::string path, std::function<bool(std::string)> filter);
		std::vector<std::string> GetFilesOfPathRecursiveA(std::string path, std::function<bool(std::string)> filter);
		std::vector<std::string> GetFoldersOfPathRecursiveA(std::string path, std::function<bool(std::string)> filter);
		std::vector<std::string> LoadTextfileA(std::string path);
		void SaveTextfileA(std::string path, std::vector<std::string> text);
		std::vector<UINT8> LoadBinaryFileA(std::string file);
		void SaveBinaryFileA(std::string file, std::vector<UINT8> data);


		std::string WideStringToString(std::wstring ws);
		std::wstring StringToWideString(std::string s);


		bool IsKeyDown(UINT32 key);


	}

}