#include <iostream>

#include "math/vec2.hpp"
#include "math/vec3.hpp"
#include "math/vec4.hpp"
#include "math/mat4.hpp"
#include "renderer/renderer.hpp"

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/projection.hpp>

void test_vec3_class();

void test_vec3_minus(const glm::vec3& first, const glm::vec3& second) {
    std::cout << "_____ MINUS _____" << std::endl;
    vec3 one(first.x, first.y, first.z);
    vec3 two(second.x, second.y, second.z);

    glm::vec3 glm_one = first;
    glm::vec3 glm_two = second;

    vec3 result = one - two;
    glm::vec3 glm_result = glm_one - glm_two;

    assert(glm_result == glm::vec3(result.x(), result.y(), result.z()));

    std::cout << glm_result.x << " " << glm_result.y << " " << glm_result.z << " = ";
    std::cout << result.x() << " " << result.y() << " " << result.z() << std::endl;
}

void test_vec3_plus(const glm::vec3& first, const glm::vec3& second) {
    std::cout << "_____ PLUS _____" << std::endl;
    vec3 one(first.x, first.y, first.z);
    vec3 two(second.x, second.y, second.z);

    glm::vec3 glm_one = first;
    glm::vec3 glm_two = second;

    vec3 result = one + two;
    glm::vec3 glm_result = glm_one + glm_two;

    assert(glm_result == glm::vec3(result.x(), result.y(), result.z()));

    std::cout << glm_result.x << " " << glm_result.y << " " << glm_result.z << " = ";
    std::cout << result.x() << " " << result.y() << " " << result.z() << std::endl;
}

void test_vec3_multiply(const glm::vec3& first, const glm::vec3& second) {
    std::cout << "_____ MULTIPLY _____" << std::endl;
    vec3 one(first.x, first.y, first.z);
    vec3 two(second.x, second.y, second.z);

    glm::vec3 glm_one = first;
    glm::vec3 glm_two = second;

    vec3 result = one * two;
    glm::vec3 glm_result = glm_one * glm_two;

    assert(glm_result == glm::vec3(result.x(), result.y(), result.z()));

    std::cout << glm_result.x << " " << glm_result.y << " " << glm_result.z << " = ";
    std::cout << result.x() << " " << result.y() << " " << result.z() << std::endl;
}

void test_vec3_dot(const glm::vec3& first, const glm::vec3& second) {
    std::cout << "_____ DOT PRODUCT _____" << std::endl;
    vec3 one(first.x, first.y, first.z);
    vec3 two(second.x, second.y, second.z);

    glm::vec3 glm_one = first;
    glm::vec3 glm_two = second;

    real result = vec3::dot(one, two);
    real glm_result = glm::dot(glm_one, glm_two);

    assert(glm_result == result);

    std::cout << glm_result << " = ";
    std::cout << result << std::endl;
}

void test_vec3_distance(const glm::vec3& first, const glm::vec3& second) {
    std::cout << "_____ DISTANCE _____" << std::endl;
    vec3 one(first.x, first.y, first.z);
    vec3 two(second.x, second.y, second.z);

    glm::vec3 glm_one = first;
    glm::vec3 glm_two = second;

    real result = vec3::distance(one, two);
    real glm_result = glm::distance(glm_one, glm_two);

    assert(glm_result == result);

    std::cout << glm_result << " = ";
    std::cout << result << std::endl;
}

void test_vec3_normalized(const glm::vec3& first) {
    std::cout << "_____ NORMALIZED _____" << std::endl;
    vec3 one(first.x, first.y, first.z);
    glm::vec3 glm_one = first;

    vec3 result = vec3::normalized(one);
    glm::vec3 glm_result = glm::normalize(glm_one);

    assert(glm_result == glm::vec3(result.x(), result.y(), result.z()));

    std::cout << glm_result.x << " " << glm_result.y << " " << glm_result.z << " = ";
    std::cout << result.x() << " " << result.y() << " " << result.z() << std::endl;
}

void test_vec3_normalize(const glm::vec3& first) {
    std::cout << "_____ NORMALIZE _____" << std::endl;
    vec3 one(first.x, first.y, first.z);
    glm::vec3 glm_one = first;

    one.normalize();
    glm::vec3 glm_result = glm::normalize(glm_one);

    assert(glm_result == glm::vec3(one.x(), one.y(), one.z()));

    std::cout << glm_result.x << " " << glm_result.y << " " << glm_result.z << " = ";
    std::cout << one.x() << " " << one.y() << " " << one.z() << std::endl;
}

