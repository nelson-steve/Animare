#pragma once

#include "track.hpp"

class clip {
public:
    clip();

    void set_looping(bool looping) { m_looping = looping; }
    void set_duration(real duration) { m_duration = duration; }

    bool is_looping() const { return m_looping; }
    real get_duration() const { return m_duration; }
    void add_track(const track& t) { m_tracks.push_back(t); }
    std::vector<track> get_tracks() const { return m_tracks; }
    uint32_t get_clip_size() const { m_tracks.size(); }
    const track& get_track_at_index(int i) const { return m_tracks[i]; }
    track& get_track_at_index(int i) { return m_tracks[i]; }
private:
    bool m_looping;
    real m_duration;
    std::vector<track> m_tracks;
};