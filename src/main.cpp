#include<iostream>

//include glad before GLFW to avoid header conflict or define "#define GLFW_INCLUDE_NONE"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Don't forget the glm header
#include <glm/glm.hpp>
#include "stb_image.h"

#include <thread>

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
#include "cubemap/CubeMap.h"

#define SHADER_PATH "shaders/"

const int INITIAL_WINDOW_WIDTH = 500;
const int INITIAL_WINDOW_HEIGHT = 500;


Shader loadShader(const std::string &vertexPath, const std::string &fragmentPath, bool withTexture = true,
                  bool withLight = true);

void setupMeshs();

void setupTextures();

CubeMap *loadCubeMap();

void setupShadowShader(int &shadowTextureWidth, int &shadowTextureHeight, GLuint &m_ShadowMapDepthStencilTextureId,
                       GLuint &m_ShadowMapFBOId);

void renderShadowMap(Minecraft *minecraft, Shader &shadowShader, int shadowTextureWidth, int shadowTextureHeight,
                     GLuint m_ShadowMapFBOId, const glm::mat4 &lightV, const glm::mat4 &lightP);

void renderMinecraft(GLFWwindow *window, Minecraft *minecraft, Shader &shader, const CubeMap *cubeMap,
                     GLuint m_ShadowMapDepthStencilTextureId, const glm::mat4 &lightSpaceMatrix, int &width,
                     int &height);

void renderSkyBox(const Minecraft *minecraft, Shader &cubeMapShader, const CubeMap *cubeMap);

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
        default:
            std::cout << "Unknown error";
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
        default:
            std::cout << "Unknown error";
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
        default:
            std::cout << "Unknown severity";
            break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

#endif


int main() {
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
    GLFWwindow *window = glfwCreateWindow(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, "Gaspard is cool", nullptr,
                                          nullptr);

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


    setupMeshs();
    setupTextures();

    auto *minecraft = new Minecraft(200, 200, 1, 100, 5, glm::vec3(15, 1, 15), window);


    Shader shadowShader = loadShader("shadow.vert.glsl", "shadow.frag.glsl", false, false);
    Shader shader = loadShader("vertex.glsl", "fragment.glsl");
    Shader cubeMapShader = loadShader("cubemap.vert.glsl", "cubemap.frag.glsl");

    minecraft->linkShader(shader);
    minecraft->linkShader(shadowShader);

    CubeMap *cubeMap = loadCubeMap();
    // load cubemap texture (the shader is not used here, but for similarity with the other objects)
    cubeMap->makeObject(shader);

    // enable vsync
    glfwSwapInterval(1);

    /**
     * OPENGL STATIC CONFIGURATION (all settings that are not going to change during the program)
     */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_EQUAL);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    /**
     * Shadow part
     */
    int shadowTextureWidth;
    int shadowTextureHeight;
    GLuint m_ShadowMapDepthStencilTextureId;
    GLuint shadowMapFboId;

    setupShadowShader(shadowTextureWidth, shadowTextureHeight, m_ShadowMapDepthStencilTextureId, shadowMapFboId);

    // Needed since we don't touch the color buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    const glm::mat4 &lightV = minecraft->light->getSpaceMatrix();
    const glm::mat4 &lightP = minecraft->light->getOrthoProjectionMatrix();
    glm::mat4 lightSpaceMatrix = lightP * lightV;

    int width, height;


    while (!glfwWindowShouldClose(window)) {
        // process opengl events
        glfwPollEvents();

        // process input
        minecraft->processEvents(window);

        // update physics, pnjs, ...
        minecraft->updateManagers();

        renderShadowMap(
                minecraft,
                shadowShader,
                shadowTextureWidth,
                shadowTextureHeight,
                shadowMapFboId,
                lightV,
                lightP
        );

        renderMinecraft(
                window,
                minecraft,
                shader,
                cubeMap,
                m_ShadowMapDepthStencilTextureId,
                lightSpaceMatrix,
                width,
                height
        );

        renderSkyBox(minecraft, cubeMapShader, cubeMap);

        glfwSwapBuffers(window);
    }

    //clean up ressource
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void renderSkyBox(const Minecraft *minecraft, Shader &cubeMapShader, const CubeMap *cubeMap) {
    cubeMapShader.use();
    minecraft->configureMatrices(cubeMapShader);
    cubeMap->draw(cubeMapShader);
}

