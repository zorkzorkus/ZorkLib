#pragma once

#include "ZorkLibInternalDefines.hpp"
#include "ZorkLibUtility.hpp"

#include <cmath>
#include <map>
#include <vector>

// struct class Point
// struct class Line
// struct class Rectangle
// struct class Ellipse
// class Color
// struct DirectXAccess

namespace ZorkLib {

	// ------------------
	// struct class Point
	// ------------------

	struct Point {

	public:

		// Constructors
		Point();
		Point(float x, float y);

		// Copy Constructors
		Point(const Point& ref);
		Point(const Point* ref);

		// Memberfunctions
		void Move(float x, float y);
		void Move(Point p);

		// Conversion operators
		operator D2D1_POINT_2F();

		// Operators
		Point operator+(const Point& other);
		Point& operator+=(const Point& other);
		Point operator-();
		Point operator-(const Point& other);
		Point& operator-=(const Point& other);

	public:

		float x, y;

	};

	// -----------------
	// struct class Line
	// -----------------

	struct Line {

	public:

		// Constructors
		Line();
		Line(float ax, float ay, float bx, float by);
		Line(Point a, Point b);

		// Copy Constructors
		Line(const Line& ref);
		Line(const Line* ref);

		// Memberfunctions
		void Move(float x, float y);
		void Move(Point p);
		float Length();

		// Conversion operators
		///

		// Operators
		Line operator+(const Point& point);
		Line& operator+=(const Point& point);
		Line operator-(const Point& point);
		Line& operator-=(const Point& point);

	public:

		Point a, b;

	};

	// ----------------------
	// struct class Rectangle
	// ----------------------

	struct Rectangle {

	public:

		// Constructors
		Rectangle();
		Rectangle(float left, float top, float right, float bottom);
		Rectangle(Point topLeft, Point bottomRight);
		Rectangle(Line line);
		Rectangle(Bitmap bmp);

		// Copy Constructors
		Rectangle(const Rectangle& ref);
		Rectangle(const Rectangle* ref);

		// Memberfunctions
		void Scale(float scale, float centerX, float centerY);
		void Scale(float x, float y, float centerX, float centerY);
		void Scale(float scale);
		void Scale(float x, float y);
		void ShiftIn(float x, float y);
		void ShiftOut(float x, float y);
		void Move(float x, float y);
		void Move(Point p);
		float Width();
		float Height();
		Point Center();

		// Conversion operators
		operator D2D1_RECT_F();

		// Operators
		Rectangle operator+(const Point& point);
		Rectangle& operator+=(const Point& point);
		Rectangle operator-(const Point& point);
		Rectangle& operator-=(const Point& point);

	public:

		static Rectangle RectMainScreen();
		static Rectangle RectDesktop();
		static Rectangle RectCentered(Point center, float width, float height);
		static Rectangle RectCentered(float x, float y, float width, float height);
		static Rectangle Box(float leftUpperX, float leftUpperY, float width, float height);
		static Rectangle Box(Point upperLeft, float width, float height);

	public:

		float left, top, right, bottom;

	};

	// -------------------
	// struct class Circle
	// -------------------

	struct Ellipse {

	public:

		// Constructors
		Ellipse();
		Ellipse(float x, float y, float radX, float radY);
		Ellipse(Point center, float radX, float radY);
		Ellipse(float x, float y, float radius);
		Ellipse(Point center, float radius);
		Ellipse(Rectangle rect);

		// Copy Constructors
		Ellipse(const Ellipse& ref);
		Ellipse(const Ellipse* ref);

		// Memberfunctions
		void Move(float x, float y);
		void Move(Point p);

		// Conversion operators
		operator D2D1_ELLIPSE();

		// Operators
		Ellipse operator+(const Point& point);
		Ellipse& operator+=(const Point& point);
		Ellipse operator-(const Point& point);
		Ellipse& operator-=(const Point& point);


	public:

		Point center;
		float radiusX;
		float radiusY;

	};

	// -----------
	// class Color
	// -----------

	enum class ColorEnum : UINT32 {
		// Enums are 0xAARRGGBB (Alpha, Red, Green, Blue) format
		// In memory this will look like BGRA since x64-architecture is little endian.
		Black = 0xFF000000,
		DarkGray = 0xFF3F3F3F,
		Gray = 0xFF7F7F7F,
		LightGray = 0xFFBFBFBF,
		White = 0xFFFFFFFF,
		DarkRed = 0xFF7F0000,
		Red = 0xFFFF0000,
		LightRed = 0xFFFF7F7F,
		Orange = 0xFFFF7F00,
		Yellow = 0xFFFFFF00,
		Lime = 0xFF7FFF00,
		Green = 0xFF00FF00,
		Cyan = 0xFF00FFFF,
		Blue = 0xFF007FFF,
		Marine = 0xFF0000FF,
		Purple = 0xFF7F00FF,
		Magenta = 0xFFFF00FF,
		Brown = 0xFF7F3F00
	};

	class Color {

	public:

		Color();
		Color(const Color& other);
		Color(const Color* other);
		Color(ColorEnum ce, float opacity = 1.f);
		Color(float red, float green, float blue, float opacity = 1.f);
		Color(int red, int green, int blue, int opacity = 0xFF); // only supply values from 0x00 to 0xFF
		Color(UINT32 color);

		operator D2D1_COLOR_F();

		Color operator+(const Color& other);
		Color& operator+=(const Color& other);
		Color operator*(const Color& other);
		Color& operator*=(const Color& other);
		Color operator*(const float& scale);
		Color& operator*=(const float& scale);

		UINT32 GetHex();

		float GetRed();
		float GetGreen();
		float GetBlue();
		float GetAlpha();

		void SetRed(float red);
		void SetGreen(float green);
		void SetBlue(float blue);
		void SetAlpha(float alpha);

	private:

		float m_Red;
		float m_Green;
		float m_Blue;
		float m_Alpha;

	};

	// --------------------
	// struct DirectXAccess
	// --------------------

	struct DirectXAccess {

		// DXGI
		HWND hWnd;
		IDXGISwapChain* pSwapChain;
		IDXGIDevice* pDXGIDevice;
		IDXGISurface* pBackBuffer;

		// Direct3D
		ID3D11Device* pD3Device;
		ID3D11DeviceContext* pD3DeviceContext;

		// Direct2D
		ID2D1Factory7* pD2Factory;
		ID2D1Device6* pDevice;
		ID2D1DeviceContext6* pContext;
		ID2D1Bitmap1* pTargetBitmap;
		std::map<UINT32, ID2D1SolidColorBrush*> BrushMap;

		// IWIC
		IWICImagingFactory2* pIWICFactory;

		// DirectWrite
		IDWriteFactory7* pWriteFactory;
		std::vector<IDWriteTextFormat3*> pTextFormatsCenter;
		std::vector<IDWriteTextFormat3*> pTextFormatsLeft;

	};

	enum class TextAlignmentEnum : UINT32 {
		UpperLeft, Center
	};

	// Yes, we are creating 1000 IDWriteTextFormats here...
	constexpr UINT32 gc_MaxFontSize = 500;

}