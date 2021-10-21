#include "tileset.h"
#include "fileBuffer.h"
#include "texture.h"

#include <json/json.h>

namespace engine {
namespace helper {

static const std::string strImageNode = "image";
static const std::string strImageWidthNode = "imagewidth";
static const std::string strImageHeightNode = "imageheight";
static const std::string strTileWidthNode = "tilewidth";
static const std::string strTileHeightNode = "tileheight";
static const std::string strTileCountNode = "tilecount";

Tileset::Tileset() :
  m_log("TILESET"),
  m_dummy(nullptr, 0, 0, 0, 0) {
}

Tileset Tileset::loadFromFile(const std::string& path) {
  LoggerIf log("TILESET");
  Tileset tileset;

  auto tilesetFile = FileBuffer::read(path);
  if (!tilesetFile) {
    log.error("Failed to load tileset file \"%s\"", path.c_str());
    return Tileset();
  }

  Json::CharReaderBuilder rb;
  auto reader = rb.newCharReader();
  std::string errors;
  Json::Value doc(Json::objectValue);

  bool result = reader->parse(reinterpret_cast<const char*>(tilesetFile->data()),
    reinterpret_cast<const char*>(tilesetFile->data() + tilesetFile->size() - 1),
    &doc, &errors);

  if (!result) {
    log.error("Failed to parse tileset file \"%s\": %s", path.c_str(), errors.c_str());
    return Tileset();
  }

  if (!doc[strImageNode]) {
    log.error("Can't find image node");
    return Tileset();
  }

  // extract directory from path
  size_t endp = path.find_last_of('/');
  if (endp == std::string::npos) {
    endp = path.length();
  }

  std::string dir(path, 0, endp + 1);
  std::string imagePath = dir + doc[strImageNode].asString();
  auto tex = resources::Texture::loadFromPNG(imagePath.c_str());
  if (!tex) {
    log.error("Can't load texture \"%s\"", imagePath);
    return Tileset();
  }

  uint32_t tileWidth, tileHeight;

  if (!doc[strTileWidthNode]) {
    log.error("Can't find tile width node");
    return Tileset();
  }
  tileWidth = doc[strTileWidthNode].asUInt();

  if (!doc[strTileHeightNode]) {
    log.error("Can't find tile height node");
    return Tileset();
  }
  tileHeight = doc[strTileHeightNode].asUInt();

  uint32_t cols, rows, tilesCount;
  cols = tex->width() / tileWidth;
  rows = tex->height() / tileHeight;

  if (!doc[strImageWidthNode]) {
    tilesCount = cols * rows;
  }
  else {
    tilesCount = doc[strImageWidthNode].asUInt();
  }

  tileset.m_tiles.reserve(tilesCount);

  uint32_t sx, sy, sw, sh;

  for (uint32_t i = 0; i < rows; ++i) {
    for (uint32_t j = 0; j < cols; ++j) {
      sx = j * tileWidth;
      sy = i * tileHeight;
      sw = sx + tileWidth;
      sh = sy + tileHeight;
      tileset.m_tiles.emplace_back(gfx::Sprite(tex, sx, sy, sw, sh));
    }
  }

  return tileset;
}

engine::gfx::Sprite& Tileset::getTile(const uint32_t id) {
  if (id >= m_tiles.size()) {
    return m_dummy;
  }

  return m_tiles[id];
}

size_t Tileset::size() const {
  return m_tiles.size();
}

} //helper
} //engine