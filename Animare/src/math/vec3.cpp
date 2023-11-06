#include "vec3.hpp"

#include "defines.hpp"

void vec3::normalize() {
	real len_square = get().len_square();
	if (len_square < VEC3_EPSILON) { return; }
	real inv_length = 1.0f / sqrtf(len_square);
	m_x = m_x * inv_length;
	m_y = m_y * inv_length;
	m_z = m_z * inv_length;
}

vec3 vec3::normalized(const vec3& vec) {
	real len_square = vec.x() * vec.x() + vec.y() * vec.y() + vec.z() * vec.z();
	if (len_square < VEC3_EPSILON) { return vec; }
	real inv_length = 1.0f / sqrtf(len_square);
	vec3 new_vec(
		vec.x() * inv_length,
		vec.y() * inv_length,
		vec.z() * inv_length
	);
	return new_vec;
}

real vec3::angle(const vec3& lhs, const vec3& rhs) {
	real square_mag_left = lhs.len_square();
	real square_mag_right = rhs.len_square();
	if (square_mag_left < VEC3_EPSILON || square_mag_right < VEC3_EPSILON) {
		return 0.0f;
	}
	real dot = vec3::dot(lhs, rhs);
	real sqrt_left = sqrtf(square_mag_left);
	real sqrt_right = sqrtf(square_mag_right);
	real value = dot / (sqrt_left * sqrt_right);
	real result = acosf(value);
	return result;
}

vec3 vec3::project(const vec3& lhs, const vec3& rhs) {
	real scale;
	if (rhs.is_normalized()) {
		scale = dot(lhs, rhs);
	}
	else {
		real magnitude_rhs = len(rhs);
		if (magnitude_rhs < VEC3_EPSILON) {
			return vec3();
		}
		scale = dot(lhs, rhs) / magnitude_rhs;
	}
	return vec3(vec3::normalized(rhs)) * scale;
}

vec3 vec3::reject(const vec3& lhs, const vec3& rhs) {
	return lhs - project(lhs, rhs);
}

vec3 vec3::reflect(const vec3& lhs, const vec3& rhs) {
	float magnitude_rhs = len(rhs);
	if (magnitude_rhs < VEC3_EPSILON) {
		return vec3();
	}
	float scale = dot(lhs, rhs) / magnitude_rhs;
	vec3 projection_rhs = rhs * (scale * 2);
	return lhs - projection_rhs;
}

vec3 vec3::cross(const vec3& lhs, const vec3& rhs) {
	return vec3(
		lhs.y() * rhs.z() - lhs.z() * rhs.y(),
		lhs.z() * rhs.x() - lhs.x() * rhs.z(),
		lhs.x() * rhs.y() - lhs.y() * rhs.x()
	);
}

vec3 vec3::lerp(const vec3& start, const vec3& end, real t) {
	return vec3(
		start.x() + (end.x() - start.x()) * t,
		start.y() + (end.y() - start.y()) * t,
		start.z() + (end.z() - start.z()) * t
	);
}

vec3 vec3::slerp(const vec3& start, const vec3& end, real t) const {
	if (t < 0.01f) {
		// if t is close to 0 slerpt will give unexpected results
		// so falling back to lerp
		return lerp(start, end, t);
	}
	vec3 from = normalized(start);
	vec3 to = normalized(end);
	real theta = angle(from, to);
	real sin_theta = sinf(theta);
	real a = sinf((1.0f - t) * theta) / sin_theta;
	real b = sinf(t * theta) / sin_theta;
	return from * a + to * b;
}

vec3 vec3::nlerp(const vec3& start, const vec3& end, real t) {
	vec3 linear(
		start.x() + (end.x() - start.x()) * t,
		start.y() + (end.y() - start.y()) * t,
		start.z() + (end.z() - start.z()) * t
	);
	return normalized(linear);
}