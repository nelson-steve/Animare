#pragma once

#include "frame.hpp"
#include "math/vec3.hpp"
#include "math/quaternion.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>

class track {
public:
    enum class interpolation {
        constant,
        linear,
        cubic
    };

    enum class data_type {
        rotation,
        translation,
        scale
    };
    track() {
        m_interpolation = interpolation::linear;
    }
    track(interpolation interp) {
        m_interpolation = interp;
    }
    void add_frame(frame frame) {
        m_frames.push_back(frame);
    }
    frame& get_frame_at_index(int i) { return m_frames[i]; }
    void resize(uint32_t size) {
        m_frames.resize(size);
    }
    uint32_t get_size() {
        return m_frames.size();
    }
    interpolation get_interpolation() {
        return m_interpolation;
    }
    void set_interpolation(interpolation interp) {
        m_interpolation = interp;
    }
    void set_start_time(real time) { m_start_time = time; }
    void set_end_time(real time) { m_end_time= time; }
    real get_start_time() {
        return m_start_time;
    }
    real get_end_time() {
        return m_end_time;
    }
    void set_data_type(data_type type) { m_data_type = type; }
    data_type get_data_type() const { return m_data_type; }
    frame operator[](unsigned int index) {
        if (!m_frames.empty()) {
            return m_frames[index];
        }
        assert(false);
    }

    real sample(float time, bool looping);
    real interpolate(real a, real b, real t);
    const glm::vec3& interpolate(const glm::vec3& a, const glm::vec3& b, real t);
    const quaternion& interpolate(const glm::quat& a, const glm::quat& b, real t);

protected:
    template<typename T>
    T Hermite(float time, const T& p1, const T& s1, const T& p2, const T& s2);
    int FrameIndex(float time, bool looping);
    float AdjustTimeToFitTrack(float t, bool loop);
    template<typename T>
    T Cast(float* value);
private:
    real m_start_time = 0.0f;
    real m_end_time = 0.0f;
    std::vector<frame> m_frames;
    interpolation m_interpolation;
    data_type m_data_type;
};

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
            //result = quaternion::mix(a, -b, t);
        }
        return quaternion::normalized(result); //NLerp, not slerp
    }
}