#pragma once

#include "defines.hpp"

#include <math.h>
#include <cstdint>

class vec4 {
public:
	inline vec4() : m_x(0), m_y(0), m_z(0), m_w(0) {}
	inline vec4(real x, real y, real z, real w) : m_x(x), m_y(y), m_z(z), m_w(w) {}
	inline vec4(const vec4& vec) : m_x(vec.x()), m_y(vec.y()), m_z(vec.z()), m_w(vec.w()) {}

	// addition														
	vec4 operator+(const vec4& vec) {
		return vec4(m_x + vec.x(), m_y + vec.y(), m_z + vec.z(), m_w + vec.w());
	}
	// subtraction													
	vec4 operator-(const vec4& vec) {
		return vec4(m_x - vec.x(), m_y - vec.y(), m_z - vec.z(), m_w - vec.w());
	}
	friend vec4 operator-(const vec4& lhs, const vec4& rhs) {
		return vec4(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z(), lhs.w() - rhs.w());
	}
	// multiplying vectors											
	vec4 operator*(const vec4& vec) {
		return vec4(m_x * vec.x(), m_y * vec.y(), m_z * vec.z(), m_w * vec.w());
	}
	// scaling vector by a value									
	vec4 operator*(real value) {
		return vec4(m_x * value, m_y * value, m_z * value, m_w * value);
	}
	friend vec4 operator*(const vec4& vec, real value) {
		return vec4(value * vec.x(), value * vec.y(), value * vec.z(), value * vec.w());
	}
	real operator[](int i) const {
		assert(i >= 0 && i <= 3);
		vec4 vec(m_x, m_y, m_z, m_w);
		return vec[i];
	}
	bool operator==(const vec4& r) {
		vec4 diff(vec4(m_x, m_y, m_z, m_w) - r);
		return len_square(diff) < VEC4_EPSILON;
	}
	friend bool operator==(const vec4& lhs, const vec4& rhs) {
		vec4 diff(lhs - rhs);
		return len_square(diff) < VEC4_EPSILON;
	}
	bool operator!=(const vec4& r) {
		return !(vec4(m_x, m_y, m_z, m_w) == r);
	}
	friend bool operator!=(const vec4& l, const vec4& r) {
		return !(l == r);
	}
	// length square
	static real len_square(const vec4& vec) {
		return vec.x() * vec.x() + vec.y() * vec.y() + vec.z() * vec.z();
	}
	// length
	real len(const vec4& vec) const {
		real len_square = vec.x() * vec.x() + vec.y() * vec.y() + vec.z() * vec.z();
		if (len_square < VEC4_EPSILON) {
			return 0.0f;
		}
		return sqrtf(len_square);
	}
	// distance between two vectors
	real distance(const vec4& lhs, const vec4& rhs) const {
		len(lhs - rhs);
	}

	// getters
	inline real x() const { return m_x; }
	inline real y() const { return m_y; }
	inline real z() const { return m_z; }
	inline real w() const { return m_w; }
	inline vec4 get() const { return vec4(m_x, m_y, m_z, m_w); }

	// setters
	inline void set_x(real x) { m_x = x; }
	inline void set_y(real y) { m_y = y; }
	inline void set_z(real z) { m_z = z; }
	inline void set_w(real w) { m_w = w; }
	inline void set_vec(const vec4& vec) {
		m_x = vec.x();
		m_y = vec.y();
		m_z = vec.z();
		m_w = vec.w();
	}
private:
	real m_x, m_y, m_z, m_w;
};