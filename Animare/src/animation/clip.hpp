#pragma once

#include "track.hpp"

class clip {
public:
    clip();

    void set_looping(bool looping) { m_looping = looping; }
    void set_duration(real duration) { m_duration = duration; }

    bool is_looping() const { return m_looping; }
    real get_duration() const { return m_duration; }
    std::vector<track> get_tracks() const { return m_tracks; }
    uint32_t get_clip_size() const { m_tracks.size(); }
private:
    bool m_looping;
    real m_duration;
    std::vector<track> m_tracks;
};