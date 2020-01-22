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

  bool getHFlip() const;
  bool getVFlip() const;

  void setHFlip( const bool );
  void setVFlip( const bool );

  glm::ivec2 size() const;

  resources::TexturePtr      m_texture;
  std::array< glm::vec2, 2 > m_rect;
  glm::ivec2                 m_pivot;

private:
  bool       m_hflip;
  bool       m_vflip;
  glm::ivec2 m_size;
};

}
}
