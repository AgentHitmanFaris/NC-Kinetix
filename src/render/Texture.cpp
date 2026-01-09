#include "nc/render/Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

namespace nc::render {

    Texture::Texture() : m_textureID(0), m_width(0), m_height(0), m_channels(0) {}

    Texture::~Texture() {
        if (m_textureID) {
            glDeleteTextures(1, &m_textureID);
        }
    }

    bool Texture::loadFromFile(const std::string& path) {
        // Load image
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
        
        if (!data) {
            std::cerr << "Texture: Failed to load image: " << path << std::endl;
            return false;
        }

        GLenum format = GL_RGB;
        if (m_channels == 1) format = GL_RED;
        else if (m_channels == 3) format = GL_RGB;
        else if (m_channels == 4) format = GL_RGBA;

        glGenTextures(1, &m_textureID);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        // glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        return true;
    }

    void Texture::bind(unsigned int slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_textureID);
    }

    void Texture::unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}
