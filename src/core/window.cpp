#include <window.h>
#include <config.h>
#include <application.h>

#include <GLFW/glfw3.h>

namespace engine {

static const char* wndModeToString(const Window::Mode mode) {
    switch (mode) {
        case Window::Mode::wndUnknown:            return "Unknown";
        case Window::Mode::wndWindowed:           return "Windowed";
        case Window::Mode::wndWindowedBorderless: return "Windowed borderless";
        case Window::Mode::wndFullscreen:         return "Fullscreen";
        default:                                  return "Unknown mode value";
    }
}

Window::Window(ApplicationBase& app):
    m_log("WINDOW"),
    m_width(640),
    m_height(480),
    m_mode(Mode::wndWindowed),
    m_vsync(0),
    m_maximized(false),
    m_app(app) {
}

bool Window::init(const std::string& title) {
    m_log.debug("init");

    Config& cfg = m_app.config();

    cfg.getInt("window", "width", &m_width);
    cfg.getInt("window", "height", &m_height);

    int32_t res = 1;
    cfg.getInt("window", "mode", &res);
    m_mode = static_cast<Mode>(res);

    res = 0;
    cfg.getInt("window", "maximized", &res);
    m_maximized = res != 0;

    cfg.getInt("window", "vsync", &m_vsync);
    // limit vsync value to 1
    m_vsync = m_vsync > 1 ? 1 : m_vsync;

    m_log.debug("config: %dx%d, mode: %s, maximized: %s, vsync: %d", m_width, m_height,
        wndModeToString(m_mode), m_maximized?"true":"false", m_vsync);

    if (GL_TRUE != glfwInit()) {
        m_log.error("glfwInit failed");
        return false;
    }
    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    int width = m_width;
    int height = m_height;
    
    GLFWmonitor* monitor = NULL;

    // http://www.glfw.org/docs/latest/window.html#window_full_screen
    if (m_mode == Mode::wndFullscreen && (monitor = glfwGetPrimaryMonitor()) == nullptr) {
        m_log.warning("glfwGetPrimaryMonitor() returned NULL");
        m_mode = Mode::wndWindowed;
    }

    // http://www.glfw.org/docs/latest/window.html#window_windowed_full_screen
    if (m_mode == Mode::wndWindowedBorderless && (monitor = glfwGetPrimaryMonitor()) != nullptr) {
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if (NULL == mode) {
            m_log.error("glfwGetVideoMode(%p) returned NULL", monitor);
            return false;
        }

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        width = mode->width;
        height = mode->height;
    }

    if (m_maximized && m_mode == Mode::wndWindowed)
        glfwWindowHint(GLFW_MAXIMIZED , GL_TRUE);

    m_glfwWindow = glfwCreateWindow(width, height, title.c_str(), monitor, NULL);
    if (NULL == m_glfwWindow) {
        m_log.error("glfwCreateWindow() returned NULL");
        return false;
    }

    glfwSetWindowUserPointer(m_glfwWindow, this);
    glfwSetWindowSizeCallback(m_glfwWindow, windowSizeCallback);
    glfwSetFramebufferSizeCallback(m_glfwWindow, framebufferSizeCallback);

    glfwMakeContextCurrent(m_glfwWindow);
    glfwSwapInterval(m_vsync);

    m_width = width;
    m_height = height;
    cfg.setInt("window", "width", m_width);
    cfg.setInt("window", "heigth", m_height);

    return true;
}

void Window::deinit() {
    m_log.debug("deinit");
    glfwDestroyWindow(m_glfwWindow);
    glfwTerminate();
}

GLFWwindow* Window::handle() {
    return m_glfwWindow;
}

ApplicationBase& Window::application() {
    return m_app;
}

void Window::onResize(int width, int height) {
    m_width = width;
    m_height = height;
    
    int maximized = glfwGetWindowAttrib(m_glfwWindow, GLFW_MAXIMIZED);
    m_maximized = (maximized == 0) ? false : true;

    Config& cfg = m_app.config();
    cfg.setInt("window", "width", m_width);
    cfg.setInt("window", "heigth", m_height);
    cfg.setInt("window", "maximized", m_maximized ? 1 : 0);
}

void Window::getSize(int* width, int* height) {
    glfwGetWindowSize(m_glfwWindow, width, height);
}

void Window::windowSizeCallback(GLFWwindow* wnd, int width, int height) {
    if (Window* window = static_cast<Window*>(glfwGetWindowUserPointer(wnd)))
        window->onResize(width, height);
}

void Window::framebufferSizeCallback(GLFWwindow* wnd, int width, int height) {
    if (Window* window = static_cast<Window*>(glfwGetWindowUserPointer(wnd)))
        window->m_app.device().onResize(width, height);
}

void Window::cursorPositionCallback(GLFWwindow* wnd, double x, double y) {

}

} //engine
