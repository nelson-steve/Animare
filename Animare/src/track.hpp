#pragma once

#include "frame.hpp"
#include "math/vec3.hpp"
#include "math/quaternion.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>

enum interpolation {
    constant,
    linear,
    cubic
};

template<typename T, int n>
class track {
public:
    track();
    
    void resize(uint32_t size);
    uint32_t get_size();
    interpolation get_interpolation();
    void set_interpolation(interpolation interp);
    real get_start_time();
    real get_end_time();
    T Sample(float time, bool looping);
    frame<n>& operator[](unsigned int index);

protected:
    T SampleConstant(float time, bool looping);
    T SampleLinear(float time, bool looping);
    T SampleCubic(float time, bool looping);
    T Hermite(float time, const T& p1, const T& s1, const T& p2, const T& s2);
    int FrameIndex(float time, bool looping);
    float AdjustTimeToFitTrack(float t, bool loop);
    T Cast(float* value);
protected:
    std::vector<frame<n>> m_frames;
    interpolation m_interpolation;
};

typedef track<float, 1> ScalarTrack;
typedef track<glm::vec3, 3> VectorTrack;
typedef track<glm::quat, 4> QuaternionTrack;

namespace track_helper {
    inline float Interpolate(float a, float b, float t) {
        return a + (b - a) * t;
    }
    inline vec3 Interpolate(const vec3& a, const vec3& b, float t) {
        return vec3::lerp(a, b, t);
    }
    inline quaternion Interpolate(const quaternion& a, const quaternion& b,
        float t) {
        quaternion result = quaternion::mix(a, b, t);
        if (quaternion::dot(a, b) < 0) { // Neighborhood
            result = quaternion::mix(a, -b, t);
        }
        return quaternion::normalized(result); //NLerp, not slerp
    }
}