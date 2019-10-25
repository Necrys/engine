#pragma once

#include <keys.h>
#include <logger.h>

#include <functional>
#include <vector>

struct GLFWwindow;

namespace engine {

class Window;

class Input {
public:
    using KeyActionCallback = std::function<void()>;

    Input(Window& window);
    ~Input() = default;
    Input(const Input&) = delete;
    Input& operator= (const Input&) = delete;

    bool init();

    void onKeyAction(const Key key, const Action action, KeyActionCallback callback);
    void getCursorPos(int& x, int& y);

    void update();

private:
    LoggerIf m_log;
    std::vector<KeyActionCallback> m_keyActionCallbacks[Key::kMax][Action::aMax];
    bool m_mousePressed[3];

    Window& m_window;

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow*, int button, int action, int mods);
};

} //engine
