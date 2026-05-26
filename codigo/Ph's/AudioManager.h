#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <glm.hpp>
#include <string>
#include <unordered_map>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();
    bool Init();
    bool LoadWAV(const std::string& name, const std::string& path, float volumen = 1.0f);
    void PlayAudio(const std::string& name, glm::vec3 pos);
    void SetListenerPos(glm::vec3 pos, glm::vec3 front, glm::vec3 up);
    void Cleanup();
public:
    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;
    std::unordered_map<std::string, ALuint> buffers;
    std::unordered_map<std::string, ALuint> sources;
};