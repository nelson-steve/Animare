#pragma once

#include "../defines.hpp"

#include <math.h>
#include <cstdint>

// templated vec4 class
// can create int vec4 -> i_vec4
// and unsigned vec4 -> u_vec4
// and real(float, double) vec4 -> vec4
template<typename T>
class vec4_t {
public:
	inline vec4_t() : m_x(0), m_y(0), m_z(0), m_w(0) {}
	inline vec4_t(T x, T y, T z, T w) : m_x(x), m_y(y), m_z(z), m_w(w) {}
	inline vec4_t(const vec4_t& vec) : m_x(vec.x()), m_y(vec.y()), m_z(vec.z()), m_w(vec.w()) {}

	// addition														
	vec4_t operator+(const vec4_t& vec) {
		return vec4_t(m_x + vec.x(), m_y + vec.y(), m_z + vec.z(), m_w + vec.w());
	}
	// subtraction													
	vec4_t operator-(const vec4_t& vec) {
		return vec4_t(m_x - vec.x(), m_y - vec.y(), m_z - vec.z(), m_w - vec.w());
	}
	friend vec4_t operator-(const vec4_t& lhs, const vec4_t& rhs) {
		return vec4_t(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z(), lhs.w() - rhs.w());
	}
	// multiplying vectors											
	vec4_t operator*(const vec4_t& vec) {
		return vec4_t(m_x * vec.x(), m_y * vec.y(), m_z * vec.z(), m_w * vec.w());
	}
	// scaling vector by a value									
	vec4_t operator*(real value) {
		return vec4_t(m_x * value, m_y * value, m_z * value, m_w * value);
	}
	friend vec4_t operator*(const vec4_t& vec, real value) {
		return vec4_t(value * vec.x(), value * vec.y(), value * vec.z(), value * vec.w());
	}
	real operator[](int i) const {
		assert(i >= 0 && i <= 3);
		vec4_t vec(m_x, m_y, m_z, m_w);
		return vec[i];
	}
	bool operator==(const vec4_t& r) {
		vec4_t diff(vec4_t(m_x, m_y, m_z, m_w) - r);
		return len_square(diff) < VEC4_EPSILON;
	}
	friend bool operator==(const vec4_t& lhs, const vec4_t& rhs) {
		vec4_t diff(lhs - rhs);
		return len_square(diff) < VEC4_EPSILON;
	}
	bool operator!=(const vec4_t& r) {
		return !(vec4_t(m_x, m_y, m_z, m_w) == r);
	}
	friend bool operator!=(const vec4_t& l, const vec4_t& r) {
		return !(l == r);
	}
	// length square
	static T len_square(const vec4_t& vec) {
		return vec.x() * vec.x() + vec.y() * vec.y() + vec.z() * vec.z();
	}
	// length
	T len(const vec4_t& vec) const {
		T len_square = vec.x() * vec.x() + vec.y() * vec.y() + vec.z() * vec.z();
		if (len_square < VEC4_EPSILON) {
			return 0.0f;
		}
		return sqrtf(len_square);
	}
	// distance between two vectors
	real distance(const vec4_t& lhs, const vec4_t& rhs) const {
		len(lhs - rhs);
	}

	// getters
	inline T x() const { return m_x; }
	inline T y() const { return m_y; }
	inline T z() const { return m_z; }
	inline T w() const { return m_w; }
	inline vec4_t get() const { return vec4_t(m_x, m_y, m_z, m_w); }

	// setters
	inline void set_x(T x) { m_x = x; }
	inline void set_y(T y) { m_y = y; }
	inline void set_z(T z) { m_z = z; }
	inline void set_w(T w) { m_w = w; }
	inline void set_vec(const vec4_t& vec) {
		m_x = vec.x();
		m_y = vec.y();
		m_z = vec.z();
		m_w = vec.w();
	}
private:
	T m_x, m_y, m_z, m_w;
};

typedef vec4_t<real> vec4;
typedef vec4_t<int> i_vec4;
typedef vec4_t<uint32_t> u_vec4;