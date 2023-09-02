#pragma once

#include "vec4.hpp"

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