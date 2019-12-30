#pragma once

#include <glm/vec2.hpp>

#include <memory>
#include <array>

namespace engine {

namespace resources {
  class Texture;
  typedef std::shared_ptr< Texture > TexturePtr;
}

namespace gfx {

struct Sprite {
public:
  Sprite( resources::TexturePtr tex, const uint32_t x, const uint32_t y, const uint32_t w, const uint32_t h, const uint32_t pivot_x = 0, const uint32_t pivot_y = 0 );

  resources::TexturePtr       m_texture;
  std::array< glm::ivec2, 2 > m_rect;
  glm::ivec2                  m_pivot;
};

}
}
