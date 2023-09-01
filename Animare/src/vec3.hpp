#pragma once

#include "defines.h"

// Vector class
class vec3 {
public:
	// constructors ----------------------------------------------- //
	inline vec3() : m_x(0.0f), m_y(0.0f), m_z(0.0f) {}				//
	inline vec3(real x, real y, real z) : m_x(x), m_y(y), m_z(z) {}	//
	// ------------------------------------------------------------ //

	// addition														
	vec3 operator+(const vec3& vec) {								
		return vec3(m_x + vec.x(), m_y + vec.y(), m_z + vec.z());	
	}
	// subtraction													
	vec3 operator-(const vec3& vec) {								
		return vec3(m_x - vec.x(), m_y - vec.y(), m_z - vec.z());	
	}
	// multiplying vectors											
	vec3 operator*(const vec3& vec) {								
		return vec3(m_x * vec.x(), m_y * vec.y(), m_z * vec.z());	
	}
	// scaling vector by a value									
	vec3 operator*(float value) {									
		return vec3(m_x * value, m_y * value, m_z * value);			
	}

	// dot product
	// | +ive if the vectors point in the same direction
	// | -ive if the vectors point in opposite direction
	// | 0 if the vectors are perpendicular '|__'
	// | if both lhs and rhs have a unit length, the dot product will have a range of '-1 to 1'
	// | A.B = |A|.|B|cosθ ('|A|' -> length of 'A')
	float dot(const vec3& lhs, const vec3& rhs) {
		return  lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
	}

	// getters
	inline real x() const { return m_x; }
	inline real y() const { return m_y; }
	inline real z() const { return m_z; }
	inline vec3 get() const { return vec3(m_x, m_y, m_z); }
private:
	real m_x, m_y, m_z;
};