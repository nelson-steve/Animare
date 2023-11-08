#include <iostream>

#include "renderer/renderer.hpp"
#include "math/mat4.hpp"

bool compare_glm_mat4_with_mat4(glm::mat4 glm_matrix, mat4 matrix) {
    return(glm_matrix == glm::mat4(
        glm::vec4(matrix.get_right().x(), matrix.get_right().y(), matrix.get_right().z(), matrix.get_right().w()),
        glm::vec4(matrix.get_up().x(), matrix.get_up().y(), matrix.get_up().z(), matrix.get_up().w()),
        glm::vec4(matrix.get_forward().x(), matrix.get_forward().y(), matrix.get_forward().z(), matrix.get_forward().w()),
        glm::vec4(matrix.get_position().x(), matrix.get_position().y(), matrix.get_position().z(), matrix.get_position().w())
    ));
}

int main(int argc, char** argv) {
    renderer* r = new renderer();
    r->render();
    r->destroy();
    delete r;
}