void test_vec3_angle(const glm::vec3& first, const glm::vec3& second) {
    std::cout << "_____ ANGLE (RADIANS) _____" << std::endl;
    vec3 one(first.x, first.y, first.z);
    vec3 two(second.x, second.y, second.z);

    glm::vec3 glm_one = first;
    glm::vec3 glm_two = second;

    real result = one.angle(one, two);
    real glm_result = glm::angle(glm::normalize(glm_one), glm::normalize(glm_two));

    real value = glm_result - result;
    if (value < 0) {
        value = -1 * value;
    }
    // lazy compare
    assert(value < 0.000001);

    std::cout << glm_result << " = ";
    std::cout << result << std::endl;
}

void test_vec3_angle_in_degrees(const glm::vec3& first, const glm::vec3& second) {
    std::cout << "_____ ANGLE IN DEGREES _____" << std::endl;
    vec3 one(first.x, first.y, first.z);
    vec3 two(second.x, second.y, second.z);

    glm::vec3 glm_one = first;
    glm::vec3 glm_two = second;

    real result = one.angle_in_degrees(one, two);
    real glm_result = glm::angle(glm::normalize(glm_one), glm::normalize(glm_two)) * 57.29577951f;

    real value = glm_result - result;
    if (value < 0) {
        value = -1 * value;
    }
    // lazy compare
    assert(value < 0.0001);

    //assert(glm_result == result);

    std::cout << glm_result << " = ";
    std::cout << result << std::endl;
}

void test_vec3_project(const glm::vec3& first, const glm::vec3& second) {
    std::cout << "_____ PROJECT _____" << std::endl;
    vec3 one(first.x, first.y, first.z);
    vec3 two(second.x, second.y, second.z);

    glm::vec3 glm_one = first;
    glm::vec3 glm_two = second;

    vec3 result = vec3::project(one, two);
    glm::vec3 glm_result = glm::proj(glm_one, glm_two);

    real res_glm = vec3(glm_result.x, glm_result.y, glm_result.z).len_square();
    real res = result.len_square();
    real value = res - res_glm;
    if (value < 0) {
        value = value * -1;
    }
    assert(value < 0.01);

    std::cout << glm_result.x << " " << glm_result.y << " " << glm_result.z << " = ";
    std::cout << result.x() << " " << result.y() << " " << result.z() << std::endl;
}

void test_vec3_reflect(const glm::vec3& first, const glm::vec3& second) {
    std::cout << "_____ REFLECT _____" << std::endl;
    vec3 one(first.x, first.y, first.z);
    vec3 two(second.x, second.y, second.z);


    glm::vec3 glm_one = first;
    glm::vec3 glm_two = second;

    //two.normalize();
    vec3 result = vec3::reflect(one, two);
    glm_two = glm::normalize(glm_two);
    glm::vec3 glm_result = glm::reflect(glm_one, glm_two);

    real res_glm = vec3(glm_result.x, glm_result.y, glm_result.z).len_square();
    real res = result.len_square();
    real value = res - res_glm;
    if (value < 0) {
        value = value * -1;
    }
    assert(value < 0.1);

    std::cout << glm_result.x << " " << glm_result.y << " " << glm_result.z << " = ";
    std::cout << result.x() << " " << result.y() << " " << result.z() << std::endl;
}

void test_vec3_cross(const glm::vec3& first, const glm::vec3& second) {
    std::cout << "_____ CROSS PRODUCT _____" << std::endl;
    vec3 one(first.x, first.y, first.z);
    vec3 two(second.x, second.y, second.z);

    glm::vec3 glm_one = first;
    glm::vec3 glm_two = second;

    vec3 result = vec3::cross(one, two);
    glm::vec3 glm_result = glm::cross(glm_one, glm_two);

    assert(glm_result == glm::vec3(result.x(), result.y(), result.z()));
    
    std::cout << glm_result.x << " " << glm_result.y << " " << glm_result.z << " = ";
    std::cout << result.x() << " " << result.y() << " " << result.z() << std::endl;
}

void test_vec3_lerp(const glm::vec3& first, const glm::vec3& second) {
    std::cout << "_____ LERP _____" << std::endl;
    vec3 one(first.x, first.y, first.z);
    vec3 two(second.x, second.y, second.z);

    glm::vec3 glm_one = first;
    glm::vec3 glm_two = second;

    vec3 result = vec3::lerp(one, two, 0.5f);

    std::cout << result.x() << " " << result.y() << " " << result.z() << std::endl;
}

