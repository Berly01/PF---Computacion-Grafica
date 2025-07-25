#include "Renderer.hpp"

//get_corners()
bool Renderer::initialize(const int& width, const int& height, const std::string& filepath) {

    if (!glfwInit()) {
        std::cerr << "Error inicializando GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Renderer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error creando ventana GLFW" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Error inicializando GLEW" << std::endl;
        return false;
    }

    glViewport(0, 0, width, height);

    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, mouse_call_back);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
    shader_model = std::make_unique<Shader>(MODEL_FILE_VERT_PATH, MODEL_FILE_FRAG_PATH);
    shader_background = std::make_unique<Shader>(BACKGROUND_FILE_VERT_PATH, BACKGROUND_FILE_FRAG_PATH);
    background = std::make_unique<Background>(width, height, 0);

    if (!load_model(filepath)) return false;
    

    return true;
}

bool Renderer::initialize_only_model(const int& width, const int& height, const std::string& filepath) {
    if (!glfwInit()) {
        std::cerr << "Error inicializando GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Renderer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error creando ventana GLFW" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Error inicializando GLEW" << std::endl;
        return false;
    }

    glViewport(0, 0, width, height);   

    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, mouse_call_back);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
    shader_model = std::make_unique<Shader>(MODEL_FILE_VERT_PATH, MODEL_FILE_FRAG_PATH);

    if (!load_model(filepath)) {
        return false;
    }

    return true;
}

bool Renderer::initialize_only_background(const int& width, const int& height) {

    if (!glfwInit()) {
        std::cerr << "Error inicializando GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Renderer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error creando ventana GLFW" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Error inicializando GLEW" << std::endl;
        return false;
    }

    glViewport(0, 0, width, height);
   
    shader_background = std::make_unique<Shader>(BACKGROUND_FILE_VERT_PATH, BACKGROUND_FILE_FRAG_PATH);
    background = std::make_unique<Background>(width, height, 0);

    return true;
}

bool Renderer::load_model(const std::string& filepath) {
    OBJLoader loader;
    std::vector<ut::Vertex3D> vertices;
    std::vector<unsigned int> indices;
    ut::Material material;

    if (!loader.load_OBJ(filepath, vertices, indices, material)) {
        return false;
    }

    mesh_model = std::make_unique<Mesh>(vertices, indices, material);
    return true;
}

void Renderer::render_only_model() {

    show_camera_control();

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        process_input();
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (mesh_model && shader_model && camera) {
            shader_model->use();

            auto model = glm::mat4(1.0f);
            if (autoRotate) {
                modelRotationY += 0.5f * deltaTime; 
            }
            model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(scale, scale, scale));

            std::cout << modelRotationY * (180 / 3.1415) << "\n";

            glm::mat4 view = camera->get_view_matrix();

            std::cout << camera->get_front().x << " " << camera->get_front().y<< " " << camera->get_front().z << "\n";
            std::cout << camera->get_position().x << " " << camera->get_position().y << " " << camera->get_position().z << "\n";
            std::cout << camera->get_zoom()<< "\n";
            std::cout << camera->get_yaw() << " " << camera->get_pitch() << "\n";

            /* rotationY
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    std::cout << view[i][j] << " ";
                }
                std::cout << "\n"; process
            }*/

            int width{};
            int height{};

            glfwGetFramebufferSize(window, &width, &height);
            float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

            glm::mat4 projection = glm::perspective(glm::radians(camera->get_zoom()),
                aspectRatio, 0.1f, 100.0f);

            shader_model->set_matrix4("model", model);
            shader_model->set_matrix4("view", view);
            shader_model->set_matrix4("projection", projection);

            shader_model->set_vec3("light.position", glm::vec3(2.0f, 2.0f, 2.0f));
            shader_model->set_vec3("light.ambient", glm::vec3(1.5f, 1.5f, 1.5f));
            shader_model->set_vec3("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));

            glm::vec3 lightPos = camera->get_position() + camera->get_front() * 2.0f;
            shader_model->set_vec3("lightPos", lightPos);
            shader_model->set_vec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
            shader_model->set_vec3("objectColor", glm::vec3(0.8f, 0.6f, 0.4f));

            mesh_model->draw(*shader_model);
        }

        glfwSwapBuffers(window);
    }

}

void Renderer::render_only_background() {

    glDisable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        background->draw(*shader_background);

        glfwSwapBuffers(window);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

}

void Renderer::render() {
    show_camera_control();

    PatternDetector patternDetector(7, 7); 

    while (!glfwWindowShouldClose(window)) {
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        process_input();
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (background && shader_background) {
            glDisable(GL_DEPTH_TEST);
            background->update_texture();
            background->draw(*shader_background);
        }

      


        const cv::Mat& frame = background->get_frame();
        cv::Mat flipped;
        cv::flip(frame, flipped, -1);
        
        

        //int detect = hand_detector->detectGesture(flipped);

        // Detectar patrón de ajedrez
        bool found = patternDetector.detect(flipped);
        //std::cout << detect << "\n";



        if (found && mesh_model && shader_model && camera) {
            glEnable(GL_DEPTH_TEST);
            shader_model->use();

            //std::cout << "detectado";

            auto model = glm::mat4(1.0f);

            /*
            if (detect == 2)
                modelRotationY += 0.0f * deltaTime;
            else
                modelRotationY += 0.0f * deltaTime;
            */
            
            model = glm::rotate(model, modelRotationY, glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(scale, scale, scale));

            

            camera->process_from_corners(patternDetector.get_corners());

            std::cout << camera->get_front().x << " " << camera->get_front().y<< " " << camera->get_front().z << " <- orientación de camara\n";
            std::cout << camera->get_position().x << " " << camera->get_position().y << " " << camera->get_position().z << "<- posición de la camara\n";
            //std::cout << camera->get_zoom()<< "\n";
            //std::cout << camera->get_yaw() << " " << camera->get_pitch() << "\n";

            glm::mat4 view = camera->get_view_matrix();

            int width{};
            int height{};
            glfwGetFramebufferSize(window, &width, &height);
            float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

            glm::mat4 projection = glm::perspective(glm::radians(camera->get_zoom()),
                aspectRatio, 0.1f, 100.0f);




            shader_model->set_matrix4("model", model);
            shader_model->set_matrix4("view", view);
            shader_model->set_matrix4("projection", projection);

            shader_model->set_vec3("light.position", glm::vec3(2.0f, 2.0f, 2.0f));
            shader_model->set_vec3("light.ambient", glm::vec3(1.5f, 1.5f, 1.5f));
            shader_model->set_vec3("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
            shader_model->set_vec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

            glm::vec3 lightPos = camera->get_position() + camera->get_front() * 2.0f;
            shader_model->set_vec3("lightPos", lightPos);
            shader_model->set_vec3("lightColor", glm::vec3(1.0f));
            shader_model->set_vec3("objectColor", glm::vec3(0.8f, 0.6f, 0.4f));

            mesh_model->draw(*shader_model);
        }

        glfwSwapBuffers(window);
    }
}

void Renderer::cleanup() {
    mesh_model.reset();
    shader_model.reset();
    shader_background.reset();
    camera.reset();
    background.reset();
    glfwTerminate();
}

void Renderer::set_model_scale(const float& scale) {
    this->scale = scale;
}

void Renderer::show_camera_control() const {

    std::cout << "WASD: Mover camara " << std::endl;
    std::cout << "Espacio: Subir camara" << std::endl;
    std::cout << "Shift: Bajar camara" << std::endl;
    std::cout << "Mouse: Rotar camara" << std::endl;
    std::cout << "Scroll: Zoom" << std::endl;


}
