#include <iostream>

#include "renderer/renderer.hpp"

int main(int argc, char** argv) {
    renderer* r = new renderer();
    r->render();
    r->destroy();
    delete r;
}