void test_vec3_slerp(const glm::vec3& first, const glm::vec3& second) {
    std::cout << "_____ SLERP _____" << std::endl;
    vec3 one(first.x, first.y, first.z);
    vec3 two(second.x, second.y, second.z);

    glm::vec3 glm_one = first;
    glm::vec3 glm_two = second;

    vec3 result = vec3::slerp(one, two, 0.5);
    glm::vec3 glm_result = glm::slerp(glm::normalize(glm_one), glm::normalize(glm_two), 0.5f);

    real res_glm = vec3(glm_result.x, glm_result.y, glm_result.z).len_square();
    real res = result.len_square();
    real value = res - res_glm;
    if (value < 0) {
        value = value * -1;
    }
    assert(value < 0.000001);

    std::cout << glm_result.x << " " << glm_result.y << " " << glm_result.z << " = ";
    std::cout << result.x() << " " << result.y() << " " << result.z() << std::endl;
}

// ------------------------------------------------------------------------------------

void test_vec2_minus(const glm::vec2& first, const glm::vec2& second) {
    std::cout << "_____ MINUS _____" << std::endl;
    vec2 one(first.x, first.y);
    vec2 two(second.x, second.y);

    glm::vec2 glm_one = first;
    glm::vec2 glm_two = second;

    vec2 result = one - two;
    glm::vec2 glm_result = glm_one - glm_two;

    assert(glm_result == glm::vec2(result.x(), result.y()));

    std::cout << glm_result.x << " " << glm_result.y << " " << " = ";
    std::cout << result.x() << " " << result.y() << " " << std::endl;
}

void test_vec2_plus(const glm::vec2& first, const glm::vec2& second) {
    std::cout << "_____ PLUS _____" << std::endl;
    vec2 one(first.x, first.y);
    vec2 two(second.x, second.y);

    glm::vec2 glm_one = first;
    glm::vec2 glm_two = second;

    vec2 result = one + two;
    glm::vec2 glm_result = glm_one + glm_two;

    assert(glm_result == glm::vec2(result.x(), result.y()));

    std::cout << glm_result.x << " " << glm_result.y << " " << " = ";
    std::cout << result.x() << " " << result.y() << " " << std::endl;
}

void test_vec2_multiply(const glm::vec2& first, const glm::vec2& second) {
    std::cout << "_____ MULTIPLY _____" << std::endl;
    vec2 one(first.x, first.y);
    vec2 two(second.x, second.y);

    glm::vec2 glm_one = first;
    glm::vec2 glm_two = second;

    vec2 result = one * two;
    glm::vec2 glm_result = glm_one * glm_two;

    assert(glm_result == glm::vec2(result.x(), result.y()));

    std::cout << glm_result.x << " " << glm_result.y << " " << " = ";
    std::cout << result.x() << " " << result.y() << " " << std::endl;
}

void test_vec2_dot(const glm::vec2& first, const glm::vec2& second) {
    std::cout << "_____ DOT PRODUCT _____" << std::endl;
    vec2 one(first.x, first.y);
    vec2 two(second.x, second.y);

    glm::vec2 glm_one = first;
    glm::vec2 glm_two = second;

    real result = vec2::dot(one, two);
    real glm_result = glm::dot(glm_one, glm_two);

    assert(glm_result == result);

    std::cout << glm_result << " = ";
    std::cout << result << std::endl;
}

int main(int argc, char** argv) {
    test_vec3_class();

    test_vec2_minus(glm::vec2(3, 4), glm::vec2(5, 6));
    test_vec2_minus(glm::vec2(45, 23), glm::vec2(34, 34));
    test_vec2_minus(glm::vec2(333, 45), glm::vec2(543, 6345));
    std::cout << std::endl;

    test_vec2_plus(glm::vec2(3, 4), glm::vec2(5, 6));
    test_vec2_plus(glm::vec2(45, 23), glm::vec2(34, 34));
    test_vec2_plus(glm::vec2(333, 45), glm::vec2(543, 6345));
    std::cout << std::endl;

    test_vec2_multiply(glm::vec2(3, 4), glm::vec2(5, 6));
    test_vec2_multiply(glm::vec2(45, 23), glm::vec2(34, 34));
    test_vec2_multiply(glm::vec2(333, 45), glm::vec2(543, 6345));
    std::cout << std::endl;

    test_vec2_dot(glm::vec2(3, 4), glm::vec2(5, 6));
    test_vec2_dot(glm::vec2(45, 23), glm::vec2(34, 34));
    test_vec2_dot(glm::vec2(333, 45), glm::vec2(543, 6345));
    std::cout << std::endl;

    //renderer* r = new renderer();
    //r->render();
    //r->destroy();
    //delete r;
}

