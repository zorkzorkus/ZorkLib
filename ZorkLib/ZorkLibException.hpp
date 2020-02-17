#pragma once

#include "ZorkLibInternalDefines.hpp"

namespace ZorkLib {

	namespace Exception {

		struct Exception {
			Exception(std::wstring msg) : Message(msg) {}
			const std::wstring Message;
		};

		struct Fatal : Exception {
			Fatal(std::wstring msg) : Exception(msg) {}
		};

		struct HResult : Exception {
			HResult(std::wstring msg, HRESULT hr) : Exception(msg), hr(hr) {}
			HRESULT hr;
		};

		struct Unimplemented : Exception {
			Unimplemented(std::wstring msg) : Exception(msg) {}
		};

		inline void AbortIfFailure(bool cont, std::wstring msg) {
			if (!cont) {
				throw Fatal(msg);
			}
		}

		inline void AbortIfFailure(HRESULT hr, std::wstring msg) {
			if (FAILED(hr)) {
				throw HResult(msg, hr);
			}
		}

	}

}