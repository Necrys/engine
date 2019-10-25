#include <config.h>
#include <helpers/fileBuffer.h>

#include <json/json.h>

#include <cassert>
#include <iostream>

namespace engine {

bool Config::init(const std::string& path) {
    m_root.reset(new Json::Value(Json::objectValue));

    auto cfgFile = helper::FileBuffer::read(path);
    if (!cfgFile) {
        std::cout << "no file" << std::endl;
        return false;
    }

    Json::CharReaderBuilder rb;
    auto reader = rb.newCharReader();
    std::string errors;

    bool result = reader->parse(reinterpret_cast<const char*>(cfgFile->data()),
        reinterpret_cast<const char*>(cfgFile->data() + cfgFile->size() - 1),
        m_root.get(), &errors);

    if (!result) {
        std::cout << errors << std::endl;
        delete reader;
        return false;
    }

    delete reader;
    return true;
}

bool Config::getInt(const std::string& section, const std::string& name, int32_t* value) const {
    assert(value);

    const auto& sectionNode = (*m_root)[section];
    if (sectionNode.isNull())
        return false;

    const auto& valueNode = sectionNode[name];
    if (valueNode.isNull())
        return false;

    if (!valueNode.isInt())
        return false;

    *value = valueNode.asInt();
    return true;
}

bool Config::getFloat(const std::string& section, const std::string& name, float* value) const {
    assert(value);

    const auto& sectionNode = (*m_root)[section];
    if (sectionNode.isNull())
        return false;

    const auto& valueNode = sectionNode[name];
    if (valueNode.isNull())
        return false;

    if (!valueNode.isDouble())
        return false;

    *value = valueNode.asFloat();
    return true;
}

bool Config::getString(const std::string& section, const std::string& name, std::string& value) const {
    const auto& sectionNode = (*m_root)[section];
    if (sectionNode.isNull())
        return false;

    const auto& valueNode = sectionNode[name];
    if (valueNode.isNull())
        return false;

    if (!valueNode.isString())
        return false;

    value = valueNode.asString();
    return true;
}

void Config::setInt(const std::string& section, const std::string& name, const int32_t value) {
    (*m_root)[section][name] = value;
}

void Config::setFloat(const std::string& section, const std::string& name, const float value) {
    (*m_root)[section][name] = value;
}

void Config::setString(const std::string& section, const std::string& name, const std::string& value) {
    (*m_root)[section][name] = value;
}

} // engine
