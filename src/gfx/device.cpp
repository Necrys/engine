#include <glapi.h>
#include <device.h>
#include <config.h>
#include <window.h>

#include <GLFW/glfw3.h>

#include <GL/gl.h>

namespace engine {
namespace gfx {

Device::Device(Window& wnd):
    m_log("GFX_DEVICE"),
    m_window(wnd) {
}

bool Device::init() {
    m_log.debug("init");

    if (!glapi::init()) {
        OGL_CHECK_ERRORS();
        return false;
    }

    glClearColor(0.1f, 0.1f, 0.25f, 1.0f);
    glClearDepth(1.0f);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return OGL_CHECK_ERRORS();
}

void Device::swapBuffers() {
    glFinish();
    glfwSwapBuffers(m_window.handle());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    OGL_CHECK_ERRORS();
}

void Device::onResize(const int width, const int height) {
    glViewport(0, 0, width, height);
    OGL_CHECK_ERRORS();

    for (auto cb: m_resizeCallbacks) {
        cb(width, height);
    }
}

void Device::registerOnResizeCallback(ResizeCb cb) {
    m_resizeCallbacks.push_back(cb);
}

void Device::getFramebufferSize(int* width, int* height) {
    glfwGetFramebufferSize(m_window.handle(), width, height);
}

} //gfx
} //engine
