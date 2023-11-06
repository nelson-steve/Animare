#include "track.hpp"

real track::sample(float time, bool looping) {
    return 0.1;
}

real track::interpolate(real a, real b, real t) {
    return a + (b - a) * t;
}

const glm::vec3& track::interpolate(const glm::vec3& a, const glm::vec3& b, real t) {
    vec3 _a(a.x, a.y, a.z);
    vec3 _b(b.x, b.y, b.z);
    vec3 result = vec3::lerp(_a, _b, t);
    return glm::vec3(result.x(), result.y(), result.z());
}

const quaternion& track::interpolate(const glm::quat& a, const glm::quat& b, real t) {
    glm::mix(a, b, t);
    return quaternion();
}