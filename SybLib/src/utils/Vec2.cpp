#include "utils/Vec2.hpp"
#include "utils/Utils.hpp"


namespace syb
{
	Vec2::Vec2() :
		x(0.0),
		y(0.0)
	{
	}

	Vec2::Vec2(double x_, double y_) :
		x(x_),
		y(y_)
	{
	}

	bool Vec2::IsZero() const
	{
		return (x * x + y * y) < MinDouble;
	}

	void Vec2::Normalize()
	{
		double length = this->Length();
		if (length > std::numeric_limits<double>::epsilon())
		{
			this->x /= length;
			this->y /= length;
		}
	}

	double Vec2::Length() const
	{
		return sqrt(x * x + y * y);
	}
	
	double Vec2::LengthSq() const
	{
		return x * x + y * y;
	}

	double Vec2::Distance(const Vec2 &v2) const
	{
		double ySeparation = v2.y - y;
		double xSeparation = v2.x - x;

		return sqrt(ySeparation * ySeparation + xSeparation * xSeparation);
	}
	
	double Vec2::DistanceSq(const Vec2 &v2) const
	{
		double ySeparation = v2.y - y;
		double xSeparation = v2.x - x;

		return ySeparation * ySeparation + xSeparation * xSeparation;
	}

	double Vec2::Dot(const Vec2 &v2) const
	{
		return x * v2.x + y * v2.y;
	}

	Vec2 Vec2::GetPerpendicular() const
	{
		return Vec2(-y, x);
	}
	const Vec2& Vec2::operator += (const Vec2 &rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	const Vec2& Vec2::operator -= (const Vec2 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	const Vec2& Vec2::operator *= (const double &rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}
	
	const Vec2& Vec2::operator /= (const double &rhs)
	{
		x /= rhs;
		y /= rhs;

		return *this;
	}

	bool Vec2::operator == (const Vec2 &rhs) const
	{
		return (x == rhs.x) && (y == rhs.y);
	}
	
	bool Vec2::operator != (const Vec2 &rhs) const
	{
		return (x != rhs.x) || (y != rhs.y);
	}

	Vec2 operator * (const Vec2 &lhs, double rhs)
	{
		Vec2 result(lhs);
		result *= rhs;
		return result;
	}

	Vec2 operator * (double lhs, const Vec2 &rhs)
	{
		Vec2 result(rhs);
		result *= lhs;
		return result;
	}

	Vec2 operator - (const Vec2 &lhs, const Vec2 &rhs)
	{
		Vec2 result(lhs);
		result.x -= rhs.x;
		result.y -= rhs.y;

		return result;
	}

	Vec2 operator + (const Vec2 &lhs, const Vec2 &rhs)
	{
		Vec2 result(lhs);
		result.x += rhs.x;
		result.y += rhs.y;

		return result;
	}

	Vec2 operator / (const Vec2 &lhs, double val)
	{
		Vec2 result(lhs);
		result.x /= val;
		result.y /= val;

		return result;
	}
} // namespace syb