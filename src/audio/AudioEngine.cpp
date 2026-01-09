#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "nc/audio/AudioEngine.h"
#include <iostream>

namespace nc::audio {

    AudioEngine::AudioEngine() {
        m_engine = new ma_engine();
    }

    AudioEngine::~AudioEngine() {
        shutdown();
        delete m_engine;
    }

    bool AudioEngine::init() {
        ma_result result = ma_engine_init(NULL, m_engine);
        if (result != MA_SUCCESS) {
            std::cerr << "AudioEngine: Failed to initialize miniaudio engine." << std::endl;
            return false;
        }
        
        m_initialized = true;
        std::cout << "AudioEngine: Initialized successfully." << std::endl;
        return true;
    }

    void AudioEngine::shutdown() {
        if (m_initialized) {
            // Uninit sounds
            for (auto& [path, soundData] : m_sounds) {
                if (soundData.sound) {
                    ma_sound_uninit(soundData.sound);
                    delete soundData.sound;
                }
            }
            m_sounds.clear();

            ma_engine_uninit(m_engine);
            m_initialized = false;
        }
    }

    void AudioEngine::playSound(const std::string& path) {
        if (!m_initialized) return;

        // Check if sound already loaded
        if (m_sounds.find(path) == m_sounds.end()) {
            ma_sound* sound = new ma_sound();
            ma_result result = ma_sound_init_from_file(m_engine, path.c_str(), 0, NULL, NULL, sound);
            if (result != MA_SUCCESS) {
                std::cerr << "AudioEngine: Failed to load sound " << path << std::endl;
                delete sound;
                return;
            }
            m_sounds[path] = { sound, path };
        }

        // Play
        ma_sound_start(m_sounds[path].sound);
        m_isPlaying = true; // Use this as a flag that *something* started
    }

    void AudioEngine::stopAll() {
        if (!m_initialized) return;
        
        for (auto& [path, soundData] : m_sounds) {
            ma_sound_stop(soundData.sound);
            ma_sound_seek_to_pcm_frame(soundData.sound, 0);
        }
        m_isPlaying = false;
    }

    float AudioEngine::getTime() const {
        if (!m_initialized) return 0.0f;
        
        // Return engine time (global timeline)
        // For a music video engine, usually we want the time of the *music track*.
        // But for v0.2, let's just use the global engine time which advances when *any* sound is mixed.
        // A better approach for NC-Kinetix is to return the time of the *primary* track.
        // Let's assume the first playing sound drives the clock.
        
        for (const auto& [path, soundData] : m_sounds) {
            if (ma_sound_is_playing(soundData.sound)) {
                 // Get cursor position in seconds
                 float cursor;
                 ma_sound_get_cursor_in_seconds(soundData.sound, &cursor);
                 return cursor;
            }
        }
        
        return 0.0f; // Fallback or accumulative dt if no audio
    }

    void AudioEngine::setTime(float time) {
         if (!m_initialized) return;
         
         for (auto& [path, soundData] : m_sounds) {
             ma_sound_seek_to_pcm_frame(soundData.sound, (ma_uint64)(time * 44100)); // Approx sample rate, miniaudio handles conversion
         }
    }

}
