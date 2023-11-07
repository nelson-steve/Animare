#pragma once

#include "defines.hpp"

#include <math.h>
#include <assert.h>
#include <cstdint>

class vec2 {
public:
	inline vec2() : m_x(0), m_y(0) {}
	inline vec2(real x, real y) : m_x(x), m_y(y) {}
	inline vec2(const vec2& vec) : m_x(vec.x()),m_y(vec.y()) {}

	// addition														
	vec2 operator+(const vec2& vec) {
		return vec2(m_x + vec.x(), m_y + vec.y());
	}
	// subtraction													
	vec2 operator-(const vec2& vec) {
		return vec2(m_x - vec.x(), m_y - vec.y());
	}
	friend vec2 operator-(const vec2& lhs, const vec2& rhs) {
		return vec2(lhs.x() - rhs.x(), lhs.y() - rhs.y());
	}
	// multiplying vectors											
	vec2 operator*(const vec2& vec) {
		return vec2(m_x * vec.x(), m_y * vec.y());
	}
	// scaling vector by a value									
	vec2 operator*(real value) {
		return vec2(m_x * value, m_y * value);
	}
	friend vec2 operator*(const vec2& vec, real value) {
		return vec2(value * vec.x(), value * vec.y());
	}
	real operator[](int i) const {
		assert(i >= 0 && i <= 1);
		vec2 vec(m_x, m_y);
		return vec[i];
	}
	bool operator==(const vec2& r) {
		vec2 diff(vec2(m_x, m_y) - r);
		return len_square(diff) < VEC3_EPSILON;
	}
	friend bool operator==(const vec2& l, const vec2& r) {
		vec2 diff(l - r);
		return len_square(diff) < VEC3_EPSILON;
	}
	bool operator!=(const vec2& r) {
		return !(vec2(m_x, m_y) == r);
	}
	friend bool operator!=(const vec2& l, const vec2& r) {
		return !(l == r);
	}

	// dot product
	// | +ive if the vectors point in the same direction
	// | -ive if the vectors point in opposite direction
	// | 0 if the vectors are perpendicular '|__'
	// | if both lhs and rhs have a unit length, the dot product will have a range of '-1 to 1'
	// | A.B = |A||B|cosθ ('|A|' -> length of 'A')
	static real dot(const vec2& lhs, const vec2& rhs) {
		return lhs.x() * rhs.x() + lhs.y() * rhs.y();
	}
	// length square
	static real len_square(const vec2& vec) {
		return vec.x() * vec.x() + vec.y() * vec.y();
	}
	// length
	static real len(const vec2& vec) {
		auto len_square = vec.x() * vec.x() + vec.y() * vec.y();
		if (len_square < VEC2_EPSILON) {
			return 0.0f;
		}
		return sqrtf(len_square);
	}
	// distance between two vectors
	static real distance(const vec2& lhs, const vec2& rhs) {
		len(lhs - rhs);
	}

	// getters
	inline real x() const { return m_x; }
	inline real y() const { return m_y; }
	inline vec2 get() const { return vec2(m_x, m_y); }

	// setters
	inline void set_x(real x) { m_x = x; }
	inline void set_y(real y) { m_y = y; }
	inline void set_at_index(int index, real value) {
		if (index == 0) m_x = value;
		else if (index == 1) m_y = value;
		else assert(false);
	}
	inline void set_vec(const vec2& vec) {
		m_x = vec.x();
		m_y = vec.y();
	}
private:
	real m_x, m_y;
};