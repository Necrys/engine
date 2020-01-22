#include <sprite.h>
#include <texture.h>

namespace engine {
namespace gfx {

Sprite::Sprite( resources::TexturePtr tex, const uint32_t x, const uint32_t y, const uint32_t w, const uint32_t h, const uint32_t pivot_x, const uint32_t pivot_y ):
  m_texture( tex ),
  m_pivot( pivot_x, pivot_y ),
  m_hflip( false ),
  m_vflip( false ),
  m_size( w, h ) {
  double tex_w = m_texture ? m_texture->width() : 1.0;
  double tex_h = m_texture ? m_texture->height() : 1.0;

  m_rect[ 0 ].x = ( double )x / tex_w;
  m_rect[ 0 ].y = ( double )y / tex_h;
  m_rect[ 1 ].x = ( double )( x + w ) / tex_w;
  m_rect[ 1 ].y = ( double )( y + h ) / tex_h;
}

bool Sprite::getHFlip() const {
  return m_hflip;
}

bool Sprite::getVFlip() const {
  return m_vflip;
}

void Sprite::setHFlip( const bool hflip ) {
  if ( m_hflip != hflip ) {
    m_hflip = hflip;
    std::swap( m_rect[ 0 ].x, m_rect[ 1 ].x );
  }
}

void Sprite::setVFlip( const bool vflip ) {
  if ( m_vflip != vflip ) {
    m_vflip = vflip;
    std::swap( m_rect[ 0 ].y, m_rect[ 1 ].y );
  }
}

glm::ivec2 Sprite::size() const {
  return m_size;
}

} //gfx
} //engine
