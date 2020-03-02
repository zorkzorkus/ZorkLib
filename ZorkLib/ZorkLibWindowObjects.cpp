#include "ZorkLibWindowObjects.hpp"

namespace ZorkLib {

	// ------------
	// struct Point
	// ------------

	Point::Point() : x(0.f), y(0.f) {}

	Point::Point(float x, float y) : x(x), y(y) {}

	Point::Point(const Point& ref) : x(ref.x), y(ref.y) {}

	Point::Point(const Point* ref) : x(ref->x), y(ref->y) {}

	Point& Point::Move(float x, float y) {
		this->x += x;
		this->y += y;
		return *this;
	}

	Point& Point::Move(Point p) {
		x += p.x;
		y += p.y;
		return *this;
	}

	Point Point::Moved(float x, float y) {
		return *this + Point(x, y);
	}

	Point Point::Moved(Point p) {
		return *this + p;
	}

	Point& Point::Scale(float s) {
		x *= s;
		y *= s;
		return *this;
	}

	Point Point::Scaled(float s) {
		return Point(s * x, s * y);
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

	Point Point::operator-() {
		Point result;
		result.x = -this->x;
		result.y = -this->y;
		return result;
	}

	Point Point::operator-(const Point& other) {
		Point result = this;
		result -= other;
		return result;
	}

	Point& Point::operator-=(const Point& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Point Point::operator*(const float s) {
		Point result = this;
		result *= s;
		return result;
	}

	Point& Point::operator*=(const float s) {
		x *= s;
		y *= s;
		return *this;
	}

	Point Point::operator/(const float s) {
		Point result = this;
		result /= s;
		return result;
	}

	Point& Point::operator/=(const float s) {
		x /= s;
		y /= s;
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

	Line& Line::Move(float x, float y) {
		a.Move(x, y);
		b.Move(x, y);
		return *this;
	}

	Line& Line::Move(Point p) {
		a.Move(p);
		b.Move(p);
		return *this;
	}

	Line Line::Moved(float x, float y) {
		Line res = this;
		res.Move(x, y);
		return res;
	}

	Line Line::Moved(Point p) {
		Line res = this;
		res.Move(p);
		return res;
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

	Line Line::operator-(const Point& point) {
		Line ret = this;
		ret -= point;
		return ret;
	}

	Line& Line::operator-=(const Point& point) {
		a -= point;
		b -= point;
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

	Rectangle::Rectangle(const Rectangle& ref) : left(ref.left), top(ref.top), right(ref.right), bottom(ref.bottom) {}

	Rectangle::Rectangle(const Rectangle* ref) : left(ref->left), top(ref->top), right(ref->right), bottom(ref->bottom) {}

	Rectangle::operator D2D1_RECT_F() {
		return D2D1::RectF(left, top, right, bottom);
	}

	Rectangle Rectangle::operator+(const Point& point) {
		Rectangle result = this;
		result += point;
		return result;
	}

	Rectangle& Rectangle::operator+=(const Point& point) {
		Move(point.x, point.y);
		return *this;
	}

	Rectangle Rectangle::operator-(const Point& point) {
		Rectangle result = this;
		result -= point;
		return result;
	}

	Rectangle& Rectangle::operator-=(const Point& point) {
		Move(-point.x, -point.y);
		return *this;
	}

	Rectangle& Rectangle::Scale(float scale, float centerX, float centerY) {
		left = centerX - (centerX - left) * scale;
		right = centerX + (right - centerX) * scale;
		top = centerY - (centerY - top) * scale;
		bottom = centerY + (bottom - centerY) * scale;
		return *this;
	}

	Rectangle& Rectangle::Scale(float x, float y, float centerX, float centerY) {
		left = centerX - (centerX - left) * x;
		right = centerX + (right - centerX) * x;
		top = centerY - (centerY - top) * y;
		bottom = centerY + (bottom - centerY) * y;
		return *this;
	}

	Rectangle& Rectangle::Scale(float scale) {
		Scale(scale, scale, Center().x, Center().y);
		return *this;
	}

	Rectangle& Rectangle::Scale(float x, float y) {
		Scale(x, y, Center().x, Center().y);
		return *this;
	}

	Rectangle& Rectangle::ShiftIn(float x, float y) {
		left += x;
		top += y;
		right -= x;
		bottom -= y;
		return *this;
	}

	Rectangle& Rectangle::ShiftOut(float x, float y) {
		left -= x;
		top -= y;
		right += x;
		bottom += y;
		return *this;
	}

	Rectangle& Rectangle::Move(float x, float y) {
		left += x;
		right += x;
		top += y;
		bottom += y;
		return *this;
	}

	Rectangle& Rectangle::Move(Point p) {
		(*this) += p;
		return *this;
	}

	Rectangle Rectangle::Scaled(float scale, float centerX, float centerY) {
		Rectangle res = this;
		res.Scale(scale, centerX, centerY);
		return res;
	}

	Rectangle Rectangle::Scaled(float x, float y, float centerX, float centerY) {
		Rectangle res = this;
		res.Scale(x, y, centerX, centerY);
		return res;
	}

	Rectangle Rectangle::Scaled(float scale) {
		Rectangle res = this;
		res.Scale(scale);
		return res;
	}

	Rectangle Rectangle::Scaled(float x, float y) {
		Rectangle res = this;
		res.Scale(x, y);
		return res;
	}

	Rectangle Rectangle::ShiftedIn(float x, float y) {
		Rectangle res = this;
		res.ShiftIn(x, y);
		return res;
	}

	Rectangle Rectangle::ShiftedOut(float x, float y) {
		Rectangle res = this;
		res.ShiftOut(x, y);
		return res;
	}

	Rectangle Rectangle::Moved(float x, float y) {
		Rectangle res = this;
		res.Move(x, y);
		return res;
	}

	Rectangle Rectangle::Moved(Point p) {
		Rectangle res = this;
		res.Move(p);
		return res;
	}

	float Rectangle::Width() {
		return std::abs(right - left);
	}

	float Rectangle::Height() {
		return std::abs(bottom - top);
	}

	Point Rectangle::Center() {
		return Point(0.5f * left + 0.5f * right, 0.5f * top + 0.5f * bottom);
	}

	Rectangle Rectangle::RectMainScreen() {
		auto x = GetSystemMetrics(SM_CXSCREEN);
		auto y = GetSystemMetrics(SM_CYSCREEN);
		return Rectangle(0.f, 0.f, static_cast<float>(x), static_cast<float>(y));
	}

	Rectangle Rectangle::RectVirtualScreen() {
		auto x = static_cast<float>(GetSystemMetrics(SM_XVIRTUALSCREEN));
		auto y = static_cast<float>(GetSystemMetrics(SM_YVIRTUALSCREEN));
		auto w = static_cast<float>(GetSystemMetrics(SM_CXVIRTUALSCREEN));
		auto h = static_cast<float>(GetSystemMetrics(SM_CYVIRTUALSCREEN));
		return Rectangle(x, y, x + w, y + h);
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

	Ellipse::Ellipse(const Ellipse& ref) : center(ref.center), radiusX(ref.radiusX), radiusY(ref.radiusY) {}

	Ellipse::Ellipse(const Ellipse* ref) : center(ref->center), radiusX(ref->radiusX), radiusY(ref->radiusY) {}

	Ellipse& Ellipse::Move(float x, float y) {
		center.Move(x, y);
		return *this;
	}

	Ellipse& Ellipse::Move(Point p) {
		center.Move(p);
		return *this;
	}

	Ellipse& Ellipse::Scale(float scale) {
		radiusX *= scale;
		radiusY *= scale;
		return *this;
	}

	Ellipse& Ellipse::Scale(float x, float y) {
		radiusX *= x;
		radiusY *= y;
		return *this;
	}

	Ellipse Ellipse::Moved(float x, float y) {
		Ellipse res = this;
		res.Move(x, y);
		return res;
	}

	Ellipse Ellipse::Moved(Point p) {
		Ellipse res = this;
		res.Move(p);
		return res;
	}

	Ellipse Ellipse::Scaled(float scale) {
		Ellipse res = this;
		res.Scale(scale);
		return res;
	}

	Ellipse Ellipse::Scaled(float x, float y) {
		Ellipse res = this;
		res.Scale(x, y);
		return res;
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

	Ellipse Ellipse::operator-(const Point& point) {
		Ellipse ret = this;
		ret -= point;
		return ret;
	}

	Ellipse& Ellipse::operator-=(const Point& point) {
		center -= point;
		return *this;
	}

	// -----------------
	// class Color (RGB)
	// -----------------

	Color::Color() : m_Red(0.f), m_Green(0.f), m_Blue(0.f), m_Alpha(0.f) {}

	Color::Color(const Color& other) : Color(other.m_Red, other.m_Green, other.m_Blue, other.m_Alpha) {}

	Color::Color(const Color* other) : Color(other->m_Red, other->m_Green, other->m_Blue, other->m_Alpha) {}

	Color::Color(ColorEnum ce, float opacity /*= 1.f*/) : Color((static_cast<UINT32>(ce) & 0x00FFFFFFUL) | (static_cast<UINT32>(opacity * 255) << 24)) {}

	Color::Color(float red, float green, float blue, float opacity /*=  1.f*/) : m_Red(red), m_Green(green), m_Blue(blue), m_Alpha(opacity) {}

	Color::Color(int red, int green, int blue, int opacity /* = 0xFF*/) : m_Red(static_cast<float>(red) / 255.f), m_Green(static_cast<float>(green) / 255.f), m_Blue(static_cast<float>(blue) / 255.f), m_Alpha(static_cast<float>(opacity) / 255.f) {}

	Color::Color(UINT32 color) : Color(static_cast<UINT8>(color >> 16), static_cast<UINT8>(color >> 8), static_cast<UINT8>(color), static_cast<UINT8>(color >> 24)) {}

	Color::Color(ColorHSV colorHSV) {

		float h = colorHSV.GetHue();
		float s = colorHSV.GetSaturation();
		float v = colorHSV.GetValue();

		float p, q, t, ff;
		int i;

		m_Alpha = colorHSV.GetAlpha();

		if (s <= 0.f) {
			m_Red = v;
			m_Green = v;
			m_Blue = v;
		}

		while (h >= 360.f) h -= 360.f;
		h /= 60.f;
		i = static_cast<int>(h);
		ff = h - i;
		p = v * (1.f - s);
		q = v * (1.f - (s * ff));
		t = v * (1.f - (s * (1.f - ff)));

		switch (i) {
			case 0:
				m_Red = v;
				m_Green = t;
				m_Blue = p;
				return;
			case 1:
				m_Red = q;
				m_Green = v;
				m_Blue = p;
				return;
			case 2:
				m_Red = p;
				m_Green = v;
				m_Blue = t;
				return;
			case 3:
				m_Red = p;
				m_Green = q;
				m_Blue = v;
				return;
			case 4:
				m_Red = t;
				m_Green = p;
				m_Blue = v;
				return;
			case 5:
			default:
				m_Red = v;
				m_Green = p;
				m_Blue = q;
				return;
		}

	}

	Color::operator D2D1_COLOR_F() {
		return D2D1::ColorF(m_Red, m_Green, m_Blue, m_Alpha);
	}

	Color Color::operator+(const Color& other) {
		Color result = this;
		result += other;
		return result;
	}

	Color& Color::operator+=(const Color& other) {
		m_Red = ZorkLib::Utility::MinMax(m_Red + other.m_Red, 0.f, 1.f);
		m_Blue = ZorkLib::Utility::MinMax(m_Blue + other.m_Blue, 0.f, 1.f);
		m_Green = ZorkLib::Utility::MinMax(m_Green + other.m_Green, 0.f, 1.f);
		m_Alpha = ZorkLib::Utility::MinMax(m_Alpha + other.m_Alpha, 0.f, 1.f);
		return *this;
	}

	Color Color::operator*(const Color& other) {
		Color result = this;
		result *= other;
		return result;
	}

	Color& Color::operator*=(const Color& other) {
		m_Red = ZorkLib::Utility::MinMax(m_Red * other.m_Red, 0.f, 1.f);
		m_Blue = ZorkLib::Utility::MinMax(m_Blue * other.m_Blue, 0.f, 1.f);
		m_Green = ZorkLib::Utility::MinMax(m_Green * other.m_Green, 0.f, 1.f);
		m_Alpha = ZorkLib::Utility::MinMax(m_Alpha * other.m_Alpha, 0.f, 1.f);
		return *this;
	}

	Color Color::operator*(const float& scale) {
		Color result = this;
		result *= scale;
		return result;
	}

	Color& Color::operator*=(const float& scale) {
		m_Red = ZorkLib::Utility::MinMax(m_Red * scale, 0.f, 1.f);
		m_Blue = ZorkLib::Utility::MinMax(m_Blue * scale, 0.f, 1.f);
		m_Green = ZorkLib::Utility::MinMax(m_Green * scale, 0.f, 1.f);
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
		m_Red = Utility::MinMax(red, 0.f, 1.f);
	}

	void Color::SetGreen(float green) {
		m_Green = Utility::MinMax(green, 0.f, 1.f);
	}

	void Color::SetBlue(float blue) {
		m_Blue = Utility::MinMax(blue, 0.f, 1.f);
	}

	void Color::SetAlpha(float alpha) {
		m_Alpha = Utility::MinMax(alpha, 0.f, 1.f);
	}

	float Color::AddRed(float red) {
		return m_Red = Utility::MinMax(m_Red + red, 0.f, 1.f);
	}

	float Color::AddGreen(float green) {
		return m_Green = Utility::MinMax(m_Green + green, 0.f, 1.f);
	}

	float Color::AddBlue(float blue) {
		return m_Blue = Utility::MinMax(m_Blue + blue, 0.f, 1.f);
	}

	float Color::AddAlpha(float alpha) {
		return m_Alpha = Utility::MinMax(m_Alpha + alpha, 0.f, 1.f);
	}

	// --------------
	// class ColorHSV
	// --------------

	ColorHSV::ColorHSV() : m_Hue(0.f), m_Saturation(0.f), m_Value(0.f), m_Alpha(0.f) {}

	ColorHSV::ColorHSV(const ColorHSV& other) : ColorHSV(other.m_Hue, other.m_Saturation, other.m_Value, other.m_Alpha) {}

	ColorHSV::ColorHSV(const ColorHSV* other) : ColorHSV(other->m_Hue, other->m_Saturation, other->m_Value, other->m_Alpha) {}

	ColorHSV::ColorHSV(ColorEnum ce, float opacity /*= 1.f*/) : ColorHSV(Color(ce, opacity)) {}

	ColorHSV::ColorHSV(float h, float s, float v, float opacity /*=  1.f*/) : m_Hue(h), m_Saturation(s), m_Value(v), m_Alpha(opacity) {}

	ColorHSV::ColorHSV(UINT32 ColorHSV) : ColorHSV(static_cast<UINT8>(ColorHSV >> 16), static_cast<UINT8>(ColorHSV >> 8), static_cast<UINT8>(ColorHSV), static_cast<UINT8>(ColorHSV >> 24)) {}

	ColorHSV::ColorHSV(Color colorRGB) {

		float r = colorRGB.GetRed();
		float g = colorRGB.GetGreen();
		float b = colorRGB.GetBlue();

		float min, max, delta;

		m_Alpha = colorRGB.GetAlpha();

		min = Utility::Min(Utility::Min(r, g), b);
		max = Utility::Max(Utility::Max(r, g), b);

		m_Value = max;

		delta = max - min;

		if (delta < 0.00001f) {
			m_Saturation = 0;
			m_Hue = 0; // undefined
			return;
		}

		if (max > 0.f) { // NOTE: if Max is == 0, this divide would cause a crash
			m_Saturation = (delta / max);
		} else {
			// if max is 0, then r = g = b = 0
			// s = 0, h is undefined
			m_Saturation = 0.f;
			m_Hue = 0.f;
			return;
		}

		if (r >= max) {
			m_Hue = (g - b) / delta; // between yellow & magenta
		} else {
			if (g >= max) m_Hue = 2.f + (b - r) / delta; // between cyan & yellow
			else m_Hue = 4.f + (r - g) / delta; // between magenta & cyan
		}

		m_Hue *= 60.f;

		if (m_Hue < 0.f) m_Hue += 360.f;

	}

	ColorHSV::operator D2D1_COLOR_F() {
		return Color(this).operator D2D1_COLOR_F();
	}

	UINT32 ColorHSV::GetHex() {
		return Color(this).GetHex();
	}

	float ColorHSV::GetHue() {
		return m_Hue;
	}

	float ColorHSV::GetSaturation() {
		return m_Saturation;
	}

	float ColorHSV::GetValue() {
		return m_Value;
	}

	float ColorHSV::GetAlpha() {
		return m_Alpha;
	}

	void ColorHSV::SetHue(float h) {
		m_Hue = Utility::Wrap(h, 0.f, 360.f);
	}

	void ColorHSV::SetSaturation(float s) {
		m_Saturation = Utility::MinMax(s, 0.f, 1.f);
	}

	void ColorHSV::SetValue(float v) {
		m_Saturation = Utility::MinMax(v, 0.f, 1.f);
	}

	void ColorHSV::SetAlpha(float a) {
		m_Alpha = Utility::MinMax(a, 0.f, 1.f);
	}

	float ColorHSV::AddHue(float h) {
		return m_Hue = Utility::Wrap(m_Hue + h, 0.f, 360.f);
	}

	float ColorHSV::AddSaturation(float s) {
		return m_Saturation = Utility::MinMax(m_Saturation + s, 0.f, 1.f);
	}

	float ColorHSV::AddValue(float v) {
		return m_Value = Utility::MinMax(m_Value + v, 0.f, 1.f);
	}

	float ColorHSV::AddAlpha(float a) {
		return m_Alpha = Utility::MinMax(m_Alpha + a, 0.f, 1.f);
	}

}