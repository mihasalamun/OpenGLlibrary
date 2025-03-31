#include "HRTF.h"

HRTF::HRTF(std::string soundFile)
    : m_context(nullptr), m_source(0), m_buffer(0), m_state(0), m_angle(0), m_playing(true), m_has_angle_ext(false)
{
    ALCdevice *device;
    const char *hrtfname;
    ALCint hrtf_state;
    ALCint num_hrtf;

    // Check for HRTF support
    m_context = alcGetCurrentContext();
    device = alcGetContextsDevice(m_context);
    if (!alcIsExtensionPresent(device, "ALC_SOFT_HRTF"))
    {
        fprintf(stderr, "Error: ALC_SOFT_HRTF not supported\n");
        CloseAL();
        exit(1);
    }

/* Define a macro to help load the function pointers. */
#define LOAD_PROC(d, T, x) ((x) = FUNCTION_CAST(T, alcGetProcAddress((d), #x)))
    LOAD_PROC(device, LPALCRESETDEVICESOFT, alcResetDeviceSOFT);
#undef LOAD_PROC

    /* Check for the AL_EXT_STEREO_ANGLES extension to be able to also rotate
     * stereo sources.
     */
    m_has_angle_ext = alIsExtensionPresent("AL_EXT_STEREO_ANGLES");

    hrtfname = NULL;

    /* Enumerate available HRTFs, and reset the device using one. */
    alcGetIntegerv(device, ALC_NUM_HRTF_SPECIFIERS_SOFT, 1, &num_hrtf);
    if (!num_hrtf)
        printf("No HRTFs found\n"); // TODO: daj to drugam in potem uporabi drugo stvar
    else
    {
        ALCint attr[5];
        ALCint index = -1;
        ALCint i;

        i = 0;
        attr[i++] = ALC_HRTF_SOFT;
        attr[i++] = ALC_TRUE;
        if (index == -1)
        {
            if (hrtfname)
                printf("HRTF \"%s\" not found\n", hrtfname);
        }
        else
        {
            printf("Selecting HRTF %d...\n", index);
            attr[i++] = ALC_HRTF_ID_SOFT;
            attr[i++] = index;
        }
        attr[i] = 0;

        if (!alcResetDeviceSOFT(device, attr))
            printf("Failed to reset device: %s\n", alcGetString(device, alcGetError(device)));
    }

    /* Check if HRTF is enabled, and show which is being used. */
    alcGetIntegerv(device, ALC_HRTF_SOFT, 1, &hrtf_state);
    if (!hrtf_state)
        printf("HRTF not enabled!\n");

    fflush(stdout);

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
    alSourcei(m_source, AL_SOURCE_RELATIVE, AL_TRUE);
    alSource3f(m_source, AL_POSITION, 0.0f, 0.0f, -1.0f);
    alSourcei(m_source, AL_LOOPING, AL_TRUE);
    alSourcei(m_source, AL_BUFFER, (ALint)m_buffer);
    assert(alGetError() == AL_NO_ERROR && "Failed to setup sound source");
}

HRTF::~HRTF()
{
    m_playing = false;
    if (thr.joinable())
        thr.join();
}

void HRTF::Play()
{
    auto f = [&]()
    {
        m_angle = 0.0;
        alSourcePlay(m_source);
        do
        {
            al_nssleep(10000000);

            alcSuspendContext(m_context);
            m_angle += 0.01 * M_PI * 0.5;
            if (m_angle > M_PI)
                m_angle -= M_PI * 2.0;

            /* This only rotates mono sounds. */
            alSource3f(m_source, AL_POSITION, (ALfloat)sin(m_angle), 0.0f, -(ALfloat)cos(m_angle));

            if (m_has_angle_ext)
            {
                ALfloat angles[2] = {(ALfloat)(M_PI / 6.0 - m_angle), (ALfloat)(-M_PI / 6.0 - m_angle)};
                alSourcefv(m_source, AL_STEREO_ANGLES, angles);
            }
            alcProcessContext(m_context);

            alGetSourcei(m_source, AL_SOURCE_STATE, &m_state);
        } while (alGetError() == AL_NO_ERROR && m_state == AL_PLAYING && m_playing);
    };

    thr = std::thread(f);
}

void HRTF::Free()
{
    alDeleteSources(1, &m_source);
    alDeleteBuffers(1, &m_buffer);
}
