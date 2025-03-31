#ifndef HRTF_H
#define HRTF_H

#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>

#include "sndfile.h"

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

#include "alhelpers.h"
#include "alloadsound.h"

#include "win_main_utf8.h"

class HRTF
{
private:
    ALCcontext *m_context;
    ALuint m_source, m_buffer;
    ALenum m_state;

    double m_angle;
    bool m_playing, m_has_angle_ext;

    std::thread thr;

public:
    HRTF(std::string soundFile);
    ~HRTF();

    void Play();
    void Free();
};

#endif
