#ifndef SYB_VEC2_HPP
#define SYB_VEC2_HPP

#ifndef SYB_DEF_HPP
#include "core/SybDef.hpp"
#endif


namespace syb
{
	struct SYB_API Vec2
	{
		double x, y;

		Vec2();
		Vec2(double x_, double y_);
		
		bool IsZero() const;
		void Normalize();

		double Length() const;
		double LengthSq() const;

		double Distance(const Vec2 &v2) const;
		double DistanceSq(const Vec2 &v2) const;

		double Dot(const Vec2 &v2) const;
		Vec2 GetPerpendicular() const;

		const Vec2& operator += (const Vec2 &rhs);
		const Vec2& operator -= (const Vec2 &rhs);
		const Vec2& operator *= (const double &rhs);
		const Vec2& operator /= (const double &rhs);
		bool operator == (const Vec2 &rhs) const;
		bool operator != (const Vec2 &rhs) const;
	};

	SYB_API Vec2 operator * (const Vec2 &lhs, const Vec2 &rhs);
	SYB_API Vec2 operator * (double lhs, const Vec2 &rhs);
	SYB_API Vec2 operator - (const Vec2 &lhs, const Vec2 &rhs);
	SYB_API Vec2 operator + (const Vec2 &lhs, const Vec2 &rhs);
	SYB_API Vec2 operator / (const Vec2 &lhs, double val);
} // namespace syb

#endif // SYB_VEC2_HPP