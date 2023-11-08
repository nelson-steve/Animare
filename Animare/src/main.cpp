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
    glm::mat4 glm_mat1 = glm::mat4(
        glm::vec4(1, 2, 3, 4),
        glm::vec4(5, 6, 7, 8),
        glm::vec4(9, 10, 11, 12),
        glm::vec4(13, 14, 15, 16));

    glm::mat4 glm_mat2 = glm::mat4(
        glm::vec4(2, 4, 5, 7),
        glm::vec4(5, 34, 45, 56),
        glm::vec4(34, 12, 15, 15),
        glm::vec4(23, 63, 63, 34));

    mat4 mat1 = mat4(
        vec4(1, 2, 3, 4),
        vec4(5, 6, 7, 8),
        vec4(9, 10, 11, 12),
        vec4(13, 14, 15, 16));

    mat4 mat2 = mat4(
        vec4(2, 4, 5, 7),
        vec4(5, 34, 45, 56),
        vec4(34, 12, 15, 15),
        vec4(23, 63, 63, 34));
    glm::mat4 result_glm = glm::inverse(glm_mat1);
    mat4 result = mat4::inverse(mat1);

    assert(compare_glm_mat4_with_mat4(result_glm, result));

    //renderer* r = new renderer();
    //r->render();
    //r->destroy();
    //delete r;
}