#include "nc/render/TextRenderer.h"
#include <iostream>

namespace nc::render {

    TextRenderer::TextRenderer() : m_face(nullptr) {
        if (FT_Init_FreeType(&m_ft)) {
            std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        }
    }

    TextRenderer::~TextRenderer() {
        FT_Done_Face(m_face);
        FT_Done_FreeType(m_ft);
    }

    bool TextRenderer::loadFont(const std::string& fontPath, int fontSize) {
        if (FT_New_Face(m_ft, fontPath.c_str(), 0, &m_face)) {
            std::cerr << "ERROR::FREETYPE: Failed to load font: " << fontPath << std::endl;
            return false;
        }
        FT_Set_Pixel_Sizes(m_face, 0, fontSize);
        std::cout << "Loaded font: " << fontPath << " size: " << fontSize << std::endl;
        
        // Note: For SDF, we would normally use FT_Render_Glyph with FT_RENDER_MODE_SDF
        // if FreeType version supports it (2.11+), or generate it manually.
        // This is a stub for the architecture.
        
        return true;
    }

    void TextRenderer::renderText(const std::string& text, float x, float y, float scale, const float color[4]) {
        // Architecture Stub:
        // 1. Iterate through characters
        // 2. Look up texture atlas/glyph cache
        // 3. Generate vertices
        // 4. Submit to BatchRenderer
        
        // For now, just print to console to prove linkage
        // std::cout << "Rendering Text: " << text << " at (" << x << "," << y << ")" << std::endl;
    }

}