void test_vec3_class() {
    test_vec3_minus(glm::vec3(3, 4, 5), glm::vec3(5, 6, 7));
    test_vec3_minus(glm::vec3(45, 23, 246), glm::vec3(34, 34, 53));
    test_vec3_minus(glm::vec3(333, 45, 534), glm::vec3(543, 6345, 347));
    std::cout << std::endl;

    test_vec3_plus(glm::vec3(3, 4, 5), glm::vec3(5, 6, 7));
    test_vec3_plus(glm::vec3(45, 23, 246), glm::vec3(34, 34, 53));
    test_vec3_plus(glm::vec3(333, 45, 534), glm::vec3(543, 6345, 347));
    std::cout << std::endl;

    test_vec3_multiply(glm::vec3(3, 4, 5), glm::vec3(5, 6, 7));
    test_vec3_multiply(glm::vec3(45, 23, 246), glm::vec3(34, 34, 53));
    test_vec3_multiply(glm::vec3(333, 45, 534), glm::vec3(543, 6345, 347));
    std::cout << std::endl;

    test_vec3_dot(glm::vec3(3, 4, 5), glm::vec3(5, 6, 7));
    test_vec3_dot(glm::vec3(45, 23, 246), glm::vec3(34, 34, 53));
    test_vec3_dot(glm::vec3(333, 45, 534), glm::vec3(543, 6345, 347));
    std::cout << std::endl;

    test_vec3_distance(glm::vec3(3, 4, 5), glm::vec3(5, 6, 7));
    test_vec3_distance(glm::vec3(45, 23, 246), glm::vec3(34, 34, 53));
    test_vec3_distance(glm::vec3(333, 45, 534), glm::vec3(543, 6345, 347));
    std::cout << std::endl;

    test_vec3_normalized(glm::vec3(3, 4, 5));
    test_vec3_normalized(glm::vec3(45, 23, 246));
    test_vec3_normalized(glm::vec3(333, 45, 534));
    std::cout << std::endl;

    test_vec3_normalize(glm::vec3(3, 4, 5));
    test_vec3_normalize(glm::vec3(45, 23, 246));
    test_vec3_normalize(glm::vec3(333, 45, 534));
    std::cout << std::endl;

    test_vec3_angle(glm::vec3(3, 4, 5), glm::vec3(5, 6, 7));
    test_vec3_angle(glm::vec3(45, 23, 246), glm::vec3(34, 34, 53));
    test_vec3_angle(glm::vec3(333, 45, 534), glm::vec3(543, 6345, 347));
    std::cout << std::endl;

    test_vec3_angle_in_degrees(glm::vec3(3, 4, 5), glm::vec3(5, 6, 7));
    test_vec3_angle_in_degrees(glm::vec3(45, 23, 246), glm::vec3(34, 34, 53));
    test_vec3_angle_in_degrees(glm::vec3(333, 45, 534), glm::vec3(543, 6345, 347));
    std::cout << std::endl;

    test_vec3_project(glm::vec3(3, 4, 5), glm::vec3(5, 6, 7));
    test_vec3_project(glm::vec3(45, 23, 246), glm::vec3(34, 34, 53));
    test_vec3_project(glm::vec3(333, 45, 534), glm::vec3(543, 6345, 347));
    std::cout << std::endl;

    test_vec3_reflect(glm::vec3(3, 4, 5), glm::vec3(5, 6, 7));
    test_vec3_reflect(glm::vec3(45, 23, 246), glm::vec3(34, 34, 53));
    test_vec3_reflect(glm::vec3(333, 45, 534), glm::vec3(543, 6345, 347));
    std::cout << std::endl;

    test_vec3_cross(glm::vec3(3, 4, 5), glm::vec3(5, 6, 7));
    test_vec3_cross(glm::vec3(45, 23, 246), glm::vec3(34, 34, 53));
    test_vec3_cross(glm::vec3(333, 45, 534), glm::vec3(543, 6345, 347));
    std::cout << std::endl;

    test_vec3_lerp(glm::vec3(3, 4, 5), glm::vec3(5, 6, 7));
    test_vec3_lerp(glm::vec3(45, 23, 246), glm::vec3(34, 34, 53));
    test_vec3_lerp(glm::vec3(333, 45, 534), glm::vec3(543, 6345, 347));
    std::cout << std::endl;

    test_vec3_slerp(glm::vec3(3, 4, 5), glm::vec3(5, 6, 7));
    test_vec3_slerp(glm::vec3(45, 23, 246), glm::vec3(34, 34, 53));
    test_vec3_slerp(glm::vec3(333, 45, 534), glm::vec3(543, 6345, 347));
    std::cout << std::endl;
}