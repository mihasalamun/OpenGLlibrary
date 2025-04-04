/* Define the alignment attribute for externally callable functions. */
#define FORCE_ALIGN

/* Define if deprecated EAX extensions are enabled */
/* #undef ALSOFT_EAX */

/* Define if HRTF data is embedded in the library */
#define ALSOFT_EMBED_HRTF_DATA

/* Define if we have the proc_pidpath function */
/* #undef HAVE_PROC_PIDPATH */

/* Define if we have DBus/RTKit */
#define HAVE_RTKIT

/* Define if we have SSE CPU extensions */
#define HAVE_SSE
#define HAVE_SSE2
#define HAVE_SSE3
#define HAVE_SSE4_1

/* Define if we have ARM Neon CPU extensions */
/* #undef HAVE_NEON */

/* Define if we have the ALSA backend */
#define HAVE_ALSA

/* Define if we have the OSS backend */
#define HAVE_OSS

/* Define if we have the PipeWire backend */
#define HAVE_PIPEWIRE

/* Define if we have the Solaris backend */
/* #undef HAVE_SOLARIS */

/* Define if we have the SndIO backend */
/* #undef HAVE_SNDIO */

/* Define if we have the WASAPI backend */
/* #undef HAVE_WASAPI */

/* Define if we have the DSound backend */
/* #undef HAVE_DSOUND */

/* Define if we have the Windows Multimedia backend */
/* #undef HAVE_WINMM */

/* Define if we have the PortAudio backend */
#define HAVE_PORTAUDIO

/* Define if we have the PulseAudio backend */
#define HAVE_PULSEAUDIO

/* Define if we have the JACK backend */
#define HAVE_JACK

/* Define if we have the CoreAudio backend */
/* #undef HAVE_COREAUDIO */

/* Define if we have the OpenSL backend */
/* #undef HAVE_OPENSL */

/* Define if we have the Oboe backend */
/* #undef HAVE_OBOE */

/* Define if we have the Wave Writer backend */
#define HAVE_WAVE

/* Define if we have the SDL2 backend */
/* #undef HAVE_SDL2 */

/* Define if we have dlfcn.h */
#define HAVE_DLFCN_H

/* Define if we have pthread_np.h */
/* #undef HAVE_PTHREAD_NP_H */

/* Define if we have cpuid.h */
#define HAVE_CPUID_H

/* Define if we have intrin.h */
/* #undef HAVE_INTRIN_H */

/* Define if we have guiddef.h */
/* #undef HAVE_GUIDDEF_H */

/* Define if we have GCC's __get_cpuid() */
#define HAVE_GCC_GET_CPUID

/* Define if we have the __cpuid() intrinsic */
/* #undef HAVE_CPUID_INTRINSIC */

/* Define if we have SSE intrinsics */
#define HAVE_SSE_INTRINSICS

/* Define if we have pthread_setschedparam() */
#define HAVE_PTHREAD_SETSCHEDPARAM

/* Define if we have pthread_setname_np() */
/* #undef HAVE_PTHREAD_SETNAME_NP */

/* Define if we have pthread_set_name_np() */
/* #undef HAVE_PTHREAD_SET_NAME_NP */

/* Define the installation data directory */
#define ALSOFT_INSTALL_DATADIR "./test/share"

/* Define whether build alsoft for winuwp */
/* #undef ALSOFT_UWP */
