#include <iostream>

#include "vec3.hpp"

int main(int argc, char** argv) {
	vec3 vec(0.1, 1.0, 4.2);

	std::cout << vec.x();

	vec.dot();
}