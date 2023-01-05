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
#include "objects/camera/Camera.h"
#include "controls/player_controls/PlayerControls.h"
#include "utils/world/generate_world.h"
#include "texture/manager/TextureManager.h"
#include "controls/camera/CameraControls.h"
#include "game/Minecraft.h"
#include "objects/mesh/manager/MeshManager.h"

#define SHADER_PATH "shaders/"

const int width = 500;
const int height = 500;


Shader loadShader(const std::string &vertexPath, const std::string &fragmentPath, bool withTexture = true,
                  bool withLight = true);

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

    MeshManager::linkMesh(MeshType::BLOCK, "resources/objects/cube.obj");
    MeshManager::linkMesh(MeshType::HUMAN, "resources/objects/stevy.obj");
    MeshManager::linkMesh(MeshType::SHEEP, "resources/objects/sheep/sheep.obj");
    MeshManager::linkMesh(MeshType::VILLAGER, "resources/objects/villager.obj");
    MeshManager::linkMesh(MeshType::CUBEMAP, "resources/objects/grass.obj");


    TextureManager::linkTexture(TextureType::DIRT, "resources/textures/dirt.jpg");
    TextureManager::linkTexture(TextureType::WOOD, "resources/textures/wood.jpg");
    TextureManager::linkTexture(TextureType::LEAF, "resources/textures/leaves.jpg");
    TextureManager::linkTexture(TextureType::PLAYER, "resources/textures/steve.jpg");
    TextureManager::linkTexture(TextureType::GLOW_STONE, "resources/textures/glowstone.jpg");
    TextureManager::linkTexture(TextureType::WHITE_SHEEP, "resources/textures/sheep.jpg");
    TextureManager::linkTexture(TextureType::BROWN_VILLAGER, "resources/textures/villager.jpg");
    TextureManager::linkTexture(TextureType::GRASS, "resources/textures/grass.jpg");


    auto *minecraft = new Minecraft(150, 100, 1, 10, glm::vec3(15, 1, 15), window);


    Shader shadowShader = loadShader("shadow.vert.glsl", "shadow.frag.glsl", false, false);
    Shader shader = loadShader("vertex.glsl", "fragment.glsl");
    minecraft->linkShader(shader);
    minecraft->linkShader(shadowShader);


    glfwSwapInterval(1);

    // previous mouse positionx
    double prevX = 0;
    double prevY = 0;

    glfwGetCursorPos(window, &prevX, &prevY);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);


    /**
     * Shadow part
     */

    int shadowTextureWidth = 2048;
    int shadowTextureHeight = 2048;

    GLuint m_ShadowMapDepthStencilTextureId;
    GLuint m_ShadowMapFBOId;

    glGenFramebuffers(1, &m_ShadowMapFBOId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowMapFBOId);

    glGenTextures(1, &m_ShadowMapDepthStencilTextureId);
    glBindTexture(GL_TEXTURE_2D, m_ShadowMapDepthStencilTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 shadowTextureWidth, shadowTextureHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowMapDepthStencilTextureId, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // Needed since we don't touch the color buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);



    const glm::mat4 &lightP = glm::ortho(-120.0f, 120.0f, -120.0f, 120.0f, 0.1f, 100.0f);
    minecraft->light->transform->rotation = glm::vec3(-80, 0, 20);
    minecraft->light->transform->markAsDirtyState();

    const glm::mat4 &lightV = minecraft->light->getSpaceMatrix();
    glm::mat4 lightSpaceMatrix = lightP * lightV;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glm::vec3 blockPos = glm::vec3(70, 0, 70);
    GameObject *block = minecraft->world->getBlockAt(blockPos);
    glm::vec4 frag_coord = block->transform.getModel() * glm::vec4(block->mesh->vertices.at(0).position, 1.0);
    glm::vec4 tmp = lightP*lightV*frag_coord;
    // print w
    std::cout << tmp.w << std::endl;
    std::cout << tmp.x/tmp.w << " " << tmp.y/tmp.w << " " << tmp.z/tmp.w << std::endl;


    while (!glfwWindowShouldClose(window)) {
        minecraft->processEvents(window);
        minecraft->updateManagers();

        int width, height;

        glfwPollEvents();
        // print shadowMapFBO

        shadowShader.use();
        glDisable(GL_MULTISAMPLE);
        glActiveTexture(GL_TEXTURE0);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowMapFBOId);
        glViewport(0, 0, shadowTextureWidth, shadowTextureHeight);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glClearDepth(1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);

        shadowShader.setMatrix4("P", lightP);
        shadowShader.setMatrix4("V",lightV);

        minecraft->render(shadowShader);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        glfwGetFramebufferSize(window, &width, &height);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        shader.use();
        glViewport(0, 0, width, height);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0+1);
        glBindTexture(GL_TEXTURE_2D, m_ShadowMapDepthStencilTextureId);


        shader.setMatrix4("lightSpaceMatrix", lightSpaceMatrix);
        minecraft->configureMatrices(shader);

        minecraft->render(shader);


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

Shader loadShader(const std::string &vertexPath, const std::string &fragmentPath, bool withTexture, bool withLight) {
    // join folder path with shader file name (platform independent)
    std::string vertexShaderPath = SHADER_PATH + vertexPath;
    std::string fragmentShaderPath = SHADER_PATH + fragmentPath;

    return {vertexShaderPath, fragmentShaderPath, withTexture, withLight};
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

