#pragma once

#include <engine.h>

#include <config.h>
#include <logger.h>
#include <window.h>
#include <input.h>
#include <device.h>
#include <batchrenderer.h>
#include <guirenderer.h>

#include <imgui.h>

namespace engine {

class ENGINE_LIB_API ApplicationBase {
public:
    ApplicationBase(const std::string& name);
    virtual ~ApplicationBase() = default;
    ApplicationBase(const ApplicationBase&) = delete;
    ApplicationBase& operator= (const ApplicationBase&) = delete;

    void run(const char* configPath = "config.json");

    double tickTime() const;
    double renderTime() const;
    double dt() const;

    Config& config();
    gfx::Device& device();
    gfx::BatchRenderer& renderer();
    Input& input();

protected:
    virtual bool init() = 0;
    virtual void deinit() = 0;
    virtual bool tick (const double dt) = 0;

    LoggerIf    m_log;
    Window      m_window;

private:
    bool internalInit();
    void internalDeinit();
    bool initImGui();
    void updateImGui();

    std::string m_name;

    double m_tickTime;
    double m_renderTime;
    double m_dt;

    gfx::Device m_device;
    Config      m_config;
    gfx::BatchRenderer m_renderer;
    gfx::GUIRenderer   m_guiRenderer;
    ImGuiContext* m_imguiCtx;
    Input m_input;
};

} //engine
