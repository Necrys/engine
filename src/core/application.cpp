#include <application.h>

#include <chrono>
#include <iostream>

#include <GLFW/glfw3.h>
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

namespace engine {

ApplicationBase::ApplicationBase(const std::string& name):
    m_log("APPLICAITON"),
    m_window(*this),    // Ouch!!!
    m_name(name),
    m_device(m_window),
    m_imguiCtx(nullptr),
    m_input(m_window) {
}

void ApplicationBase::run(const char* configPath) {
    m_config.init(configPath);
    if (!Logger::instance().init(m_config)) {
        std::cout << "ERROR: Failed to init logger" << std::endl;
        return;
    }

    if (!internalInit()) {
        internalDeinit();
        return;
    }

    m_log.debug("Run user init");
    if (!init()) {
        m_log.error("User init failed, run user deinit.");
        deinit();
        internalDeinit();
        return;
    }

    m_log.debug("Initialization done, start main loop. Good luck!");

    std::chrono::steady_clock timer;
    auto start = timer.now();
    auto stop = timer.now();
    auto tickEnd = timer.now();
    auto renderStart = timer.now();
    using ms = std::chrono::duration<double, std::milli>;
    m_dt = std::chrono::duration_cast<ms>(stop - start).count();

    for (;;) {
        start = timer.now();

        if (glfwWindowShouldClose(m_window.handle())) {
            break;
        }
        glfwPollEvents();
        updateImGui();

        if (!tick(m_dt)) {
            break;
        }
        tickEnd = timer.now();
        m_tickTime = std::chrono::duration_cast<ms>(tickEnd - start).count();

        renderStart = timer.now();
        // do rendering
        m_renderer.render();
        m_guiRenderer.render();
        m_device.swapBuffers();

        stop = timer.now();
        m_renderTime = std::chrono::duration_cast<ms>(stop - renderStart).count();
        m_dt = std::chrono::duration_cast<ms>(stop - start).count();
    }

    m_log.debug("Main loop exited. Start user deinit.");

    deinit();
    internalDeinit();
}

bool ApplicationBase::internalInit() {
    m_log.debug("internalInit");

    if (!m_window.init(m_name)) {
        return false;
    }

    if (!m_device.init()) {
        return false;
    }

    if (!m_renderer.init(m_device)) {
        return false;
    }

    if (!(m_imguiCtx = ImGui::CreateContext())) {
        m_log.error("Failed to create IMGUI context");
        return false;
    }

    if (!m_guiRenderer.init()) {
        return false;
    }

    if (!m_input.init()) {
        return false;
    }

    if (!initImGui()) {
        return false;
    }

    return true;
}

void ApplicationBase::internalDeinit() {
    m_log.debug("internalDeinit");

    m_guiRenderer.deinit();
    if (m_imguiCtx)
        ImGui::DestroyContext(m_imguiCtx);
    m_renderer.deinit();
    m_window.deinit();
}

bool ApplicationBase::initImGui() {
    ImGuiIO& io = ImGui::GetIO();

    io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
    io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
    io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

#ifdef _WIN32
    io.ImeWindowHandle = glfwGetWin32Window(m_window.handle());
#endif

    return true;
}

void ApplicationBase::updateImGui() {
    ImGuiIO& io = ImGui::GetIO();

    auto glfwWindow = m_window.handle();

    int wndWidth, wndHeight;
    int fbWidth, fbHeght;
    glfwGetWindowSize(glfwWindow, &wndWidth, &wndHeight);
    glfwGetFramebufferSize(glfwWindow, &fbWidth, &fbHeght);

    io.DisplaySize = ImVec2((float)wndWidth, (float)wndHeight);
    io.DisplayFramebufferScale = ImVec2(wndWidth > 0 ? ((float)fbWidth / wndWidth) : 0,
        wndHeight > 0 ? ((float)fbHeght / wndHeight) : 0);
    io.DeltaTime = static_cast<float>((m_dt > 0.0) ? (m_dt / 1000.0) : 0.001);

    m_input.update();

    ImGui::NewFrame();
}

Config& ApplicationBase::config() {
    return m_config;
}

gfx::Device& ApplicationBase::device() {
    return m_device;
}

gfx::BatchRenderer& ApplicationBase::renderer() {
    return m_renderer;
}

Input& ApplicationBase::input() {
    return m_input;
}

} //engine
