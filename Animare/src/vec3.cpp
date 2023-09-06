#include "vec3.hpp"

void vec3::normalize(vec3& vec) {
	real len_square = vec.x() * vec.x() + vec.y() * vec.y() + vec.z() * vec.z();
	if (len_square < VEC3_EPSILON) { return; }
	real inv_length = 1.0f / sqrtf(len_square);
	vec.set_x(vec.x() * inv_length);
	vec.set_y(vec.y() * inv_length);
	vec.set_z(vec.z() * inv_length);
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

real vec3::angle(const vec3& lhs, const vec3& rhs) const {
	real square_mag_left = lhs.x() * lhs.x() + lhs.y() * lhs.y() + lhs.z() * lhs.z();
	real square_mag_right = rhs.x() * lhs.x() + lhs.y() * lhs.y() + lhs.z() * lhs.z();
	if (square_mag_left < VEC3_EPSILON || square_mag_right < VEC3_EPSILON) {
		return 0.0f;
	}
	real dot = lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
	real len = sqrtf(square_mag_left) * sqrtf(square_mag_right);
	return acosf(dot / len);
}

vec3 vec3::project(const vec3& lhs, const vec3& rhs) const {
	real scale;
	if (rhs.is_normal()) {
		scale = dot(lhs, rhs);
	}
	else {
		real magnitude_rhs = len(rhs);
		if (magnitude_rhs < VEC3_EPSILON) {
			return vec3();
		}
		scale = dot(lhs, rhs) / magnitude_rhs;
	}
	return rhs * scale;
}

vec3 vec3::reject(const vec3& lhs, const vec3& rhs) const {
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