#include <Apch.h>
#include "Primitives.h"

namespace Ange {

	template <class T>
	Dimension<T>& Dimension<T>::operator+=(const Dimension<T>& rhs)
	{
		tWidth += rhs.tWidth;
		tHeight += rhs.tHeight;
		return *this;
	}

	template <class T>
	Dimension<T> Dimension<T>::operator+(const Dimension<T>& rhs)
	{
		Dimension<T> output;
		output.tWidth = tWidth + rhs.tWidth;
		output.tHeight = tHeight + rhs.tHeight;
		return output;
	}

	template <class T>
	Dimension<T> Dimension<T>::operator-(const Dimension<T>& rhs)
	{
		Dimension<T> output;
		output.tWidth = tWidth - rhs.tWidth;
		output.tHeight = tHeight - rhs.tHeight;
		return output;
	}

	template <class T>
	Dimension<T>& Dimension<T>::operator-=(const Dimension<T>& rhs)
	{
		tWidth -= rhs.tWidth;
		tHeight -= rhs.tHeight;
		return *this;
	}

	template <class T>
	Dimension<T>::Dimension() : Dimension(0, 0)
	{
	}

	template <class T>
	Dimension<T>::Dimension(T width, T height)
	{
		tWidth = width;
		tHeight = height;
	}

	template <class T>
	Dimension<T>::~Dimension()
	{
	}

	template <class T>
	void Dimension<T>::Set(T width, T height)
	{
		tWidth = width;
		tHeight = height;
	}

	template <class T>
	std::string Dimension<T>::ToString()
	{
		char buffer[32];
		sprintf(buffer, "Dimension [%u x %u]", (int)tWidth, (int)tHeight);
		return std::string(buffer);
	}

	template <class T>
	Point<T>& Point<T>::operator+=(const Point<T>& rhs)
	{
		tX += rhs.tX;
		tY += rhs.tY;
		return *this;
	}

	template <class T>
	Point<T> Point<T>::operator+(const Point<T>& rhs)
	{
		Point<T> output;
		output.tX = tX + rhs.tX;
		output.tY = tY + rhs.tY;
		return output;
	}

	template <class T>
	Point<T> Point<T>::operator-(const Point<T>& rhs)
	{
		Point<T> output;
		output.tX = tX - rhs.tX;
		output.tY = tY - rhs.tY;
		return output;
	}

	template <class T>
	Point<T>& Point<T>::operator-=(const Point<T>& rhs)
	{
		tX -= rhs.tX;
		tY -= rhs.tY;
		return *this;
	}

	template <class T>
	Point<T>::Point() : Point(0, 0)
	{
	}

	template <class T>
	Point<T>::Point(T x, T y)
	{
		tX = x;
		tY = y;
	}

	template <class T>
	Point<T>::~Point()
	{
	}

	template <class T>
	void Point<T>::Set(T width, T height)
	{
		tX = width;
		tY = height;
	}

	template <class T>
	std::string Point<T>::ToString()
	{
		char buffer[128];
		sprintf(buffer, "Point [%i x %i]", (int)tX, (int)tY);
		return std::string(buffer);
	}

	Color::Color(int r, int g, int b, int a)
	{
		this->r = (float)r / 255;
		this->g = (float)g / 255;
		this->b = (float)b / 255;
		this->a = (float)a / 255;
		Clamp();
	}

	Color::Color(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
		Clamp();
	}

	Color::Color(int hexValue)
	{
		r = (float)((hexValue >> 16) & 0xFF) / 255;  /// Extract the RR byte
		g = (float)((hexValue >> 8) & 0xFF) / 255;   /// Extract the GG byte
		b = (float)((hexValue) & 0xFF) / 255;        /// Extract the BB byte
		a = 1.0f;
	}

	Color::Color(const Color & c)
	{
		r = c.r;
		g = c.g;
		b = c.b;
		a = c.a;
	}

	Color::~Color()
	{
	}

	bool Color::operator==(const Color & rhs) const
	{
		if (r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a) return true;
		return false;
	}

	bool Color::operator!=(const Color & rhs) const
	{
		return !(*this == rhs);
	}

	void Color::operator+=(const Color& rhs)
	{
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		a += rhs.a;
		Clamp();
	}

	void Color::operator-=(const Color& rhs)
	{
		r -= rhs.r;
		g -= rhs.g;
		b -= rhs.b;
		a -= rhs.a;
		Clamp();
	}

	Color Color::operator+(const Color& rhs)
	{
		Color output;
		output.a = a + rhs.a;
		output.r = r + rhs.r;
		output.g = g + rhs.g;
		output.b = b + rhs.b;
		output.Clamp();
		return output;
	}

	Color Color::operator-(const Color& rhs)
	{
		Color output;
		output.a = a - rhs.a;
		output.r = r - rhs.r;
		output.g = g - rhs.g;
		output.b = b - rhs.b;
		output.Clamp();
		return output;
	}

	void Color::Clamp()
	{
		if (r > 1.0f) r = 1.0f;
		if (r < 0.0f) r = 0.0f;
		if (g > 1.0f) g = 1.0f;
		if (g < 0.0f) g = 0.0f;
		if (b > 1.0f) b = 1.0f;
		if (b < 0.0f) b = 0.0f;
		if (a > 1.0f) a = 1.0f;
		if (a < 0.0f) a = 0.0f;
	}

	void Color::SetOpacity(int a)
	{
		this->a = (float)a / 255;
		if (this->a > 1.0f) {
			this->a = 1.0f;
		} else if (this->a < 0.0f){
			this->a = 0.0f;
		}
	}

	void Color::SetOpacity(float a)
	{
		this->a = a;
		if (this->a > 1.0f) {
			a = 1.0f;
		} else if (this->a < 0.0f) {
			a = 0.0f;
		}
	}

	void Color::SetColor(int r, int g, int b, int a)
	{
		this->r = (float)r / 255;
		this->g = (float)g / 255;
		this->b = (float)b / 255;
		this->a = (float)a / 255;
		Clamp();
	}

	void Color::SetColor(float r, float g, float b, float a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
		Clamp();
	}

	int Color::GetBrightness()
	{
		return (int)(((r + g + b) / 3 * a) * 255);
	}

	glm::vec3 Color::GetVec3()
	{
		return glm::vec3(r, g, b);
	}

	glm::vec4 Color::GetVec4()
	{
		return glm::vec4(r, g, b, a);
	};


	template struct Dimension<int>;
	template struct Dimension<size_t>;
	template struct Dimension<float>;
	template struct Dimension<double>;

	template struct Point<int>;
	template struct Point<size_t>;
	template struct Point<float>;
	template struct Point<double>;
}