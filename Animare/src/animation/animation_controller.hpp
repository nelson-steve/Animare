#pragma once 

#include "clip.hpp"
#include "renderer/model.hpp"

#include <tiny_gltf.h>

class shader;
class node;

class animation_controller {
public:
    animation_controller(model& _model);
    void play_animation(model& _model, real dt, shader& _shader);
    void update(node* _node, shader& _shader);
private:
    real m_time = 0.0f;
};