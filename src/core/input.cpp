#include <input.h>
#include <window.h>
#include <application.h>

#include <GLFW/glfw3.h>

#include <cassert>
#include <cmath> // floor

namespace engine {

Input::Input(Window& window):
    m_log("INPUT"),
    m_mousePressed{false, false, false},
    m_window(window) {
}

bool Input::init() {
    m_log.debug("init");
    
    glfwSetKeyCallback(m_window.handle(), Input::keyCallback);
    glfwSetMouseButtonCallback(m_window.handle(), Input::mouseButtonCallback);

    return true;
}

void Input::onKeyAction(const Key key, const Action action, KeyActionCallback callback) {
    m_log.debug("Register callback for key %d, action %d", key, action);

    assert(key < Key::kMax);
    assert(action < Action::aMax);

    m_keyActionCallbacks[key][action].push_back(callback);
}

void Input::keyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods) {
    if (Window* window = static_cast<Window*>(glfwGetWindowUserPointer(wnd))) {
        Input& input = window->application().input();

        auto& callbacks = input.m_keyActionCallbacks[key][action];
        for (auto& cb: callbacks) {
            cb();
        }
    }

    ImGuiIO& io = ImGui::GetIO();
    if (action == GLFW_PRESS) {
        io.KeysDown[key] = true;
    }
    if (action == GLFW_RELEASE) {
        io.KeysDown[key] = false;
    }

    (void)mods; // Modifiers are not reliable across systems
    io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
    io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
}

void Input::mouseButtonCallback(GLFWwindow* wnd, int button, int action, int mods) {
    if (Window* window = static_cast<Window*>(glfwGetWindowUserPointer(wnd))) {
        Input& input = window->application().input();
        if (action == GLFW_PRESS && button >= 0 && button < 3) {
            input.m_mousePressed[button] = true;
        }
    }
}

void Input::getCursorPos(int& x, int& y) {
    double xpos, ypos;
    glfwGetCursorPos(m_window.handle(), &xpos, &ypos);
    x = std::floor(xpos);
    y = std::floor(ypos);
}

void Input::update() {
    ImGuiIO& io = ImGui::GetIO();
    auto glfwWindow = m_window.handle();

    if (glfwGetWindowAttrib(glfwWindow, GLFW_FOCUSED)) {
        // Set OS mouse position if requested (only used when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
        if (io.WantSetMousePos) {
            glfwSetCursorPos(glfwWindow, (double)io.MousePos.x, (double)io.MousePos.y);
        }
        else {
            double cursorX, cursorY;
            glfwGetCursorPos(glfwWindow, &cursorX, &cursorY);
            io.MousePos = ImVec2((float)cursorX, (float)cursorY);
        }
    }
    else {
        io.MousePos = ImVec2(-FLT_MAX,-FLT_MAX);
    }

    for (int i = 0; i < 3; i++)
    {
        // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
        io.MouseDown[i] = m_mousePressed[i] || glfwGetMouseButton(glfwWindow, i) != 0;
        m_mousePressed[i] = false;
    }
}

}
