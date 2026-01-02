#pragma once

#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace nc::render {

    // Interface for Text Rendering
    class ITextRenderer {
    public:
        virtual ~ITextRenderer() = default;
        virtual bool loadFont(const std::string& fontPath, int fontSize) = 0;
        virtual void renderText(const std::string& text, float x, float y, float scale, const float color[4]) = 0;
    };

    class TextRenderer : public ITextRenderer {
    public:
        TextRenderer();
        ~TextRenderer();

        bool loadFont(const std::string& fontPath, int fontSize) override;
        
        // This is a stub for the actual draw call. In a real engine, this would batch quads.
        void renderText(const std::string& text, float x, float y, float scale, const float color[4]) override;

    private:
        FT_Library m_ft;
        FT_Face m_face;
    };

}
