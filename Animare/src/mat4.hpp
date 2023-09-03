#pragma once

#include "vec4.hpp"
#include "defines.hpp"

#include <assert.h>

// column major matrix
class mat4 {
public:
	inline mat4() :
		m_right    (1.0f, 0.0f, 0.0f, 0.0f),
		m_up	   (0.0f, 1.0f, 0.0f, 0.0f),
		m_forward  (0.0f, 0.0f, 1.0f, 0.0f),
		m_position (0.0f, 0.0f, 0.0f, 1.0f) 
	{}
	inline mat4(
		float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33) :
		m_right	   (_00, _01, _02, _03),
		m_up	   (_10, _11, _12, _13),
		m_forward  (_20, _21, _22, _23),
		m_position (_30, _31, _32, _33) 
	{}

	// operators
	friend bool operator==(const mat4& lhs, const mat4& rhs) {
		for (int i = 0; i < 16; ++i) {
			if (fabsf(lhs[i] - rhs[i]) > MAT4_EPSILON) {
				return false;
			}
		}
		return true;
	}
	friend bool operator!=(const mat4& a, const mat4& b) {
		return !(a == b);
	}
	real operator[](int i) const {
		assert(i >= 0 && i <= 15);
		if (i >= 0 && i <= 3) return  m_right[i];
		if (i >= 4 && i <= 7) return  m_up[i - 4];
		if (i >= 8 && i <= 11) return m_forward[i - 8];
		if (i >= 12 && i <= 15) return m_position[i - 12];
	}
	friend mat4 operator+(const mat4& a, const mat4& b) {
		return mat4(
			a.xx() + b.xx(), a.xy() + b.xy(), a.xz() + b.xz(), a.xw() + b.xw(),
			a.yx() + b.yx(), a.yy() + b.yy(), a.yz() + b.yz(), a.yw() + b.yw(),
			a.zx() + b.zx(), a.zy() + b.zy(), a.zz() + b.zz(), a.zw() + b.zw(),
			a.wx() + b.wx(), a.wy() + b.wy(), a.wz() + b.wz(), a.ww() + b.ww()
		);
	}
	friend mat4 operator*(const mat4& m, float f) {
		return mat4(
			m.xx() * f, m.xy() * f, m.xz() * f, m.xw() * f,
			m.yx() * f, m.yy() * f, m.yz() * f, m.yw() * f,
			m.zx() * f, m.zy() * f, m.zz() * f, m.zw() * f,
			m.wx() * f, m.wy() * f, m.wz() * f, m.ww() * f
		);
	}

	// getters
	vec4 get_right()	const { return m_right; }
	vec4 get_up()		const { return m_up; }
	vec4 get_forward()  const { return m_forward; }
	vec4 get_position() const { return m_position; }
	//
	real xx() const { return m_right.x(); }
	real xy() const { return m_right.y(); }
	real xz() const { return m_right.z(); }
	real xw() const { return m_right.w(); }
	//
	real yx() const { return m_up.x(); }
	real yy() const { return m_up.y(); }
	real yz() const { return m_up.z(); }
	real yw() const { return m_up.w(); }
	//
	real zx() const { return m_forward.x(); }
	real zy() const { return m_forward.y(); }
	real zz() const { return m_forward.z(); }
	real zw() const { return m_forward.w(); }
	//
	real wx() const { return m_position.x(); }
	real wy() const { return m_position.y(); }
	real wz() const { return m_position.z(); }
	real ww() const { return m_position.w(); }
private:
	vec4 m_right;
	vec4 m_up;
	vec4 m_forward;
	vec4 m_position;
};