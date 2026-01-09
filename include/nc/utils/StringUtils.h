#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace nc::utils {

    struct Codepoint {
        uint32_t value;
        int length;
    };

    inline Codepoint decodeUTF8(const char* ptr) {
        unsigned char c = static_cast<unsigned char>(*ptr);
        if (c < 0x80) {
            return { c, 1 };
        } else if ((c & 0xE0) == 0xC0) {
            return { ((c & 0x1F) << 6) | (ptr[1] & 0x3F), 2 };
        } else if ((c & 0xF0) == 0xE0) {
            return { ((c & 0x0F) << 12) | ((ptr[1] & 0x3F) << 6) | (ptr[2] & 0x3F), 3 };
        } else if ((c & 0xF8) == 0xF0) {
            return { ((c & 0x07) << 18) | ((ptr[1] & 0x3F) << 12) | ((ptr[2] & 0x3F) << 6) | (ptr[3] & 0x3F), 4 };
        }
        return { 0, 1 }; // Invalid
    }

}
