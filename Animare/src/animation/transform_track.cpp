#include "transform_track.hpp"

bool transform_track::is_valid() {
    if (m_position_track.get_size() > 0 ||
        m_rotation_track.get_size() > 0 ||
        m_scale_track.get_size() > 0) {
        return true;
    }
    return false;
}

real transform_track::get_start_time() {
    real start_time = 0.0f;
    bool is_set = false;
    if (m_position_track.get_size() > 0) {
        start_time = m_position_track.get_start_time();
        is_set = true;
    }
    if (m_rotation_track.get_size() > 0) {
        float time = m_rotation_track.get_start_time();
        if (time < start_time || !is_set) {
            start_time = time;
            is_set = true;
        }
    }
    if (m_scale_track.get_size() > 0) {
        float time = m_scale_track.get_start_time();
        if (time < start_time || !is_set) {
            start_time = time;
            is_set = true;
        }
    }
    return start_time;
}

real transform_track::get_end_time() {
    real result = 0.0f;
    bool is_set = false;
    if (m_position_track.get_size() > 1) {
        result = m_position_track.get_end_time();
        is_set = true;
    }
    if (m_rotation_track.get_size() > 1) {
        float time = m_rotation_track.get_end_time();
        if (time > result || !is_set) {
            result = time;
            is_set = true;
        }
    }
    if (m_scale_track.get_size() > 1) {
        float time = m_scale_track.get_end_time();
        if (time > result || !is_set) {
            result = time;
            is_set = true;
        }
    }
    return result;
}

void transform_track::sample() {

}