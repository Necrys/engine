#pragma once

#include <logger.h>

struct GLFWwindow;

namespace engine {

class ApplicationBase;

class Window {
public:
    enum class Mode {
        wndUnknown            = 0,
        wndWindowed           = 1,
        wndWindowedBorderless = 2,
        wndFullscreen         = 3
    };

    Window(ApplicationBase& app);

    bool init(const std::string& title);
    void deinit();

    GLFWwindow* handle();
    ApplicationBase& application();

    void getSize(int* width, int* height);

private:
    LoggerIf m_log;
    int32_t  m_width;
    int32_t  m_height;
    Mode     m_mode;
    bool     m_maximized;
    int32_t  m_vsync;

    GLFWwindow* m_glfwWindow;

    ApplicationBase& m_app;

    void onResize(int width, int height);

    static void windowSizeCallback(GLFWwindow* window, int width, int height);
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void cursorPositionCallback(GLFWwindow* window, double x, double y);
};

} //engine
