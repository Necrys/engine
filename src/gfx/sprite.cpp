#include <sprite.h>
#include <texture.h>

namespace engine {
namespace gfx {

Sprite::Sprite( resources::TexturePtr tex, const uint32_t x, const uint32_t y, const uint32_t w, const uint32_t h, const uint32_t pivot_x, const uint32_t pivot_y ):
  m_texture( tex ),
  m_rect( {{{ x, y }, { x + w, y + h }}} ),
  m_pivot( pivot_x, pivot_y ) {
}

} //gfx
} //engine