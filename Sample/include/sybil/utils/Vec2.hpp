#ifndef SYB_VEC2_HPP
#define SYB_VEC2_HPP

#include <cmath>


namespace syb
{
	/**
	* To be used with either integer coordinates or floating-point coordinates.
	* Currently, due to time constraints and server constraints and fuckall constraints,
	* should be used only with ints. The template format is maintained though, to allow
	* easier change at a later time.
	* Scratch that(-ish).
	*/
	//template <typename T>
	class Vec2
	{
	public:
		Vec2();
		Vec2(const float& x_, const float& y_);
		Vec2(const Vec2&);
		~Vec2() { }

		float Length() const;
		/// Avoids applying the sqrt function => happy frame rate
		float LengthSq() const;

		void Normalise();

		const Vec2& operator += (const Vec2& rhs);
		const Vec2& operator -= (const Vec2& rhs);
		const Vec2& operator *= (const float& rhs);

		//T x, y;
		float x, y;
	};

	inline Vec2::Vec2() :
		x(0),
		y(0)
	{ }

	inline Vec2::Vec2(const float& x_, const float& y_) :
		x(x_),
		y(y_)
	{ }

	inline Vec2::Vec2(const Vec2& vec) :
		x(vec.x),
		y(vec.y)
	{ }

	inline float Vec2::Length() const
	{
		return sqrt(x*x + y*y);
	}

	inline float Vec2::LengthSq() const
	{
		return x*x + y*y;
	}

	inline void Vec2::Normalise()
	{
		float length = Length();
		if (length)
		{
			x /= length;
			y /= length;
		}
	}

	inline const Vec2& Vec2::operator *= (const float& rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}

	inline const Vec2& Vec2::operator += (const Vec2& rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	inline const Vec2& Vec2::operator -= (const Vec2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	inline Vec2 operator - (const Vec2 &lhs, const Vec2 &rhs)
	{
		Vec2 result(lhs);
		result.x -= rhs.x;
		result.y -= rhs.y;

		return result;
	}

	inline Vec2 operator * (const float& lhs, const Vec2& rhs)
	{
		Vec2 result(rhs);
		result *= lhs;

		return result;
	}

	inline Vec2 operator * (const Vec2& lhs, const float& rhs)
	{
		Vec2 result(lhs);
		result *= rhs;

		return result;
	}

	inline Vec2 operator + (const Vec2& lhs, const Vec2& rhs)
	{
		Vec2 result(lhs);
		result.x += rhs.x;
		result.y += rhs.y;

		return result;
	}
} // namespace syb

#endif // SYB_VEC2_HPP