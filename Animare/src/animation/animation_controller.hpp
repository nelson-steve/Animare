#pragma once 

#include "clip.hpp"

#include <tiny_gltf.h>

class animation_controller {
private:
    animation_controller(tinygltf::Model model);
    void play_track();
private:
    clip m_animation_clip;
};