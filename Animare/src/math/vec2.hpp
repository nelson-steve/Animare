#pragma once

#include "../defines.hpp"

#include <math.h>
#include <assert.h>
#include <cstdint>

// templated vec2 class
// can create int vec2 -> i_vec2
// and unsigned vec2 -> u_vec2
// and real(float, double) vec2 -> vec2
template<typename T>
class vec2_t {
public:
	inline vec2_t() : m_x(0), m_y(0) {}
	inline vec2_t(T x, T y) : m_x(x), m_y(y) {}
	inline vec2_t(const vec2_t& vec) : m_x(vec.x),m_y(vec.y) {}

	// addition														
	vec2_t operator+(const vec2_t& vec) {
		return vec2_t(m_x + vec.x(), m_y + vec.y());
	}
	// subtraction													
	vec2_t operator-(const vec2_t& vec) {
		return vec2_t(m_x - vec.x(), m_y - vec.y());
	}
	friend vec2_t operator-(const vec2_t& lhs, const vec2_t& rhs) {
		return vec2_t(lhs.x() - rhs.x(), lhs.y() - rhs.y());
	}
	// multiplying vectors											
	vec2_t operator*(const vec2_t& vec) {
		return vec2_t(m_x * vec.x(), m_y * vec.y());
	}
	// scaling vector by a value									
	vec2_t operator*(real value) {
		return vec2_t(m_x * value, m_y * value);
	}
	friend vec2_t operator*(const vec2_t& vec, real value) {
		return vec2_t(value * vec.x(), value * vec.y());
	}
	real operator[](int i) const {
		assert(i >= 0 && i <= 1);
		vec2_t vec(m_x, m_y);
		return vec[i];
	}
	bool operator==(const vec2_t& r) {
		vec2_t diff(vec2_t(m_x, m_y) - r);
		return len_square(diff) < VEC3_EPSILON;
	}
	friend bool operator==(const vec2_t& l, const vec2_t& r) {
		vec2_t diff(l - r);
		return len_square(diff) < VEC3_EPSILON;
	}
	bool operator!=(const vec2_t& r) {
		return !(vec2_t(m_x, m_y) == r);
	}
	friend bool operator!=(const vec2_t& l, const vec2_t& r) {
		return !(l == r);
	}

	// dot product
	// | +ive if the vectors point in the same direction
	// | -ive if the vectors point in opposite direction
	// | 0 if the vectors are perpendicular '|__'
	// | if both lhs and rhs have a unit length, the dot product will have a range of '-1 to 1'
	// | A.B = |A||B|cosθ ('|A|' -> length of 'A')
	T dot(const vec2_t& lhs, const vec2_t& rhs) const {
		return  lhs.x() * rhs.x() + lhs.y() * rhs.y();
	}
	// length square
	static T len_square(const vec2_t& vec) {
		return vec.x() * vec.x() + vec.y() * vec.y();
	}
	// length
	static T len(const vec2_t& vec) {
		T len_square = vec.x() * vec.x() + vec.y() * vec.y();
		if (len_square < VEC3_EPSILON) {
			return 0.0f;
		}
		return sqrtf(len_square);
	}
	// distance between two vectors
	static real distance(const vec2_t& lhs, const vec2_t& rhs) {
		len(lhs - rhs);
	}

	// getters
	inline T x() const { return m_x; }
	inline T y() const { return m_y; }
	inline vec2_t get() const { return vec2_t(m_x, m_y); }

	// setters
	inline void set_x(T x) { m_x = x; }
	inline void set_y(T y) { m_y = y; }
	inline void set_at_index(int index, real value) {
		if (index == 0) m_x = value;
		else if (index == 1) m_y = value;
		else assert(false);
	}
	inline void set_vec(const vec2_t& vec) {
		m_x = vec.x();
		m_y = vec.y();
	}
private:
	T m_x, m_y;
};

typedef vec2_t<real> vec2;
typedef vec2_t<int> i_vec2;
typedef vec2_t<uint32_t> u_vec2;