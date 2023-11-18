#include "application.hpp"

#include "renderer/renderer.hpp"

void application::init() {
    renderer* r = new renderer();
    r->render();
    r->destroy();
    delete r;
}