#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Raymarch.h"
#include "TrackballControls.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

#ifdef ENABLE_GLCHECKERROR
GLenum glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)
#endif // ENABLE_GLCHECKERROR

void processInput(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int argc, char** argv){
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <shader_file>" << std::endl;
        exit(0);
    }

    std::string model_dir = argv[1];

    // glfw: Init
    int openGLMajorVersion = 4;
    int openGLMinorVersion = 3;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openGLMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openGLMinorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);


    // This macro is defined in any Apple Computer
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SNeRG viewer", NULL, NULL);
    if (window == NULL){
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    if (!gladLoadGL(glfwGetProcAddress)){
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam){
            if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
            std::cerr << "GL CALLBACK: " << message << std::endl;
        }, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    Raymarch scene;
    scene.setRootDir(model_dir);
    scene.setSize(SCR_WIDTH, SCR_HEIGHT);
    scene.setFieldOfView(35.0f);
    scene.setNearPlane(0.33f);
    if (!scene.initScene()){
        std::cerr << "Failed to initialize scene" << std::endl;
        exit(-1);
    }

    float cx = 0.0f;
    float cy = 1.0f;
    float cz = -4.0f;
    if ((bool)scene.globalSceneParams["ndc"]){
        cz = -0.25f;
    }

    Camera3D cam3d(glm::vec3(cx, cy, cz));
    cam3d.LookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    TrackballControls* controls = &TrackballControls::GetInstance(&cam3d, glm::vec4((float)SCR_WIDTH, (float)SCR_HEIGHT, 0.0f, 0.0f));
    controls->Init(window);

    while (!glfwWindowShouldClose(window)){
        processInput(window);

        controls->Update();
        scene.setCameraMatrix(cam3d.m_viewMatr);
        scene.updateScene(0.0f);
        scene.renderScene();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    scene.destroyScene();

    glfwTerminate();
    return 0;  
}

