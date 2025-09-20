///////////////////////////////////////////////////////////////////////////////
// ViewManager.h
// ============
#pragma once
#include <glm/glm.hpp>

struct GLFWwindow;
class ShaderManager;

class ViewManager
{
public:
    explicit ViewManager(ShaderManager* pShaderManager);
    ~ViewManager();

    GLFWwindow* CreateDisplayWindow(const char* windowTitle);
    void PrepareSceneView();

    void ProcessInput(GLFWwindow* window);
    void ProcessMouseMovement(float xoffset, float yoffset);
    void ProcessMouseScroll(float yoffset);

    // **NEW**: needed so SceneManager can set 'viewPos' for lighting
    glm::vec3 GetCameraPosition() const { return m_CamPos; }

private:
    ShaderManager* m_pShaderManager;
    GLFWwindow* m_pWindow;

    glm::vec3 m_CamPos;
    glm::vec3 m_CamFront;
    glm::vec3 m_CamUp;

    float m_Yaw, m_Pitch, m_Fov;
    float m_MoveSpeed, m_MouseSense;
    float m_LastFrame;

    enum class ProjectionMode { Perspective, Orthographic };
    ProjectionMode m_Mode;
    float m_OrthoWidth, m_OrthoHeight;
};
