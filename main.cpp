#include "Renderer.hpp"

int main() {
    
    Renderer renderer;

    if (!renderer.initialize_only_model(640, 480, "models/chalan.obj")) {
        return -1;
    }

    renderer.render_only_model();
    renderer.cleanup();

    return 0;
}