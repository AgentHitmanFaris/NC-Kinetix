#pragma once

#include <string>
#include <map>
#include <memory>

// Forward declare miniaudio structs to keep header clean
struct ma_engine;
struct ma_sound;

namespace nc::audio {

    class AudioEngine {
    public:
        AudioEngine();
        ~AudioEngine();

        bool init();
        void shutdown();

        // Basic playback
        void playSound(const std::string& path);
        void stopAll();

        // Time management (Crucial for timeline sync)
        float getTime() const;
        void setTime(float time);
        
        bool isPlaying() const { return m_isPlaying; }

    private:
        struct SoundData {
            ma_sound* sound; // Raw pointer as miniaudio manages memory
            std::string path;
        };

        ma_engine* m_engine{nullptr};
        std::map<std::string, SoundData> m_sounds;
        bool m_initialized{false};
        bool m_isPlaying{false};
    };

}
