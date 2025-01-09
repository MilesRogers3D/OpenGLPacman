#include "AudioEmitter.h"
#include "IO/Log.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

ma_engine m_engine;

AudioEmitter::AudioEmitter()
{
    ma_engine_init(nullptr, &m_engine);
}

AudioEmitter::~AudioEmitter()
{
    ma_engine_uninit(&m_engine);
}

void AudioEmitter::Play(const char* path)
{
    ma_engine_play_sound(&m_engine, path, nullptr);
}