void renderMinecraft(GLFWwindow *window, Minecraft *minecraft, Shader &shader, const CubeMap *cubeMap,
                     GLuint m_ShadowMapDepthStencilTextureId, const glm::mat4 &lightSpaceMatrix, int &width,
                     int &height) {
    glfwGetFramebufferSize(window, &width, &height);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    shader.use();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, width, height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, m_ShadowMapDepthStencilTextureId);
    shader.setMatrix4("lightSpaceMatrix", lightSpaceMatrix);
    minecraft->configureMatrices(shader);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap->textureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    shader.setVector3f("u_view_pos", minecraft->camera->transform.position);
    minecraft->render(shader);
}

void renderShadowMap(Minecraft *minecraft, Shader &shadowShader, int shadowTextureWidth, int shadowTextureHeight,
                     GLuint m_ShadowMapFBOId, const glm::mat4 &lightV, const glm::mat4 &lightP) {
    glActiveTexture(GL_TEXTURE0);
    glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowMapFBOId);
    glViewport(0, 0, shadowTextureWidth, shadowTextureHeight);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glClear(GL_DEPTH_BUFFER_BIT);
    shadowShader.use();
    shadowShader.setMatrix4("P", lightP);
    shadowShader.setMatrix4("V", lightV);
    minecraft->render(shadowShader);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void setupShadowShader(int &shadowTextureWidth, int &shadowTextureHeight, GLuint &m_ShadowMapDepthStencilTextureId,
                       GLuint &m_ShadowMapFBOId) {
    shadowTextureWidth = 4096;
    shadowTextureHeight = 4096;
    glGenFramebuffers(1, &m_ShadowMapFBOId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowMapFBOId);

    glGenTextures(1, &m_ShadowMapDepthStencilTextureId);
    glBindTexture(GL_TEXTURE_2D, m_ShadowMapDepthStencilTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 shadowTextureWidth, shadowTextureHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowMapDepthStencilTextureId, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
}

CubeMap *loadCubeMap() {
    std::map<std::string, GLenum> facesToLoad = {
            {"posx.jpg", GL_TEXTURE_CUBE_MAP_POSITIVE_X},
            {"posy.jpg", GL_TEXTURE_CUBE_MAP_POSITIVE_Y},
            {"posz.jpg", GL_TEXTURE_CUBE_MAP_POSITIVE_Z},
            {"negx.jpg", GL_TEXTURE_CUBE_MAP_NEGATIVE_X},
            {"negy.jpg", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y},
            {"negz.jpg", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z},
    };

    auto *dayCubeMap = new CubeMap("resources/skybox/day/skybox_", facesToLoad);
    return dayCubeMap;
}

void setupTextures() {
    TextureManager::linkTexture(DIRT, "resources/textures/dirt.jpg");
    TextureManager::linkTexture(WOOD, "resources/textures/wood.jpg");
    TextureManager::linkTexture(LEAF, "resources/textures/leaves.jpg");
    TextureManager::linkTexture(PLAYER, "resources/textures/steve.jpg");
    TextureManager::linkTexture(GLOW_STONE, "resources/textures/glowstone.jpg");
    TextureManager::linkTexture(WHITE_SHEEP, "resources/textures/sheep.jpg");
    TextureManager::linkTexture(BROWN_VILLAGER, "resources/textures/villager.jpg");
    TextureManager::linkTexture(GRASS, "resources/textures/grass.jpg");
    TextureManager::linkTexture(WATER, "resources/textures/water.jpg");
}

void setupMeshs() {
    MeshManager::linkMesh(BLOCK, "resources/objects/cube.obj");
    MeshManager::linkMesh(HUMAN, "resources/objects/stevy.obj");
    MeshManager::linkMesh(SHEEP, "resources/objects/sheep/sheep.obj");
    MeshManager::linkMesh(VILLAGER, "resources/objects/villager.obj");
    MeshManager::linkMesh(CUBEMAP, "resources/objects/grass.obj");
    MeshManager::linkMesh(PLANE, "resources/objects/plane.obj");
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
        glGetProgramInfoLog(programID, 1024, nullptr, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR:  " << infoLog << std::endl;
    }
    return programID;
}

