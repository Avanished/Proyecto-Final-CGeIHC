#include "AudioManager.h"
#include <sndfile.h>
#include <vector>
#include <stdio.h>

AudioManager::AudioManager() {}
AudioManager::~AudioManager() {}

bool AudioManager::Init() {
    device = alcOpenDevice(nullptr);
    if (!device) {
        printf("ERROR: No se pudo abrir dispositivo de audio\n");
        return false;
    }
    context = alcCreateContext(device, nullptr);
    alcMakeContextCurrent(context);
    printf("OK -> AudioManager inicializado\n");
    return true;
}

bool AudioManager::LoadWAV(const std::string& name, const std::string& path, float volumen) {
    SF_INFO info;
    SNDFILE* file = sf_open(path.c_str(), SFM_READ, &info);
    if (!file) {
        printf("ERROR: No se pudo abrir %s\n", path.c_str());
        return false;
    }

    std::vector<short> samples(info.frames * info.channels);
    sf_readf_short(file, samples.data(), info.frames);

    // Si es estéreo, convertir a mono promediando los dos canales
    if (info.channels == 2) {
        std::vector<short> mono(info.frames);
        for (int i = 0; i < info.frames; i++)
            mono[i] = (short)(((int)samples[i * 2] + (int)samples[i * 2 + 1]) / 2);
        samples = mono;
        info.channels = 1;
    }

    ALenum format = (info.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

    ALuint buf;
    alGenBuffers(1, &buf);
    alBufferData(buf, format, samples.data(),
        (ALsizei)(samples.size() * sizeof(short)), info.samplerate);
    buffers[name] = buf;

    ALuint src;
    alGenSources(1, &src);
    alSourcei(src, AL_BUFFER, buf);
    alSourcef(src, AL_GAIN, volumen);
    alSourcef(src, AL_ROLLOFF_FACTOR, 1.5f);
    alSourcef(src, AL_REFERENCE_DISTANCE, 20.0f);
    alSourcef(src, AL_MAX_DISTANCE, 100.0f);
    alSourcei(src, AL_LOOPING, AL_TRUE);
    sources[name] = src;

    printf("OK -> Audio cargado: %s\n", name.c_str());
    return true;
}

void AudioManager::PlayAudio(const std::string& name, glm::vec3 pos) {
    if (sources.find(name) == sources.end()) return;
    ALuint src = sources.at(name);
    alSource3f(src, AL_POSITION, pos.x, pos.y, pos.z);
    ALint state;
    alGetSourcei(src, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING)
        alSourcePlay(src);
}

void AudioManager::SetListenerPos(glm::vec3 pos, glm::vec3 front, glm::vec3 up) {
    alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
    ALfloat orient[6] = { front.x, front.y, front.z, up.x, up.y, up.z };
    alListenerfv(AL_ORIENTATION, orient);
}

void AudioManager::Cleanup() {
    for (auto& [name, src] : sources) alDeleteSources(1, &src);
    for (auto& [name, buf] : buffers) alDeleteBuffers(1, &buf);
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
    printf("OK -> AudioManager liberado\n");
}