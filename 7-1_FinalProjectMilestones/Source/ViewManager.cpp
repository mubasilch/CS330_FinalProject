// ViewManager.cpp
#include "ViewManager.h"

#include <iostream>
#include <cmath>

#include <GL/glew.h>
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // lookAt, perspective, ortho, radians
#include <glm/gtc/type_ptr.hpp>         // value_ptr

#include "ShaderManager.h"

// ----------------------------------------------------
// ctor / dtor
// ----------------------------------------------------
ViewManager::ViewManager(ShaderManager* pShaderManager)
    : m_pShaderManager(pShaderManager),
    m_pWindow(nullptr),
    m_CamPos(0.0f, 1.5f, 5.0f),
    m_CamFront(0.0f, 0.0f, -1.0f),
    m_CamUp(0.0f, 1.0f, 0.0f),
    m_Yaw(-90.0f),
    m_Pitch(0.0f),
    m_Fov(45.0f),
    m_MoveSpeed(3.0f),
    m_MouseSense(0.1f),
    m_LastFrame(0.0f),
    m_Mode(ProjectionMode::Perspective),
    m_OrthoWidth(6.0f),
    m_OrthoHeight(4.0f)
{
}

ViewManager::~ViewManager() = default;

// ----------------------------------------------------
// CreateDisplayWindow
// ----------------------------------------------------
GLFWwindow* ViewManager::CreateDisplayWindow(const char* windowTitle)
{
    const int WIDTH = 1280;
    const int HEIGHT = 720;

    m_pWindow = glfwCreateWindow(WIDTH, HEIGHT, windowTitle, nullptr, nullptr);
    if (!m_pWindow)
    {
        std::cerr << "ERROR: Failed to create GLFW window.\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(m_pWindow);

    // Initial viewport and keep it in sync on resize
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(m_pWindow, [](GLFWwindow*, int w, int h) {
        glViewport(0, 0, w, h);
        });

    // Optional: VSync
    glfwSwapInterval(1);

    return m_pWindow;
}

// ----------------------------------------------------
// PrepareSceneView: build view/projection and upload
// ----------------------------------------------------
void ViewManager::PrepareSceneView()
{
    // View from camera state
    glm::mat4 view = glm::lookAt(m_CamPos, m_CamPos + m_CamFront, m_CamUp);

    // Aspect from framebuffer size
    int w = 1, h = 1;
    if (m_pWindow) glfwGetFramebufferSize(m_pWindow, &w, &h);
    float aspect = (h > 0) ? (static_cast<float>(w) / static_cast<float>(h)) : 1.0f;

    glm::mat4 projection(1.0f);

    if (m_Mode == ProjectionMode::Perspective)
    {
        // Note: m_Fov is kept reasonable in scroll handler
        projection = glm::perspective(glm::radians(m_Fov), aspect, 0.1f, 100.0f);
    }
    else
    {
        // Orthographic: keep proportions vs aspect to avoid stretching
        float halfW = m_OrthoWidth * 0.5f;
        float halfH = m_OrthoHeight * 0.5f;

        if (aspect >= 1.0f)
            halfW = halfH * aspect;
        else
            halfH = halfW / aspect;

        projection = glm::ortho(-halfW, halfW, -halfH, halfH, 0.1f, 100.0f);
    }

    // Get currently bound program (ShaderManager->use() should have been called)
    GLint program = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);
    if (program == 0) return;

    auto setMat4IfPresent = [program](const char* name, const glm::mat4& m)
        {
            GLint loc = glGetUniformLocation(program, name);
            if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
            return loc != -1;
        };

    auto setVec3IfPresent = [program](const char* name, const glm::vec3& v)
        {
            GLint loc = glGetUniformLocation(program, name);
            if (loc != -1) glUniform3fv(loc, 1, glm::value_ptr(v));
            return loc != -1;
        };

    // Common uniform names used in the course shaders
    setMat4IfPresent("view", view) || setMat4IfPresent("uView", view);
    setMat4IfPresent("projection", projection) || setMat4IfPresent("uProjection", projection);

    // For Phong specular (many fragment shaders expect this)
    setVec3IfPresent("viewPos", m_CamPos);
}

// ----------------------------------------------------
// ProcessInput (keyboard each frame)
// ----------------------------------------------------
void ViewManager::ProcessInput(GLFWwindow* window)
{
    // Delta time
    float current = static_cast<float>(glfwGetTime());
    float delta = current - m_LastFrame;
    m_LastFrame = current;

    if (!window) return;

    // Close on ESC
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    // Toggle projection
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) m_Mode = ProjectionMode::Perspective;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) m_Mode = ProjectionMode::Orthographic;

    float velocity = m_MoveSpeed * delta;

    // Right vector from current front & up
    glm::vec3 right = glm::normalize(glm::cross(m_CamFront, m_CamUp));

    // WASD move
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) m_CamPos += m_CamFront * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) m_CamPos -= m_CamFront * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) m_CamPos -= right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) m_CamPos += right * velocity;

    // Q/E vertical
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) m_CamPos -= m_CamUp * velocity;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) m_CamPos += m_CamUp * velocity;
}

// ----------------------------------------------------
// Mouse look (dx, dy in pixels)
// ----------------------------------------------------
void ViewManager::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= m_MouseSense;
    yoffset *= m_MouseSense;

    m_Yaw += xoffset;
    m_Pitch += yoffset;

    // Clamp pitch manually (no std::clamp)
    if (m_Pitch > 89.0f)  m_Pitch = 89.0f;
    if (m_Pitch < -89.0f) m_Pitch = -89.0f;

    // Rebuild front vector
    const float cy = std::cos(glm::radians(m_Yaw));
    const float sy = std::sin(glm::radians(m_Yaw));
    const float cp = std::cos(glm::radians(m_Pitch));
    const float sp = std::sin(glm::radians(m_Pitch));

    glm::vec3 front;
    front.x = cy * cp;
    front.y = sp;
    front.z = -sy * cp; // right-handed: -Z forward
    m_CamFront = glm::normalize(front);
}

// ----------------------------------------------------
// Mouse scroll: adjust movement speed (per rubric)
// ----------------------------------------------------
void ViewManager::ProcessMouseScroll(float yoffset)
{
    // Scale speed gently; avoid std::clamp by bounding manually
    m_MoveSpeed *= (1.0f + 0.1f * static_cast<float>(yoffset));
    if (m_MoveSpeed < 0.5f)  m_MoveSpeed = 0.5f;
    if (m_MoveSpeed > 20.0f) m_MoveSpeed = 20.0f;

    // (Optional) If you also want to zoom FOV, uncomment:
    // m_Fov -= yoffset;
    // if (m_Fov < 20.0f) m_Fov = 20.0f;
    // if (m_Fov > 75.0f) m_Fov = 75.0f;
}
