#pragma once

#include "defines.hpp"
#include <math.h>
#include <assert.h>

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
	friend vec3 operator-(const vec3& lhs, const vec3& rhs) {
		return vec3(lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z());
	}
	// multiplying vectors											
	vec3 operator*(const vec3& vec) {								
		return vec3(m_x * vec.x(), m_y * vec.y(), m_z * vec.z());	
	}
	// scaling vector by a value									
	vec3 operator*(real value) {
		return vec3(m_x * value, m_y * value, m_z * value);
	}
	friend vec3 operator*(const vec3& vec, real value) {
		return vec3(value * vec.x(), value * vec.y(), value * vec.z());
	}
	real operator[](int i) const {
		assert(i >= 0 && i <= 2);
		vec3 vec;
		return vec[i];
	}
	bool operator==(const vec3& r) {
		vec3 diff(vec3(m_x, m_y, m_z) - r);
		return len_square(diff) < VEC3_EPSILON;
	}
	friend bool operator==(const vec3& l, const vec3& r) {
		vec3 diff(l - r);
		return len_square(diff) < VEC3_EPSILON;
	}
	bool operator!=(const vec3& r) {
		return !(vec3(m_x, m_y, m_z) == r);
	}
	friend bool operator!=(const vec3& l, const vec3& r) {
		return !(l == r);
	}

	// dot product
	// | +ive if the vectors point in the same direction
	// | -ive if the vectors point in opposite direction
	// | 0 if the vectors are perpendicular '|__'
	// | if both lhs and rhs have a unit length, the dot product will have a range of '-1 to 1'
	// | A.B = |A||B|cosθ ('|A|' -> length of 'A')
	real dot(const vec3& lhs, const vec3& rhs) const {
		return  lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
	}
	// length square
	static real len_square(const vec3& vec) {
		return vec.x() * vec.x() + vec.y() * vec.y() + vec.z() * vec.z();
	}
	// length
	real len(const vec3& vec) const {
		real lenSquare = vec.x() * vec.x() + vec.y() * vec.y() + vec.z() * vec.z();
		if (lenSquare < VEC3_EPSILON) {
			return 0.0f;
		}
		return sqrtf(lenSquare);
	}
	// distance between two vectors
	real distance(const vec3& lhs, const vec3& rhs) const {
		len(lhs - rhs);
	}
	// normalizing a vector
	// normal / unit vector -> a vector with a length of '1'
	// normalizes the vector given
	void normalize(vec3& vec) const;
	// creates a new normalized vector from the given vector and returns it
	vec3 normalized(const vec3& vec) const;
	// angle between two vectors
	// cosθ = A.B    (where A and B are unit vectors)     
    // cosθ = A.B / |A||B|    (if A and B are not unit vectors)
	// θ = cos-1 A.B / |A||B|
	// gives angle in radians
	real angle(const vec3& lhs, const vec3& rhs) const;
	real angle_in_degrees(const vec3& lhs, const vec3& rhs) const {
		// multiply by 0.0174533 to convert degrees to radians
		return angle(lhs, rhs) * 57.2958f; // converting angle from raidans to degrees
	}
	//     /\
	//     /
	//  A / | < Rejection
	//   /  | 
	//  /   |    B
	// ------------->
	//  ^ Projection 
	// 
	// if the vector being projected onto is a unit vector the the lenth of A in direction
	// of B can be found using a simple dot(A, B)
	// if neither is a unit vector then the dot(A, B) needs to be divided by length of B
	vec3 project(const vec3& lhs, const vec3& rhs) const;
	vec3 reject(const vec3& lhs, const vec3& rhs) const;
	// vector reflection - bounce reflection
	//      B
	//     /\  /|
	// \   |   /
	//  \  |  /
	// A \ | / A'
	//    \ /
	// -------------->
	vec3 reflect(const vec3& lhs, const vec3& rhs) const;
	// cross product
	// returns a third product perpendicular to the two vectors given
	// AxB = |A||B|sinθ ('|A|' -> length of 'A')
	vec3 cross(const vec3& lhs, const vec3& rhs) const;
	// interpolating vectors
	// takes the shortest path between two vectors
	// linearly scaling the difference between two vectors and adding it back to the original
	// abbreviated as lerp
	// lerp value is between 0 - 1
	vec3 lerp(const vec3& start, const vec3& end, real t) const;
	// spherical linear interpolation - slerp
	// takes the shortest arc between two vectors
	// the angle between the two vectors needs to be known
	// slerp is constant in velocity
	// slerp is only used when constant interpolation velocity is required
	// slerp(A,B,t) = (sin(1-t)θ/sinθ)A + (sin(tθ)/sinθ)B
	vec3 slerp(const vec3& start, const vec3& end, real t) const;
	// normalized linear interpolation - nlerp
	// nlerp is normalized lerp
	// nlerp is not constant in velocity
	// nlerp approximates slerp
	// nlerp is faster than slerp
	vec3 nlerp(const vec3& start, const vec3& end, real t) const;

	// getters
	inline real x() const { return m_x; }
	inline real y() const { return m_y; }
	inline real z() const { return m_z; }
	inline vec3 get() const { return vec3(m_x, m_y, m_z); }
	inline bool is_normal() const { return m_is_normal; }

	// setters
	inline void set_x(real x) { m_x = x; }
	inline void set_y(real y) { m_y = y; }
	inline void set_z(real z) { m_z = z; }
	inline void set_vec(const vec3& vec) { 
		m_x = vec.x(); 
		m_y = vec.y();
		m_z = vec.z();
	}
private:
	real m_x, m_y, m_z;
	bool m_is_normal = false;
};