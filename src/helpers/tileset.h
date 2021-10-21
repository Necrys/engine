#pragma once

#include <logger.h>
#include <sprite.h>

#include <vector>

namespace engine {
namespace helper {

class Tileset {
public:
  Tileset();

  static Tileset loadFromFile(const std::string& path);

  gfx::Sprite& getTile(const uint32_t id);
  size_t size() const;

private:
  LoggerIf                 m_log;
  std::vector<gfx::Sprite> m_tiles;
  gfx::Sprite              m_dummy;
};

} //helper
} //engine