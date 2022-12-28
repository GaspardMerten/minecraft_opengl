#include<iostream>

//include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Don't forget the glm header
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include <chrono>
#include <thread>

#include "utils/shader/load/load_shader.h"
#include "utils/shader/shader/Shader.h"
#include "objects/game_object/GameObject.h"
#include "utils/shader/shader/Light.h"
#include "objects/cubemap/Cubemap.h"
#include "objects/camera/Camera.h"
#include "controls/player_controls/PlayerControls.h"
#include "utils/world/generate_world.h"
#include "texture/manager/TextureManager.h"

#define SHADER_PATH "shaders/"

const int width = 500;
const int height = 500;


GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader);


Shader loadShader(const std::string &vertexPath, const std::string &fragmentPath);

#ifndef NDEBUG

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char *message,
                            const void *userParam) {
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            std::cout << "Source: API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            std::cout << "Source: Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            std::cout << "Source: Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            std::cout << "Source: Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            std::cout << "Source: Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            std::cout << "Source: Other";
            break;
    }
    std::cout << std::endl;

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            std::cout << "Type: Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cout << "Type: Deprecated Behaviour";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cout << "Type: Undefined Behaviour";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cout << "Type: Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cout << "Type: Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::cout << "Type: Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            std::cout << "Type: Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            std::cout << "Type: Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cout << "Type: Other";
            break;
    }
    std::cout << std::endl;

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "Severity: high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "Severity: medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "Severity: low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            std::cout << "Severity: notification";
            break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

#endif


int main(int argc, char *argv[]) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialise GLFW \n");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifndef NDEBUG
    //create a debug context to help with Debugging
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

    //Create the window
    GLFWwindow *window = glfwCreateWindow(width, height, "Gaspard is cool", nullptr, nullptr);

    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window\n");
    }

    glfwMakeContextCurrent(window);

    //load openGL function
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

#ifndef NDEBUG
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
#endif


    const glm::vec3 light_pos = glm::vec3(0.5, 0.5, -0.7);


    double prev = 0;
    int deltaFrame = 0;

    TextureManager::linkTexture(TextureType::DIRT,  "resources/textures/dirt.jpg");
    TextureManager::linkTexture(TextureType::PLAYER, "resources/textures/negy.jpg");

    Shader cubemapShader = loadShader("cubemap_vert.glsl", "cubemap_frag.glsl");
    cubemapShader.use();
    auto *cubemap = new Cubemap("resources/objects/cube.obj", cubemapShader);
    cubemap->makeObject();

    Shader shader = loadShader("vertex.glsl", "fragment.glsl");
    shader.use();

    auto *player = new GameObject("resources/objects/stevy.obj", shader);
    player->makeObject();
    player->setTextureID(TextureManager::getTextureID(TextureType::PLAYER));

    Camera camera = Camera(player->transform);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 perspective = camera.getProjectionMatrix();

    player->transform.position = glm::vec3(10, 1, 10);
    // Adding texture to cube

    auto *cube = new GameObject("resources/objects/cube.obj", shader);
    cube->makeObject();
    cube->setTextureID(TextureManager::getTextureID(TextureType::DIRT));

    glfwSwapInterval(1);

    // previous mouse positionx
    double prevX = 0;
    double prevY = 0;

    glfwGetCursorPos(window, &prevX, &prevY);

    Light light(
            shader,
            glm::vec3(34, 20, 66),
            glm::vec3(0.0, 0.0, 0.0),
            0.9,
            0.8,
            0.5,
            32.0,
            0.14,
            0.01,
            1.0
    );
    light.init();

    World world = generateFlatWorld(100, 100, 1);
    world.instantiateObjects(shader, "resources/objects/cube.obj");

    PlayerControls controls = PlayerControls(player->transform, camera, world);
    //2. Use the shader Class to send the uniform
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(1.0, 1.0, 2.0));
    model = glm::scale(model, glm::vec3(1.2, 1.2, 1.2));
    light.use(camera, model);

    while (!glfwWindowShouldClose(window)) {
        controls.processControls(window);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        view = camera.getViewMatrix();
        glfwPollEvents();
        double now = glfwGetTime();
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        cubemap->draw(camera);


        shader.use();
        shader.setMatrix4("V", view);
        shader.setMatrix4("P", perspective);

        cube->draw();
        world.draw();
        player->draw();


        // get current mouse position
        double xpos, ypos;

        glfwGetCursorPos(window, &xpos, &ypos);


        // compute new orientation
        prevX = xpos;
        prevY = ypos;

        glfwSwapBuffers(window);

    }

    //clean up ressource
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

Shader loadShader(const std::string &vertexPath, const std::string &fragmentPath) {
    // join folder path with shader file name (platform independent)
    std::string vertexShaderPath = SHADER_PATH + vertexPath;
    std::string fragmentShaderPath = SHADER_PATH + fragmentPath;

    return {vertexShaderPath, fragmentShaderPath};
}


GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint programID = glCreateProgram();

    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    GLchar infoLog[1024];
    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR:  " << infoLog << std::endl;
    }
    return programID;
}

