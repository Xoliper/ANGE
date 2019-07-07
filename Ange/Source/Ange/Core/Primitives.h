#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Ange {

	//-----------------------------------------------------------------------
	//Structs
	//-----------------------------------------------------------------------

	/*!
	Describes the size of two-dimensional space. Available <typename> variants: size_t, int, float.
	*/
	template <typename T>
	struct Dimension
	{
		/*!
		Operator overload (+=) - allows to add objects in easy way.
		*/
		Dimension<T>& operator+=(const Dimension<T>& rhs);

		/*!
		Operator overload (+) - allows to add objects in easy way.
		*/
		Dimension<T> operator+(const Dimension<T>& rhs);

		/*!
		Operator overload (-) - allows to subtract objects in easy way.
		*/
		Dimension<T> operator-(const Dimension<T>& rhs);

		/*!
		Operator overload (-=) - allows to subtract objects in easy way.
		*/
		Dimension<T>& operator-=(const Dimension<T>& rhs);

		/*!
		Default constructor. Width and Height values are set to 0.
		*/
		Dimension();

		/*!
		Constructor that sets values.
		*/
		Dimension(T width, T height);

		/*!
		Destructor.
		*/
		~Dimension();

		/*!
		Dimension setter.
		*/
		void Set(T width, T height);
		
		/*!
		Returns a string describing the dimensions of the space. Notice: it will cast the data to 'int'.
		*/
		std::string ToString();

		/* Specifies the width. */
		T tWidth;

		/* Specifies the height. */
		T tHeight;
	};

	//---------------------------------------------------------------------------------------------------------------

	/*!
	Describes the position/point in two-dimensional space.  Available <typename> variants: size_t, int, float.
	*/
	template <typename T>
	struct Point
	{
		/*!
		Operator overload (+=) - allows to add objects in easy way.
		*/
		Point<T>& operator+=(const Point<T>& rhs);

		/*!
		Operator overload (+) - allows to add objects in easy way.
		*/
		Point<T> operator+(const Point<T>& rhs);

		/*!
		Operator overload (-) - allows to subtract objects in easy way.
		*/
		Point<T> operator-(const Point<T>& rhs);

		/*!
		Operator overload (-=) - allows to subtract objects in easy way.
		*/
		Point<T>& operator-=(const Point<T>& rhs);

		/*!
		Default constructor. X and Y values are set to 0.
		*/
		Point();

		/*!
		Constructor that sets values.
		*/
		Point(T x, T y);
		
		/*!
		Destructor.
		*/
		~Point();

		/*!
		Point/Position setter.
		*/
		void Set(T x, T y);

		/*!
		Returns a string describing the point in the space. Notice: it will cast the data to 'int'.
		*/
		std::string ToString();

		/* Specifies the horizontal 'X' position. */
		T tX;

		/* Specifies the vertical 'Y' position. */
		T tY;
	};

	template<typename T>
	using Scale = Point<T>;

	//-----------------------------------------------------------------------
	//Classes
	//-----------------------------------------------------------------------

	/*!
	Specifies the colors in RGBA space. All graphics rendering widgets rely on the data contained in the "Color" objects.
	The data are stored in floats. A component value of 1.0f means full saturation.
	*/
	class Color {
	public:

		/*!
		Constructor that set all components (including transparency) using the int type. A conversion from 'int' in the
		range of 0-255 is made, which is reflected in 0.0f - 1.0f float. 
		*/
		Color(int r, int g, int b, int a);

		/*!
		Constructor that set all components (including transparency) using the float type.
		*/
		Color(float r, float g, float b, float a);

		/*!
		Default constructor that set all components (EXCLUDING transparency) using the single 24-bit HEX value. The transparency
		is set to 0. With default value all color components are fully saturated which translates into a white color.
		*/
		Color(int hexValue = 0xFFFFFF);

		/*!
		Copy constructor.
		*/
		Color(const Color & c);
		
		/*!
		Destructor. In principle, does nothing.
		*/
		virtual ~Color();

		/*!
		Comparison operator. Checks whether the objects are identical.
		*/
		bool operator== (const Color & rhs) const;

		/*!
		Comparison operator. Checks whether the objects are different.
		*/
		bool operator!= (const Color & rhs) const;

		/*!
		Allows to easily add colors. If any component exceed the range (0.0f - 1.0f) it is clamped.
		*/
		void operator+= (const Color& rhs);

		/*!
		Allows to easily subtract colors. If any component exceed the range (0.0f - 1.0f) it is clamped.
		*/
		void operator-= (const Color& rhs);

		/*!
		Operator overload (+) - allows to add objects in easy way.
		*/
		Color operator+(const Color& rhs);

		/*!
		Operator overload (-) - allows to subtract objects in easy way.
		*/
		Color operator-(const Color& rhs);

		/*!
		Sets the transparency component using the int type.  A conversion from 'int' in the range of 0-255 is made, which
		is reflected in 0.0f - 1.0f float. 
		*/
		void SetOpacity(int a);

		/*!
		Sets the transparency component using the float type.
		*/
		void SetOpacity(float a);

		/*!
		Sets all components (including transparency) using the int type. A conversion from 'int' in the
		range of 0-255 is made, which is reflected in 0.0f - 1.0f float.
		*/
		void SetColor(int r, int g, int b, int a);

		/*!
		Set all components (including transparency) using the float type.
		*/
		void SetColor(float r, float g, float b, float a);

		/*!
		Returns stored color brightness. The value is in 0-255 range.
		*/
		int GetBrightness();

		/*!
		Helper function to clamp components to 0.0f - 1.0f.
		*/
		void Clamp();

		/*!
		Returns a glm vector consisting of three (RGB) components corresponding to the stored color.
		*/
		glm::vec3 GetVec3();
		
		/*!
		Returns a glm vector consisting of four (RGBA) components corresponding to the stored color.
		*/
		glm::vec4 GetVec4();

		/* Color component. */
		float r, g, b, a;

	};


}