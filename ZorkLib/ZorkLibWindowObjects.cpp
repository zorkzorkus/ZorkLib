#include "ZorkLibWindowObjects.hpp"

namespace ZorkLib {

	// ------------
	// struct Point
	// ------------

	Point::Point() : x(0.f), y(0.f) {}

	Point::Point(float x, float y) : x(x), y(y) {}

	Point::Point(const Point& ref) : x(ref.x), y(ref.y) {}

	Point::Point(const Point* ref) : x(ref->x), y(ref->y) {}

	void Point::Move(float x, float y) {
		this->x += x;
		this->y += y;
	}

	void Point::Move(Point p) {
		x += p.x;
		y += p.y;
	}

	Point::operator D2D1_POINT_2F() {
		return D2D1::Point2F(x, y);
	}

	Point Point::operator+(const Point& other) {
		Point result = this;
		result += other;
		return result;
	}

	Point& Point::operator+=(const Point& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	// -----------
	// struct Line
	// -----------

	Line::Line() {}

	Line::Line(float ax, float ay, float bx, float by) : a(ax, ay), b(bx, by) {}

	Line::Line(Point a, Point b) : a(a), b(b) {}

	Line::Line(const Line& ref) : a(ref.a), b(ref.b) {}

	Line::Line(const Line* ref) : a(ref->a), b(ref->b) {}

	void Line::Move(float x, float y) {
		a.Move(x, y);
		b.Move(x, y);
	}

	void Line::Move(Point p) {
		a.Move(p);
		b.Move(p);
	}

	float Line::Length() {
		return std::sqrt(std::pow(std::abs(a.x - b.x), 2.f) + std::pow(std::abs(a.y - b.y), 2.f));
	}

	Line Line::operator+(const Point& point) {
		Line ret = this;
		ret += point;
		return ret;
	}

	Line& Line::operator+=(const Point& point) {
		a += point;
		b += point;
		return *this;
	}

	// ----------------
	// struct Rectangle
	// ----------------

	Rectangle::Rectangle() : left(0.f), top(0.f), right(0.f), bottom(0.f) {}

	Rectangle::Rectangle(float left, float top, float right, float bottom) : left(left), top(top), right(right), bottom(bottom) {}

	Rectangle::Rectangle(Point topLeft, Point bottomRight) : left(topLeft.x), top(topLeft.y), right(bottomRight.x), bottom(bottomRight.y) {}

	Rectangle::Rectangle(Line line) : Rectangle(line.a, line.b) {}

	Rectangle::Rectangle(Bitmap bmp) : left(0.f), top(0.f), right(bmp->GetSize().width), bottom(bmp->GetSize().height) {}

	Rectangle::Rectangle(const Rectangle & ref) : left(ref.left), top(ref.top), right(ref.right), bottom(ref.bottom) {}

	Rectangle::Rectangle(const Rectangle * ref) : left(ref->left), top(ref->top), right(ref->right), bottom(ref->bottom) {}

	Rectangle::operator D2D1_RECT_F() {
		return D2D1::RectF(left, top, right, bottom);
	}

	Rectangle Rectangle::operator+(const Point & point) {
		Rectangle result = this;
		result += point;
		return result;
	}

	Rectangle & Rectangle::operator+=(const Point & point) {
		Move(point.x, point.y);
		return *this;
	}

	void Rectangle::Scale(float scale, float centerX, float centerY) {
		left = centerX - (centerX - left) * scale;
		right = centerX + (right - centerX) * scale;
		top = centerY - (centerY - top) * scale;
		bottom = centerY + (bottom - centerY) * scale;
	}

	void Rectangle::Scale(float x, float y, float centerX, float centerY) {
		left = centerX - (centerX - left) * x;
		right = centerX + (right - centerX) * x;
		top = centerY - (centerY - top) * y;
		bottom = centerY + (bottom - centerY) * y;
	}

	void Rectangle::Scale(float scale) {
		Scale(scale, scale, Center().x, Center().y);
	}

	void Rectangle::Scale(float x, float y) {
		Scale(x, y, Center().x, Center().y);
	}

	void Rectangle::ShiftIn(float x, float y) {
		left += x;
		top += y;
		right -= x;
		bottom -= y;
	}

	void Rectangle::ShiftOut(float x, float y) {
		left -= x;
		top -= y;
		right += x;
		bottom += y;
	}

	void Rectangle::Move(float x, float y) {
		left += x;
		right += x;
		top += y;
		bottom += y;
	}

	void Rectangle::Move(Point p) {
		(*this) += p;
	}

	float Rectangle::Width() {
		return std::abs(right - left);
	}

	float Rectangle::Height() {
		return std::abs(bottom - top);
	}

	Point Rectangle::Center() {
		return Point(0.5f * left + 0.5f * right, 0.5f * top + 0.5f*bottom);
	}

	Rectangle Rectangle::RectMainScreen() {
		auto x = GetSystemMetrics(SM_CXSCREEN);
		auto y = GetSystemMetrics(SM_CYSCREEN);
		return Rectangle(0.f, 0.f, static_cast<float>(x), static_cast<float>(y));
	}

	Rectangle Rectangle::RectDesktop() {
		// TODO
		auto x = GetSystemMetrics(SM_CXSCREEN);
		auto y = GetSystemMetrics(SM_CYSCREEN);
		return Rectangle(0.f, 0.f, static_cast<float>(x), static_cast<float>(y));
	}

	Rectangle Rectangle::RectCentered(Point center, float width, float height) {
		return Rectangle(center.x - width / 2, center.y - height / 2, center.x + width / 2, center.y + height / 2);
	}

	Rectangle Rectangle::RectCentered(float x, float y, float width, float height) {
		return Rectangle(x - width / 2, y - height / 2, x + width / 2, y + height / 2);
	}

	Rectangle Rectangle::Box(float x, float y, float width, float height) {
		return Rectangle(x, y, x + width, y + height);
	}

	Rectangle Rectangle::Box(Point upperLeft, float width, float height) {
		return Rectangle(upperLeft.x, upperLeft.y, upperLeft.x + width, upperLeft.y + height);
	}

	// --------------
	// struct Ellipse
	// --------------

	Ellipse::Ellipse() : center(0.f, 0.f), radiusX(0.f), radiusY(0.f) {}

	Ellipse::Ellipse(float x, float y, float radX, float radY) : center(x, y), radiusX(radX), radiusY(radY) {}

	Ellipse::Ellipse(Point center, float radX, float radY) : center(center), radiusX(radX), radiusY(radY) {}

	Ellipse::Ellipse(float x, float y, float radius) : center(x, y), radiusX(radius), radiusY(radius) {}

	Ellipse::Ellipse(Point center, float radius) : center(center), radiusX(radius), radiusY(radius) {}

	Ellipse::Ellipse(Rectangle rect) : center(rect.Center()), radiusX(rect.Width() * 0.5f), radiusY(rect.Height() * 0.5f) {}

	Ellipse::Ellipse(const Ellipse & ref) : center(ref.center), radiusX(ref.radiusX), radiusY(ref.radiusY) {}

	Ellipse::Ellipse(const Ellipse * ref) : center(ref->center), radiusX(ref->radiusX), radiusY(ref->radiusY) {}

	void Ellipse::Move(float x, float y) {
		center.Move(x, y);
	}

	void Ellipse::Move(Point p) {
		center.Move(p);
	}

	Ellipse::operator D2D1_ELLIPSE() {
		return D2D1::Ellipse(center, radiusX, radiusY);
	}

	Ellipse Ellipse::operator+(const Point& point) {
		Ellipse ret = this;
		ret += point;
		return ret;
	}

	Ellipse& Ellipse::operator+=(const Point& point) {
		center += point;
		return *this;
	}

	// -----------
	// class Color
	// -----------

	Color::Color() : m_Red(0.f), m_Green(0.f), m_Blue(0.f), m_Alpha(0.f) {}

	Color::Color(const Color& other) : Color(other.m_Red, other.m_Green, other.m_Blue, other.m_Alpha) {}

	Color::Color(const Color* other) : Color(other->m_Red, other->m_Green, other->m_Blue, other->m_Alpha) {}

	Color::Color(ColorEnum ce, float opacity /*= 1.f*/) : Color((static_cast<UINT32>(ce) & 0x00FFFFFFUL) | (static_cast<UINT32>(opacity * 255) << 24)) {}

	Color::Color(float red, float green, float blue, float opacity /*=  1.f*/) : m_Red(red), m_Green(green), m_Blue(blue), m_Alpha(opacity) {}

	Color::Color(int red, int green, int blue, int opacity /* = 0xFF*/) : m_Red(static_cast<float>(red) / 255.f), m_Green(static_cast<float>(green) / 255.f), m_Blue(static_cast<float>(blue) / 255.f), m_Alpha(static_cast<float>(opacity) / 255.f) {}

	Color::Color(UINT32 color) : Color(static_cast<UINT8>(color >> 16), static_cast<UINT8>(color >> 8), static_cast<UINT8>(color), static_cast<UINT8>(color >> 24)) {}

	Color::operator D2D1_COLOR_F() {
		return D2D1::ColorF(m_Red, m_Green, m_Blue, m_Alpha);
	}

	Color Color::operator+(const Color& other) {
		Color result = this;
		result += other;
		return result;
	}

	Color& Color::operator+=(const Color& other) {
		m_Red = ZorkLib::Utility::SetMinMax(m_Red + other.m_Red, 0.f, 1.f);
		m_Blue = ZorkLib::Utility::SetMinMax(m_Blue + other.m_Blue, 0.f, 1.f);
		m_Green = ZorkLib::Utility::SetMinMax(m_Green + other.m_Green, 0.f, 1.f);
		m_Alpha = ZorkLib::Utility::SetMinMax(m_Alpha + other.m_Alpha, 0.f, 1.f);
		return *this;
	}

	Color Color::operator*(const Color& other) {
		Color result = this;
		result *= other;
		return result;
	}

	Color& Color::operator*=(const Color& other) {
		m_Red = ZorkLib::Utility::SetMinMax(m_Red * other.m_Red, 0.f, 1.f);
		m_Blue = ZorkLib::Utility::SetMinMax(m_Blue * other.m_Blue, 0.f, 1.f);
		m_Green = ZorkLib::Utility::SetMinMax(m_Green * other.m_Green, 0.f, 1.f);
		m_Alpha = ZorkLib::Utility::SetMinMax(m_Alpha * other.m_Alpha, 0.f, 1.f);
		return *this;
	}

	UINT32 Color::GetHex() {
		UINT32 r = static_cast<UINT32>(m_Red * 255.f);
		UINT32 g = static_cast<UINT32>(m_Green * 255.f);
		UINT32 b = static_cast<UINT32>(m_Blue * 255.f);
		UINT32 a = static_cast<UINT32>(m_Alpha * 255.f);
		UINT32 result = a << 24 | r << 16 | g << 8 | b;
		return result;
	}

	float Color::GetRed() {
		return m_Red;
	}

	float Color::GetGreen() {
		return m_Green;
	}

	float Color::GetBlue() {
		return m_Blue;
	}

	float Color::GetAlpha() {
		return m_Alpha;
	}

	void Color::SetRed(float red) {
		m_Red = Utility::SetMinMax(red, 0.f, 1.f);
	}

	void Color::SetGreen(float green) {
		m_Green = Utility::SetMinMax(green, 0.f, 1.f);
	}

	void Color::SetBlue(float blue) {
		m_Blue = Utility::SetMinMax(blue, 0.f, 1.f);
	}

	void Color::SetAlpha(float alpha) {
		m_Alpha = Utility::SetMinMax(alpha, 0.f, 1.f);
	}

}