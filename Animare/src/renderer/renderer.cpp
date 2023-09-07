#include "renderer.hpp"

#include <assert.h>

#include <GLFW/glfw3.h>

renderer::renderer() {
    if (!glfwInit())
        assert(false);
}

void renderer::update() {

}

void renderer::destroy() {

}