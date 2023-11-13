#pragma once

#include "track.hpp"

class clip {
public:
    clip(){}

    void set_looping(bool looping) { m_looping = looping; }

    bool is_looping() const { return m_looping; }
    void add_track(track t) { m_tracks.push_back(t); }
    const std::vector<track>& get_tracks() const { return m_tracks; }
    std::vector<track>& get_tracks() { return m_tracks; }
    uint32_t get_clip_size() const { m_tracks.size(); }
    const track& get_track_at_index(int i) const { return m_tracks[i]; }
    track& get_track_at_index(int i) { return m_tracks[i]; }
    void set_target_node(uint32_t track_index, node* _node) {
        m_tracks[track_index].set_target_node(_node);
    }
public:
    real start = std::numeric_limits<float>::max();
    real end = std::numeric_limits<float>::min();
private:
    bool m_looping;
    std::vector<track> m_tracks;
};