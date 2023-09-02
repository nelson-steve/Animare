#include <iostream>

#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

int main(int argc, char** argv) {
	vec2 vec(0.1f, 1.0f);

	std::cout << vec.x();
}