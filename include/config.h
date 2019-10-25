#pragma once

#include <string>
#include <memory>

namespace Json {
    class Value;
}

using JsonValuePtr = std::shared_ptr<Json::Value>;

namespace engine {

class Config {
public:
    Config() = default;

    Config(const Config&) = delete;
    Config& operator= (const Config&) = delete;

    bool init(const std::string& path);

    bool getInt(const std::string& section, const std::string& name, int32_t* value) const;
    bool getFloat(const std::string& section, const std::string& name, float* value) const;
    bool getString(const std::string& section, const std::string& name, std::string& value) const;

    void setInt(const std::string& section, const std::string& name, const int32_t value);
    void setFloat(const std::string& section, const std::string& name, const float value);
    void setString(const std::string& section, const std::string& name, const std::string& value);

private:
    JsonValuePtr m_root;
};

} //engine
