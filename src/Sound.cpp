#include "Sound.h"

Sound::Sound(std::string soundFile) : m_source(0), m_buffer(0)
{
    /* Load the sound into a buffer. */
    m_buffer = LoadSound(soundFile.c_str());
    if (!m_buffer)
    {
        CloseAL();
        exit(1);
    }

    /* Create the source to play the sound with. */
    m_source = 0;
    alGenSources(1, &m_source);
    alSourcei(m_source, AL_BUFFER, (ALint)m_buffer);
    assert(alGetError() == AL_NO_ERROR && "Failed to setup sound source");
}

Sound::~Sound()
{
    Free();
}

void Sound::Play()
{
    alSourcePlay(m_source);
}

void Sound::Free()
{
    alDeleteSources(1, &m_source);
    alDeleteBuffers(1, &m_buffer);
}
