#pragma once

#include "track.hpp"

class transform_track {
public:
    transform_track(){}
    
    void set_position_track(const track& _track) { m_position_track = _track; }
    void set_rotation_track(const track& _track) { m_position_track = _track; }
    void set_scale_track(const track& _track) { m_position_track = _track; }

    const track& get_position_track() const { return m_position_track; }
    track& get_position_track() { return m_position_track; }
    const track& get_rotation_track() const { return m_rotation_track; }
    track& get_rotation_track() { return m_rotation_track; }
    const track& get_scale_track() const { return m_scale_track; }
    track& get_scale_track() { return m_scale_track; }

    bool is_valid();
    real get_start_time();
    real get_end_time();
    void sample();

private:
    track m_position_track;
    track m_rotation_track;
    track m_scale_track;
};