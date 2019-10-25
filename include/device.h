#pragma once

#include <logger.h>

#include <functional>
#include <vector>

namespace engine {

class Config;
class Window;

namespace gfx {

class Device {
public:
    using ResizeCb = std::function<void(const int32_t, const int32_t)>;

    Device(Window& wnd);
    ~Device() = default;

    Device(const Device&) = delete;
    Device& operator= (const Device&) = delete;

    bool init();

    void swapBuffers();
    void onResize(const int width, const int height);

    void registerOnResizeCallback(ResizeCb cb);

    void getFramebufferSize(int* width, int* height);

private:
    LoggerIf m_log;
    Window&  m_window;

    std::vector<ResizeCb> m_resizeCallbacks;
};

} //gfx
} //engine
