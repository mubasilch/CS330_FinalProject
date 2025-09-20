#include <iostream>         // error handling and output
#include <cstdlib>          // EXIT_FAILURE

#include <GL/glew.h>        // GLEW library
#include "GLFW/glfw3.h"     // GLFW library

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SceneManager.h"
#include "ViewManager.h"
#include "ShapeMeshes.h"
#include "ShaderManager.h"

// -------------------- Globals --------------------
namespace
{
    // Window title
    const char* const WINDOW_TITLE = "7-1 FinalProject and Milestones";

    // Main GLFW window
    GLFWwindow* g_Window = nullptr;

    // Managers
    SceneManager* g_SceneManager = nullptr;
    ShaderManager* g_ShaderManager = nullptr;
    ViewManager* g_ViewManager = nullptr;
}

// -------------------- Prototypes --------------------
bool InitializeGLFW();
bool InitializeGLEW();

// Input callbacks (registered with GLFW)
void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);
void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

// ====================================================
// main
// ====================================================
int main(int /*argc*/, char* /*argv*/[])
{
    // 1) Init GLFW
    if (!InitializeGLFW())
        return EXIT_FAILURE;

    // 2) Create managers
    g_ShaderManager = new ShaderManager();
    g_ViewManager = new ViewManager(g_ShaderManager);

    // 3) Create window via ViewManager
    g_Window = g_ViewManager->CreateDisplayWindow(WINDOW_TITLE);
    if (!g_Window)
        return EXIT_FAILURE;

    // 4) Capture mouse and hook callbacks for camera look/zoom
    glfwSetInputMode(g_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(g_Window, MousePositionCallback);
    glfwSetScrollCallback(g_Window, MouseScrollCallback);

    // 5) Init GLEW (must be after context creation)
    if (!InitializeGLEW())
        return EXIT_FAILURE;

    // 6) Load shaders
    g_ShaderManager->LoadShaders(
        "Utilities/shaders/vertexShader.glsl",
        "Utilities/shaders/fragmentShader.glsl");
    g_ShaderManager->use();

    // 7) Build scene
    g_SceneManager = new SceneManager(g_ShaderManager);
    g_SceneManager->PrepareScene();

    // 8) Main loop
    while (!glfwWindowShouldClose(g_Window))
    {
        // Enable depth
        glEnable(GL_DEPTH_TEST);

        // Clear frame + depth buffers
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Let ViewManager process keyboard input (WASD, etc.)
        if (g_ViewManager)
        {
            // If your ViewManager uses a different name, change this call:
            // e.g., HandleInput / Update / ProcessKeyboard
            g_ViewManager->ProcessInput(g_Window); // <-- rename if needed
        }

        // Set view/projection and render scene
        g_ViewManager->PrepareSceneView();

        // NEW: tell shader the camera position for specular highlights
        if (g_ShaderManager)
            g_ShaderManager->setVec3Value("viewPos", g_ViewManager->GetCameraPosition());

        g_SceneManager->RenderScene();


        // Present and poll
        glfwSwapBuffers(g_Window);
        glfwPollEvents();
    }

    // Cleanup
    delete g_SceneManager;  g_SceneManager = nullptr;
    delete g_ViewManager;   g_ViewManager = nullptr;
    delete g_ShaderManager; g_ShaderManager = nullptr;

    exit(EXIT_SUCCESS);
}

// ====================================================
// InitializeGLFW
// ====================================================
bool InitializeGLFW()
{
    glfwInit();

#ifdef __APPLE__
    // macOS uses 3.3 core in this template
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    // Windows/Linux: request 4.6 core (works with the SNHU starter)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    return true;
}

// ====================================================
// InitializeGLEW
// ====================================================
bool InitializeGLEW()
{
    GLenum GLEWInitResult = glewInit();
    if (GLEW_OK != GLEWInitResult)
    {
        std::cerr << glewGetErrorString(GLEWInitResult) << std::endl;
        return false;
    }

    std::cout << "INFO: OpenGL Successfully Initialized\n";
    std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << "\n" << std::endl;
    return true;
}

// ====================================================
// Mouse callbacks -> forward to ViewManager
// ====================================================
void MousePositionCallback(GLFWwindow* /*window*/, double xpos, double ypos)
{
    static bool   first = true;
    static double lastX = xpos, lastY = ypos;
    if (first) { lastX = xpos; lastY = ypos; first = false; }

    float dx = static_cast<float>(xpos - lastX);
    float dy = static_cast<float>(lastY - ypos); // invert Y for typical FPS look
    lastX = xpos;
    lastY = ypos;

    if (g_ViewManager)
    {
        // If your ViewManager uses a different name, change this call:
        // e.g., ProcessMouseLook / OnMouseMove
        g_ViewManager->ProcessMouseMovement(dx, dy); // <-- rename if needed
    }
}

void MouseScrollCallback(GLFWwindow* /*window*/, double /*xoffset*/, double yoffset)
{
    if (g_ViewManager)
    {
        // If your ViewManager uses a different name, change this call:
        // e.g., OnScroll / Zoom
        g_ViewManager->ProcessMouseScroll(static_cast<float>(yoffset)); // <-- rename if needed
    }
}
