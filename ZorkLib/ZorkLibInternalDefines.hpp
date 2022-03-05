#pragma once

#pragma comment(lib, "d2d1")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "windowscodecs")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "Shlwapi")
#pragma comment(lib, "dwmapi")

#ifndef UNICODE
#define UNICODE
#endif

//Windows Header
#include <Windows.h>
#include <windowsx.h>
#include <dxgi1_6.h>
#include <d3d11_4.h>
#include <d2d1_3.h>
#include <wincodec.h>
#include <dwrite_3.h>
#include <Shlwapi.h>
#include <dwmapi.h>

#include <memory>
#include <string>

#undef PlaySound
#undef LoadBitmap

namespace ZorkLib {

	enum class BitmapExtension {
		BMP, PNG, JPG
	};

	template <typename T>
	using Pointer = std::shared_ptr<T>;

	using Bitmap = ID2D1Bitmap1 * ;
	using SolidColorBrush = ID2D1SolidColorBrush * ;

}