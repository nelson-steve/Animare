#pragma once

#include "defines.hpp"

#include <glm/glm.hpp>

#include <cstdint>
#include <assert.h>
#include <vector>

class frame {
public:
    enum class frame_type {
        scalar,
        vector,
        quaternion
    } m_frame_type;

    frame(){}
    frame(frame_type type) {
        if (type == frame_type::scalar) {
            //m_value.resize(1);
            //m_in_tangent.resize(1);
            //m_out_tangent.resize(1);
        }
        else if (type == frame_type::vector) {
            //m_value.resize(3);
            //m_in_tangent.resize(3);
            //m_out_tangent.resize(3);
        }
        else if (type == frame_type::quaternion) {
            //m_value.resize(4);
            //m_in_tangent.resize(4);
            //m_out_tangent.resize(4);
        }
        else {
            assert(false);
        }
    }

    real get_time() const { return m_time; }

    const glm::vec4& get_value() const { return m_value; }

    void set_time(real time) { m_time = time; }
    void set_value(glm::vec4 value) { m_value = value; }

    void set_frame(frame_type type) {
        if (type == frame_type::scalar) {
            //m_value.resize(1);
            //m_in_tangent.resize(1);
            //m_out_tangent.resize(1);
        }
        else if (type == frame_type::vector) {
            //m_value.resize(3);
            //m_in_tangent.resize(3);
            //m_out_tangent.resize(3);
        }
        else if (type == frame_type::quaternion) {
            //m_value.resize(4);
            //m_in_tangent.resize(4);
            //m_out_tangent.resize(4);
        }
        else {
            assert(false);
        }
    }
private:
    glm::vec4 m_value;
    std::vector<real> m_in_tangent;
    std::vector<real> m_out_tangent;
    real m_time;
};