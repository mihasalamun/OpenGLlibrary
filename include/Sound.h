#ifndef SOUND_H
#define SOUND_H

#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

#include "sndfile.h"

#include "AL/al.h"
#include "AL/alext.h"

#include "alhelpers.h"
#include "alloadsound.h"

#include "win_main_utf8.h"

class Sound
{
private:
    ALuint m_source, m_buffer;

public:
    Sound(std::string soundFile);
    ~Sound();

    void Play();
    void Free();
};

#endif
