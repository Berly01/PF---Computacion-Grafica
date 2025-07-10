#include "Renderer.hpp"

int main() {
    
    Renderer renderer;

    if (!renderer.initialize(640, 480, "models/chalan.obj")) {
        return -1;
    }

    renderer.render();
    renderer.cleanup();

    return 0;
}

