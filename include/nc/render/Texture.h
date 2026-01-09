#pragma once

#include <string>
#include <glad/glad.h>

namespace nc::render {

    class Texture {
    public:
        Texture();
        ~Texture();

        bool loadFromFile(const std::string& path);
        void bind(unsigned int slot = 0) const;
        void unbind() const;

        int getWidth() const { return m_width; }
        int getHeight() const { return m_height; }
        unsigned int getID() const { return m_textureID; }

    private:
        unsigned int m_textureID;
        int m_width;
        int m_height;
        int m_channels;
    };

}
