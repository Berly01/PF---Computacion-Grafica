#include "Renderer.hpp"

int main() {
    
    Renderer renderer;

    if (!renderer.initialize_only_model(640, 480, "models/cargador-f.obj")) {
        return -1;
    }

    renderer.set_model_scale(0.005f);
    renderer.render_only_model();
    renderer.cleanup();

    return 0;
}

